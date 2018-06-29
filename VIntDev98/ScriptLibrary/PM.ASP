<SCRIPT RUNAT=SERVER Language="JavaScript">
//*****************************************************************
// Microsoft Script Library
// Visual InterDev 6.0 Scripting Object Model framework for server.
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
//*****************************************************************

//*****************************************************************
// function StartPageProcessing()
//	This is the method which starts page processing.
//	The following sequence of events will occur :
//		1. create thisPage object ( programming model object )
//		2. invoke all constructors on the page  (  xxx_ctor functions )
//		3. fire 'oninit' events
//		4. fire 'onenter' events
//		5. dispatch to appropriate server method
//		6. return TRUE if processing is complete
//		   return FALSE to continue processing of page
//*****************************************************************
function StartPageProcessing()
{
	if (typeof(thisPage) != 'object')
		thisPage = new _SOMObject;

	if (typeof(RuntimeObject) != 'undefined')
	{
		var ctors = RuntimeObject('*_ctor');
		for (var name in ctors)
		{	// invoke all constructor functions  ( xxx_ctor functions )
			if (typeof(ctors[name]) == 'function')
				ctors[name]();
		}
	}
	else
	{	// display warning
		var strWarning = '<br><hr><b>WARNING: Web server "' + Request.ServerVariables('SERVER_NAME') + '" requires an updated JSCRIPT.DLL.';
		strWarning += '<br>The Visual InterDev 6.0 Scripting Object Model requires JSCRIPT.DLL Version 4.0.0.2405 or greater.';
		Response.write(strWarning);
	}

	thisPage._objEventManager.fireEvent(PAGE_ONINIT);
	thisPage._objEventManager.fireEvent(PAGE_ONENTER);

	thisPage.dispatch();
	
	if (!thisPage._bEndPageProcessing)
		thisPage._objEventManager.fireEvent(PAGE_ONSHOW);
		
	if (thisPage._bEndPageProcessing)
		EndPageProcessing();
	
	return thisPage._bEndPageProcessing;
}

//*****************************************************************
// function EndPageProcessing()
//	This is the method which ends page processing.
//	The following sequence of events will occur :
//		1. if execute method was invoked, do nothing
//		2. fire 'onexit' events
//		3. invoke all destructors on the page  (  xxx_dtor methods )
//		4. if redirect defined, invoke redirect, otherwise
//			1. persist state in hidden field
//
//*****************************************************************
function EndPageProcessing()
{
	if (typeof(thisPage) == 'object' && !thisPage._bExecuteMethodInvoked)
	{
		if (thisPage._objEventManager != null)
			thisPage._objEventManager.fireEvent(PAGE_ONEXIT);

		var dtors = RuntimeObject('*_dtor');
		for (var name in dtors)
		{	// invoke all destructor functions  ( xxx_dtor functions )
			if (typeof(dtors[name]) == 'function')
				dtors[name]();
		}
		
		if (thisPage._redirect != '')
			Response.redirect(thisPage._redirect);

		thisPage._generateHiddenFields();
	}
}

