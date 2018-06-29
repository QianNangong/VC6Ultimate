<SCRIPT RUNAT=SERVER LANGUAGE="JavaScript">
// ************************************************************************ 
// Microsoft Script Library 
// Visual InterDev 6.0 Listbox Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 
function CreateListbox(strName,funcInit,objParent)
{	
	if (typeof(strName) == 'string' && strName != '')
	{
		var objListbox = new _Listbox(strName,objParent);
		eval(strName + ' = objListbox');

		objListbox._funcInit = funcInit;
		thisPage.advise(PAGE_ONINIT,strName + '._restoreState()');
		return objListbox;
	}
	return null;	
}

function _Listbox(strName,objParent)
{	
	if (typeof(_bLBPrototypeCalled) == 'undefined')
		_LB__Prototype();

	// public members
	this.id = strName;
	this.name = strName;

	// private members
	this._options = new Array;
	this._objEventManager = CreateEventManager();
	this._objEventManager.adviseDefaultHandler(this.name,LB_ONCHANGE);
}

function _LB__Prototype()
{	
	_bLBPrototypeCalled = 1;
	//events
	LB_ONCHANGE = 'onchange';
	
	// public members
	_Listbox.prototype.disabled = false;
	_Listbox.prototype.size = 1;
	_Listbox.prototype.selectedIndex = -1;
	_Listbox.prototype.maintainState = true;

	// private members
	_Listbox.prototype._bVisible = true;
	_Listbox.prototype._objRowSource = null;
	_Listbox.prototype._objDataSource = null;
	_Listbox.prototype._strDataField = '';
	
	//public methods
	_Listbox.prototype.isVisible	= _LB_isVisible;
	_Listbox.prototype.show = _LB_show;
	_Listbox.prototype.hide = _LB_hide;
	_Listbox.prototype.addItem = _LB_addItem;
	_Listbox.prototype.removeItem = _LB_removeItem;
	_Listbox.prototype.clear = _LB_clear;
	_Listbox.prototype.getCount = _LB_getCount;
	_Listbox.prototype.getValue = _LB_getValue;
	_Listbox.prototype.setValue = _LB_setValue;
	_Listbox.prototype.getText = _LB_getText;
	_Listbox.prototype.setText = _LB_setText;
	_Listbox.prototype.selectByValue = _LB_selectByValue;
	_Listbox.prototype.selectByText = _LB_selectByText;
	_Listbox.prototype.getRowSource = _LB_getRowSource;
	_Listbox.prototype.setRowSource = _LB_setRowSource;
	_Listbox.prototype.getDataSource = _SOM_getDataSource;
	_Listbox.prototype.setDataSource = _SOM_setDataSource;
	_Listbox.prototype.getDataField = _SOM_getDataField;
	_Listbox.prototype.setDataField = _SOM_setDataField;
	_Listbox.prototype.advise = _LB_advise;
	_Listbox.prototype.unadvise = _LB_unadvise;
	_Listbox.prototype.display = _LB_display;


	//private methods
	_Listbox.prototype._fireEvent = _EM__fireEvent;
	_Listbox.prototype._preserveState = _LB__preserveState;
	_Listbox.prototype._restoreState = _LB__restoreState;
	_Listbox.prototype._hasState = _LB__hasState;
	_Listbox.prototype._onrowenter = _LB__onrowenter;
	_Listbox.prototype._onbeforeupdate = _LB__onbeforeupdate;
	
	//scope implementation in _LB__Prototype function

	function _LB__Option()
	{
		this.text = '';
		this.value = '';	
	}

	function _LB_isVisible()
	{	return this._bVisible;	}

	function _LB_show()
	{	this._bVisible = true;	}

	function _LB_hide()
	{	this._bVisible = false;	}

	function _LB_addItem(strText,value,nIndex)
	{	
		var nCount = this.getCount();
		if (typeof(value) == 'undefined')
			value = strText;
		if (typeof(nIndex) == 'undefined')
			nIndex = nCount;
		if (!isNaN(parseInt(nIndex)))
		{	// add item at given nIndex
			var opt = new _LB__Option;
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

	function _LB_removeItem(nIndex)
	{	
		if (typeof(nIndex) == 'undefined')
			nIndex = this.selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
		{	// remove item at nIndex
			var aTemp = this._options.slice(Number(nIndex)+1);
			this._options.length = nIndex;
			this._options = this._options.concat(aTemp);
			return true;
		}
		return false;
	}

	function _LB_clear()
	{
		this.selectedIndex = -1;
		this._options.length = 0;
	}

	function _LB_getCount()
	{	return this._options.length;	}

	function _LB_getValue(nIndex)
	{
		if (typeof(nIndex) == 'undefined')
			nIndex = this.selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
				return this._options[nIndex].value;
		return '';
	}
	
	function _LB_setValue(value,nIndex)
	{
		if (typeof(nIndex) == 'undefined')
			nIndex = this.selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
		{
			this._options[nIndex].value = value;
			return true;
		}
		return false;
	}

	function _LB_getText(nIndex)
	{
		if (typeof(nIndex) == 'undefined')
			nIndex = this.selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
			return this._options[nIndex].text;
		return '';
	}
	
	function _LB_setText(strText,nIndex)
	{
		if (typeof(nIndex) == 'undefined')
			nIndex = this.selectedIndex;
		if (!isNaN(parseInt(nIndex)) && nIndex >= 0 && nIndex < this.getCount())
		{
			this._options[nIndex].text = String(strText);
			return true;
		}
		return false;
	}

	function _LB_selectByText(strText)
	{	// check for match in _options array
		for (var i=0; i < this._options.length; i++)
		{
			if (String(strText) == this._options[i].text)
			{
				this.selectedIndex = i;
				return i;
			}
		}
		return -1;
	}

	function _LB_selectByValue(value)
	{	// check for match in _options array
		for (var i=0; i < this._options.length; i++)
		{
			if (value == this._options[i].value)
			{
				this.selectedIndex = i;
				return i;
			}
		}
		return -1;
	}

	function _LB_getRowSource()
	{	return this._objRowSource;	}

	function _LB_setRowSource(objRecordset,listField,boundField)
	{
		if (typeof(objRecordset) == 'object' && this.getCount() == 0)
		{
			var nPos = objRecordset.absolutePosition;
			this._objRowSource = objRecordset;
			objRecordset.moveFirst();
			while (!objRecordset.EOF)
			{
				this.addItem(objRecordset.fields.getValue(listField),objRecordset.fields.getValue(boundField));
				objRecordset.moveNext();
			}
			objRecordset.moveAbsolute(nPos);
			return true;
		}		
		return false;
	}

	function _LB_advise(strEvent,funcToCall)
	{	return this._objEventManager.advise(strEvent,funcToCall);	}

	function _LB_unadvise(strEvent,nAdviseID)
	{	return this._objEventManager.unadvise(strEvent,nAdviseID);	}

	function _LB_display()
	{	
		if (this._bVisible)
		{
			var strHTML;
			if (this.disabled && !thisPage.isDHTMLBrowser())
			{	// mimic disabled for non-DHTML browsers
				strHTML = '<TABLE border=1 bordercolor=#808080 bordercolordark=#696969 bordercolorlight=#C0C0C0 cellpadding=0 cellspacing=0><TR><TD>\n';
				strHTML += '<TABLE border=0 cellpadding=0 cellspacing=0>\n';
				if (this.size <= 1)
				{	// mimic a disabled dropdown
					strHTML += '<TR><TD width=30>';
					if (this.selectedIndex >= 0 && this.selectedIndex < this.getCount())
						strHTML += '<FONT color=#808080 size=2>&nbsp;' + Server.HTMLEncode(this.getText()) + '&nbsp;&nbsp;&nbsp;';
					strHTML += '</TD><TD width=15 bgcolor=#808080>&nbsp;</TD></TR>'
				}
				else
				{	// mimic a disabled list
					for (var i=0; i < this.size; i++)
					{
						strHTML += '<TR>';
						if (i < this._options.length)	
						{
							if (this.selectedIndex == i)
								strHTML += '<TD width=30 bgcolor=#696969><FONT color=#C0C0C0>&nbsp;' + Server.HTMLEncode(this._options[i].text) + '&nbsp;&nbsp;&nbsp;</TD>';
							else
								strHTML += '<TD width=30><FONT color=#696969>&nbsp;' + Server.HTMLEncode(this._options[i].text) + '&nbsp;&nbsp;&nbsp;</TD>';
						}
						else
							strHTML += '<TD width=30>&nbsp;</TD>';
						strHTML += '</TR>\n';
					}
				}	
				strHTML += '</TABLE></TD></TR></TABLE>\n';
			}
			else
			{
				strHTML = '<' + 'SELECT name=' + this.name + ' id=' + this.id + ' size=' + this.size;
				if (!this.disabled)
				{	// output client events to callback to server
					var strHandler = this._objEventManager.generateClientHandlers(this.name);	
					strHTML += strHandler;
				}
				else
					strHTML += ' disabled';
				strHTML += '>\n';		
				for (var i=0; i < this._options.length; i++)
				{	// generate _options
					strHTML += '<' + 'OPTION ';
					if (this.selectedIndex == i)
						strHTML += 'selected ';
					strHTML += 'value="' + Server.HTMLEncode(this._options[i].value) + '">' + Server.HTMLEncode(this._options[i].text) + '</OPTION>\n';
				}	
				strHTML += '</SELECT>\n';	
			}
			Response.write(strHTML);
		}
		this._preserveState();
	}

	function _LB__preserveState()
	{
		if (this.maintainState)
		{	// preserve state in hidden field
			var state = new Object;
			if (!this._bVisible)
				state._bVisible = false;
			if (this.disabled)
				state.disabled = true;
			if (this.size != 1)
				state.size = this.size;
			state.selectedIndex = this.selectedIndex;
			state._nCount = this._options.length;
			for (var i=0; i < this._options.length; i++)
			{
				state['t' + i] = this._options[i].text;
				if (this._options[i].value != this._options[i].text)
					state['v' + i] = this._options[i].value;
			}
			return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _LB__restoreState()
	{
		var r = false;
		this._fireEvent(this._funcInit);
		if (this.maintainState)
		{	// attempt to restore previous state
			var state = thisPage.unpersistState(this.name);
			if (state != null)
			{	// restore previous state
				this.clear();
				for (var i=0; i < state._nCount; i++)
					this.addItem(state['t' + i],state['v' + i]);
				if (state._bVisible != null)
					this._bVisible = (state._bVisible != '0' && state._bVisible != 'false');
				else
					this._bVisible = true;

				if (state.disabled != null)
					this.disabled = (state.disabled != '0' && state.disabled != 'false');
				else
					this.disabled = false;

				if (state.size != null)
					this.size = Number(state.size);
				else
					this.size = 1;

				this.selectedIndex = Number(state.selectedIndex);
				r = true;
			}
		}
		var newState = Request.Form(this.name) + '';
		if (newState != 'undefined')
			this.selectByValue(newState);
		return r;
	}

	function _LB__hasState()
	{
		if (this.maintainState)
			return thisPage.isStatePersisted(this.name);
		return false;
	}
	
	// eventhandler for databinding
	function _LB__onrowenter(objRecordset,dataField)
	{
		this.selectByValue(objRecordset.fields.getValue(dataField) + '');
		return true;
	}

	// eventhandler for databinding
	function _LB__onbeforeupdate(objRecordset,dataField)
	{
		objRecordset.fields.setValue(dataField,this.getValue());
		return true;
	}	

}	// end of _LB__Prototype function

</SCRIPT>
