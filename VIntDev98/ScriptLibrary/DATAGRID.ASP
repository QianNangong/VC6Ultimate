<SCRIPT RUNAT=SERVER LANGUAGE="JavaScript">
// ************************************************************************ 
// MSL : Microsoft Scripting Libary 
// Visual InterDev 6.0 DataGrid Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 
function CreateDataGrid(strName,funcInit,objParent)
{	
	if (typeof(strName) == 'string' && strName != '')
	{
		var objDataGrid = new _DataGrid(strName,objParent);
		eval(strName + ' = objDataGrid');

		objDataGrid._funcInit = funcInit;
		thisPage.advise(PAGE_ONINIT,strName + '._restoreState()');
		return objDataGrid;
	}
	return null;	
}

function _DataGrid(strName,objParent)
{	
	if (typeof(_bDGPrototypeCalled) == 'undefined')
		_DG__Prototype();

	// public members
	this.id = strName;
	this.name = strName;

	// private members
	// see the display method description below
	this.tableAttributes = '';		// <TABLE tableAttributes>
	this.headerAttributes = '';		// <TR headerAttributes>
	this.headerWidth = new Array;	// <TH> width attributes
	this.headerFormat = '';			// <TH>headerFormat colHeader[nCol]</TH>
	this.colHeader = new Array;
	this.rowAttributes = new Array;	// <TR rowAttributes[nRow]>
	this.rowFormat = new Array;		
	this.colData = new Array;		// <TD colAttributes[nCol]>rowFormat[nRow] colData[nCol]</TD>
	this.colAttributes = new Array;	// OR
	this.colFormat = new Array;		// <TD colAttributes[nCol]>colFormat[nCol] colData[nCol]</TD>
	
	this._objDataSource = null;
	this._markPos = 0;
}

