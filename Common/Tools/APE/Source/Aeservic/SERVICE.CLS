VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Service"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Service"
Option Explicit

Implements APEInterfaces.IService

Private Const msJetDBName = "APETest.mdb"       ' Name of Jet database used for database tests

' Text of "DRIVER" specifications of connect string for supported database types
Private Const msAccessDriver = "Microsoft Access Driver (*.mdb)"
Private Const msSQLServerDriver = "SQL Server"

Private Const miMinQueryRetryDelay As Integer = 20      ' Min delay (ms) between retries of a query that failed due to a locking contention
Private Const miMaxQueryRetryDelay As Integer = 100      ' Max delay (ms) between retries of a query that failed due to a locking contention

Private moWorkspace As Object     ' The workspace for the connection
Private moConnection As Object     ' The connection established by calling the Initialize method
Private moDatabase As DAO.Database    ' The database used for DAO operations
Private mvServiceConfiguration As Variant   ' Service configuration options (array)
Private mbConnectionOpen As Boolean     ' Flag indicating that there is a connection opened in the Initialize procedure
Private mbLogDatabaseEvents As Boolean
Private mbLogMTSTransactions As Boolean
Private mbShowMTSTransactions As Boolean
Private msServiceID As String
Private mDBType As ape_DbServerOptions      ' Type of database that the service connects to

' Handles for the ODBC API calls
Private mhEnvironment As Long
Private mhConnection As Long
Private mhStatement As Long
Const msERROR_SOURCE As String = "AEService"

Public Sub Execute(ByRef sServiceID As String, ByRef sCommand As String, Optional ByRef vServiceData As Variant, Optional ByRef vReturn As Variant)
Attribute Execute.VB_Description = "AEWorker.Worker objects call this method when using late binding."
    '-------------------------------------------------------------------------
    'Purpose:
    '   Provides an interface for late binding.  Late binding is only provided
    '   for test comparison.  Other custom services should only use the implemented
    '   interface.
    '-------------------------------------------------------------------------
    Dim bDataMissing As Boolean
    Dim bReturnMissing As Boolean
    bDataMissing = IsMissing(vServiceData)
    bReturnMissing = IsMissing(vReturn)
    If bDataMissing And bReturnMissing Then
        IService_Execute sServiceID, sCommand
    ElseIf bReturnMissing Then
        IService_Execute sServiceID, sCommand, vServiceData
    ElseIf bDataMissing Then
        IService_Execute sServiceID, sCommand, , vReturn
    Else
        IService_Execute sServiceID, sCommand, vServiceData, vReturn
    End If
End Sub

Public Sub Initialize(vServiceConfiguration As Variant)
    '-------------------------------------------------------------------------
    'Purpose:
    '   Provides an interface for late binding.  Late binding is only provided
    '   for test comparison.  Other custom services should only use the implemented
    '   interface.
    '-------------------------------------------------------------------------
    IService_Initialize vServiceConfiguration
End Sub

