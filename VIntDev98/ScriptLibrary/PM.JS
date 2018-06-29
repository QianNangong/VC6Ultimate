// ************************************************************************<BR>
// Microsoft Script Library<BR>
// Visual InterDev 6.0 Scripting Object Model framework for client<BR>
// Copyright 1998 Microsoft Corporation. All Rights Reserved.<BR>
// <B>Do not modify in design view. Switch to source view.</B><BR>
// ************************************************************************<BR>
// <SCRIPT>
var thisPage = new _SOMObject;
function _SOMObject()
{	//public members
	this.invokeMethod = _SOM_invokeMethod;
	this.navigateURL = _SOM_navigateURL;
	this.setState = _SOM_setState;
	this.getState = _SOM_getState;
	this.advise = _SOM_advise;
	this.unadvise = _SOM_unadvise;
	this.cancelEvent = false;
	this.navigateTarget = window;

	//events
	PAGE_ONBEFORESERVEREVENT = 'onbeforeserverevent';

	//private members
	this._fireEvent = _SOM__fireEvent;
	this._buildURL = _SOM__buildURL;
	this._extractValue = _SOM__extractValue;
	this._insertValue = _SOM__insertValue;
	this._adviseList = null;
	this._nLastAdviseID = 0;
}

function _SOM_invokeMethod(url,method,args)
{	
	if (url == '') url = this._location;
	this.navigateURL(this._buildURL(url,method,args));		
}

function _SOM_navigateURL(url)
{
	// if navigating to another frame, change a relative url to an absolute url.
	if (this.navigateTarget != window)
	{
		if (url.indexOf("/") == 0)
			url = window.location.protocol + "//" + window.location.hostname + url;
		else
		{
			var nProtocol = url.indexOf("://");
			var nParameters = url.indexOf("?");

			if (nProtocol == -1 || (nParameters != -1 && nParameters < nProtocol))
			{
				var thisURL = window.location.href;

				if (thisURL.substring(thisURL.length, -1) != "/")
					thisURL = thisURL.substring(0, thisURL.lastIndexOf("/")+1);

				url = thisURL + url;
			}
		}
	}

	if (typeof(this.navigateTarget.document.thisForm) == 'object' && typeof(this.navigateTarget.document.thisForm._method) == 'object')
	{	// use form submit to POST state back to server
		this.navigateTarget.document.thisForm.action = this.navigateTarget.location.href;
		this.navigateTarget.document.thisForm._method.value = url;
		this.navigateTarget.document.thisForm.submit();
	}
	else	
		this.navigateTarget.location.href = url;
}

function _SOM_setState(strName,value)
{	
	if (typeof(document.thisForm) == 'object' && typeof(document.thisForm._thisPage_state) == 'object')
	{	// insert state into hidden field
		return this._insertValue(document.thisForm._thisPage_state,strName,value);
	}
	return false;
}

function _SOM_getState(strName)
{	
	if (typeof(document.thisForm) == 'object' && typeof(document.thisForm._thisPage_state) == 'object')
	{	// extract state from hidden field
		var strState = document.thisForm._thisPage_state.value;
		return this._extractValue(strState,strName);
	}
	return null;
}

function _SOM_advise(strEvent,funcToCall)
{
	if ((typeof(strEvent) == 'string') && 
		(strEvent == PAGE_ONBEFORESERVEREVENT) && 
		(funcToCall != null))
	{
		if (this._adviseList == null)
			this._adviseList = new Object;
		this._adviseList[++this._nLastAdviseID] = funcToCall;
		return this._nLastAdviseID;
	}
	return -1;
}

function _SOM_unadvise(strEvent,nAdviseID)
{
	if (this._adviseList != null &&
		(typeof(strEvent) == 'string') && 
		(strEvent == PAGE_ONBEFORESERVEREVENT) && 
		(nAdviseID != null))
	{
		if (this._adviseList[nAdviseID] != null)
		{
			this._adviseList[nAdviseID] = null;
			return true;
		}
	}
	return false;
}

function _SOM__fireEvent(strName,strEvent)
{	
	// fire client-side onbeforeserverevent hook
	if (typeof(thisPage_onbeforeserverevent) == 'function')
	{	// fire default onbeforeserverevent handler
		thisPage_onbeforeserverevent(strName,strEvent);
	}
	if (this._adviseList != null)
	{	// fire handlers in advise list
		for (var i = 1; i <= this._nLastAdviseID; i++)
		{
			var adviseType = typeof(this._adviseList[i]);
			if (adviseType == 'function')
				this._adviseList[i](strName,strEvent);
			else if (adviseType == 'string')
				eval(this._adviseList[i]);
		}
	}

	// propogate event to server if not cancelled
	if (this.cancelEvent == '0' || this.cancelEvent == 'false')
	{
		//alert('_fireEvent(' + strName + ',' + strEvent + ')');
		thisPage.invokeMethod('','_EM__onclientevent',new Array(strName,strEvent));
	}
	this.cancelEvent = false;
}

function _SOM__escapePlus(value)
{
	for (var i = 0, newValue = ''; i < value.length; i++)
		newValue += (value.charAt(i) == '+' ? '%2B' : value.charAt(i));
	return newValue;
}

function _SOM__buildURL(url,method,args)
{
	if (typeof(method) == 'string')
	{
		url += '?_method=' + method
		var params = '&pcount=0';
		if (args != null && args.length)
		{	// add parameters
			params = '&pcount=' + args.length 
			for (var i = 0; i < args.length; i++) 
			{
				var arg = args[i];
				if (arg != null)
					params += '&p' + i + '=' + _SOM__escapePlus(escape(arg));
			}
		}
		url += params;
	}
	return url;
}

function _SOM__extractValue(s,name)
{
	var chStart = s.indexOf(name + '=');
	if (chStart != -1)
	{
		chStart += name.length + 1;
		var chEnd = s.indexOf('&',chStart);
		if (chEnd == -1)
			chEnd = s.length + 1;
		return unescape(s.substring(chStart,chEnd));
	}
	return null;
}

function _SOM__insertValue(sPtr,name,value)
{
	var s = sPtr.value;
	var chStart = s.indexOf(name + '=');
	if (chStart == -1)
	{
		if (value != null)
		{
			if (s.length > 0)
				s += '&';
			s += name + '=' + value;
		}
	}
	else
	{
		chStart += name.length + 1;
		var chEnd = s.indexOf('&',chStart);
		if (chEnd == -1)
			chEnd = s.length + 1;
		if (value == null)
			s = s.substring(0,chStart-name.length-1) + s.substring(chEnd,s.length+1);
		else
			s = s.substring(0,chStart-1) + '=' + escape(value) + s.substring(chEnd,s.length+1);
	}
	sPtr.value = s;
	return true;
}

function __Function()
{	// required to support JavaScript 1.0  ( IE3 )
	var args = __Function.arguments;
	var nargs = args.length - 1;
	var params = '';
	for (var i=0; i < nargs; i++)
	{	// extract expected arguments
		if (i != 0) params += ',';
		params += args[i];
	}
	for (var i=0; i < 16; i++)
	{	// support upto 16 optional arguments
		if (i != 0 || nargs != 0) params += ',';
		params += '_tp' + i;
	}
	return eval('function t(' + params + ') {' + args[nargs] + '} t');
}
// define Function routine for JavaScript 1.0 ( IE3 )
if (typeof(Function) != 'function')
	Function = __Function;