function _DG__Prototype()
{	
	_bDGPrototypeCalled = 1;
	
	//private members		
	_DataGrid.prototype.pageSize = 0;
	_DataGrid.prototype.hasPageNumber = false;
	_DataGrid.prototype.allColumns = false;
	_DataGrid.prototype.displayHeader = true;
	_DataGrid.prototype.navbarAlignment = 'center';
	_DataGrid.prototype.hiliteAttributes = '';		// attributes applied to row of the current record
	_DataGrid.prototype.maintainState = true;
	
	_DataGrid.prototype._bVisible = true;
	_DataGrid.prototype._objPageNavbar = null;	
	_DataGrid.prototype._objRecordNavbar = null;

	//public methods
	_DataGrid.prototype.show = _DG_show;
	_DataGrid.prototype.hide = _DG_hide;
	_DataGrid.prototype.isVisible = _DG_isVisible;
	_DataGrid.prototype.getPagingNavbar = _DG_getPagingNavbar;
	_DataGrid.prototype.getRecordsetNavbar = _DG_getRecordsetNavbar;
	_DataGrid.prototype.bindAllColumns = _DG_bindAllColumns;
	
	//private methods
	_DataGrid.prototype.getDataSource = _DG_getDataSource;
	_DataGrid.prototype.setDataSource = _DG_setDataSource;
	_DataGrid.prototype.showPageNavbar = _DG_showPageNavbar;
	_DataGrid.prototype.showRecordNavbar = _DG_showRecordNavbar;
	_DataGrid.prototype.anchor = _DG_anchor;	
	_DataGrid.prototype.display = _DG_display;
	
	_DataGrid.prototype._fireEvent = _EM__fireEvent;
	_DataGrid.prototype._preserveState = _DG__preserveState;
	_DataGrid.prototype._restoreState = _DG__restoreState;
	_DataGrid.prototype._onrowenter = _DG__onrowenter;
	
	thisPage.registerMethod('_DG__selectRow',_DG__selectRow);
	
	//scope implementation in _DG__Prototype function
	function _DG_show()
	{	this._bVisible = true;			}

	function _DG_hide()
	{	this._bVisible = false;			}
	
	function _DG_isVisible()
	{	return this._bVisible			}
	
	function _DG_getPagingNavbar()
	{	return this._objPageNavbar;			}

	function _DG_getRecordsetNavbar()
	{	return this._objRecordNavbar;		}

	function _DG_bindAllColumns()
	{	this.allColumns = true;			}
	
	function _DG_getDataSource()
	{	return this._objDataSource;		}

	function _DG_setDataSource(objDataSource)
	{
		if (typeof(objDataSource) == 'object')
			this._objDataSource = objDataSource;
	}

	function _DG_showPageNavbar(nMask,nAlignment)
	{
		this._objPageNavbar = CreateRecordsetNavbar(this.name + '_PageNavbar',null,null);
		this._objPageNavbar.setAlignment(nAlignment);
		this._objPageNavbar.setButtonStyles(nMask);
		this._objPageNavbar.pageSize = this.pageSize;
		this._objPageNavbar.updateOnMove = false;
		this._objPageNavbar.setDataSource(this._objDataSource);
		return this._objPageNavbar;
	}

	function _DG_showRecordNavbar(nMask,nAlignment)
	{
		this._objRecordNavbar = CreateRecordsetNavbar(this.name + '_RecordNavbar',null,null);
		this._objRecordNavbar.setAlignment(nAlignment);
		this._objRecordNavbar.setButtonStyles(nMask);
		this._objRecordNavbar.pageSize = 1;
		this._objRecordNavbar.updateOnMove = false;
		this._objRecordNavbar.setDataSource(this._objDataSource);
		return this._objRecordNavbar;
	}

	function _DG_anchor(strText)
	{
		var nIndex = this._objDataSource.absolutePosition;
		if (Number(nIndex) > 0)
		{
			if (typeof(strText) == 'undefined' || strText == '')
				strText = nIndex;
			return '<a href="JavaScript:thisPage.invokeMethod(' + 
					"''" +
					",'_DG__selectRow'," +
					"new Array('" + this.name + "','" + nIndex + "'))" +
					'">' + strText + '</a>';
		}
		return '';
	}
	
	function _DG_display()
	{	
		if (this._bVisible)
		{
			var objRS = this._objDataSource;
			if (objRS != null && !objRS._isEmpty())
			{
				// commandeer EventManager of Recordset to prevent 
				// onrowenter events to other bound controls while 
				// displaying the grid
				var objRSEM = objRS._objEventManager;
				objRS._objEventManager = CreateEventManager();
		
				if (objRS.BOF)	objRS.moveFirst();
				else if (objRS.EOF)	objRS.moveLast();
				
				if (this.allColumns)
				{	// auto-generate data from recordset
					this.colHeader.length = 0;
					this.colData.length = 0;
					this.headerWidth.length = 0;
					var nCount = objRS.fields.getCount();
					for (var i=0; i < nCount; i++)
					{
						if (this.displayHeader)
						{
							this.colHeader[i] = '"' + objRS.fields.getName(i) + '"';
							this.headerWidth[i] = '';
						}
						this.colData[i] = objRS.name + '.fields.getValue(' + i + ')';
					}
				}	
				var nCols = this.colData.length;
				if (nCols > 0)
				{
					var strHTML = '<TABLE border=0 cellpadding=0 cellspacing=0><TR><TD>\n';
					strHTML += '<TABLE ' + this.tableAttributes + '>\n';
					var nHeaderCols = this.colHeader.length;
					if (nHeaderCols && this.displayHeader)
					{
						strHTML += '<TR ' + this.headerAttributes + '>\n';
						for (var nCol=0; nCol < nCols; nCol++)
						{
							strHTML += '<TH ' + this.headerWidth[nCol] + '>' + this.headerFormat + eval(this.colHeader[nCol]) + '</TH>\n';
						}
						strHTML += '</TR>\n';
					}
					Response.write(strHTML);	// output table header


					this._markPos = objRS.absolutePosition; // mark current position
					var curPage = 0;
					var nAdviseID = -1;
					this._rowCount = 0;

					if (this.pageSize > 0 && this._markPos > 0)
					{	// display current page only
						curPage = parseInt((this._markPos - 1) / this.pageSize);
						objRS.moveAbsolute((curPage * this.pageSize) + 1);
						nAdviseID = objRS.advise(RS_ONROWENTER, this.id + '._onrowenter()');
						for (var i=1; i < this.pageSize; i++)
						{
							if (objRS.EOF)
								break;
							objRS.moveNext();
						}
					}
					else
					{	// display ALL records
						objRS.moveFirst();
						nAdviseID = objRS.advise(RS_ONROWENTER, this.id + '._onrowenter()');
						while (!objRS.EOF)
						{
							objRS.moveNext();
						}
					}

					objRS.unadvise(RS_ONROWENTER,nAdviseID);
					objRS.moveAbsolute(this._markPos);
					Response.write('</TABLE>\n');

					if (this._objPageNavbar != null || this._objRecordNavbar != null || this.hasPageNumber)
					{	// display navbars and/or page numbering
						strHTML = '</TD></TR>\n<TR><TD align=center>\n';
						strHTML += '<TABLE width=100% border=0><TR>\n';
						Response.write(strHTML);
						Response.write('<TD align=' + this.navbarAlignment + '>');
						if (this._objPageNavbar != null)
						{
							if (this._objRecordNavbar != null)
							{
								if (this._objPageNavbar.isVisible())
								{
									if (this._objPageNavbar.getButton(0) != null)
										this._objPageNavbar.getButton(0).display();
									if (this._objPageNavbar.getButton(1) != null)
										this._objPageNavbar.getButton(1).display();
									this._objRecordNavbar.display();
									if (this._objPageNavbar.getButton(2) != null)
										this._objPageNavbar.getButton(2).display();
									if (this._objPageNavbar.getButton(3) != null)
										this._objPageNavbar.getButton(3).display();
								}
								else
								{
									this._objPageNavbar.display();
									this._objRecordNavbar.display();
								}
							}
							else
								this._objPageNavbar.display();
						}
						else if (this._objRecordNavbar != null)
							this._objRecordNavbar.display();
						Response.write('</TD>\n');
						if (this.hasPageNumber)
						{
							var nPages = 1;
							var nCount = objRS.getCount();

							if (nCount > 0 && this.pageSize > 0)
								nPages = ((nCount - 1 ) / this.pageSize) + 1;
							strHTML = '<TD align=right valign=middle>';
							strHTML += '<FONT size=2><NOBR>Page : ' + String(curPage+1) + ' of ' + parseInt(nPages) + '</NOBR></FONT>';
							strHTML += '</TD>\n';
							Response.write(strHTML);
						}
						Response.write('</TR></TABLE>\n');
					}
					Response.write('</TD></TR></TABLE>\n');
				}
				// restore original RS EventManager
				objRS._objEventManager = objRSEM;
			}
		}
		this._preserveState();
	}
	
	function _DG__preserveState()
	{
		if (this.maintainState)
		{	// preserve state in hidden field
			var state = new Object;
			state._bVisible = this._bVisible;
			if (this.allColumns)
				state.allColumns = true;
			return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _DG__restoreState()
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

				if (state.allColumns != null)
					this.allColumns = (state.allColumns != '0' && state.allColumns != 'false');
				else
					this.allColumns = false;

				r = true;
			}
		}

		return r;
	}
	
	function _DG__onrowenter()
	{	// output a single row <TR> for current record
		if (this._objDataSource.EOF)
			return;

		var hilite = '';
		if (this.hiliteAttributes.length && 
			this._markPos == this._objDataSource.absolutePosition)
			hilite = this.hiliteAttributes;

		var strHTML = '<TR>\n';
		if (this.rowAttributes.length)
		{	// apply row attributes to <TR>
			var nRow = this._rowCount % this.rowAttributes.length;
			strHTML = '<TR ' + hilite + ' ' + this.rowAttributes[nRow] + '>\n';
		}
		else
			strHTML = '<TR ' + hilite + '>\n';

		// output <TD> for each column in row
		var nCols = this.colData.length;
		for (var nCol=0; nCol < nCols; nCol++)
		{	
			// apply column attributes to <TD>
			if (nCol < this.colAttributes.length)
				strHTML += '<TD ' + hilite + ' ' + this.colAttributes[nCol] + '>';
			else
				strHTML += '<TD>';

			// determine format per data column
			if (typeof(this.colFormat[nCol]) != 'undefined')
			{ // use column format
				strHTML += this.colFormat[nCol];
			}
			else if (this.rowFormat.length)
			{ // use row format
				var nRow = this._rowCount % this.rowFormat.length;
				strHTML += this.rowFormat[nRow];
			}

			strHTML += eval(this.colData[nCol]);
			strHTML += '</TD>\n';
		}

		strHTML += '</TR>\n';
		Response.write(strHTML);
		this._rowCount++;
	}

}	// end of _DG__Prototype function


function _DG__selectRow(strName,nIndex)
{
	var object = null;
	if (typeof(RuntimeObject) != 'undefined')
	{	// use global namespace to acquire object of 'strName'
		var objects = RuntimeObject(strName);
		object = objects[strName];
	}
	if (object != null && typeof(object) == 'object' && typeof(object._objDataSource) == 'object')
		object._objDataSource.moveAbsolute(Number(nIndex));
}

</SCRIPT>
