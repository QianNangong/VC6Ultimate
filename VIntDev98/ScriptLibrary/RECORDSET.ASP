<SCRIPT RUNAT=SERVER LANGUAGE="JavaScript">
// ************************************************************************ 
// MSL : Microsoft Scripting Libary 
// Visual InterDev 6.0 Recordset Object for ASP
//
// Copyright 1998 Microsoft Corporation. All Rights Reserved.
// ************************************************************************ 
function CreateRecordset(strName,funcInit,objParent)
{	
	if (typeof(strName) != 'string' || strName == '')
		return null;

	var objRecordset = new _Recordset(strName);
	eval(strName + ' = objRecordset');
	// always fire init for recordset
	objRecordset._funcInit = funcInit;
	thisPage.advise(PAGE_ONINIT,strName + '._restoreState()',10);
	return objRecordset;
}

function _Recordset(strName)
{
	if (typeof(_bRSPrototypeCalled) == 'undefined')
		_RS__Prototype();

	// public members
	this.id = strName;
	this.name = strName;
	this.fields = null;
	this.absolutePosition = 0;
	this.BOF = true;
	this.EOF = true;
	this.maintainState = true;

	// private members
	this._rsADO = null;
	this._count = -1;
	this._allowUpdate = false;
	this._objDBConn = null;
	this._bookmark = 0;
	this._params = null;
	this._bCancelUpdate = false;
	this._bFiringOnBeforeUpdate = false;
	this._bAddNew = false;
	this._bAddNewImmediate = false;
	this._bExecuted = false;
	this._strSQL = '';
	
	// advise for default eventhandlers
	this._objEventManager = CreateEventManager();
	this._objEventManager.adviseDefaultHandler(this.name,RS_ONBEFOREOPEN);
	// set default handlers AFTER all other controls are initialized
	thisPage.advise(PAGE_ONINIT,this.name + '.adviseDefaultHandler("' + this.name + '",RS_ONROWENTER)',-10);
	thisPage.advise(PAGE_ONINIT,this.name + '.adviseDefaultHandler("' + this.name + '",RS_ONDATASETCHANGED)',-10);
	thisPage.advise(PAGE_ONINIT,this.name + '.adviseDefaultHandler("' + this.name + '",RS_ONDATASETCOMPLETE)',-10);
	thisPage.advise(PAGE_ONINIT,this.name + '.adviseDefaultHandler("' + this.name + '",RS_ONROWEXIT)',-10);
	thisPage.advise(PAGE_ONINIT,this.name + '.adviseDefaultHandler("' + this.name + '",RS_ONBEFOREUPDATE)',-10);
	thisPage.advise(PAGE_ONINIT,this.name + '.adviseDefaultHandler("' + this.name + '",RS_ONAFTERUPDATE)',-10);
}