//*****************************************************************
// function _SOMObject()
//	Constructor function for the 'thisPage' object.
//	This object provides the programming model services for the
//	page and is instantiated as the 'thisPage' object.
//
//*****************************************************************
function _SOMObject()
{	
	//events
	PAGE_ONINIT = 'oninit';
	PAGE_ONENTER = 'onenter';
	PAGE_ONSHOW = 'onshow';
	PAGE_ONEXIT = 'onexit';

	//constants
	PAGE_NAVIGATE = 'navigate';
	PAGE_EXECUTE = 'execute';
	
	//public members
	this.createDE = _SOM_createDE;
	this.dispatch = _SOM_dispatch;
	this.invokeMethod = _SOM_invokeMethod;
	this.navigateURL = _SOM_navigateURL;
	this.registerMethod = _SOM_registerMethod;
	this.registerVTable = _SOM_registerVTable;
	this.setState = _SOM_setState;
	this.getState = _SOM_getState;
	this.persistState = _SOM_persistState;
	this.unpersistState = _SOM_unpersistState;
	this.isStatePersisted = _SOM_isStatePersisted;
	this.advise = _SOM_advise;
	this.unadvise = _SOM_unadvise;
	this.startPageContent = _SOM_startPageContent;
	this.endPageContent = _SOM_endPageContent;
	this.isDHTMLBrowser = _SOM_isDHTMLBrowser;
	this.firstEntered = (Request.Form('_method').Item == null);

	//private members
	this._dispatchGET = _SOM__dispatchGET;
	this._dispatchPOST = _SOM__dispatchPOST;
	this._buildURL = _SOM__buildURL;
	this._extractValue = _SOM__extractValue;
	this._generateHiddenFields = _SOM__generateHiddenFields;
	this._bDECreated = false;
	this._bPageContentStarted = false;
	this._bEndPageProcessing = false;
	this._bExecuteMethodInvoked = false;
	this._vtableNavigate = null;
	this._vtableExecute = null;
	this._objState = this.unpersistState('thisPage');
	if (this._objState == null)
		this._objState = new Object;
	this._objStateObjects = null;
	this._location = escape(Request.ServerVariables('SCRIPT_NAME'));
	this._redirect = '';
	this._objEventManager = CreateEventManager();
	this._objEventManager.adviseDefaultHandler('thisPage',PAGE_ONENTER);	
	this._objEventManager.adviseDefaultHandler('thisPage',PAGE_ONSHOW);	
	this._objEventManager.adviseDefaultHandler('thisPage',PAGE_ONEXIT);	
	this.registerMethod('_EM__onclientevent',_EM__onclientevent);

	//scope implementation to _SOMObject constructor function

	//*****************************************************************
	// function _SOM_createDE()
	//	Create the data environment object as DE
	//*****************************************************************
	function _SOM_createDE()
	{
		if (!this._bDECreated)
		{
			DE = Server.CreateObject('DERuntime.DERuntime');
			DE.Init(Application('DE'));
			this._bDECreated = true;
		}
	}

	//*****************************************************************
	// function _SOM_dispatch()
	//	This function dispatches to the method indicated by the URL.
	//*****************************************************************
	function _SOM_dispatch()
	{
		var bDispatched = false;
		var requestMethod = Request.ServerVariables('REQUEST_METHOD').Item;
		if (requestMethod == 'GET')
			bDispatched = this._dispatchGET();
		else if (requestMethod == 'POST')
			bDispatched = this._dispatchPOST();

		// output client-side thisPage when navigating within this ASP
		if (!this._bExecuteMethodInvoked && this._redirect == '')
			this.startPageContent(0);
		return bDispatched;
	}

	//*****************************************************************
	// function _SOM_invokeMethod(url,method,args)
	//	This function navigates to the method indicated.
	//*****************************************************************
	function _SOM_invokeMethod(url,method,args)
	{
		this.navigateURL(this._buildURL(url,method,args));
	}

	//*****************************************************************
	// function _SOM_navigateURL(url)
	//	This function navigates to the URL indicated.
	//*****************************************************************
	function _SOM_navigateURL(url)
	{	// optimize to dispatch when URL is this file
		this._redirect = url;
		EndPageProcessing();
	}

	//*****************************************************************
	// function _SOM_registerMethod(strName,fPtr,mType)
	//	Register the given name/function as an exported server method.
	//	The mType should indicate either PAGE_NAVIGATE or PAGE_EXECUTE
	//*****************************************************************
	function _SOM_registerMethod(strName,fPtr,mType)
	{	// default mType = PAGE_NAVIGATE if not provided
		if (typeof(strName) == 'string' && typeof(fPtr) == 'function')
		{
			if (mType == PAGE_EXECUTE)
			{
				if (this._vtableExecute == null)
					this._vtableExecute = new Object;
				this._vtableExecute[strName] = fPtr;
				return true;
			}
			if (mType == PAGE_NAVIGATE || mType == null)
			{
				if (this._vtableNavigate == null)
					this._vtableNavigate = new Object;
				this._vtableNavigate[strName] = fPtr;
				return true;
			}
		}
		return false;
	}

	//*****************************************************************
	// function _SOM_registerVTable(objVTable,mType)
	//	Register the given object of function ptrs as exported methods.
	//	The mType should indicate either PAGE_NAVIGATE or PAGE_EXECUTE
	//*****************************************************************
	function _SOM_registerVTable(objVTable,mType)
	{	// default mType = PAGE_NAVIGATE if not provided
		if (typeof(objVTable) == 'object')
		{
			if (mType == PAGE_EXECUTE)
			{
				if (this._vtableExecute == null)
					this._vtableExecute = new Object;
				for (var name in objVTable)
				{
					if (typeof(objVTable[name]) == 'function')
						this._vtableExecute[name] = objVTable[name];
				}
				return true;
			}
			if (mType == PAGE_NAVIGATE || mType == null)
			{
				if (this._vtableNavigate == null)
					this._vtableNavigate = new Object;
				for (var name in objVTable)
				{
					if (typeof(objVTable[name]) == 'function')
						this._vtableNavigate[name] = objVTable[name];
				}
				return true;
			}
		}
		return false;
	}

	//*****************************************************************
	// function _SOM_setState(strName,value)
	//	Stores the given value as state using the given name as a key.
	//*****************************************************************
	function _SOM_setState(strName,value)
	{
		if (this._objState == null)
			this._objState = new Object;
		this._objState[strName] = value;
	}

	//*****************************************************************
	// function _SOM_getState(strName)
	//	Retrieves the given value for the state of given name.
	//*****************************************************************
	function _SOM_getState(strName)
	{
		if (this._objState == null)
			return null;
			
		if (this._objState[strName] == null)
			return null;

		return this._objState[strName];
	}

	//*****************************************************************
	// function _SOM_persistState(strName,objState)
	//	Persists the given object as state using the given name as key.
	//	Current implementation will persist state in a hidden field at
	//	the end of the file via the _generateHiddenFields method.
	//*****************************************************************
	function _SOM_persistState(strName,objState)
	{
		if (typeof(strName) == 'string' && typeof(objState) == 'object' &&
			strName != '' && objState != null)
		{
			if (this._objStateObjects == null)
				this._objStateObjects = new Object;

			this._objStateObjects[strName] = objState;
			return true;
		}
		return false;
	}

	//*****************************************************************
	// function _SOM_unpersistState(strName)
	//	Retrieves a state object using the given name as a key.
	//	Current implementation retrieves state from a hidden field.
	//*****************************************************************
	function _SOM_unpersistState(strName)
	{
		if (typeof(strName) == 'string' && strName != '')
		{
			var strState = Request.Form('_' + strName + '_state').Item;
			if (strState != '' && strState != null)
			{
				var state = new Object;
				var nEq, nAmp, name, value;
				while (strState.length > 0)
				{
					nEq = strState.indexOf('=');
					nAmp = strState.indexOf('&');
					if (nAmp == -1)
						nAmp = strState.length + 1;
					name = strState.substring(0,nEq);
					value = strState.substring(nEq+1,nAmp);
					state[name] = unescape(value);
					strState = strState.substring(nAmp+1,strState.length);
				}
				return state;
			}
		}
		return null;
	}

	//*****************************************************************
	// function _SOM_isStatePersisted(strName)
	//	Returns TRUE if there exists a state for the given key name.
	//*****************************************************************
	function _SOM_isStatePersisted(strName)
	{
		if (typeof(strName) == 'string' && strName != '')
		{
			var strState = Request.Form('_' + strName + '_state').Item;
			if (strState != '' && strState != null)
				return true;
		}
		return false;
	}

	//*****************************************************************
	// function _SOM_advise()
	//	Allows others to advise for MSPM events.
	//*****************************************************************
	function _SOM_advise(strEvent,funcToCall,nPriority)
	{
		return this._objEventManager.advise(strEvent,funcToCall,nPriority);
	}

	//*****************************************************************
	// function _SOM_unadvise()
	//	Allows others to unadvise for MSPM events.
	//*****************************************************************
	function _SOM_unadvise(strEvent,nAdviseID)
	{	
		return this._objEventManager.unadvise(strEvent,nAdviseID);	
	}

	//*****************************************************************
	// function _SOM_startPageContent(bFormType)
	//	Output client-side MSPM and thisForm if requested.
	//						0	:	NO FORM
	//		'undefined' or	1	:	FORM method=POST
	//						2	:	FORM
	//*****************************************************************
	function _SOM_startPageContent(bFormType)
	{	
		if (!this._bPageContentStarted)
		{
			this._bPageContentStarted = true;
			var secondSlash;
			var webRoot = String(this._location);
			if ((secondSlash = webRoot.indexOf('/',1)) != -1)
				webRoot = webRoot.substring(0,secondSlash);
			else
				webRoot = '';
			var strHTML = '<' + 'SCRIPT LANGUAGE=JavaScript SRC="' + webRoot + '/_ScriptLibrary/pm.js"><' + '/SCRIPT>\n';
			strHTML += '<' + 'SCRIPT LANGUAGE=JavaScript>thisPage._location = "' + this._location + '";<' + '/SCRIPT>\n';

			if (typeof(bFormType) == 'undefined' || bFormType == 1)
				strHTML += '<FORM name=thisForm method=POST>' + '\n';
				//strHTML += '<FORM name=thisForm method=POST language=JAVASCRIPT onsubmit="return thisPage.formIsValid;">' + '\n';
			else if (bFormType == 2)
				strHTML += '<FORM name=thisForm language=JAVASCRIPT onsubmit="return false;">' + '\n';
			Response.Write(strHTML);	
		}
	}
	
	//*****************************************************************
	// function _SOM_endPageContent()
	//	Provided for user methods to end page processing and
	//	cancel processing of the 'show' part of the page.
	//*****************************************************************
	function _SOM_endPageContent()
	{	this._bEndPageProcessing = true;	}

	//*****************************************************************
	// function _SOM_isDHTMLBrowser()
	//	Return TRUE for those browsers which support DHTML.
	//	Currently this method looks for MSIE 4.x and 5.x.
	//*****************************************************************
	function _SOM_isDHTMLBrowser()
	{	
		if (typeof(this._isDHTMLBrowser) == 'undefined')
		{
			this._isDHTMLBrowser = false;
			var userAgent = String(Request.ServerVariables('HTTP_USER_AGENT'));
			if (userAgent.indexOf('MSIE 4.') != -1 ||
				userAgent.indexOf('MSIE 5.') != -1)
				this._isDHTMLBrowser = true;
		}
		return this._isDHTMLBrowser;
	}

	//*****************************************************************
	// function _SOM__dispatchGET()
	//	Dispatches methods invoked via an HTTP GET request.
	//*****************************************************************
	function _SOM__dispatchGET()
	{
		var methodname = Request.QueryString('_method').Item;

		var methodtype = Request.QueryString('_mtype').Item;
		if (methodtype == PAGE_EXECUTE)
		{
			// return if methodname is not provided
			if (methodname == '' || methodname == null)
				return false;
			if (this._vtableExecute != null && typeof(RSDispatch) == 'function')
			{
				this._bExecuteMethodInvoked = RSDispatch(this._vtableExecute,methodname);
				this._bEndPageProcessing = this._bExecuteMethodInvoked;
				return this._bExecuteMethodInvoked;
			}
			return false;
		}
		// default to show if methodname is not provided
		if (methodname == '' || methodname == null)
			methodname = 'show';
		// default to navigate if methodtype is not provided	
		if (methodtype != '' && methodtype != null && methodtype != PAGE_NAVIGATE)
			return false;
		if (this._vtableNavigate != null && typeof(this._vtableNavigate[methodname]) == 'function')
		{	// validate that function is part of this._vtableNavigate description
			// extract parameters and dispatch to this._vtableNavigate.methodname
			var params = '';
			var pcount = Request.QueryString('pcount').Item;
			if (pcount == '' || pcount == null) pcount = 0;
			// extract parameter values
			for (var i=0; i < pcount; i++)
			{
				var param = Request.QueryString('p'+i).Item;
				if (param == null)
					params += 'null';
				else
					params += 'Request.QueryString("p' + i + '").Item';
				if (i < pcount-1)
					params += ',';
			}
			var dispatch = 'this._vtableNavigate.' + methodname + '(' + params + ')';
			// validated against this._vtableNavigate, safe to eval on the server
			eval(dispatch);
			return true;
		}
		if (methodname == 'show')
			return true;
		return false;
	}

	//*****************************************************************
	// function _SOM__dispatchPOST()
	//	Dispatches methods invoked via an HTTP POST request.
	//*****************************************************************
	function _SOM__dispatchPOST()
	{
		var queryString = Request.Form('_method').Item;
		if (queryString == '' || queryString == null)
			return false;

		if (queryString.indexOf(this._location) != 0)
		{	// redirect to another page
			this._redirect = queryString;
			this._bEndPageProcessing = true;
			return true;
		}

		queryString += '&';
		var methodname = this._extractValue(queryString,'?_method');
		if (methodname == null)
			methodname = 'show';
		if (this._vtableNavigate != null && typeof(this._vtableNavigate[methodname]) == 'function')
		{	// validate that function is part of this._vtableNavigate description
			// extract parameters and dispatch to thisPage.Navigate.methodname
			var params = '';
			var pcount = this._extractValue(queryString,'pcount');
			if (pcount == null) pcount = 0;
			// extract parameter values
			for (var i=0; i < pcount; i++)
			{
				var param = this._extractValue(queryString,'p'+i);
				if (param == null)
					params += 'null';
				else
					params += 'unescape(this._extractValue(queryString,"p' + i + '"))';
				if (i < pcount-1)
					params += ',';
			}

			var dispatch = 'this._vtableNavigate.' + methodname + '(' + params + ')';
			// validated against this._vtableNavigate, safe to eval on the server
			eval(dispatch);
			return true;
		}
		if (methodname == 'show')
			return true;
		return false;
	}

	//*****************************************************************
	// function _SOM__buildURL(url,method,args)
	//	PRIVATE HELPER FUNCTION
	//	Create URL which represents call to given method.
	//*****************************************************************
	function _SOM__buildURL(url,method,args)
	{
		if (typeof(method) == 'string')
		{
			url += '?_method=' + method
			var params = '&pcount=0';
			if (typeof(args) != 'undefined' && args.length)
			{	// add parameters
				params = '&pcount=' + args.length 
				for (var i = 0; i < args.length; i++) 
				{
					var arg = args[i];
					params += '&p' + i + '=' + escape(arg);
				}
			}
			url += params;
		}
		return url;
	}

	//*****************************************************************
	// function _SOM__extractValue(s,name)
	//	PRIVATE HELPER FUNCTION
	//	Extracts the value of a name/value pair from the given string,
	//	if it conforms to the following format :  'name=value&'
	//*****************************************************************
	function _SOM__extractValue(s,name)
	{
		var chStart = s.indexOf(name + '=');
		if (chStart != -1)
		{
			chStart += name.length + 1;
			var chEnd = s.indexOf('&',chStart);
			if (chEnd == -1)
				chEnd = s.length + 1;
			return s.substring(chStart,chEnd);
		}
		return null;
	}

	//*****************************************************************
	// function _SOM__generateHiddenFields()
	//	PRIVATE HELPER FUNCTION
	//	Generates hidden fields for _method and all state that needs
	//	to be round-tripped to/from the client.
	//*****************************************************************
	function _SOM__generateHiddenFields()
	{
		var strHTML = '\n<' + 'INPUT type=hidden name="_method">\n';			
		this.persistState('thisPage',this._objState);
		if (this._objStateObjects != null)
		{	// output a hidden field for each state object
			for (var hiddenName in this._objStateObjects)
			{
				var objState = this._objStateObjects[hiddenName];
				var strState = '';
				for (var name in objState)
				{
					if (objState[name] != null)
					{
						if (strState != '') strState += '&';
						strState += name + '=' + escape(objState[name]);
					}
				}
				strHTML += '<' + 'INPUT type=hidden name="_' + hiddenName + '_state" value="' + strState + '">\n';
			}
		}
		Response.Write(strHTML);
	}

}	// end of _SOM_Object constructor function