Private Sub IService_Execute(sServiceID As String, sCommand As String, Optional vServiceData As Variant, Optional vReturn As Variant)
    '-------------------------------------------------------------------------
    'Purpose:
    '   In response to the data it receives, it may return data of different
    '   sizes are types and/or sleep or burn processor cycles for a certain
    '   amount of time.
    'In:
    '   [sCommand]
    '       A string the represents what this procedure should do.  It is intended
    '       be used to case and call an appropriate procedure in response if this
    '       was a real solution service provider.  This object expects either
    '       "UseProcessor" or "DontUserProcessor".
    '   [vServiceData]
    '       This object expects a variant array or variant collection in which
    '       the elements of the array or collection specify how much data to
    '       return, what type of data to return, and how long to wait before
    '       releasing the calling worker.
    'Out:
    '   [vReturn]
    '       This could be a variant array or collection of any specified size
    '       it will be returned by the calling worker to the client or the
    '       expediter
    '-------------------------------------------------------------------------
    'sCommand   What to do.  Ex: "Method"
    'vServiceData      The data needed to carrry out sCommand.
    Dim iLensCommand As Integer
    Dim iChar As Integer
    Dim iLastChar As Integer
    Dim lEndTicks As Long
    Dim lRecordNumRows As Long
    Dim lRecordRowSize As Long
    Dim lRecordTaskDuration As Long, lRecordSleepPeriod As Long
    Dim lRecordContainerType As Long
    Dim sRecordDatabaseQuery As String
    Dim rsService As Recordset
    Dim s As String
    Dim i As Integer
    Dim v() As Variant
    
    msServiceID = sServiceID
    On Error GoTo Service_ExecuteError
    If Not IsMissing(vServiceData) Then
        'Are we using a Varriant Array or Collection?
        If VarType(vServiceData) = (vbArray + vbVariant) Then
            'It's an array
            lRecordNumRows = vServiceData(giRECORD_NUMROWS)
            lRecordRowSize = vServiceData(giRECORD_ROWSIZE)
            lRecordTaskDuration = vServiceData(giRECORD_TASK_DURATION)
            lRecordSleepPeriod = vServiceData(giRECORD_SLEEP_PERIOD)
            lRecordContainerType = vServiceData(giRECORD_CONTAINER_TYPE)
            sRecordDatabaseQuery = vServiceData(giRECORD_DATABASE_QUERY)
            mvServiceConfiguration = vServiceData(giRECORD_SERVICE_CONFIGURATION)
        Else
            'It's a collection
            lRecordNumRows = vServiceData.Item(CStr(giRECORD_NUMROWS))
            lRecordRowSize = vServiceData.Item(CStr(giRECORD_ROWSIZE))
            lRecordTaskDuration = vServiceData.Item(CStr(giRECORD_TASK_DURATION))
            lRecordSleepPeriod = vServiceData.Item(CStr(giRECORD_SLEEP_PERIOD))
            lRecordContainerType = vServiceData.Item(CStr(giRECORD_CONTAINER_TYPE))
            sRecordDatabaseQuery = vServiceData.Item(CStr(giRECORD_DATABASE_QUERY))
            mvServiceConfiguration = vServiceData.Item(CStr(giRECORD_SERVICE_CONFIGURATION))
        End If
        'Do we need to do anything?
        If lRecordTaskDuration < 0 Or lRecordTaskDuration > glMAX_DURATION Then Err.Raise giBAD_DURATION
    End If
    
    Dim iCommand As Integer
    iCommand = CInt(sCommand)
    
    If (iCommand And giMASK_USE_CPU_TASK) Then    'If use processor always
        If lRecordTaskDuration > 0 Then
            lEndTicks = GetTickCount() + lRecordTaskDuration
            Do While lEndTicks > GetTickCount()
                'Using Cycles
            Loop
        End If
        If lRecordSleepPeriod > 0 Then
            Sleep lRecordSleepPeriod
        End If
    End If
    
    If (iCommand And giMASK_USE_DB_TASK) Then    'If a database task
        On Error GoTo 0     ' Do not trap MTS errors or PerformQuery errors.
        If (iCommand And giMASK_WRITE_MTS_TRANSACTION) <> 0 Then
            WriteMTSTransaction mvServiceConfiguration(ape_conConnectionString), mvServiceConfiguration(ape_conConnectionOption)
        Else    ' Perform query
            PerformQuery sRecordDatabaseQuery
        End If
        On Error GoTo Service_ExecuteError
    End If
    
    If (Not IsMissing(vReturn)) And lRecordContainerType <> giCONTAINER_TYPE_NULL And lRecordNumRows <> 0 And lRecordRowSize <> 0 Then
        'Return something
        Select Case lRecordContainerType
            Case giCONTAINER_TYPE_VARRAY
                s = Space(lRecordRowSize)
                ReDim v(lRecordNumRows - 1) As Variant
                For i = 0 To lRecordNumRows - 1
                    v(i) = s
                Next i
                vReturn = v
            Case giCONTAINER_TYPE_VCOLLECTION
                Set vReturn = New Collection
                s = Space(lRecordRowSize)
                For i = 1 To lRecordNumRows
                    vReturn.Add s
                Next i
            Case giCONTAINER_TYPE_RECORDSET
                'Not yet implemented
                 Set vReturn = Nothing
            Case Else
                'Some unknown ContainterTypeValue
                Err.Raise giBAD_DATA_TYPE
         End Select
    End If
    Exit Sub
    
