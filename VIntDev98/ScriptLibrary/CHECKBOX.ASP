<SCRIPT RUNAT=SERVER LANGUAGE="JavaScript">
// ************************************************************************ 
// Microsoft Script Library 
// Visual InterDev 6.0 Checkbox Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 	
function CreateCheckbox(strName,funcInit,objParent)
{	
	if (typeof(strName) == 'string' && strName != '')
	{
		var objCheckbox = new _Checkbox(strName);
		eval(strName + " = objCheckbox");
		objCheckbox._funcInit = funcInit;
		thisPage.advise(PAGE_ONINIT,strName + '._restoreState()');				
		return objCheckbox;
	}
	return null;	
}

function _Checkbox(strName)
{	
	if (typeof(_bCHKPrototypeCalled) == 'undefined')
		_CHK__Prototype();

	// public members
	this.id = strName;
	this.name = strName;
	this.value = strName;
		
	// private members
	this._objEventManager = CreateEventManager();
	this._objEventManager.adviseDefaultHandler(this.name,CHK_ONCLICK);
}

function _CHK__Prototype()
{	
	_bCHKPrototypeCalled = 1;
	
	//events
	CHK_ONCLICK = 'onclick';
	
	//public members
	_Checkbox.prototype.disabled = false;
	_Checkbox.prototype.maintainState = true;
	
	// private members
	_Checkbox.prototype._bVisible = true;
	_Checkbox.prototype._strCaption = '';
	_Checkbox.prototype._checked = false;
	_Checkbox.prototype._objDataSource = null;
	_Checkbox.prototype._strDataField = '';	
	
	//public methods
	_Checkbox.prototype.isVisible = _CHK_isVisible;
	_Checkbox.prototype.show = _CHK_show;
	_Checkbox.prototype.hide = _CHK_hide;
	_Checkbox.prototype.getChecked = _CHK_getChecked;
	_Checkbox.prototype.setChecked = _CHK_setChecked;
	_Checkbox.prototype.getCaption = _CHK_getCaption;
	_Checkbox.prototype.setCaption = _CHK_setCaption;
	_Checkbox.prototype.getDataSource = _SOM_getDataSource;
	_Checkbox.prototype.setDataSource = _SOM_setDataSource;
	_Checkbox.prototype.getDataField = _SOM_getDataField;
	_Checkbox.prototype.setDataField = _SOM_setDataField;
	_Checkbox.prototype.advise = _CHK_advise;
	_Checkbox.prototype.unadvise = _CHK_unadvise;
	_Checkbox.prototype.display = _CHK_display;


	//private methods
	_Checkbox.prototype._fireEvent = _EM__fireEvent;
	_Checkbox.prototype._preserveState = _CHK__preserveState;
	_Checkbox.prototype._restoreState = _CHK__restoreState;
	_Checkbox.prototype._hasState = _CHK__hasState;
	_Checkbox.prototype._onrowenter = _CHK__onrowenter;
	_Checkbox.prototype._onbeforeupdate = _CHK__onbeforeupdate;
	
	//scope implementation in _CHK__Prototype function

	function _CHK_isVisible()
	{	return this._bVisible;	}

	function _CHK_show()
	{	this._bVisible = true;	}

	function _CHK_hide()
	{	this._bVisible = false;	}

	function _CHK_getCaption()
	{	return this._strCaption;	}

	function _CHK_setCaption(strCaption)
	{	this._strCaption = strCaption;	}

	function _CHK_getChecked()
	{	return this._checked;	}

	function _CHK_setChecked(value)
	{	this._checked = (value != 'false' && value != '0' && value != '');	}

	function _CHK_advise(strEvent,funcToCall)
	{	return this._objEventManager.advise(strEvent,funcToCall);	}

	function _CHK_unadvise(strEvent,nAdviseID)
	{	return this._objEventManager.unadvise(strEvent,nAdviseID);	}

	function _CHK_display()
	{
		if (this._bVisible)
		{
			var strHMTL;
			if (this.disabled && !thisPage.isDHTMLBrowser())
			{	// mimic disabled for non-DHTML browsers
				strHTML = '<FONT face=arial color=#808080><b>[';
				if (this._checked)
					strHTML += '<FONT size=2>X</FONT>';
				else
					strHTML += '&nbsp;&nbsp;';
				strHTML += ']</b></FONT>&nbsp;' + this._strCaption + '&nbsp;';
			}
			else
			{
				strHTML = '<' + 'INPUT id="' + this.name + '" name="' + this.name + '" value="' + this.value + '" type="checkbox"';
				if (this._checked)
					strHTML += ' checked';
				if (!this.disabled)
				{	// output client events to callback to server	
					var strHandler = this._objEventManager.generateClientHandlers(this.name);	
					strHTML += strHandler;
				}
				else
					strHTML += ' disabled=true';

				strHTML += '><' + 'LABEL for="' + this.name + '" htmlfor="' + this.id + '"';
				if (this.disabled)	
					strHTML += ' disabled=true';
				strHTML += '>' + this._strCaption + '</LABEL>\n';
			}
			Response.write(strHTML);
		}
		return this._preserveState();
	}

	
	function _CHK__preserveState()
	{
		if (this.maintainState)
		{	// preserve state in hidden field
			var state = new Object;
			if (!this._bVisible)
				state._bVisible = false;
			if (this.disabled)
				state.disabled = true;
			if (this._checked)
				state._checked = true;
			state._strCaption = this._strCaption;
			if (this.value != this.name)
				state.value = this.value;
			return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _CHK__restoreState()
	{
		var r = false;
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

				if (state.disabled != null) 
					this.disabled = (state.disabled != '0' && state.disabled != 'false');
				else
					this.disabled = false;

				if (state._checked != null) 				
					this._checked = (state._checked != '0' && state._checked != 'false');
				else
					this._checked = false;

				if (state._strCaption != null)
					this._strCaption = state._strCaption;
				else
					this._strCaption = ''

				if (state.value != null)
					this.value = state.value;
				else
					this.value = this.name;

				r = true;
			}
		}

		if (this._bVisible)
		{	// reflect current state returned in thisForm
			var newValue = Request.Form(this.name) + '';
			if (newValue != 'undefined' && newValue == this.value)
				this.setChecked(true);
			else if (this._hasState())
				this.setChecked(false);
		}
		return r;
	}

	function _CHK__hasState()
	{
		if (this.maintainState)
			return thisPage.isStatePersisted(this.name);
		return false;
	}

	// eventhandler for databinding
	function _CHK__onrowenter(objRecordset,strDataField)
	{
		this.setChecked(objRecordset.fields.getValue(strDataField));
	}

	// eventhandler for databinding
	function _CHK__onbeforeupdate(objRecordset,strDataField)
	{
		var chkValue = this.getChecked();
		if (typeof(chkValue) != 'undefined' && chkValue != null)
			objRecordset.fields.setValue(strDataField,chkValue);
	}

}	// end of _CHK__Prototype function

</SCRIPT>