// shared functions for handling databinding
function _SOM_getDataSource()
{	return this._objDataSource;		}

function _SOM_setDataSource(objDataSource)
{
	if (typeof(objDataSource) != 'object')
		return false;
		
	if ((this._objDataSource != null) && (this._strDataField != ''))
	{
		this._objDataSource.unadvise(this._rowEnterID);
		this._objDataSource.unadvise(this._beforeUpdateID);
	}					
	this._objDataSource = objDataSource;
	if ((this._objDataSource != null) && (this._strDataField != ''))
	{	
		this._rowEnterID = this._objDataSource.advise(RS_ONROWENTER, this.id + '._onrowenter(' + this._objDataSource.id + ', "' + this._strDataField + '");');
		this._beforeUpdateID = this._objDataSource.advise(RS_ONBEFOREUPDATE, this.id + '._onbeforeupdate(' + this._objDataSource.id + ', "' + this._strDataField + '");');
	}
	return true;
}
		
function _SOM_getDataField()
{	return this._strDataField;		}

function _SOM_setDataField(strDataField)
{	
	if ((this._objDataSource != null) && (this._strDataField != ''))
	{
		this._objDataSource.unadvise(this._rowEnterID);
		this._objDataSource.unadvise(this._beforeUpdateID);
	}
	this._strDataField = strDataField;
	if ((this._objDataSource != null) && (this._strDataField != ''))
	{
		this._rowEnterID = this._objDataSource.advise(RS_ONROWENTER, this.id + '._onrowenter(' + this._objDataSource.id + ', "' + this._strDataField + '");');
		this._beforeUpdateID = this._objDataSource.advise(RS_ONBEFOREUPDATE, this.id + '._onbeforeupdate(' + this._objDataSource.id + ', "' + this._strDataField + '");');
	}
	return true;
}