Service_ExecuteError:
    Dim lErrorNumber As Long
    Dim sErrorDescription As String
    lErrorNumber = Err.Number
    sErrorDescription = Err.Description
    Select Case lErrorNumber
        Case ERR_TYPE_MISMATCH, ERR_OVER_FLOW
            'vServiceData contained a bad data type.  Raise an appliction defined error.
            Err.Raise giBAD_DATA, msERROR_SOURCE, NestedErrorDescription(giERROR_EXECUTE_METHOD, LoadResString(giBAD_DATA))
        Case giBAD_DURATION
            'They wanted to sleep more than glMAX_DURATION
            Err.Raise lErrorNumber, msERROR_SOURCE, NestedErrorDescription(giERROR_EXECUTE_METHOD, _
                    ReplaceString(LoadResString(giBAD_DURATION), gsNUMBER_TOKEN, CStr(glMAX_DURATION)))
        Case Is > giERROR_THRESHOLD
            'Application defined error.  Since this is the only public method
            'all errors raised there will be returned to the calling program.
            Err.Raise lErrorNumber + vbObjectError, msERROR_SOURCE, NestedErrorDescription(giERROR_EXECUTE_METHOD, sErrorDescription)
        Case Else
            'VB error
            Err.Raise lErrorNumber, msERROR_SOURCE, NestedErrorDescription(giERROR_EXECUTE_METHOD, sErrorDescription)
    End Select
End Sub

Public Sub IService_Initialize(vServiceConfiguration As Variant)

    ' If a connection is already open, then close it (object is being reinitialized)
    If Not IsEmpty(mvServiceConfiguration) Then
        If (mvServiceConfiguration(ape_conConnectionOption) <> vServiceConfiguration(ape_conConnectionOption) Or _
                mvServiceConfiguration(ape_conConnectionString) <> vServiceConfiguration(ape_conConnectionString)) Then
            Debug.Assert mbConnectionOpen
            Select Case mvServiceConfiguration(ape_conConnectionOption)
                Case ape_idcODBC
                    SQLFreeHandle SQL_HANDLE_STMT, mhStatement
                    SQLEndTran SQL_HANDLE_DBC, mhConnection, SQL_COMMIT
                    SQLDisconnect mhConnection
                Case Else
                    If Not moConnection Is Nothing Then
                        moConnection.Close
                        Set moConnection = Nothing
                    End If
            End Select
            mbConnectionOpen = False
        End If
    End If
    mvServiceConfiguration = vServiceConfiguration
    ' Open a connection if a connection string is specified
    If Not (mbConnectionOpen Or IsEmpty(mvServiceConfiguration(ape_conConnectionString))) Then
        OpenServiceConnection
        mbConnectionOpen = True
    End If
    mbLogMTSTransactions = mvServiceConfiguration(ape_conLogMTSTransactions)
     If mbShowMTSTransactions <> mvServiceConfiguration(ape_conShowMTSTransactions) Then
        mbShowMTSTransactions = mvServiceConfiguration(ape_conShowMTSTransactions)
        If mbShowMTSTransactions Then
            frmService.Show
        Else
            Unload frmService
        End If
    End If
   mbLogDatabaseEvents = mvServiceConfiguration(ape_conLogDatabaseEvents)
   frmService.Reset
End Sub

Private Sub ExecuteADOCommand(sQuery As String)
    Dim conConnection As ADODB.Connection
    Dim rsRecordset As New ADODB.Recordset
    Dim s As String
    Dim lErrorNumber As Long
    Dim sa() As String
    Dim iRetries As Integer
     
    iRetries = 0
    On Error GoTo ADOTransactionError
    Set conConnection = moConnection

     ' Get array of queries
    If GetArrayFromDelimited(sQuery, sa, "//") Then
