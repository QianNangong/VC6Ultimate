VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Account"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE MTS Transaction Service (Account)"
Option Explicit

Implements APEInterfaces.IMTSAccount

Private Const E_NOTIMPL = &H80004001
Private Const mlSTARTINGBALANCE = 1000000     ' Starting

Public Sub Post(sConnect As String, eConnectOptions As ape_DbConnectionOptions, lAccountNo As Long, lAmount As Long)
    '-------------------------------------------------------------------------
    'Purpose:
    '   Provides an interface for late binding.  Late binding is only provided
    '   for test comparison.  Other custom services should only use the implemented
    '   interface.
    '-------------------------------------------------------------------------
    IMTSAccount_Post sConnect, eConnectOptions, lAccountNo, lAmount
End Sub

Private Sub IMTSAccount_Post(sConnect As String, eConnectOptions As APEInterfaces.ape_DbConnectionOptions, lAccountNo As Long, lAmount As Long)
    Dim lBalance As Long
    Dim sSQLUpdate As String
    Dim sSQLBalance As String
    
    ' Get our object context
    Dim ctxObject As ObjectContext
    Set ctxObject = GetObjectContext()
    
    sSQLUpdate = "UPDATE Account SET Balance = Balance + " + Str$(lAmount) + " WHERE AccountNo = " + Str$(lAccountNo)
    sSQLBalance = "SELECT Balance FROM Account WHERE AccountNo = " + Str$(lAccountNo)
    On Error GoTo PostError
    
    Select Case eConnectOptions
        Case ape_DbConnectionOptions.ape_idcADO
            If lAmount < 0 Then     ' If debit, then get balance
                PostADO sConnect, sSQLUpdate, sSQLBalance, lBalance
            Else
                PostADO sConnect, sSQLUpdate, sSQLBalance
            End If
        Case ape_DbConnectionOptions.ape_idcRDO
            If lAmount < 0 Then     ' If debit, then get balance
                PostRDO sConnect, sSQLUpdate, sSQLBalance, lBalance
            Else
                PostRDO sConnect, sSQLUpdate, sSQLBalance
            End If
        Case ape_DbConnectionOptions.ape_idcDAO
            If lAmount < 0 Then     ' If debit, then get balance
                PostDAO sConnect, sSQLUpdate, sSQLBalance, lBalance
            Else
                PostDAO sConnect, sSQLUpdate, sSQLBalance
            End If
        Case ape_DbConnectionOptions.ape_idcODBC
            If lAmount < 0 Then     ' If debit, then get balance
                PostODBCAPI sConnect, sSQLUpdate, sSQLBalance, lBalance
            Else
                PostODBCAPI sConnect, sSQLUpdate, sSQLBalance
            End If
        Case Else
            Err.Raise E_NOTIMPL
    End Select
    If lAmount < 0 And lBalance < 0 Then   ' If a debit and acount is overdrawn
        IMTSAccount_Post sConnect, eConnectOptions, lAccountNo, mlSTARTINGBALANCE         ' Give a new starting balance
    End If

    ctxObject.SetComplete          ' Transaction completed
    Exit Sub

PostError:
    Dim lErrorNumber As Long
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    ctxObject.SetAbort              ' Transaction aborted
    ' Need to explicitly set the error source to AEMTSSvc
    Err.Raise lErrorNumber, "AEMTSSvc", LoadResString(ERROR_POSTING_TO_ACCOUNT) & " (" & sErrorDescription & ")"
End Sub

Private Sub PostADO(sConnect As String, sSQLUpdate As String, sSQLBalance As String, Optional lBalance As Long)
    ' If adoConnection throws an exception
    On Error GoTo PostadoError
    
    ' Obtain the ADO connection
    Dim adoConn As New ADODB.Connection
    adoConn.Open sConnect
    
    ' Update the balance
    adoConn.Execute sSQLUpdate
    If Not IsMissing(lBalance) Then
        ' Get resulting balance which may have been further updated via triggers
        Dim adoRS As ADODB.Recordset
        Set adoRS = adoConn.Execute(sSQLBalance)
        If Not adoRS.EOF Then
            lBalance = adoRS.Fields("Balance").Value
        Else
            Err.Raise errInvalidAccount
        End If
        adoRS.Close
    End If
    
    adoConn.Close
    Exit Sub

