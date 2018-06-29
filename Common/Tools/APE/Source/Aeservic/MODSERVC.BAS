Attribute VB_Name = "modService"
Option Explicit
'-------------------------------------------------------------------------
'The project is a default Service object provided with APE.
'It is loaded and called by the Worker.  There are
'configurations for this object exposed on the AEManager U/I.  All it does
'is resond to an Execute method.  In response to the data it receives, it
'may return data of different sizes are types and/or sleep or burn processor
'cycles for a certain amount of time.
'-------------------------------------------------------------------------

'Declares
Public Declare Function GetTickCount Lib "Kernel32" () As Long
Public Declare Sub Sleep Lib "Kernel32" (ByVal dwMilliseconds As Long)

'General Service Constants
Public Const glMAX_DURATION As Long = 60000             'The longest an Service is allowed to take

'Service Error Constants
Public Const giERROR_THRESHOLD As Long = 32000          'Any error above this number is application defined
Public Const giTIMER_INTERVAL As Integer = 500
Public Const giMAX_ALLOWED_RETRIES  As Integer = 500
Public Const giRETRY_WAIT_MIN  As Integer = 500      'Retry Wait is measure in DoEvent cyles
Public Const giRETRY_WAIT_MAX  As Integer = 2500

'Also used to lookup error strings from the RES file
'The actual error number raised to the calling program
'is "cont + vbObjectError"
Public Const giBAD_DATA As Long = 32766
Public Const giBAD_DURATION As Long = 32765
Public Const giBAD_DATA_TYPE As Long = 32764

'Data Access constants
Public Const gsDBName As String = "AEServic.mdb"
Public Const gsFIND_CRITERIA As String = "OrderID=1"
Public Const gsFIELD_TO_READ As String = "Balance"
 
Public Const gsERROR_PREFIX As String = "Error: "
Public Const giServiceName As Integer = 2
Public Const giBEGIN_MTS_TRANSACTION As Integer = 3
Public Const giEND_MTS_TRANSACTION_FAILED As Integer = 4
Public Const giEND_MTS_TRANSACTION_SUCCEEDED As Integer = 5
Public Const giMTS_FORM_CAPTION As Integer = 6
Public Const giSUCCEEDED_TRANSACTIONS_CAPTION As Integer = 7
Public Const giABORTED_TRANSACTIONS_CAPTION As Integer = 8
Public Const giBEGIN_DATABASE_QUERY As Integer = 9
Public Const giEND_DATABASE_QUERY  As Integer = 10

Public Const giERROR_OPENING_SERVICE_CONNECTION As Integer = 20
Public Const giERROR_EXECUTE_METHOD As Integer = 21
Public Const giERROR_PERFORM_DATABASE_QUERY As Integer = 22
Public Const giERROR_PARSING_QUERY As Integer = 23
Public Const giERROR_CREATE_MTS_OBJECT As Integer = 24

Public Const giFONT_CHARSET_INDEX As Integer = 30
Public Const giFONT_NAME_INDEX As Integer = 31
Public Const giFONT_SIZE_INDEX  As Integer = 32

Public goLogger As APEInterfaces.ILogger

Public Const RPC_E_CALL_REJECTED = &H80010001

Sub Main()
    Set goLogger = CreateObject("AELogger.Logger")
End Sub

Public Sub LogEvent(intMessage As Integer, sServiceID As String)
    '-------------------------------------------------------------------------
    'Purpose:   Receives Message key which is used to look
    '           up a resource string.  The logrecord is sent to the
    '           Logger object if gbLog is true
    'In:        [intMessage]
    '               A valid Resource string key for the message to be logged
    '           [sServiceID]
    '               Service Request ID to be logged
    'Assumption:
    '           If gbLog is true then goLogger is a valid reference to
    '           AELogger.Logger class object
    '-------------------------------------------------------------------------
    
    On Error GoTo LogEventError

    goLogger.Record LoadResString(giServiceName), sServiceID, LoadResString(intMessage), GetTickCount()
    Exit Sub
LogEventError:
    Select Case Err.Number
        Case RPC_E_CALL_REJECTED
               'Collision error, the OLE server is busy
               Dim iRetry As Integer
               Dim il As Integer
               Dim ir As Integer
               If iRetry < giMAX_ALLOWED_RETRIES Then
                   iRetry = iRetry + 1
                   ir = Int((giRETRY_WAIT_MAX - giRETRY_WAIT_MIN + 1) * Rnd + giRETRY_WAIT_MIN)
                   For il = 0 To ir
                       DoEvents
                   Next il
                   Resume
               Else
                   'We reached our max retries
                   'This would occur when clients are sending
                   'there logs
                   LogError Err, sServiceID
                   Exit Sub
               End If
        Case Else
            LogError Err, sServiceID
            Exit Sub
    End Select
    Exit Sub
End Sub

Public Sub LogError(ByVal oErr As ErrObject, sServiceID As String)
    '-------------------------------------------------------------------------
    'Purpose:   To add an error record to the AELogger.Logger object
    'In:        [oErr]
    '               Error object which hold the error information that will
    '               form the log record
    '           [sServiceID]
    '               The ID of the Service or Task request that will form part of
    '               the log record
    'Assumption:
    '           goLogger is a valid AELogger.Logger object
    '-------------------------------------------------------------------------
    goLogger.Record LoadResString(giServiceName), sServiceID, gsERROR_PREFIX & Str$(oErr.Number) & gsSEPERATOR & oErr.Source & gsSEPERATOR & oErr.Description, GetTickCount()
End Sub

Public Function NestedErrorDescription(iMainDescriptionResourceID As Integer, sNestedErrorDescription As String) As String
    'Creates a nested string for error descriptions.
    NestedErrorDescription = LoadResString(iMainDescriptionResourceID) & " (" & sNestedErrorDescription & ")"
End Function

Public Sub ParseQuery(sQuery As String)
' Parses the specified query string and performs substitutions such as random numbers where required.
' Currently supported substitutions are:
'   %<min>-<max>%       = a random value in the range of <min> through <max> (e.g., "%1-1000%")
    Dim lStart As Long, lEnd As Long, lHyphenPos As Long
    Dim lMin As Long, lMax As Long
    Dim sToken As String

    On Error GoTo ParseError
    lStart = InStr(sQuery, "%")     ' Find starting %
    Do While lStart > 0
        lEnd = InStr(lStart + 1, sQuery, "%")       ' Find ending %
        If lEnd = 0 Then
            Exit Do
        End If
        sToken = Mid$(sQuery, lStart + 1, lEnd - lStart - 1)
        ' Check if the token specifies a random range
        lHyphenPos = InStr(sToken, "-")
        If lHyphenPos > 0 Then
            lMin = Left$(sToken, lHyphenPos - 1)
            lMax = Mid$(sToken, lHyphenPos + 1)
            sQuery = Left$(sQuery, lStart - 1) & Int(Min(lMin, lMax) + Rnd * (Abs(lMax - lMin) + 1)) & Mid$(sQuery, lEnd + 1)      ' Perform substitution
        End If
        lStart = InStr(lEnd + 1, sQuery, "%")     ' Find starting %
    Loop
    Exit Sub
    
ParseError:
    Err.Raise Err.Number, , LoadResString(giERROR_PARSING_QUERY)
End Sub