TryAgain:
        conConnection.BeginTrans
        Dim i As Integer
        For i = LBound(sa) To UBound(sa)
            If Left(sa(i), 6) = "SELECT" Then
               ' Only SELECT queries return records
               rsRecordset.Open sa(i), conConnection, adOpenKeyset ' Default CursorType (adOpenForwardOnly) doesn't support MoveLast
               rsRecordset.MoveFirst
               Do While Not rsRecordset.EOF
                    rsRecordset.MoveNext
                Loop
               rsRecordset.Close
           Else
               conConnection.Execute sa(i)
           End If
        Next
        conConnection.CommitTrans
    End If
    Exit Sub
    
ADOTransactionError:
    ' If the error is due to a locking contention, roll back the transaction and try again
    If Err.Number = -2147467259 And iRetries < giMAX_ALLOWED_RETRIES Then
        conConnection.RollbackTrans
        iRetries = iRetries + 1
        Sleep miMinQueryRetryDelay + (miMaxQueryRetryDelay - miMinQueryRetryDelay) * Rnd      ' Randomize the delay to avoid repeated contentions
        Resume TryAgain
    End If
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    conConnection.RollbackTrans
    Err.Raise lErrorNumber, , sErrorDescription
 End Sub
    
Private Sub ExecuteRDOCommand(sQuery As String)
    Dim conConnection As rdoConnection
    Dim rsResultset As rdoResultset
    Dim s As String
    Dim lErrorNumber As Long
    Dim sa() As String
    Dim iRetries As Integer
         
    iRetries = 0
    On Error GoTo RDOTransactionError
    
    Set conConnection = moConnection
    If GetArrayFromDelimited(sQuery, sa, "//") Then
TryAgain:
        conConnection.BeginTrans
        Dim i As Integer
        For i = LBound(sa) To UBound(sa)
            If Left(sa(i), 6) = "SELECT" Then
                ' Only SELECT queries return records
                Set rsResultset = conConnection.OpenResultset(sa(i), dbOpenDynaset)
                rsResultset.MoveFirst
                Do While Not rsResultset.EOF
                    rsResultset.MoveNext
                Loop
                rsResultset.Close
            Else
                conConnection.Execute sa(i)
            End If
        Next
        conConnection.CommitTrans
    End If
    Exit Sub

RDOTransactionError:
    ' If the error is due to a locking contention, roll back the transaction and try again
    If Err.Number = 40002 And iRetries < giMAX_ALLOWED_RETRIES Then
        ' First make sure the cause really is a locking contention
        ' Determine which database-specific error number to check for
        Select Case mDBType
            Case ape_idsJet
                lErrorNumber = -1102
            Case ape_idsSqlServer
                lErrorNumber = 1205
            Case Else
                Err.Raise E_NOTIMPL
        End Select
        Dim RDOErr As RDO.rdoError
        For Each RDOErr In rdoEngine.rdoErrors
            If RDOErr.Number = lErrorNumber Then
                conConnection.RollbackTrans
                iRetries = iRetries + 1
                Sleep miMinQueryRetryDelay + (miMaxQueryRetryDelay - miMinQueryRetryDelay) * Rnd      ' Randomize the delay to avoid repeated contentions
                Resume TryAgain
            End If
        Next
    End If
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    conConnection.RollbackTrans
    Err.Raise lErrorNumber, , sErrorDescription
End Sub

Private Sub ExecuteDAOCommand(sQuery As String)
    Dim rsRecordset As DAO.Recordset
    Dim s As String
    Dim lErrorNumber As Long
    Dim sa() As String
    Dim iRetries As Integer
         
    iRetries = 0
    On Error GoTo DAOTransactionError
    
    If GetArrayFromDelimited(sQuery, sa, "//") Then
TryAgain:
        moWorkspace.BeginTrans
        Dim i As Integer
        For i = LBound(sa) To UBound(sa)
            If Left(sa(i), 6) = "SELECT" Then
                ' Only SELECT queries return records
                Set rsRecordset = moDatabase.OpenRecordset(sa(i), dbOpenDynaset, dbSQLPassThrough)
                rsRecordset.MoveFirst
                Do While Not rsRecordset.EOF
                    rsRecordset.MoveNext
                Loop
                rsRecordset.Close
            Else
                moDatabase.Execute sa(i), dbSQLPassThrough
            End If
        Next
        moWorkspace.CommitTrans
    End If
    Exit Sub

