<SCRIPT LANGUAGE="JavaScript" RUNAT=SERVER>
// ************************************************************************ 
// Microsoft Script Library 
// Visual InterDev 6.0 RecordsetNavbar Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 
function CreateRecordsetNavbar(strName,funcInit,objParent)
{
	if (typeof(strName) == 'string' && strName != '')
	{		
		var objRecordsetNavbar = new _RecordsetNavbar(strName);
		eval(strName + ' = objRecordsetNavbar;');
		objRecordsetNavbar._funcInit = funcInit;
		thisPage.advise(PAGE_ONINIT,strName + '._restoreState()');
		return objRecordsetNavbar;
	}
	return null;
}

function _RecordsetNavbar(strName)
{
	if (typeof(_bRSNBPrototypeCalled) == 'undefined')
		_RSNB__Prototype();

	//public members
	this.id = strName;
	this.name = strName;

	// private properties
	this._buttons = new Array(this._nButtonCount);

	// advise for default eventhandlers
	this._objEventManager = CreateEventManager();
	this._objEventManager.adviseDefaultHandler(this.name,RSNB_ONFIRSTCLICK);
	this._objEventManager.adviseDefaultHandler(this.name,RSNB_ONPREVIOUSCLICK);
	this._objEventManager.adviseDefaultHandler(this.name,RSNB_ONNEXTCLICK);
	this._objEventManager.adviseDefaultHandler(this.name,RSNB_ONLASTCLICK);
}

