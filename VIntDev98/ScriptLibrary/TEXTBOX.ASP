<SCRIPT RUNAT=SERVER LANGUAGE="JavaScript">
// ************************************************************************ 
// Microsoft Script Library 
// Visual InterDev 6.0 Textbox Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 	
function CreateTextbox(strName,funcInit,objParent)
{	
	if (typeof(strName) == 'string' && strName != '')
	{
		var objTextbox = new _Textbox(strName);
		eval(strName + " = objTextbox;");

		objTextbox._funcInit = funcInit;
		thisPage.advise(PAGE_ONINIT,strName + '._restoreState()');
		return objTextbox;
	}
	return null;	
}

function _Textbox(strName)
{	
	if (typeof(_bTXTPrototypeCalled) == 'undefined')
		_TXT__Prototype();

	// public members
	this.id = strName;
	this.name = strName;

	// private members
	this._objEventManager = CreateEventManager();
	this._objEventManager.adviseDefaultHandler(this.name,TXT_ONCHANGE);
}

function _TXT__Prototype()
{	
	_bTXTPrototypeCalled = 1;
	//events
	TXT_ONCHANGE = 'onchange';
	//constants
	TXT_TEXTBOX = 0;
	TXT_TEXTAREA = 1;
	TXT_PASSWORD = 2;
	
	// public members
	_Textbox.prototype.disabled = false;
	_Textbox.prototype.value = '';
	_Textbox.prototype.maintainState = true;
	
	// private members
	_Textbox.prototype._bVisible = true;
	_Textbox.prototype._objDataSource = null;
	_Textbox.prototype._strDataField = '';
	_Textbox.prototype._nRowCount = 1;
	_Textbox.prototype._nColumnCount = -1;
	_Textbox.prototype._nMaxLength = -1;
	_Textbox.prototype._nStyle = TXT_TEXTBOX;

	//public methods
	_Textbox.prototype.isVisible	= _TXT_isVisible;
	_Textbox.prototype.show = _TXT_show;
	_Textbox.prototype.hide = _TXT_hide;
	_Textbox.prototype.getColumnCount = _TXT_getColumnCount;
	_Textbox.prototype.setColumnCount = _TXT_setColumnCount;
	_Textbox.prototype.getRowCount = _TXT_getRowCount;
	_Textbox.prototype.setRowCount = _TXT_setRowCount;	
	_Textbox.prototype.getMaxLength = _TXT_getMaxLength;
	_Textbox.prototype.setMaxLength = _TXT_setMaxLength;
	_Textbox.prototype.getDataSource = _SOM_getDataSource;
	_Textbox.prototype.setDataSource = _SOM_setDataSource;
	_Textbox.prototype.getDataField = _SOM_getDataField;
	_Textbox.prototype.setDataField = _SOM_setDataField;
	_Textbox.prototype.getStyle = _TXT_getStyle;
	_Textbox.prototype.setStyle = _TXT_setStyle;
	_Textbox.prototype.advise = _TXT_advise;
	_Textbox.prototype.unadvise = _TXT_unadvise;
	_Textbox.prototype.display = _TXT_display;
	
	//private methods
	_Textbox.prototype._fireEvent = _EM__fireEvent;
	_Textbox.prototype._preserveState = _TXT__preserveState;
	_Textbox.prototype._restoreState = _TXT__restoreState;
	_Textbox.prototype._hasState = _TXT__hasState;
	_Textbox.prototype._onrowenter = _TXT__onrowenter;
	_Textbox.prototype._onbeforeupdate = _TXT__onbeforeupdate;
	
	//scope implementation in _TXT__Prototype function

	function _TXT_isVisible()
	{	return this._bVisible;	}

	function _TXT_show()
	{	this._bVisible = true;	}

	function _TXT_hide()
	{	this._bVisible = false;	}

	function  _TXT_getColumnCount()
	{	return this._nColumnCount;	}

	function  _TXT_setColumnCount(nColumnCount)
	{
		if (!isNaN(parseInt(nColumnCount)) && nColumnCount > 0)
		{
			this._nColumnCount = Number(nColumnCount);
			return true;
		}
		return false;
	}

	function  _TXT_getRowCount()
	{	return this._nRowCount;		}
	
	function  _TXT_setRowCount(nRowCount)
	{
		if (!isNaN(parseInt(nRowCount)) && nRowCount > 0)
		{
			this._nRowCount = Number(nRowCount);
			return true;
		}
		return false;				
	}

	function  _TXT_getMaxLength()
	{	return this._nMaxLength;	}

	function  _TXT_setMaxLength(nMaxLength)
	{
		if (!isNaN(parseInt(nMaxLength)) && nMaxLength > 0)
		{
			this._nMaxLength = Number(nMaxLength);
			return true;
		}
		return false;
	}

	function  _TXT_getStyle()
	{	return this._nStyle;	}

	function  _TXT_setStyle(nStyle)
	{
		if (!isNaN(parseInt(nStyle)) && nStyle >= TXT_TEXTBOX && nStyle <= TXT_PASSWORD)
		{
			this._nStyle = Number(nStyle);
			return true;
		}
		return false;
	}
	
	function _TXT_advise(strEvent,funcToCall)
	{	return this._objEventManager.advise(strEvent,funcToCall);	}

	function _TXT_unadvise(strEvent,nAdviseID)
	{	return this._objEventManager.unadvise(strEvent,nAdviseID);	}

	function _TXT_display()
	{
		if (this._bVisible)
		{
			var strHTML;
			if (this.disabled && !thisPage.isDHTMLBrowser())
			{	// mimic disabled textbox on non-DHTML browsers
				strHTML = '<TABLE border=1 bordercolor=#808080 bordercolordark=#696969 bordercolorlight=#C0C0C0 cellpadding=0 cellspacing=0><TR><TD>'
				strHTML += '<TABLE border=0><TR><TD width=' + this._nColumnCount * 8 + '><FONT color=#696969>&nbsp;' + this.value + '&nbsp;&nbsp;</FONT></TD></TR></TABLE>';
				strHTML += '</TD></TR></TABLE>\n';
			}
			else
			{
				if (this._nStyle == TXT_TEXTAREA)
				{
					strHTML = '<' + 'TEXTAREA id="' + this.id + '" name="' + this.name + '" rows=' + this._nRowCount;
					if (this._nColumnCount != -1)
						strHTML += ' cols=' + this._nColumnCount;
					if (!this.disabled)
					{	// output client events to callback to server
						var strHandler = this._objEventManager.generateClientHandlers(this.name);	
						strHTML += strHandler;
					}
					else
						strHTML += ' disabled';
					strHTML += '>' + this.value + '</TEXTAREA>\n';			
				}
				else 
				{
					if (this._nStyle == TXT_PASSWORD)
						strHTML = '<' + 'INPUT type="password" id="' 
					else
						strHTML = '<' + 'INPUT type="text" id="'	
					strHTML += this.id + '" name="' + this.name + '"';
					if (this._nColumnCount != -1)
						strHTML += ' size=' + this._nColumnCount;
					if (this._nMaxLength != -1)
						strHTML += ' maxLength=' + this._nMaxLength;
					if (this._nStyle != TXT_PASSWORD)						
						strHTML += ' value="' + Server.HTMLEncode(this.value) + '"';
					if (!this.disabled)
					{	// output client events to callback to server
						var strHandler = this._objEventManager.generateClientHandlers(this.name);	
						strHTML += strHandler;
					}
					else
						strHTML += ' disabled';
					strHTML += '>\n';
				}
			}
			Response.Write(strHTML);				 
		}
		this._preserveState();
	}

	function _TXT__preserveState()
	{
		if (this.maintainState)
		{	// preserve state in hidden field
			var state = new Object;
			state._bVisible = this._bVisible;
			if (this.disabled)
				state.disabled = true;
			if (this._nStyle != TXT_TEXTBOX)
				state._nStyle = this._nStyle;
			if (this._nRowCount != 1)
				state._nRowCount = this._nRowCount;
			if (this._nColumnCount != -1)	
				state._nColumnCount = this._nColumnCount;
			if (this._nMaxLength != -1)	
				state._nMaxLength = this._nMaxLength;
			if ((this.disabled || !this._bVisible) && this._nStyle != TXT_PASSWORD)
				state.value = this.value;
			return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _TXT__restoreState()
	{
		var r = false;
		this._fireEvent(this._funcInit);
		if (this.maintainState)
		{	// attempt to restore previous state
			var state = thisPage.unpersistState(this.name);
			if (state != null)
			{
				if (state._bVisible != null) 
					this._bVisible = (state._bVisible != '0' && state._bVisible != 'false');
				else
					this._bVisible = true;

				if (state.disabled != null) 
					this.disabled = (state.disabled != '0' && state.disabled != 'false');
				else
					this.disabled = false;

				if (state._nStyle != null) 
					this._nStyle = Number(state._nStyle);
				else
					this._nStyle = TXT_TEXTBOX;

				if (state._nRowCount != null) 
					this._nRowCount = Number(state._nRowCount);
				else
					this._nRowCount = 1;

				if (state._nColumnCount != null) 
					this._nColumnCount = Number(state._nColumnCount);
				else
					this._nColumnCount = -1;

				if (state._nMaxLength != null) 
					this._nMaxLength = Number(state._nMaxLength);
				else
					this._nMaxLength = -1;

				if (state.value != null)
					this.value = state.value;
				else
					this.value = '';

				r = true;
			}
		}
		var newValue = Request.Form(this.name) + '';
		if (newValue != 'undefined')
			this.value = newValue;
		return r;
	}

	function _TXT__hasState()
	{
		if (this.maintainState)
			return thisPage.isStatePersisted(this.name);
		return false;
	}
	
	// eventhandler for databinding
	function _TXT__onrowenter(objRecordset,strDataField)
	{
		this.value = objRecordset.fields.getValue(strDataField) + '';
	}

	// eventhandler for databinding
	function _TXT__onbeforeupdate(objRecordset,strDataField)
	{
		var txtValue = this.value;
		if (typeof(txtValue) != 'undefined')
			objRecordset.fields.setValue(strDataField,txtValue);
	}

}	// end of _TXT__Prototype function

</SCRIPT>