DAOTransactionError:
    ' If the error is due to a locking contention, roll back the transaction and try again
    If Err.Number = 3146 And iRetries < giMAX_ALLOWED_RETRIES Then
        ' First make sure the cause really is a locking contention
        ' Determine which database-specific error number to check for
        Select Case mDBType
            Case ape_idsJet
                lErrorNumber = -1102
            Case ape_idsSqlServer
                lErrorNumber = 1205
            Case Else
                Err.Raise E_NOTIMPL
        End Select
        Dim DAOErr As DAO.Error
        For Each DAOErr In DBEngine.Errors
            If DAOErr.Number = lErrorNumber Then
                moWorkspace.Rollback
                iRetries = iRetries + 1
                Sleep miMinQueryRetryDelay + (miMaxQueryRetryDelay - miMinQueryRetryDelay) * Rnd      ' Randomize the delay to avoid repeated contentions
                Resume TryAgain
            End If
        Next
    End If
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    moWorkspace.Rollback
    Err.Raise lErrorNumber, , sErrorDescription
End Sub

Private Sub ExecuteODBCAPICommand(sQuery As String)
    Dim lErrorNumber As Long
    Dim sa() As String
    Dim iRetries As Integer
         
    iRetries = 0
    On Error GoTo ODBCAPITransactionError
    
    If GetArrayFromDelimited(sQuery, sa, "//") Then
TryAgain:
        Dim i As Integer
        For i = LBound(sa) To UBound(sa)
            If Left(sa(i), 6) = "SELECT" Then
                ' Only SELECT queries return records
                If Not ODBCAPICallSuccessful(SQLExecDirect(mhStatement, sa(i), Len(sa(i)))) Then
                     Err.Raise ErrorExecuteQuery, , LoadResString(ErrorExecuteQuery)
                End If
                Dim bEOF As Boolean
                Select Case SQLFetchScroll(mhStatement, SQL_FETCH_FIRST, 0)
                    Case SQL_NO_DATA
                        bEOF = True
                    Case SQL_SUCCESS, SQL_SUCCESS_WITH_INFO
                        bEOF = False
                    Case Else
                        SQLCloseCursor mhStatement
                         Err.Raise ErrorFetchRecord, , LoadResString(ErrorFetchRecord)
                End Select
                Do While Not bEOF
                    Select Case SQLFetchScroll(mhStatement, SQL_FETCH_NEXT, 0)
                        Case SQL_NO_DATA
                            bEOF = True
                        Case SQL_SUCCESS, SQL_SUCCESS_WITH_INFO
                            bEOF = False
                        Case Else
                            SQLCloseCursor mhStatement
                             Err.Raise ErrorFetchRecord, , LoadResString(ErrorFetchRecord)
                    End Select
                Loop
                SQLCloseCursor mhStatement
            Else
                If Not ODBCAPICallSuccessful(SQLExecDirect(mhStatement, sa(i), Len(sa(i)))) Then
                    Err.Raise ErrorExecuteQuery, , LoadResString(ErrorExecuteQuery)
                End If
            End If
        Next
        If Not ODBCAPICallSuccessful(SQLEndTran(SQL_HANDLE_DBC, mhConnection, SQL_COMMIT)) Then
            Err.Raise ErrorEndTransaction, , LoadResString(ErrorEndTransaction)
        End If
    End If
    Exit Sub

ODBCAPITransactionError:
    ' If the error is due to a resource deadlock, roll back the transaction and try again
    If Err.Number = ErrorExecuteQuery And iRetries < giMAX_ALLOWED_RETRIES Then
        Dim iRecNum As Integer, iTextLen As Integer
        Dim sSQLState As String * 5, sMsgText As String * 1
        Dim lNativeErrorPtr As Long
        iRecNum = 1
        Do While ODBCAPICallSuccessful(SQLGetDiagRec(SQL_HANDLE_STMT, mhStatement, iRecNum, sSQLState, lNativeErrorPtr, _
                sMsgText, 0, iTextLen))
            If sSQLState = "40001" Then
                SQLEndTran SQL_HANDLE_DBC, mhConnection, SQL_ROLLBACK
                iRetries = iRetries + 1
                Sleep miMinQueryRetryDelay + (miMaxQueryRetryDelay - miMinQueryRetryDelay) * Rnd      ' Randomize the delay to avoid repeated contentions
                Resume TryAgain
            End If
            iRecNum = iRecNum + 1
        Loop
    End If
    Dim sErrorDescription As String
    sErrorDescription = Err.Description
    lErrorNumber = Err.Number
    SQLEndTran SQL_HANDLE_DBC, mhConnection, SQL_ROLLBACK
    Err.Raise lErrorNumber, , sErrorDescription