// ************************************************************************ 
// Microsoft Scripting Libary 
// Visual InterDev 6.0 EventManager Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 
function CreateEventManager()
{
	return new _EventManager();
}

function _EventManager()
{
	if (typeof(_bEMPrototypeCalled) == 'undefined')
		_EM_Prototype();
	this._events = new Array;
	this._nLastAdviseID = 0;
}

function _EM_Prototype()
{	// public
	_EventManager.prototype.advise = _EM_advise;
	_EventManager.prototype.adviseDefaultHandler = _EM_adviseDefaultHandler;
	_EventManager.prototype.unadvise = _EM_unadvise;
	_EventManager.prototype.fireEvent = _EM_fireEvent;
	_EventManager.prototype.getEventCount = _EM_getEventCount;
	_EventManager.prototype.getEvent = _EM_getEvent;
	_EventManager.prototype.generateClientHandler = _EM_generateClientHandler;
	_EventManager.prototype.generateClientHandlers = _EM_generateClientHandlers;

	_bEMPrototypeCalled = 1;
	
	//scope implementation to _EM_Prototype function

	function _EM_Advisor()
	{
		this.funcToCall = 0;
		this.nAdviseID = 0;
		this.nPriority = 0;
	}

	function _EM_advise(strEvent,funcToCall,nPriority)
	{
		if ((typeof(strEvent) == 'string') && (strEvent != '') && 
			(typeof(funcToCall) != 'undefined'))
		{
			if (typeof(this._events[strEvent]) == 'undefined')
				this._events[strEvent] = new Array;

			var adviseList = this._events[strEvent];
			var objAdvisor = new _EM_Advisor;
			var nLen = adviseList.length;
			objAdvisor.funcToCall = funcToCall;
			objAdvisor.nAdviseID = ++this._nLastAdviseID;
			
			// maintain list sorted in descending priority
			if (typeof(nPriority) == 'number')
				objAdvisor.nPriority = nPriority;
				
			if (nLen == 0 || objAdvisor.nPriority <= adviseList[nLen-1].nPriority)
			{	// same or lower priority as last item, append to end
				adviseList[nLen] = objAdvisor;
			}
			else
			{	// higher priority than last item, insert in priority order
				for (var i=0; i < nLen; i++)
				{
					if (objAdvisor.nPriority > adviseList[i].nPriority)
						break;
				}
				// insert item at priority ordered index
				var aTemp = adviseList.slice(i);
				adviseList.length = i;
				adviseList[i] = objAdvisor;
				this._events[strEvent] = adviseList.concat(aTemp);
			}
	
			return this._nLastAdviseID;
		}
		return -1;
	}

	function _EM_adviseDefaultHandler(strName,strEvent)
	{
		var strDefaultHandler = strName + '_' + strEvent;
		if (eval('typeof(' + strDefaultHandler + ')') == 'function' ||
			eval('typeof(' + strDefaultHandler + ')') == 'unknown')
			return this.advise(strEvent,strDefaultHandler + '()');
		return -1;
	}

	function _EM_unadvise(strEvent,nAdviseID)
	{
		if ((typeof(strEvent) != 'undefined') && 
			(nAdviseID != 'undefined') && 
			(typeof(this._events[strEvent]) != 'undefined'))
		{
			var adviseList = this._events[strEvent];
			var nLen = adviseList.length
			for (var i=0; i < nLen; i++)
			{
				if (adviseList[i].nAdviseID == nAdviseID)
				{
					var aTemp = adviseList.slice(i+1);
					adviseList.length = i;
					this._events[strEvent] = adviseList.concat(aTemp);
					return true;
				}
			}
		}
		return false;
	}

	function _EM_fireEvent(strEvent)
	{
		if (typeof(this._events[strEvent]) != 'undefined')
		{
			var bReturn = true;
			for (var i=0; i < this._events[strEvent].length; i++)
				_EM__fireEvent(this._events[strEvent][i].funcToCall);
		}
	}

	function _EM_getEventCount()
	{
			var nLength = 0;
			for (var event in this._events)
				nLength++;
			return nLength;
	}

	function _EM_getEvent(nIndex)
	{
		var i = 0;
		for (var event in this._events)
		{
			if (nIndex == i)
				return event;
		}
		return '';
	}

	function _EM_generateClientHandler(strName,strEvent)
	{
		return "thisPage._fireEvent('" + strName + "','" + strEvent + "');";
	}

	function _EM_generateClientHandlers(strName)
	{
		var strHandlers = '';
		for (var event in this._events)
			strHandlers += ' ' + event + '="' + this.generateClientHandler(strName,event) + ' return false;"';
		return strHandlers;
	}

}	// end of _EM_Prototype function

// shared function for firing events, used by other script components
function _EM__fireEvent(funcToFire)
{
	if (typeof(funcToFire) == 'string')
		eval(funcToFire);
	else if (typeof(funcToFire) == 'function')
		funcToFire();
}

// shared function for mapping client events to server eventhandlers
function _EM__onclientevent(strName,strEvent)
{	// validate strName is an existing object
	var object = null;
	if (typeof(RuntimeObject) != 'undefined')
	{	// use global namespace to acquire object of 'strName'
		var objects = RuntimeObject(strName);
		object = objects[strName];
	}

	if (object != null && typeof(object) == 'object' && typeof(object._objEventManager) == 'object')
		object._objEventManager.fireEvent(strEvent);
}

</SCRIPT>
