<SCRIPT RUNAT=SERVER LANGUAGE="JavaScript">
// ************************************************************************ 
// Microsoft Script Library 
// Visual InterDev 6.0 Label Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 	
function CreateLabel(strName,funcInit,objParent)
{	
	if (typeof(strName) == 'string' && strName != '')
	{
		var objLabel = new _Label(strName);
		eval(strName + ' = objLabel');
		objLabel._funcInit = funcInit;
		thisPage.advise(PAGE_ONINIT,strName + '._restoreState()');
		return objLabel;
	}
	return null;	
}

function _Label(strName)
{	
	if (typeof(_bLBLPrototypeCalled) == 'undefined')
		_LBL__Prototype();

	//public members
	this.id = strName;
	this.name = strName;
}

function _LBL__Prototype()
{	
	_bLBLPrototypeCalled = 1;
	
	//public members
	_Label.prototype.maintainState = true;
	
	//private members
	_Label.prototype._bVisible = true;
	_Label.prototype._objDataSource = null;
	_Label.prototype._strDataField = '';
	_Label.prototype._strCaption = '';
	_Label.prototype._dataFormatAs = 'text';
	
	//public methods
	_Label.prototype.isVisible = _LBL_isVisible;
	_Label.prototype.show = _LBL_show;
	_Label.prototype.hide = _LBL_hide;
	_Label.prototype.getCaption = _LBL_getCaption;
	_Label.prototype.setCaption = _LBL_setCaption;
	_Label.prototype.getDataFormatAs = _LBL_getDataFormatAs;
	_Label.prototype.setDataFormatAs = _LBL_setDataFormatAs;
	_Label.prototype.getDataSource = _LBL_getDataSource;
	_Label.prototype.setDataSource = _LBL_setDataSource;
	_Label.prototype.getDataField = _LBL_getDataField;
	_Label.prototype.setDataField = _LBL_setDataField;
	_Label.prototype.display = _LBL_display;

	//private methods
	_Label.prototype._fireEvent = _EM__fireEvent;
	_Label.prototype._preserveState = _LBL__preserveState;
	_Label.prototype._restoreState = _LBL__restoreState;
	_Label.prototype._hasState = _LBL__hasState;
	_Label.prototype._onrowenter = _LBL__onrowenter;

	//scope implementation in _LBL__Prototype function

	function _LBL_isVisible()
	{	return this._bVisible;	}

	function _LBL_show()
	{	this._bVisible = true;	}

	function _LBL_hide()
	{	this._bVisible = false;	}

	function _LBL_getCaption()
	{	return this._strCaption;	}

	function _LBL_setCaption(strCaption)
	{	this._strCaption = strCaption;	}

	function _LBL_getDataFormatAs()
	{	return this._dataFormatAs;	}

	function _LBL_setDataFormatAs(strValue)
	{
		var strFormat = String(strValue);
		strFormat = strFormat.toLowerCase();
		if (strFormat == 'text' || strFormat == 'html')
		{
			this._dataFormatAs = strFormat;
			return true;
		}
		return false;
	}

	function _LBL_getDataSource()
	{	return this._objDataSource;	}

	function _LBL_setDataSource(objDataSource)
	{
		if (typeof(objDataSource) != 'object')
			return false;
		if ((this._objDataSource != null) && (this._strDataField != ''))
			this._objDataSource.unadvise(this._rowEnterID);
		this._objDataSource = objDataSource;
		if ((this._objDataSource != null) && (this._strDataField != ''))
			this._rowEnterID = this._objDataSource.advise(RS_ONROWENTER, this.id + '._onrowenter(' + this._objDataSource.id + ', "' + this._strDataField + '");');
		return true;
	}
		
	function _LBL_getDataField()
	{	return this._strDataField;	}

	function _LBL_setDataField(strDataField)
	{	
		if ((this._objDataSource != null) && (this._strDataField != ''))
			this._objDataSource.unadvise(this._rowEnterID);
		this._strDataField = strDataField;
		if ((this._objDataSource != null) && (this._strDataField != ''))
			this._rowEnterID = this._objDataSource.advise(RS_ONROWENTER, this.id + '._onrowenter(' + this._objDataSource.id + ', "' + this._strDataField + '");');
		return true;
	}

	function _LBL_display()
	{
		if (this._bVisible)
		{
			var strHTML = this._strCaption;
 			if (this._dataFormatAs == 'text')
				strHTML = Server.HTMLEncode(strHTML);
			Response.write(strHTML);
		}
		return this._preserveState();
	}

	function _LBL__preserveState()
	{
		if (this.maintainState)
		{	// preserve state in hidden field
			var state = new Object;
			if (!this._bVisible)
				state._bVisible = false;
			if (this._dataFormatAs != 'text')
				state._dataFormatAs = this._dataFormatAs;
			state._strCaption = this._strCaption;
			return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _LBL__restoreState()
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

				if (state._dataFormatAs != null)
					this._dataFormatAs = state._dataFormatAs;
				else
					this._dataFormatAs = 'text';

				this._strCaption = state._strCaption;
				return true;
			}
		}
		return false;
	}

	function _LBL__hasState()
	{
		if (this.maintainState)
			return thisPage.isStatePersisted(this.name);
		return false;
	}
	
	// eventhandler for databinding
	function _LBL__onrowenter(objRecordset,strDataField)
	{
		this.setCaption(objRecordset.fields.getValue(strDataField) + '');
	}

}	// end of _LBL__Prototype function


</SCRIPT>