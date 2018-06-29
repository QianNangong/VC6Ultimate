<SCRIPT RUNAT=SERVER LANGUAGE="JavaScript">
// ************************************************************************ 
// Microsoft Script Library 
// Visual InterDev 6.0 Button Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 	
function CreateButton(strName,funcInit,objParent)
{
	if (typeof(strName) == 'string' && strName != '')
	{		
		var objButton = new _Button(strName);
		eval(strName + ' = objButton;');
		objButton._funcInit = funcInit;
		thisPage.advise(PAGE_ONINIT,strName + '._restoreState()');
		return objButton;
	}
	return null;
}

function _Button(strName)
{
	if (typeof(_bBTNPrototypeCalled) == 'undefined')
		_BTN__Prototype();

	// public members
	this.id = strName;
	this.name = strName;
	
	// advise for default eventhandlers
	this._objEventManager = CreateEventManager();
	this._objEventManager.adviseDefaultHandler(this.name,BTN_ONCLICK);
}	

function _BTN__Prototype()
{
	_bBTNPrototypeCalled = 1;
	// events
	BTN_ONCLICK = 'onclick';
	// constants
	BTN_TEXT = 0;
	BTN_IMAGE = 1;
	
	// public members
	_Button.prototype.disabled = false;
	_Button.prototype.value = '';
	_Button.prototype.src = '';
	_Button.prototype.alt = '';
	_Button.prototype.maintainState = true;

	// private members
	_Button.prototype._nStyle = BTN_TEXT;
	_Button.prototype._bVisible = true;

	// public methods
	_Button.prototype.isVisible = _BTN_isVisible;
	_Button.prototype.show = _BTN_show;
	_Button.prototype.hide = _BTN_hide;
	_Button.prototype.setStyle = _BTN_setStyle;
	_Button.prototype.getStyle = _BTN_getStyle;
	_Button.prototype.advise = _BTN_advise;
	_Button.prototype.unadvise = _BTN_unadvise;
	_Button.prototype.display = _BTN_display;

	// private methods
	_Button.prototype._fireEvent = _EM__fireEvent;
	_Button.prototype._preserveState = _BTN__preserveState;
	_Button.prototype._restoreState = _BTN__restoreState;
	_Button.prototype._hasState = _BTN__hasState;
	
	//scope implementation to _BTN__Prototype function

	function _BTN_isVisible()
	{	return this._bVisible;	}

	function _BTN_show()
	{	this._bVisible = true;	}

	function _BTN_hide()
	{	this._bVisible = false;	}

	function _BTN_setStyle(nStyle)
	{	this._nStyle = nStyle;	}
	
	function _BTN_getStyle(nStyle)
	{	return this._nStyle;	}
	
	function _BTN_advise(strEvent, funcToCall)
	{	return this._objEventManager.advise(strEvent, funcToCall);	}

	function _BTN_unadvise(strEvent, nAdviseID)
	{	return this._objEventManager.unadvise(strEvent,nAdviseID);	}

	function _BTN_display()
	{
		if (this._bVisible)
		{
			var strHTML;
			if (this.disabled && this._nStyle == BTN_TEXT && !thisPage.isDHTMLBrowser())
			{	// mimic disabled button on non-DHTML browsers
				strHTML = '<TABLE border=2 bgcolor=#C0C0C0 bordercolor=#808080 bordercolordark=#696969 bordercolorlight=#C0C0C0 cellpadding=0 cellspacing=0>'
				strHTML += '<TR><TD><FONT color=#696969>&nbsp;&nbsp;' + this.value + '&nbsp;&nbsp;</FONT></TD></TR></TABLE>';
			}
			else if (this._nStyle == BTN_IMAGE && !thisPage.isDHTMLBrowser())
			{	// use image tag on non-DHTML browsers
				strHTML = '<IMAGE border=0 name="' + this.name + '" id="' + this.id + '" src="' + this.src + '"';
				if (this.alt != '')
					strHTML += ' alt="' + this.alt + '"';
				strHTML += '>';
				if (!this.disabled && 
					this._objEventManager.getEventCount() && 
					this._objEventManager.getEvent(0) == BTN_ONCLICK)
				{	// use anchor tag to support onclick on non-DHTML browsers
					var strHandler = this._objEventManager.generateClientHandler(this.name,BTN_ONCLICK);
					strHTML = '<A href="javascript:' + strHandler + '">' + strHTML + '</A>';
				}
			}
			else
			{
				if (this._nStyle == BTN_IMAGE)
				{
					strHTML = '<' + 'INPUT border=0 type=image name="' + this.name + '" id="' + this.id + '" src="' + this.src + '"';
					if (this.alt != '')
						strHTML += ' alt="' + this.alt + '"';
				}
				else
					strHTML = '<' + 'INPUT type=button name="' + this.name + '" id="' + this.id + '" value="' + Server.HTMLEncode(this.value) + '"';
				
				if (!this.disabled)
				{	// output client events to callback to server
					var strHandler = this._objEventManager.generateClientHandlers(this.name);	
					strHTML += strHandler;
				}
				else
					strHTML += ' disabled';

				strHTML += '>\n';
			}
			Response.Write(strHTML);
		}
		this._preserveState();
	}
	
	function _BTN__preserveState()
	{
		if (this.maintainState)
		{
			var state = new Object;
			if (!this._bVisible)
				state._bVisible = false;
			if (this._nStyle != BTN_TEXT)
				state._nStyle = this._nStyle;
			if (this.disabled)
				state.disabled = true;
			state.value = this.value;
			if (this.src != '')
				state.src = this.src;
			if (this.alt != '')
				state.alt = this.alt;
			return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _BTN__restoreState()
	{
		this._fireEvent(this._funcInit);
		if (this.maintainState)
		{
			var state = thisPage.unpersistState(this.name);
			if (state != null)
			{
				if (state._bVisible != null) 
					this._bVisible = (state._bVisible != '0' && state._bVisible != 'false');
				else
					this._bVisible = true;

				if (state._nStyle != null)
					this._nStyle = Number(state._nStyle);
				else
					this._nStyle = BTN_TEXT;

				if (state.disabled != null) 
					this.disabled = (state.disabled != '0' && state.disabled != 'false');
				else
					this.disabled = false;

				if (state.value != null)
					this.value = state.value;
				else
					this.value = '';

				if (state.src != null)
					this.src = state.src;
				else
					this.src = '';
	
				if (state.alt != null)
					this.alt = state.alt;
				else
					this.alt = '';

				return true;
			}
		}
		return false;
	}

	function _BTN__hasState()
	{
		if (this.maintainState)
			return thisPage.isStatePersisted(this.name);
		return false;
	}

}	// end of _BTN__Prototype function
</SCRIPT>