PostadoError:
    Dim lErrorNumber As Long
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    If Not adoRS Is Nothing Then
        adoRS.Close
    End If
    If Not adoConn Is Nothing Then
        adoConn.Close
    End If
    If lErrorNumber <> 0 Then
        Err.Raise lErrorNumber, , sErrorDescription
    End If
End Sub

Private Sub PostRDO(sConnect As String, sSQLUpdate As String, sSQLBalance As String, Optional lBalance As Long)
    ' If rdoConnection throws an exception
    On Error GoTo PostRDOError
    
    ' Obtain the RDO environment and connection
    Dim rdoConn As rdoConnection
    Set rdoConn = rdoEngine.rdoEnvironments(0).OpenConnection("", rdDriverNoPrompt, False, sConnect)

    ' Update the balance
    rdoConn.Execute sSQLUpdate
  
    If Not IsMissing(lBalance) Then
        ' Get resulting balance which may have been further updated via triggers
        Dim rdoRS As rdoResultset
        Set rdoRS = rdoConn.OpenResultset(sSQLBalance)
        If Not rdoRS.EOF Then
            lBalance = rdoRS.rdoColumns("Balance")
        Else
            Err.Raise errInvalidAccount
        End If
        rdoRS.Close
    End If
    
    rdoConn.Close
    Exit Sub

PostRDOError:
    Dim lErrorNumber As Long
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    If Not rdoRS Is Nothing Then
        rdoRS.Close
    End If
    If Not rdoConn Is Nothing Then
        rdoConn.Close
    End If
    If lErrorNumber <> 0 Then
        Err.Raise lErrorNumber, , sErrorDescription
    End If
End Sub

Private Sub PostDAO(sConnect As String, sSQLUpdate As String, sSQLBalance As String, Optional lBalance As Long)
    ' If daoConnection throws an exception
    On Error GoTo PostDAOError
    
    ' Obtain the DAO workspace and connection
    Dim daoWorkspace As Workspace
    Dim daoConn As Connection
    Set daoWorkspace = CreateWorkspace("", "", "", dbUseODBC)
    Set daoConn = daoWorkspace.OpenConnection("", dbDriverNoPrompt, False, "ODBC;" & sConnect)

    ' Update the balance
    daoConn.Execute sSQLUpdate
  
    If Not IsMissing(lBalance) Then
        ' Get resulting balance which may have been further updated via triggers
        Dim daoRS As Recordset
        Set daoRS = daoConn.OpenRecordset(sSQLBalance)
        If Not daoRS.EOF Then
            lBalance = daoRS.Fields("Balance").Value
        Else
            Err.Raise errInvalidAccount
        End If
        daoRS.Close
    End If
    
    daoConn.Close
    Exit Sub

PostDAOError:
    Dim lErrorNumber As Long
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    If Not daoRS Is Nothing Then
        daoRS.Close
    End If
    If Not daoConn Is Nothing Then
        daoConn.Close
    End If
    If lErrorNumber <> 0 Then
        Err.Raise lErrorNumber, , sErrorDescription
    End If
End Sub