function _RS__Prototype()
{
	//public members
	_Recordset.prototype.getCount = _RS_getCount;
	_Recordset.prototype.moveNext = _RS_moveNext;
	_Recordset.prototype.movePrevious = _RS_movePrevious;
	_Recordset.prototype.moveFirst = _RS_moveFirst;
	_Recordset.prototype.moveLast = _RS_moveLast;
	_Recordset.prototype.moveAbsolute = _RS_moveAbsolute;
	_Recordset.prototype.move = _RS_move;
	_Recordset.prototype.updateRecord = _RS_updateRecord;
	_Recordset.prototype.cancelUpdate = _RS_cancelUpdate;
	_Recordset.prototype.addRecord = _RS_addRecord;
	_Recordset.prototype.addImmediate = _RS_addImmediate;
	_Recordset.prototype.deleteRecord = _RS_deleteRecord;
	_Recordset.prototype.advise = _RS_advise;
	_Recordset.prototype.unadvise = _RS_unadvise;
	_Recordset.prototype.adviseDefaultHandler = _RS_adviseDefaultHandler;

	_Recordset.prototype.getRecordSource = _RS_getRecordSource;
	_Recordset.prototype.setRecordSource = _RS_setRecordSource;
	_Recordset.prototype.open = _RS_open;
	_Recordset.prototype.isOpen = _RS_isOpen;
	_Recordset.prototype.close = _RS_close;
	_Recordset.prototype.getConnectString = _RS_getConnectString;
	_Recordset.prototype.getSQLText = _RS_getSQLText;
	_Recordset.prototype.setSQLText = _RS_setSQLText;
	_Recordset.prototype.requery = _RS_requery;
	_Recordset.prototype.setBookmark = _RS_setBookmark;
	_Recordset.prototype.getBookmark = _RS_getBookmark;
	_Recordset.prototype.setParameter = _RS_setParameter;
	_Recordset.prototype.getParameter = _RS_getParameter;
	_Recordset.prototype.isDHTMLAware = _RS_isDHTMLAware;
	_Recordset.prototype.getDHTMLDataSourceID = _RS_getDHTMLDataSourceID;
	
	//events
	RS_ONBEFOREOPEN = 'onbeforeopen';
	RS_ONROWENTER = 'onrowenter';
	RS_ONROWEXIT = 'onrowexit';
	RS_ONDATASETCHANGED = 'ondatasetchanged';
	RS_ONDATASETCOMPLETE = 'ondatasetcomplete';
	RS_ONBEFOREUPDATE = 'onbeforeupdate';
	RS_ONAFTERUPDATE = 'onafterupdate';

	//private members
	_Recordset.prototype._syncBOFandEOF = _RS__syncBOFandEOF;
	_Recordset.prototype._fireEvent = _EM__fireEvent;
	_Recordset.prototype._preserveState = _RS__preserveState;
	_Recordset.prototype._restoreState = _RS__restoreState;
	_Recordset.prototype._hasState = _RS__hasState;
	_Recordset.prototype._isEmpty = _RS__isEmpty;
	_Recordset.prototype._resetMembers = _RS__resetMembers;
	_bRSPrototypeCalled = 1;

	//scope implementation to _RS__Prototype function

	function _RS_getCount()
	{
		if (this._count < 0)
		{
			if (this.isOpen())
			{
				if (!this.BOF || !this.EOF)
				{
					this._count = this._rsADO.RecordCount;
					if (this._count <= 0)
					{	
						var curPos = this._rsADO.AbsolutePosition;
						if (curPos > 0)
						{
							this._count = 0;
							this._rsADO.MoveFirst();
							while (!this._rsADO.EOF)
							{	
								this._count++;	
								this._rsADO.MoveNext();	
							}
							this._rsADO.AbsolutePosition = curPos;
						}
					}
				}
				else
					this._count = 0;

				return this._count;
			}
			return 0;
		}
		return this._count;
	}
	
	function _RS_moveNext(nDirection)
	{
		if (typeof(nDirection) == 'undefined')
				nDirection = 1;
				
		if (!this._isEmpty())
		{
			this._objEventManager.fireEvent(RS_ONROWEXIT);	
			this._rsADO.Move(nDirection);
			this._syncBOFandEOF();
			if (this.EOF || this.BOF)
				return false;
			this.fields._reset(this._rsADO);
			this._objEventManager.fireEvent(RS_ONROWENTER);
			return true;
		}
		return false;
	}

	function _RS_movePrevious()
	{	return this.moveNext(-1);	}

	function _RS_moveLast(bReverse)
	{
		if (!this._isEmpty())
		{
			this._objEventManager.fireEvent(RS_ONROWEXIT);
			
			if ((bReverse + '') == 'true')
				this._rsADO.MoveFirst();
			else			
				this._rsADO.MoveLast();
				
			this._syncBOFandEOF();
			this.fields._reset(this._rsADO);
			this._objEventManager.fireEvent(RS_ONROWENTER);
			return true;
		}
		return false;
	}

	function _RS_moveFirst()	
	{	return this.moveLast(true);	}

	function _RS_moveAbsolute(nIndex)
	{
		if (!this._isEmpty() && 
			typeof(nIndex) == 'number')
		{
			this._objEventManager.fireEvent(RS_ONROWEXIT);
			this._rsADO.AbsolutePosition = nIndex;
			this._syncBOFandEOF();
			this.fields._reset(this._rsADO);
			this._objEventManager.fireEvent(RS_ONROWENTER);
			return true;
		}
		return false;
	}	

	function _RS_move(nIndex)
	{	// move relative to current record
		if (!this._isEmpty() && 
			typeof(nIndex) == 'number')
		{
			this._objEventManager.fireEvent(RS_ONROWEXIT);
			this._rsADO.Move(nIndex);
			this._syncBOFandEOF();
			this.fields._reset(this._rsADO);
			this._objEventManager.fireEvent(RS_ONROWENTER);
			return true;
		}
		return false;
	}	

	function _RS_updateRecord()
	{	
		//bFiringOnBeforeUpdate: If the user is calling this function from
		//the OnBeforeUpdate, the call should be rejected.
		if (this._allowUpdate && (!this._isEmpty() || this._bAddNewImmediate) && !this._bFiringOnBeforeUpdate) 
		{ 	
			if (this._bAddNewImmediate)
			{
				if (this._count >= 0)
					++this._count;
				this._rsADO.AddNew();
				this.fields._isValid = true;
			}
			
			this._bFiringOnBeforeUpdate = true;				
			this._objEventManager.fireEvent(RS_ONBEFOREUPDATE);
			this._bFiringOnBeforeUpdate = false;	
			if (!this._bCancelUpdate)
			{
				this._rsADO.Update();
				if (this._rsADO.LockType == 4)
					this._rsADO.UpdateBatch();
		
				if (this._bAddNewImmediate)
					this._syncBOFandEOF();
					
				this._bAddNewImmediate = false;
				this._objEventManager.fireEvent(RS_ONAFTERUPDATE);
				return true;
			}
			else
				this._bCancelUpdate = false;
		} 
		return false; 
	}

	function _RS_cancelUpdate()
	{	
		if (this._allowUpdate && this.isOpen()) 
		{	
			//adEditAdd = 2
			if (this._rsADO.EditMode == 2)
			{
				if (this._bAddNewImmediate)
				{
					if (this._count >= 0)
						--this._count;
				}
				if (!this._isEmpty())
				{
					this._rsADO.CancelUpdate();	
					this._syncBOFandEOF();
					this.fields._reset(this._rsADO);	
				}
				else
				//ADO doesn't support CancelUpdate with empty recordset
					this.fields._isValid = false;
			}
			
			//adEditInProgress = 1
			if (this._rsADO.EditMode == 1)	
				this._rsADO.CancelUpdate(); 
				
			this._bAddNewImmediate = false;
			this._bCancelUpdate = true;
			this._objEventManager.fireEvent(RS_ONROWENTER);
			return true; 
		} 
		else 
			return false; 
	}

	function _RS_addRecord()
	{	
		//bFiringOnBeforeUpdate: If the user is calling this function from
		//the OnBeforeUpdate, the call should be rejected.
		if (this._allowUpdate && this.isOpen() && !this._bFiringOnBeforeUpdate) 
		{	
			this._bAddNewImmediate = false;
			this._objEventManager.fireEvent(RS_ONROWEXIT);
			// set flag to indicate new record for subsequent update
			this._bAddNew = true;
			this.fields._reset(this._rsADO);
			this.fields._newRecord = true;
			this._objEventManager.fireEvent(RS_ONROWENTER);
			return true; 
		}
		return false; 
	}
	
	function _RS_addImmediate(fieldList, fieldValues)
	{	
		if (this._allowUpdate && this.isOpen() && !this._bFiringOnBeforeUpdate) 
		{	
			this._bAddNewImmediate = false;
			this._objEventManager.fireEvent(RS_ONROWEXIT);
			this._rsADO.AddNew(fieldList, fieldValues);
			if (this._rsADO.LockType == 4)
				this._rsADO.UpdateBatch();
			this._syncBOFandEOF();
			if (this._count >= 0)
				++this._count;
			this.fields._reset(this._rsADO);
			this._objEventManager.fireEvent(RS_ONROWENTER);
			return true; 		
		}
		return false;
	}
	
	function _RS_deleteRecord()
	{	
		if (this._allowUpdate && !this._isEmpty()) 
		{ 
			this._bAddNewImmediate = false;
			this._objEventManager.fireEvent(RS_ONROWEXIT);
			this._rsADO.Delete();
			if (this._rsADO.LockType == 4)
				this._rsADO.UpdateBatch();
			this._rsADO.moveNext();
			if (this._rsADO.EOF)
				this._rsADO.movePrevious();	
									
			this._syncBOFandEOF();
			if (this._count >= 0)
				--this._count;
			this.fields._reset(this._rsADO);
			this._objEventManager.fireEvent(RS_ONROWENTER);	 				
			
			return true; 
		} 
		return false; 
	}

	function _RS_advise(strEvent,funcToCall)
	{
		if (this.isOpen() &&
			(strEvent == RS_ONROWENTER || strEvent == RS_ONDATASETCOMPLETE || strEvent == RS_ONDATASETCHANGED))
		{	// fire immediately if data is available
			this._fireEvent(funcToCall);
		}
		return this._objEventManager.advise(strEvent,funcToCall);
	}

	function _RS_unadvise(strEvent,nAdviseID)
	{
		return this._objEventManager.unadvise(strEvent,nAdviseID);	
	}
	
	function _RS_adviseDefaultHandler(strName,strEvent)
	{
		var nAdviseID = this._objEventManager.adviseDefaultHandler(strName,strEvent);
		if (this.isOpen() && nAdviseID > 0 &&
			(strEvent == RS_ONROWENTER || strEvent == RS_ONDATASETCOMPLETE || strEvent == RS_ONDATASETCHANGED))
		{	// fire immediately if data is available
			var funcToCall = strName + '_' + strEvent + '()';
			this._fireEvent(funcToCall);
		}
		return nAdviseID;
	}


	function _RS_getRecordSource()
	{	return this._rsADO;		}

	//function _RS_setRecordSource(rsADO) OR
	function _RS_setRecordSource(strConn,strSQL)
	{
		if (typeof(strConn) == 'string')
		{	// given a connect and SQL string
			this._resetMembers(true);
			this._objDBConn = Server.CreateObject('ADODB.Connection');
			this._objDBConn.ConnectionString = strConn;
			this._objDBConn.Open();
			if (!this.setSQLText(strSQL))
				return false;
		}
		else if (typeof(strConn) == 'object')
		{
			// given an ADO object
			var strBook = this._bookmark;
			this._objDBConn = null;
			this._resetMembers(true);
			this._bookmark = strBook;
			this._rsADO = strConn;
		}
		else
			return false;

		if (this._rsADO.LockType == 1)		
			this._allowUpdate = false;
		else								
			this._allowUpdate = true;

		if (this.isOpen())
		{
			this.fields = CreateFields(this._rsADO);
			if (this._bookmark != 0)
				this.setBookmark(this._bookmark);
			else
				this._syncBOFandEOF();

			this._objDBConn = this._rsADO.ActiveConnection;			
			
			this._objEventManager.fireEvent(RS_ONROWENTER);
			this._objEventManager.fireEvent(RS_ONDATASETCHANGED);
			this._objEventManager.fireEvent(RS_ONDATASETCOMPLETE);
		}
			
		return true;
	}

	function _RS_isOpen()
	{	
		if ((this._rsADO && this._rsADO.state != 0)	|| (this._bExecuted))
			return true;
		return false;
	}

	function _RS_open()
	{	
		if (this._rsADO != null)
		{
			if (!this.isOpen())
			{
				this._objEventManager.fireEvent(RS_ONBEFOREOPEN);
				if (this._params != null)
				{
					for (var i=0; i < this._params.length; i++)
					{
						if (typeof(this._params[i]) != 'undefined')
							this._rsADO.ActiveCommand.Parameters(Number(i)).value = this._params[i];		
					}
				}

				// force AbsolutePosition to work in server-side cursor for SQL
				if (this._rsADO.CursorLocation == 2)
					this._rsADO.Properties('IRowsetScroll') = true;
				
				// double-check after ONBEFOREOPEN is fired				
				if (!this.isOpen())
					this._rsADO.Open();
					
				this._objDBConn = this._rsADO.ActiveConnection;
				this.fields = CreateFields(this._rsADO);
				this._bExecuted = true;

				if (this._bookmark != 0)
					this.setBookmark(this._bookmark);
				else
					this._syncBOFandEOF();
				
// fire initial events		
				this._objEventManager.fireEvent(RS_ONROWENTER);
				this._objEventManager.fireEvent(RS_ONDATASETCHANGED);
				this._objEventManager.fireEvent(RS_ONDATASETCOMPLETE);
				return true;
			}
		}
		return false;
	}

	function _RS_close()
	{	
		if (this.isOpen())
			this._rsADO.Close();

		this._resetMembers(true);
		return true;
	}

	function _RS_getConnectString()
	{	
		if (this._rsADO != null)
			return this._rsADO.ActiveConnection.ConnectionString;
		return '';
	}

	function _RS_getSQLText()
	{
		if (this._rsADO != null)
			return this._rsADO.ActiveCommand.CommandText;
		return '';
	}

	function _RS_setSQLText(strSQL)
	{
		if (typeof(strSQL) == 'string')
		{
			if (!this.isOpen())
			{	// recordset is not open
				if (this._rsADO != null)
				{	// try to use existing recordset
					var objDBCmd = this._rsADO.ActiveCommand;
					//adCmdUnknown = 8
					objDBCmd.CommandType = 8;
					objDBCmd.CommandText = strSQL;
					this._strSQL = strSQL;
					return true;
				}
				
				// create an ADO recordset
				if (this._objDBConn != null)
				{
					var objDBCmd = Server.CreateObject('ADODB.Command');
					//adCmdUnknown = 8
					objDBCmd.CommandType = 8;
					objDBCmd.CommandText = strSQL;
					objDBCmd.ActiveConnection = this._objDBConn;
					this._rsADO = Server.CreateObject('ADODB.Recordset');
					this._rsADO.Source = objDBCmd;
					this.setRecordSource(this._rsADO);
					return true;
				}
			}
			else
			{	// recordset is open, set CommandText to strSQL
				// strSQL must be valid for current CommandType
				this._rsADO.ActiveCommand.CommandText = strSQL;
				this._strSQL = strSQL;
				return true;
			}
		}
		return false;
	}

	function _RS_requery()
	{
		if (this.isOpen())
		{
			this._rsADO.Requery();
			this._objEventManager.fireEvent(RS_ONROWEXIT);
			this._resetMembers();
			this._syncBOFandEOF();
			this.fields._reset(this._rsADO);
			this._objEventManager.fireEvent(RS_ONROWENTER);
			this._objEventManager.fireEvent(RS_ONDATASETCHANGED);
			this._objEventManager.fireEvent(RS_ONDATASETCOMPLETE);
			return true;
		}
		return false;
	}

	function _RS_setBookmark(bookmark)
	{
		var nAbsPos = bookmark;
		var strKeys = '';
		var nSemi = bookmark.indexOf(';');
		if (nSemi != -1)
		{	// bookmark may contain keyfields for validation
			nAbsPos = bookmark.substring(0,nSemi);
			strKeys = bookmark.substring(nSemi+1,bookmark.length);
		}
		if (!isNaN(parseInt(nAbsPos)))
		{
			this._bookmark = bookmark;
			if (this.isOpen() && !(this._rsADO.EOF && this._rsADO.BOF))
			{	// reset to previous absolute position
				this._objEventManager.fireEvent(RS_ONROWEXIT);
				var rsADO = this._rsADO;
				var keyFields = null;
				var bSeek = (strKeys.length > 1);
				if (bSeek)
				{	// extract key field values from bookmark
					keyFields = new Object;
					var nEq, nSemi, keyName, keyValue;
					while (strKeys.length > 1)
					{	// extract keyfield name/value pairs
						nEq = strKeys.indexOf('=');
						nSemi = strKeys.indexOf(';');
						keyName = strKeys.substring(0,nEq);
						keyValue = strKeys.substring(nEq+1,nSemi);
						keyFields[keyName] = unescape(keyValue);		
						strKeys = strKeys.substring(nSemi+1,strKeys.length);
					}
				}
				if (nAbsPos > 0)
				{
					rsADO.AbsolutePosition = (nAbsPos * 1);
					if (bSeek)
					{	// validate position using key field values
						bSeek = false;
						for (var keyName in keyFields)
						{
							if (typeof(rsADO.Fields(keyName)) != 'undefined' &&
								keyFields[keyName] != rsADO.Fields(keyName))
							{
								bSeek = true;
								break;
							}
						}
					}
				}
				else if (nAbsPos == -2)		// adPosBOF
					rsADO.MoveFirst();
				else if (nAbsPos == -3)		// adPosEOF
					rsADO.MoveLast();


				if (bSeek)
				{	// use key fields to seek to bookmarked record
					var strWhere = '';
					var nKeys = 0;
					for (var keyName in keyFields)
					{
						if (typeof(rsADO.Fields(keyName)) != 'undefined')
						{
							var keyValue = keyFields[keyName];						
							var keyType = rsADO.Fields(keyName).type;
							// put single quotes around the following field types
							// adChar, adVarChar, adLongVarChar, adWChar, adVarWChar, adLongVarWChar,
							// adBSTR, adDate, adGUID, adVariant, adUserDefined,
							// adDBDate, adDBTime, adDBTimeStamp
							if (keyType == 129 || keyType == 200 || keyType == 201 || keyType == 130 || keyType == 202 || keyType == 203 ||
								keyType == 8 || keyType == 7 || keyType == 72 || keyType == 12 || keyType == 132 ||
								keyType == 133 || keyType == 134 || keyType == 135)
								keyValue = "'" + keyValue + "'";
							if (nKeys > 0)	strWhere += ' AND ';
							strWhere += keyName + '=' + keyValue;
							nKeys++;
						}
						else
						{	// abort, field names do not match
							nKeys = 0;
							break;
						}
					}
					var bSeekFailed = false;
					if (nKeys == 1)
					{	// single key field, seek using Find
						rsADO.MoveFirst();
						rsADO.Find(strWhere);
						if (rsADO.AbsolutePosition < 1)
							bSeekFailed = true;
					}
					else if (nKeys > 1)
					{	// multiple key fields, seek using Filter
						var strPrevFilter = rsADO.Filter;
						rsADO.Filter = strWhere;
						if (rsADO.RecordCount)
						{
							var tmpBookmark = rsADO.Bookmark;
							rsADO.Filter = '';
							rsADO.Filter = strPrevFilter;
							rsADO.Bookmark = tmpBookmark;
						}
						else
						{	// seek failed
							bSeekFailed = true;
							rsADO.Filter = '';
							rsADO.Filter = strPrevFilter;
						}
					}
					if (bSeekFailed)
					{	// use previous position OR first record
						if (nAbsPos > 0)
							rsADO.AbsolutePosition = (nAbsPos * 1);
						else
							rsADO.MoveFirst();

						this._allowUpdate = false;
					}
				}
				this._syncBOFandEOF();
				this._objEventManager.fireEvent(RS_ONROWENTER);
			}
		}
	}

	function _RS_getBookmark()
	{		
		if (this.isOpen())
		{	// use absolute position to bookmark across recordset sessions
			var rsADO = this._rsADO;
			this._bookmark = this.absolutePosition + ';';
			if (!this.BOF && !this.EOF)
			{	// look for key fields
				var bCursorLoc = rsADO.CursorLocation;
				for (var i=0; i < rsADO.Fields.Count; i++)
				{	// store key fields to validate absolute position
					var bKeyField =  (rsADO.Fields(i).Properties('KeyColumn') == 1);
					if (bKeyField)
					{	// append keyfield name/value pairs to bookmark
						var fieldValue = rsADO.Fields(i);
						var fieldType = rsADO.Fields(i).type;
						//avoid JScript formating for the following field types:
						//adDate, adDBDate, adDBTime, adDBTimeStamp, adBinary
						if ((fieldType == 7) || (fieldType == 133) || (fieldType == 134) || (fieldType == 135))
								this._bookmark += rsADO.Fields(i).name + '=' + escape(Server.HTMLEncode(fieldValue)) + ';';
						//if the field is adBinary there is nothing to do
						else if (fieldType != 128)
							this._bookmark += rsADO.Fields(i).name + '=' + escape(fieldValue) + ';';	
					}
				}
			}
		}
		return this._bookmark;
	}

	function _RS_setParameter(nIndex,value)
	{
		if (typeof(nIndex) == 'number' && typeof(value) != 'undefined')
		{
			if (this.isOpen())
				this._rsADO.ActiveCommand.Parameters(Number(nIndex)).value = value;
			else
			{
				if (this._params == null)	this._params = new Array;
				this._params[nIndex] = value;
			}
			return true;
		}
		return false;
	}

	function _RS_getParameter(nIndex)
	{
		if (typeof(nIndex) == 'number')
		{
			if (this.isOpen())
				return this._rsADO.ActiveCommand.Parameters(Number(nIndex)).value;
			else if (this.params != null)
				return this._params[nIndex];
		}
		return '';
	}

	function _RS_isDHTMLAware()
	{	return false;	}

	function _RS_getDHTMLDataSourceID()
	{	return '';	}

	function _RS__syncBOFandEOF()
	{
		if (this.isOpen())
		{	
			this.EOF = this._rsADO.EOF;	
			this.BOF = this._rsADO.BOF;
			this.absolutePosition = this._rsADO.AbsolutePosition;
		}
	}

	function _RS__preserveState()
	{
		if (this.maintainState)
		{	// preserve state in hidden field
			var bState = false;
			var state = new Object;
		
			if (this._bAddNew)
			{
				bState = true;
				state._addNew = true;
			}
			if (this.isOpen())
			{
				bState = true;
				state._bOpen = true;
				if (this._strSQL != '')
					state._strSQL = this._strSQL;
			}
			if (this._params != null)
			{
				bState = true;
				state._pCount = 0;
				state._pCount = this._params.length;
				for (var i=0; i < state._pCount; i++)
				{
					if (typeof(this._params[i]) != 'undefined') 
						state['p' + i] = this._params[i];
				}
			}
			if (bState)
				return thisPage.persistState(this.name,state);
		}
		return false;
	}

	function _RS__restoreState()
	{
		var r = false;
		this._fireEvent(this._funcInit);
		
		if (this.maintainState)
		{	// attempt to restore previous state
			var state = thisPage.unpersistState(this.name);
			if (state != null)
			{	// restore previous state
				if (state._addNew != null)
					this._bAddNewImmediate = true;
					
				if (!this.isOpen())
				{
					if (state._strSQL != null)
						this.setSQLText(state._strSQL);
						
					if (typeof(state._pCount) != null)
					{
						state._pCount = Number(state._pCount);
						if (state._pCount > 0)
						{
							this._params = new Array;
							for (var i=0; i < state._pCount; i++)
							{
								if (typeof(state['p' + i]) != 'undefined') 
									this._params[i] = state['p' + i];
							}
						}
					}
				
					if (state._bOpen != null)
						this.open();
				}
					
				r = true;
			}		
		}
			
		return r;
	}

	function _RS__hasState()
	{
		if (this.maintainState)
			return thisPage.isStatePersisted(this.name);
		return false;
	}
	
	function _RS__isEmpty()
	{
		return (!this.isOpen() || (this.BOF && this.EOF))
	}
	
	function _RS__resetMembers(bAll)
	{
		this._count = -1;
		this.absolutePosition = 0;
		this._bookmark = 0;
		this._bAddNew = false;
		this._bAddNewImmediate = false;
		this._bCancelUpdate = false;
		this._bFiringOnBeforeUpdate = false;
		if (typeof(bAll) != 'undefined' && bAll)
		{
			this.BOF = true;
			this.EOF = true;
			this._bExecuted = false;
			this.fields = null;
			this._strSQL = '';
			this._params = null;
		}
	}
}	// end of _RS__Prototype function