function _RSNB__Prototype()
{
	_bRSNBPrototypeCalled = 1;
	
	// events
	RSNB_ONFIRSTCLICK = 'onfirstclick';
	RSNB_ONPREVIOUSCLICK = 'onpreviousclick';
	RSNB_ONNEXTCLICK = 'onnextclick';
	RSNB_ONLASTCLICK = 'onlastclick';

	// constants
	RSNB_BUTTONFIRST = 0;
	RSNB_BUTTONPREV = 1;
	RSNB_BUTTONNEXT = 2;
	RSNB_BUTTONLAST = 3;
	
	RSNB_ALIGN_VERTICAL = 0;
	RSNB_ALIGN_HORIZONTAL = 1;

	RSNB_MASK_FIRSTIMAGE = 1;
	RSNB_MASK_FIRSTCAPTION = 2;
	RSNB_MASK_PREVIMAGE = 4;
	RSNB_MASK_PREVCAPTION = 8;
	RSNB_MASK_NEXTIMAGE = 16;
	RSNB_MASK_NEXTCAPTION = 32;
	RSNB_MASK_LASTIMAGE = 64;
	RSNB_MASK_LASTCAPTION = 128;
	
	//public members
	_RecordsetNavbar.prototype.updateOnMove = true;
	_RecordsetNavbar.prototype.pageSize = 1;
	_RecordsetNavbar.prototype.maintainState = true;

	//private members
	_RecordsetNavbar.prototype._bVisible = true;
	_RecordsetNavbar.prototype._nAlignment = RSNB_ALIGN_HORIZONTAL;
	_RecordsetNavbar.prototype._nButtonMask = 0;
	_RecordsetNavbar.prototype._bCancelOperation = false;
	_RecordsetNavbar.prototype._objDataSource = null;
	_RecordsetNavbar.prototype._nButtonCount = 4;
	_RecordsetNavbar.prototype._nButtonNames = new Array('First','Previous','Next','Last');
	
	//public methods
	_RecordsetNavbar.prototype.isVisible = _RSNB_isVisible;
	_RecordsetNavbar.prototype.show = _RSNB_show;
	_RecordsetNavbar.prototype.hide = _RSNB_hide;
	_RecordsetNavbar.prototype.getAlignment = _RSNB_getAlignment;
	_RecordsetNavbar.prototype.setAlignment = _RSNB_setAlignment;
	_RecordsetNavbar.prototype.setButtonStyles = _RSNB_setButtonStyles;
	_RecordsetNavbar.prototype.getButtonStyles = _RSNB_getButtonStyles;  
	_RecordsetNavbar.prototype.cancelOperation = _RSNB_cancelOperation;
	_RecordsetNavbar.prototype.isOperationCancelled = _RSNB_isOperationCancelled;
	_RecordsetNavbar.prototype.getButton = _RSNB_getButton;
	_RecordsetNavbar.prototype.getDataSource = _RSNB_getDataSource;
	_RecordsetNavbar.prototype.setDataSource = _RSNB_setDataSource;
	_RecordsetNavbar.prototype.advise = _RSNB_advise;
	_RecordsetNavbar.prototype.unadvise = _RSNB_unadvise;
	_RecordsetNavbar.prototype.display = _RSNB_display;

	//private methods
	_RecordsetNavbar.prototype._onfirstclick = _RSNB__onfirstclick;
	_RecordsetNavbar.prototype._onpreviousclick = _RSNB__onpreviousclick;
	_RecordsetNavbar.prototype._onnextclick = _RSNB__onnextclick;
	_RecordsetNavbar.prototype._onlastclick = _RSNB__onlastclick;
	_RecordsetNavbar.prototype._createButtons = _RSNB__createButtons;
	_RecordsetNavbar.prototype._fireEvent = _EM__fireEvent;
	_RecordsetNavbar.prototype._preserveState = _RSNB__preserveState;
	_RecordsetNavbar.prototype._restoreState = _RSNB__restoreState;
	_RecordsetNavbar.prototype._hasState = _RSNB__hasState;

	//scope implementation to _RSNB__Prototype function

	function _RSNB_isVisible()
	{    return this._bVisible;		}

	function _RSNB_show()
	{    this._bVisible = true;		}

	function _RSNB_hide()
	{    this._bVisible = false;	}

	function _RSNB_getAlignment()
	{    return this._nAlignment;	}

	function _RSNB_setAlignment(nAlignment)
	{    this._nAlignment = nAlignment;	}

	function _RSNB_setButtonStyles(nButtonMask)
	{	this._createButtons(nButtonMask);	}
	
	function _RSNB_getButtonStyles()
	{	return this._nButtonMask;	}
	
	function _RSNB_cancelOperation()
	{    this._bCancelOperation = true;		}

	function _RSNB_isOperationCancelled()
	{    return this._bCancelOperation;		}

	function _RSNB_getButton(nIndex)
	{
		if (nIndex >= RSNB_BUTTONFIRST && nIndex <= RSNB_BUTTONLAST)
			return this._buttons[nIndex];
		return null;
	}

	function _RSNB_getDataSource()
	{    return this._objDataSource;	}

	function _RSNB_setDataSource(objDataSource)
	{    this._objDataSource = objDataSource;	}

	function _RSNB_advise(strEvent,funcToCall)
	{    return this._objEventManager.advise(strEvent,funcToCall);	}

	function _RSNB_unadvise(strEvent,nAdviseID)
	{    return this._objEventManager.unadvise(strEvent,nAdviseID);	}

	function _RSNB_display()
	{
		if (this._bVisible)
		{
			for (var i = RSNB_BUTTONFIRST; i < this._nButtonCount; i++)
			{
				if (this._buttons[i] != null)
				{
					this._buttons[i].maintainState = this.maintainState;
					this._buttons[i].display();
					if (i != RSNB_BUTTONLAST)
					{
						if (this._nAlignment == RSNB_ALIGN_VERTICAL)
							Response.write('<BR>');
					}
				}
			}
		}
		this._preserveState();
	}

	function _RSNB__onfirstclick()
	{
		this._objEventManager.fireEvent(RSNB_ONFIRSTCLICK);
		if (this._objDataSource != null && !this._bCancelOperation)
		{
			if (this.updateOnMove)
				this._objDataSource.updateRecord();
			this._objDataSource.moveFirst();
		}
		else
			this._bCancelOperation = false;
	}

	function _RSNB__onpreviousclick()
	{
		var objRS = this._objDataSource;
		if (objRS == null)
		{
			this._objEventManager.fireEvent(RSNB_ONPREVIOUSCLICK);
			this._bCancelOperation = false;
		}
		else if (!objRS.BOF)
		{
			this._objEventManager.fireEvent(RSNB_ONPREVIOUSCLICK);
			if (!this._bCancelOperation)
			{
				if (this.updateOnMove)
					objRS.updateRecord();
				if (this.pageSize > 1)
				{	// move to first record of previous page
					var curPos = objRS.absolutePosition;
					var nCount = objRS.getCount();
					if (objRS.EOF)	curPos = nCount;
					if (curPos > 0 && nCount > 0)
					{
						var curPage = (curPos - (curPos % this.pageSize)) / this.pageSize;
						if (curPage > 1)
						{
							objRS.moveAbsolute(((curPage-1) * this.pageSize) + 1);
						}
						else
							objRS.moveFirst();
						return;
					}
				}

				// otherwise, move to previous record
				objRS.movePrevious();
				if (objRS.BOF)
					objRS.moveFirst();
			}
			else
				this._bCancelOperation = false;
		}
	}

	function _RSNB__onnextclick()
	{
		var objRS = this._objDataSource;
		if (objRS == null)
		{
			this._objEventManager.fireEvent(RSNB_ONNEXTCLICK);
			this._bCancelOperation = false;
		}
		else if (!objRS.EOF)
		{
			this._objEventManager.fireEvent(RSNB_ONNEXTCLICK);
			if (!this._bCancelOperation)
			{
				if (this.updateOnMove)
					objRS.updateRecord();
				if (this.pageSize > 1)
				{	// move to first record of next page
					var curPos = objRS.absolutePosition;
					var nCount = objRS.getCount();
					if (objRS.BOF)	curPos = 1;
					if (curPos > 0 && nCount > 0)
					{
						var curPage = (curPos - ((curPos - 1) % this.pageSize)) / this.pageSize;
						var newPos = curPage * this.pageSize;
						if (newPos + this.pageSize <= nCount)
							newPos += this.pageSize;
						objRS.moveAbsolute(newPos);
						return;
					}
				}

				// otherwise, move to next record
				objRS.moveNext();
				if (objRS.EOF)
					objRS.moveLast();
			}
			else
				this._bCancelOperation = false;
		}
	}

	function _RSNB__onlastclick()
	{
		this._objEventManager.fireEvent(RSNB_ONLASTCLICK);
		if (this._objDataSource != null && !this._bCancelOperation)
		{
			if (this.updateOnMove)
				this._objDataSource.updateRecord();
			this._objDataSource.moveLast();
		}
		else
			this._bCancelOperation = false;
	}

	function _RSNB__createButtons(nButtonMask)
	{
		var objButton = null;
		var buttonType;
		this._nButtonMask = nButtonMask;
		if ((nButtonMask & (RSNB_MASK_FIRSTIMAGE | RSNB_MASK_FIRSTCAPTION)) != 0)
		{
			buttonType = ((nButtonMask & RSNB_MASK_FIRSTIMAGE) != 0) ? 1 : 0;
			objButton = CreateButton(this.name + 'First',null,null);
			objButton.setStyle(buttonType);
			objButton.advise('onclick',this.name + '._onfirstclick()');
			this._buttons[RSNB_BUTTONFIRST] = objButton;
		}
		if ((nButtonMask & (RSNB_MASK_PREVIMAGE | RSNB_MASK_PREVCAPTION)) != 0)
		{
			buttonType = ((nButtonMask & RSNB_MASK_PREVIMAGE) != 0) ? 1 : 0;
			objButton = CreateButton(this.name + 'Previous',null,null);
			objButton.setStyle(buttonType);
			objButton.advise('onclick',this.name + '._onpreviousclick()');
			this._buttons[RSNB_BUTTONPREV] = objButton;
		}
		if ((nButtonMask & (RSNB_MASK_NEXTIMAGE | RSNB_MASK_NEXTCAPTION)) != 0)
		{
			buttonType = ((nButtonMask & RSNB_MASK_NEXTIMAGE) != 0) ? 1 : 0;
			objButton = CreateButton(this.name + 'Next',null,null);
			objButton.setStyle(buttonType);
			objButton.advise('onclick',this.name + '._onnextclick()');
			this._buttons[RSNB_BUTTONNEXT] = objButton;
		}
		if ((nButtonMask & (RSNB_MASK_LASTIMAGE | RSNB_MASK_LASTCAPTION)) != 0)
		{
			buttonType = ((nButtonMask & RSNB_MASK_LASTIMAGE) != 0) ? 1 : 0;
			objButton = CreateButton(this.name + 'Last',null,null);
			objButton.setStyle(buttonType);
			objButton.advise('onclick',this.name + '._onlastclick()');
			this._buttons[RSNB_BUTTONLAST] = objButton;
		}
	}

	function _RSNB__preserveState()
	{
		if (this.maintainState)
		{
			var state = new Object;
			state._bVisible = this._bVisible;
			if (this._nAlignment != RSNB_ALIGN_HORIZONTAL)
				state._nAlignment = this._nAlignment;
			if (!this.updateOnMove)
				state.updateOnMove = false;
			if (this.pageSize != 1)
				state.pageSize = this.pageSize;

			return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _RSNB__restoreState()
	{
		this._fireEvent(this._funcInit);
		if (this.maintainState)
		{
			var state = thisPage.unpersistState(this.name);
			if (state != null)
			{
				if (state._bVisible != null)
					this._bVisible = (state._bVisible != 'false' && state._bVisible != '0');
				else
					this._bVisible = true;

				if (state._nAlignment != null)
					this._nAlignment = Number(state._nAlignment);
				else
					this._nAlignment = RSNB_ALIGN_HORIZONTAL;

				if (state.updateOnMove != null)
					this.updateOnMove = (state.updateOnMove != 'false' && state.updateOnMove != '0');
				else
					this.updateOnMove = true;

				if (state.pageSize != null)
					this.pageSize = Number(state.pageSize);
				else
					this.pageSize = 1;
			}
			return true;
		}
		return false;
	}

	function _RSNB__hasState()
	{
		if (this.maintainState)
			return thisPage.isStatePersisted(this.name);
		return false;
	}

}	// end of _RSNB__Prototype function
</SCRIPT>