End Sub

Private Sub PerformQuery(sRecordDatabaseQuery As String)
    ' Performs the database queries
    Dim bConnectionOpenedInExecute As Boolean
    
    On Error GoTo PerformQueryError
    bConnectionOpenedInExecute = False
    If mbLogDatabaseEvents Then
        LogEvent giBEGIN_DATABASE_QUERY, msServiceID
    End If
    If Not mbConnectionOpen Then
        On Error GoTo 0     ' Do not trap OpenServiceConnection errors.
        OpenServiceConnection
        On Error GoTo PerformQueryError
        bConnectionOpenedInExecute = True
    End If
    ParseQuery sRecordDatabaseQuery
    If mvServiceConfiguration(ape_conConnectionOption) = ape_idcODBC Then
        ExecuteODBCAPICommand (sRecordDatabaseQuery)
        If bConnectionOpenedInExecute Then
            If Not ODBCAPICallSuccessful(SQLFreeHandle(SQL_HANDLE_STMT, mhStatement)) Then
                Err.Raise ErrorFreeHandle, , LoadResString(ErrorFreeHandle)
            End If
            If Not ODBCAPICallSuccessful(SQLEndTran(SQL_HANDLE_DBC, mhConnection, SQL_COMMIT)) Then
                Err.Raise ErrorEndTransaction, , LoadResString(ErrorEndTransaction)
            End If
            If Not ODBCAPICallSuccessful(SQLDisconnect(mhConnection)) Then
                Err.Raise ErrorDisconnectDriver, , LoadResString(ErrorDisconnectDriver)
            End If
       End If
    Else
        Select Case mvServiceConfiguration(ape_conConnectionOption)
            Case ape_idcADO
                ExecuteADOCommand (sRecordDatabaseQuery)
            Case ape_idcRDO
                ExecuteRDOCommand (sRecordDatabaseQuery)
            Case ape_idcDAO
                ExecuteDAOCommand (sRecordDatabaseQuery)
            Case Else
                Err.Raise E_NOTIMPL
        End Select
        If bConnectionOpenedInExecute And Not moConnection Is Nothing Then
            moConnection.Close
            bConnectionOpenedInExecute = False
        End If
    End If
    If mbLogDatabaseEvents Then
        LogEvent giEND_DATABASE_QUERY, msServiceID
    End If
    Exit Sub
    
PerformQueryError:
    Dim lErrorNumber As Long
    Dim sErrorDescription As String
    lErrorNumber = Err.Number
    sErrorDescription = Err.Description
    If bConnectionOpenedInExecute Then
        Select Case mvServiceConfiguration(ape_conConnectionOption)
            Case ape_idcODBC
                SQLFreeHandle SQL_HANDLE_STMT, mhStatement
                SQLEndTran SQL_HANDLE_DBC, mhConnection, SQL_COMMIT
                SQLDisconnect mhConnection
            Case Else
                If Not moConnection Is Nothing Then
                    moConnection.Close
                End If
        End Select
    End If
    Err.Raise lErrorNumber, msERROR_SOURCE, NestedErrorDescription(giERROR_PERFORM_DATABASE_QUERY, sErrorDescription)
End Sub