// ************************************************************************ 
// Fields Object
// ************************************************************************ 

function CreateFields(rsADO)
{
	if (typeof(rsADO) == 'object' && rsADO != null && rsADO.state != 0)
		return new _Fields(rsADO);
	return null;	
}

function _Fields(rsADO)
{	
	if (typeof(_bFSPrototypeCalled) == 'undefined')
		_FS__Prototype();

	this._rsFields = rsADO.Fields;
	this._reset(rsADO);
}

function _FS__Prototype()
{
	_Fields.prototype.getName = _FS_getName;
	_Fields.prototype.getValue = _FS_getValue;
	_Fields.prototype.setValue = _FS_setValue;
	_Fields.prototype.getCount = _FS_getCount;
	_Fields.prototype._reset = _FS__reset;
	_bFSPrototypeCalled = 1;
	//scope implementation to _FS__Prototype function

	function _FS_getName(nIndex)
	{
		if (typeof(nIndex) == 'number' && nIndex >= 0 && nIndex < this.getCount())
			return this._rsFields(nIndex).name;
		return '';
	}

	function _FS_getValue(field)
	{
		var r = '';
		if (!this._newRecord && this._isValid &&
			(typeof(field) == 'number' || typeof(field) == 'string'))
		{
			if (this._rsFields(field).value != null)
			{
				var fieldType = this._rsFields(field).type;
				//avoid JScript formating for the following field types:
				//adDate, adDBDate, adDBTime, adDBTimeStamp, adSingle
				if ((fieldType == 7) || (fieldType == 133) || (fieldType == 134) || (fieldType == 135) || (fieldType == 4))
					r = Server.HTMLEncode(this._rsFields(field));
				else
					r = this._rsFields(field).value;
			}	
		}
		return r;
	}

	function _FS_setValue(field,value)
	{
		if ((typeof(field) == 'number' || typeof(field) == 'string') && typeof(value) != 'undefined')
		{	// validate field as updatable
			if (this._isValid)
			{
				
				var nAttributes = Number(this._rsFields(field).Attributes);
				if ((nAttributes & 4) || (nAttributes & 8))
				{	// field may be updated, adFldUpdatable=4, adFldUnknownUpdatable=8
					if (value == '' && (nAttributes & 32))
					{	// field is nullable, adFldIsNullable=32
						value = null;
					}
    				this._rsFields(field).value = value;
					return true;
				}
			}
		}
		return false;
	}

	function _FS_getCount()
	{
		if (typeof(this._rsFields) == 'object')
    		return this._rsFields.count;
		return 0;
	}
	
	function _FS__reset(rsADO)
	{
		this._isValid = !(rsADO.BOF || rsADO.EOF);
		this._newRecord = false;
	}
	
}	// end of _FS__Prototype function

</SCRIPT>