Private Sub PostODBCAPI(sConnect As String, sSQLUpdate As String, sSQLBalance As String, Optional lBalance As Long)
    ' Handles for the ODBC API calls
    Dim hEnvironment As Long
    Dim hConnection As Long
    Dim hStatement As Long
    Dim iConnectLength As Integer

    On Error GoTo PostODBCAPIError
    If Not ODBCAPICallSuccessful(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, hEnvironment)) Then
         Err.Raise ErrorAllocateHandle, , LoadResString(ErrorAllocateHandle)
    End If
    If Not ODBCAPICallSuccessful(SQLSetEnvAttrLong(hEnvironment, SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3, 0)) Then
        Err.Raise ErrorSetAttribute, , LoadResString(ErrorSetAttribute)
    End If
    If Not ODBCAPICallSuccessful(SQLAllocHandle(SQL_HANDLE_DBC, hEnvironment, hConnection)) Then
        Err.Raise ErrorAllocateHandle, , LoadResString(ErrorAllocateHandle)
    End If

    If Not ODBCAPICallSuccessful(SQLDriverConnect(hConnection, 0, sConnect, _
            LenB(sConnect), vbNullString, 0, iConnectLength, SQL_DRIVER_NOPROMPT)) Then
        Err.Raise ErrorConnectDriver, , LoadResString(ErrorConnectDriver)
    End If
    If Not ODBCAPICallSuccessful(SQLAllocHandle(SQL_HANDLE_STMT, hConnection, hStatement)) Then
        Err.Raise ErrorAllocateHandle, , LoadResString(ErrorAllocateHandle)
    End If
    If Not ODBCAPICallSuccessful(SQLSetStmtAttrLong(hStatement, SQL_CURSOR_TYPE, SQL_CURSOR_STATIC, 0)) Then
        Err.Raise ErrorSetAttribute, , LoadResString(ErrorSetAttribute)
    End If
    
    If Not ODBCAPICallSuccessful(SQLExecDirect(hStatement, sSQLUpdate, Len(sSQLUpdate))) Then
        ' See if the error is due to a resource deadlock
        Dim iRecNum As Integer, iTextLen As Integer
        Dim sSQLState As String * 5, sMsgText As String * 1
        Dim lNativeErrorPtr As Long
        iRecNum = 1
        Do While ODBCAPICallSuccessful(SQLGetDiagRec(SQL_HANDLE_STMT, hStatement, iRecNum, sSQLState, lNativeErrorPtr, _
                sMsgText, 0, iTextLen))
            If sSQLState = "40001" Then
                Err.Raise ErrorResourceDeadlock, , LoadResString(ErrorResourceDeadlock)
            End If
            iRecNum = iRecNum + 1
        Loop
        ' Else, raise generic error
        Err.Raise ErrorExecuteQuery, , LoadResString(ErrorExecuteQuery)
    End If
    
    If Not IsMissing(lBalance) Then
        ' Get resulting balance
        If Not ODBCAPICallSuccessful(SQLExecDirect(hStatement, sSQLBalance, Len(sSQLBalance))) Then
             Err.Raise ErrorExecuteQuery, , LoadResString(ErrorExecuteQuery)
        End If
        
        Select Case SQLFetchScroll(hStatement, SQL_FETCH_FIRST, 0)
            Case SQL_NO_DATA
                Err.Raise ErrorFetchRecord, , LoadResString(ErrorFetchRecord)
            Case SQL_SUCCESS, SQL_SUCCESS_WITH_INFO
                ' Do nothing
            Case Else
                 Err.Raise ErrorFetchRecord, , LoadResString(ErrorFetchRecord)
        End Select

        Dim lBalanceLength As Long
        If Not ODBCAPICallSuccessful(SQLGetDataLong(hStatement, 1, SQL_INTEGER, lBalance, 0, lBalanceLength)) Then
             Err.Raise ErrorGetData, , LoadResString(ErrorGetData)
        End If
    End If
        
PostODBCAPIError:
    Dim lErrorNumber As Long
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    SQLCloseCursor hStatement
    SQLFreeHandle SQL_HANDLE_STMT, hStatement
    SQLDisconnect hConnection
    SQLFreeHandle SQL_HANDLE_DBC, hConnection
    SQLFreeHandle SQL_HANDLE_ENV, hEnvironment
    If lErrorNumber <> 0 Then
        Err.Raise lErrorNumber, , sErrorDescription
    End If
End Sub