Private Sub WriteMTSTransaction(ByVal sConnect As String, ByVal eConnectOptions As ape_DbConnectionOptions)
    ' Create the appropriate MoveMoney object
    Dim objMoveMoney As APEInterfaces.IMTSMoveMoney
    
    Const miMinQueryRetryDelay As Integer = 20      ' Min delay (ms) between retries of a query that failed due to a locking contention
    Const miMaxQueryRetryDelay As Integer = 100      ' Max delay (ms) between retries of a query that failed due to a locking contention
    Const miMAX_ALLOWED_RETRIES  As Integer = 500   ' Maximum number of retries of a query that failed due to a locking contention
    
    Dim iTransferRetries As Integer
    iTransferRetries = 0
    
    On Error GoTo ErrorHandler
    Set objMoveMoney = CreateObject("AEMTSSvc.MoveMoney")
    If mbLogMTSTransactions Then
        LogEvent giBEGIN_MTS_TRANSACTION, msServiceID
    End If
    
    Const iMAX_ACCOUNT_NO = 1000        ' Highest account number (1 is presumed to be the lowest)
    Dim lFromAccount As Long, lToAccount As Long
    lFromAccount = 1 + Int(iMAX_ACCOUNT_NO * Rnd)
    lToAccount = 1 + Int(iMAX_ACCOUNT_NO * Rnd)
    objMoveMoney.Transfer sConnect, eConnectOptions, lFromAccount, lToAccount, 1
    
    If mbLogMTSTransactions Then
        LogEvent giEND_MTS_TRANSACTION_SUCCEEDED, msServiceID
    End If
    
    If mbShowMTSTransactions Then
        frmService.MTSResults True
    End If
    Exit Sub
    
ErrorHandler:
    ' If the error is due to a locking contention, try again
    If iTransferRetries < miMAX_ALLOWED_RETRIES Then
        Dim bRetry As Boolean
        bRetry = False
        Select Case eConnectOptions
            Case ape_idcADO
                bRetry = (Err.Number = -2147467259)
            Case ape_idcDAO
                If Err.Number = 3146 Then
                    ' First make sure the cause really is a locking contention
                    Dim DAOErr As DAO.Error
                    For Each DAOErr In DBEngine.Errors
                        If DAOErr.Number = 1205 Then    ' 1205 = SQL Server record locking contention
                            bRetry = True
                        End If
                    Next
                End If
            Case ape_idcRDO
                If Err.Number = 40002 Then
                    ' First make sure the cause really is a locking contention
                    Dim RDOErr As RDO.rdoError
                    For Each RDOErr In rdoEngine.rdoErrors
                        If RDOErr.Number = 1205 Then    ' 1205 = SQL Server record locking contention
                            bRetry = True
                        End If
                    Next
                End If
            Case ape_idcODBC
                bRetry = (Err.Number = ErrorResourceDeadlock)
        End Select
        If bRetry Then
            iTransferRetries = iTransferRetries + 1
            Sleep miMinQueryRetryDelay + (miMaxQueryRetryDelay - miMinQueryRetryDelay) * Rnd      ' Randomize the delay to avoid repeated contentions
            DoEvents
            Resume
        End If
    End If
    Dim lErrorNumber As Long
    Dim sErrorSource As String
    Dim sErrorDescription As String
    lErrorNumber = Err.Number
    sErrorSource = Err.Source
    If Err.Number = ERR_CANT_CREATE_OBJECT Then
        sErrorDescription = LoadResString(giERROR_CREATE_MTS_OBJECT)
    Else
        sErrorDescription = Err.Description
    End If
    If mbLogMTSTransactions Then
        LogEvent giEND_MTS_TRANSACTION_FAILED, msServiceID
    End If
    If mbShowMTSTransactions Then
        frmService.MTSResults False
    End If
    Err.Raise lErrorNumber, sErrorSource, sErrorDescription
End Sub

