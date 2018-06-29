<SCRIPT RUNAT=SERVER LANGUAGE="JavaScript">
// ************************************************************************ 
// Microsoft Script Library 
// Visual InterDev 6.0 OptionGroup Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 	
function CreateOptionGroup(strName,funcInit,objParent)
{	
	if (typeof(strName) == 'string' && strName != '')
	{
		var objOptionGroup = new _OptionGroup(strName);
		eval(strName + " = objOptionGroup;");
		objOptionGroup._funcInit = funcInit;
		thisPage.advise(PAGE_ONINIT,strName + '._restoreState()');
		return objOptionGroup;
	}	
	return null;
}

function _OptionGroup(strName)
{	
	if (typeof(_bOPTPrototypeCalled) == 'undefined')
		_OPT__Prototype();

	// public members
	this.id = strName;
	this.name = strName;

	// private members
	this._options = new Array();
	this._objEventManager = CreateEventManager();
	this._objEventManager.adviseDefaultHandler(this.name,OPT_ONCHANGE);
}

function _OPT__Prototype()
{	
	_bOPTPrototypeCalled = 1;
	
	//events
	OPT_ONCHANGE = 'onchange';

	// public members
	_OptionGroup.prototype.maintainState = true;
	
	// private members
	_OptionGroup.prototype._bVisible = true;
	_OptionGroup.prototype._nBorder = 0;
	_OptionGroup.prototype._nAlignment = 0;
	_OptionGroup.prototype._selectedIndex = 0;
	_OptionGroup.prototype._objDataSource = null;
	_OptionGroup.prototype._strDataField = '';
	
	//public methods
	_OptionGroup.prototype.isVisible = _OPT_isVisible;
	_OptionGroup.prototype.show = _OPT_show;
	_OptionGroup.prototype.hide = _OPT_hide;
	_OptionGroup.prototype.getBorder = _OPT_getBorder;
	_OptionGroup.prototype.setBorder = _OPT_setBorder;
	_OptionGroup.prototype.getAlignment = _OPT_getAlignment;
	_OptionGroup.prototype.setAlignment = _OPT_setAlignment;
	_OptionGroup.prototype.getButton = _OPT_getButton;
	_OptionGroup.prototype.addItem = _OPT_addItem;
	_OptionGroup.prototype.removeItem = _OPT_removeItem;
	_OptionGroup.prototype.clear = _OPT_clear;
	_OptionGroup.prototype.getCount = _OPT_getCount;
	_OptionGroup.prototype.getValue = _OPT_getValue;
	_OptionGroup.prototype.setValue = _OPT_setValue;
	_OptionGroup.prototype.getCaption = _OPT_getCaption;
	_OptionGroup.prototype.setCaption = _OPT_setCaption;
	_OptionGroup.prototype.selectByValue = _OPT_selectByValue;
	_OptionGroup.prototype.selectByCaption = _OPT_selectByCaption;
	_OptionGroup.prototype.selectByIndex = _OPT_selectByIndex;
	_OptionGroup.prototype.getSelectedIndex = _OPT_getSelectedIndex;
	_OptionGroup.prototype.getRowSource = _OPT_getRowSource
	_OptionGroup.prototype.setRowSource = _OPT_setRowSource
	_OptionGroup.prototype.getDataSource = _SOM_getDataSource;
	_OptionGroup.prototype.setDataSource = _SOM_setDataSource;
	_OptionGroup.prototype.getDataField = _SOM_getDataField;
	_OptionGroup.prototype.setDataField = _SOM_setDataField;
	_OptionGroup.prototype.advise = _OPT_advise;
	_OptionGroup.prototype.unadvise = _OPT_unadvise;
	_OptionGroup.prototype.display = _OPT_display;

	//private methods
	_OptionGroup.prototype._fireEvent = _EM__fireEvent;
	_OptionGroup.prototype._preserveState = _OPT__preserveState;
	_OptionGroup.prototype._restoreState = _OPT__restoreState;
	_OptionGroup.prototype._hasState = _OPT__hasState;
	_OptionGroup.prototype._onrowenter = _OPT__onrowenter;
	_OptionGroup.prototype._onbeforeupdate = _OPT__onbeforeupdate;
	
	//scope implementation in _OPT__Prototype function

	function _OPT__Option()
	{
		this.text = '';
		this.value = '';
		this.disabled = false;	
	}

	function _OPT_isVisible()
	{	return this._bVisible;	}

	function _OPT_show()
	{	this._bVisible = true;	}

	function _OPT_hide()
	{	this._bVisible = false;	}

	function  _OPT_getBorder()
	{	return this._nBorder;	}

	function  _OPT_setBorder(nBorder)
	{	this._nBorder = nBorder;	}

	function  _OPT_getAlignment()
	{	return this._nAlignment;	}

	function  _OPT_setAlignment(nAlignment)
	{
		if (typeof(nAlignment) != 'undefined')
		{
			if (nAlignment == '0')
				this._nAlignment = 0;
			else
				this._nAlignment = 1;
			return true;
		}
		return false;
	}

	function  _OPT_getButton(nIndex)
	{
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
			return this._options[Number(nIndex)];
	}

	function _OPT_addItem(strText,value,nIndex)
	{	
		var nCount = this.getCount();
		if (typeof(nIndex) == 'undefined')
			nIndex = nCount;
		if (typeof(value) == 'undefined')
			value = strText;
		if (!isNaN(parseInt(nIndex)))
		{	// add item at given nIndex
			var opt = new _OPT__Option;
			opt.text = String(strText);
			opt.value = value;
			if (nIndex < 0)		// add as first item in list
				nIndex = 0;
			if (nIndex < nCount)
			{	// insert item at given index
				var aTemp = this._options.slice(nIndex);
				this._options.length = nIndex;
				this._options[Number(nIndex)] = opt;
				this._options = this._options.concat(aTemp);
				return nIndex;
			}	
			else
			{	// add item to end of list
				this._options[nCount] = opt;
				return nCount;		
			}
		}
		return -1;	// failed to add item
	}

	function _OPT_removeItem(nIndex)
	{	
		if (typeof(nIndex) == 'undefined')
			nIndex = this._selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
		{	// remove item at nIndex
			var aTemp = this._options.slice(Number(nIndex)+1);
			this._options.length = nIndex;
			this._options = this._options.concat(aTemp);
			return true;
		}
		return false;
	}

	function _OPT_clear()
	{	this._options.length = 0;	}

	function _OPT_getCount()
	{	return this._options.length;	}

	function _OPT_getValue(nIndex)
	{
		if (typeof(nIndex) == 'undefined')
			nIndex = this._selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
			return this._options[nIndex].value;
		return '';
	}

	function _OPT_setValue(value, nIndex)
	{
		if (typeof(nIndex) == 'undefined')
			nIndex = this._selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
		{
			this._options[nIndex].value = value;
			return true;
		}
		return false;
	}

	function _OPT_getCaption(nIndex)
	{
		if (typeof(nIndex) == 'undefined')
			nIndex = this._selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
			return this._options[nIndex].text;
		return '';
	}

	function _OPT_setCaption(strCaption,nIndex)
	{
		if (typeof(nIndex) == 'undefined')
			nIndex = this._selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
		{
			this._options[nIndex].text = String(strCaption);
			return true;
		}
		return false;
	}

	function _OPT_selectByValue(value)
	{	// check for match in _options array
		for (var i=0; i < this._options.length; i++)
		{
			if (value == this._options[i].value)
			{
				this._selectedIndex = i;
				return i;
			}
		}

		return -1;
	}

	function _OPT_selectByCaption(strCaption)
	{	// check for match in _options array
		for (var i=0; i < this._options.length; i++)
		{
			if (String(strCaption) == this._options[i].text)
			{
				this._selectedIndex = i;
				return i;
			}
		}
		return -1;
	}

	function _OPT_selectByIndex(nIndex)
	{
		if (!isNaN(parseInt(nIndex)))
		{
			this._selectedIndex = nIndex;
			return nIndex;
		}
		return -1;
	}

	function _OPT_getSelectedIndex()
	{	return this._selectedIndex;		}

	function _OPT_getRowSource()
	{	return this._objRowSource;		}

	function _OPT_setRowSource(objRecordset,listField,boundField)
	{	
		if (typeof(objRecordset) == 'object' && this.getCount() == 0)
		{	
			var nPos = objRecordset.absolutePosition;
			objRecordset.moveFirst();
			while (!objRecordset.EOF)
			{
				objFields = objRecordset.fields;
				this.addItem(objFields.getValue(listField),objFields.getValue(boundField));
				objRecordset.moveNext();
			}
			objRecordset.moveAbsolute(nPos);
			return true;
		}	
		return false;
	}

	function _OPT_advise(strEvent,funcToCall)
	{	return this._objEventManager.advise(strEvent,funcToCall);	}

	function _OPT_unadvise(strEvent,nAdviseID)
	{	return this._objEventManager.unadvise(strEvent,nAdviseID);	}

	function _OPT_display()
	{
		if (this._bVisible)
		{
			var strHTML = '<' + 'TABLE border=' + this._nBorder + ' cellpadding=0 cellspacing=0><TR><TD>\n';
			strHTML += '<' + 'TABLE id="' + this.id + '" name="' + this.name + '" border=0 cellpadding=2 cellspacing=2>\n';

			if (this._nAlignment != '0')
				strHTML += '<TR>';

			for (var i = 0; i < this._options.length; i++)
			{
				if (this._nAlignment == '0')
					strHTML += '<TR>';
				strHTML += '<TD>';
				
				if (this._options[i].disabled && !thisPage.isDHTMLBrowser())
				{	// mimic disabled for non-DHTML browsers
					strHTML += '<FONT face=arial size=2>(';
					if (this._selectedIndex == i)
						strHTML += '<FONT face=arial size=3><b>o</b></FONT>';
					else
						strHTML += '&nbsp;&nbsp;';
					strHTML += ') </FONT>' + this._options[i].text + '&nbsp;&nbsp;\n';
				}
				else
				{		
					strHTML += '<' + 'INPUT name=' + this.id + ' value="' + Server.HTMLEncode(this._options[i].value) + '" type="radio" ';
					if (this._selectedIndex == i)
						strHTML += 'checked ';
					if (!this._options[i].disabled)
					{	// output client events to callback to server
						var strHandler = this._objEventManager.generateClientHandlers(this.name);	
						strHTML += strHandler;
					}
					else
						strHTML += ' disabled';
					strHTML += '><' + 'LABEL for="' + this.id + '" htmlfor="' + this.id;
					strHTML += '">' + this._options[i].text + '</LABEL>\n';
				}

				strHTML += '</TD>';
				if (this._nAlignment == '0')
					strHTML += '</TR>\n';
			}									

			if (this._nAlignment != '0')
				strHTML += '</TR>\n';
			strHTML += '</TABLE></TD></TR></TABLE>\n';
			Response.write(strHTML);
		}
		return this._preserveState();
	}

	function _OPT__preserveState()
	{
		if (this.maintainState)
		{	// preserve state in hidden field
			var state = new Object;
			if (this._nBorder != 0)
				state._nBorder = this._nBorder;
			if (!this._bVisible)
				state._bVisible = false;
			if (this._nAlignment != 0)
				state._nAlignment = this._nAlignment;
			state._selectedIndex = this._selectedIndex;
			state._nCount = this._options.length;
			for (var i=0; i < this._options.length; i++)
			{
				state['t' + i] = this._options[i].text;
				if (this._options[i].value != this._options[i].text)
					state['v' + i] = this._options[i].value;
				if (this._options[i].disabled)
					state['d' + i] = true;
			}

			return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _OPT__restoreState()
	{
		var r = false;
		this._fireEvent(this._funcInit);
		if (this.maintainState)
		{	// attempt to restore previous state
			var state = thisPage.unpersistState(this.name);
			if (state != null)
			{
				this.clear();
				for (var i=0; i < state._nCount; i++)
				{
					this.addItem(state['t' + i],state['v' + i]);
					if (state['d' + i] != null)
						this._options[i].disabled = (state['d' + i] != '0' && state['d' + i] != 'false');
				}
				this._selectedIndex = Number(state._selectedIndex);
				if (state._nBorder != null)
					this._nBorder = state._nBorder;
				else
					this._nBorder = 0;

				if (state._bVisible != null)
					this._bVisible = (state._bVisible != '0' && state._bVisible != 'false');
				else
					this._bVisible = true;

				if (state._nAlignment != null)
					this._nAlignment = Number(state._nAlignment);
				else
					this._nAlignment = 0;

				r = true;
			}
		}
		var newValue = Request.Form(this.name) + '';
		if (newValue != 'undefined')
			this.selectByValue(newValue);
		return r;
	}

	function _OPT__hasState()
	{
		if (this.maintainState)
			return thisPage.isStatePersisted(this.name);
		return false;
	}

	// eventhandler for databinding
	function _OPT__onrowenter(objRecordset,strDataField)
	{
		this.selectByValue(objRecordset.fields.getValue(strDataField) + '');
	}

	// eventhandler for databinding
	function _OPT__onbeforeupdate(objRecordset,strDataField)
	{
		var value = this.getValue();
		if (typeof(value) != 'undefined' && value != null)
			objRecordset.fields.setValue(strDataField, value);
	}

}	// end of _OPT__Prototype function

</SCRIPT>