Private Sub OpenServiceConnection()
    Debug.Assert Not IsEmpty(mvServiceConfiguration)    ' mvServiceConfiguration must be set before calling this method
    ' Make the current directory the app path (usually starts out being the Windows System[32] directory) which is where the
    ' Jet database is stored.
    On Error GoTo ErrorHandler
    ChDir App.Path
    ChDrive App.Path
    ' Set the database type
    If InStr(mvServiceConfiguration(ape_conConnectionString), "DRIVER={" & msSQLServerDriver & "}") > 0 Then
        mDBType = ape_idsSqlServer
    ElseIf InStr(mvServiceConfiguration(ape_conConnectionString), "DRIVER={" & msAccessDriver & "}") > 0 Then
        mDBType = ape_idsJet
    Else
        Err.Raise E_NOTIMPL
    End If
    Select Case mvServiceConfiguration(ape_conConnectionOption)
        Case ape_idcADO
            Set moConnection = New ADODB.Connection
            moConnection.Open mvServiceConfiguration(ape_conConnectionString)
        Case ape_idcRDO
            Set moWorkspace = rdoEngine.rdoEnvironments(0)
            Set moConnection = moWorkspace.OpenConnection("", rdDriverNoPrompt, , _
                     mvServiceConfiguration(ape_conConnectionString))
        Case ape_idcDAO
            Set moWorkspace = DBEngine.Workspaces(0)
            ' Special handling is required for Jet databases
            If mDBType = ape_idsJet Then
                Set moDatabase = DBEngine.OpenDatabase(msJetDBName, False, False)
            Else
                Set moDatabase = DBEngine.OpenDatabase("", False, False, "ODBC;" & mvServiceConfiguration(ape_conConnectionString))
            End If
        Case ape_idcODBC
            Dim iConnectLength As Integer
            If Not ODBCAPICallSuccessful(SQLDriverConnect(mhConnection, 0, mvServiceConfiguration(ape_conConnectionString), _
                    LenB(mvServiceConfiguration(ape_conConnectionString)), vbNullString, 0, iConnectLength, SQL_DRIVER_NOPROMPT)) Then
                Err.Raise ErrorConnectDriver, , LoadResString(ErrorConnectDriver)
            End If
            If Not ODBCAPICallSuccessful(SQLAllocHandle(SQL_HANDLE_STMT, mhConnection, mhStatement)) Then
                SQLDisconnect mhConnection
                Err.Raise ErrorAllocateHandle, , LoadResString(ErrorAllocateHandle)
            End If
            If Not ODBCAPICallSuccessful(SQLSetStmtAttrLong(mhStatement, SQL_CURSOR_TYPE, SQL_CURSOR_STATIC, 0)) Then
                SQLFreeHandle SQL_HANDLE_STMT, mhStatement
                SQLDisconnect mhConnection
                Err.Raise ErrorSetAttribute, , LoadResString(ErrorSetAttribute)
            End If
        Case Else
            Err.Raise E_NOTIMPL
    End Select
    Exit Sub
    
ErrorHandler:
    Err.Raise Err.Number, msERROR_SOURCE, NestedErrorDescription(giERROR_OPENING_SERVICE_CONNECTION, Err.Description)
End Sub

Private Sub Class_Initialize()
    Randomize
    mbConnectionOpen = False
    mbLogMTSTransactions = False
    mbShowMTSTransactions = False
    
    If Not ODBCAPICallSuccessful(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, mhEnvironment)) Then
         Err.Raise ErrorAllocateHandle, , LoadResString(ErrorAllocateHandle)
    End If
    If Not ODBCAPICallSuccessful(SQLSetEnvAttrLong(mhEnvironment, SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3, 0)) Then
        SQLFreeHandle SQL_HANDLE_ENV, mhEnvironment
        Err.Raise ErrorSetAttribute, , LoadResString(ErrorSetAttribute)
    End If
    If Not ODBCAPICallSuccessful(SQLAllocHandle(SQL_HANDLE_DBC, mhEnvironment, mhConnection)) Then
        SQLFreeHandle SQL_HANDLE_ENV, mhEnvironment
        Err.Raise ErrorAllocateHandle, , LoadResString(ErrorAllocateHandle)
    End If
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Unload frmService
    If mbConnectionOpen Then
        Select Case mvServiceConfiguration(ape_conConnectionOption)
            Case ape_idcODBC
                SQLDisconnect mhConnection
            Case ape_idcDAO
                moDatabase.Close
            Case Else
                moConnection.Close
        End Select
    End If
    
    SQLFreeHandle SQL_HANDLE_DBC, mhConnection
    SQLFreeHandle SQL_HANDLE_ENV, mhEnvironment
End Sub

