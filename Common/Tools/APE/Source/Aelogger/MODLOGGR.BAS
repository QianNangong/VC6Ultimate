Attribute VB_Name = "modLogger"
Option Explicit
'-------------------------------------------------------------------------
'The project is the Logger component of the Application Performance Explorer
'The Logger is a multiuse server that objects can call to pass log records
'The logger will store the records, either in memory or in a temp file.
'The logger will then return the records the the Manager when it calls GetRecords
'
'Key Files:
'   frmLoggr.frm    Only form in app
'   clsPosFm.cls    Tool used to save form position in registry
'   Logger.cls      Multi-Use public class providing only OLE interface
'-------------------------------------------------------------------------

'API Declares
#If UNICODE Then
    Declare Function GetTempFileName Lib "Kernel32" Alias "GetTempFileNameW" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
    Declare Function GetTempPath Lib "Kernel32" Alias "GetTempPathW" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
#Else
    Declare Function GetTempFileName Lib "Kernel32" Alias "GetTempFileNameA" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
    Declare Function GetTempPath Lib "Kernel32" Alias "GetTempPathA" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
#End If
Declare Function GetTickCount Lib "Kernel32" () As Long

'Public Constants
Public Const glROWS_RETURNED_PER_GET_RECORDS As Long = 500  'Max number of records returned for
                                                        'each call of GetRecords
'Property Defaults
Public Const gbSHOW_FORM_DEFAULT As Boolean = False
Public Const gbWRITE_RECORDS_DEFAULT As Boolean = False
Public Const gbTHRESHOLD_DEFAULT As Long = 2000
Public Const glREDIM_CHUNK_SIZE As Long = 100
Public Const giNO_RECORDS As Integer = -1

'Resource string constants
Public Const giLOGGER_NAME As Integer = 2
Public Const giDISK_FULL As Integer = 3
Public Const giWRITING_TEMP_FILE As Integer = 4
Public Const giFORM_CAPTION As Integer = 5
Public Const giFONT_CHARSET_INDEX As Integer = 30
Public Const giFONT_NAME_INDEX As Integer = 31
Public Const giFONT_SIZE_INDEX  As Integer = 32

'Global Variables
Public gbShowForm As Boolean        'If true show form
Public gbWriteRecords As Boolean    'If true write records to file when Record
                                    'Threshold is reached.
Public glThreshold As Long          'Record threshold in kilobytes
Public glThresholdRecs As Long      'Record threshold in number of records
Public gsFileName As String         'FileName to write records to
Public gaRecords() As Variant       'Array used to store log records before they are written
Public glInstances As Long          'Counter of how many instances of Logger are instanciated
Public glLastAddedRecord As Long    'Last index of gaRecords that a record was added to
Public gbWritingFile As Boolean     'If true we are in WriteRecords procedure
Public gbDiskFull As Boolean        'If true Disk Full error occured
Public gbGetWrittenLogCalled As Boolean 'Get Written Log has been called by Manager
                                        'Now logger is expecting GetWrittenLog to be called
                                        'until all records are received.  The next time a record
                                        'is written the temp file will be deleated assuming that all
                                        'records were received.


Sub Main()
End Sub

Public Sub WriteRecords()
    '-------------------------------------------------------------------------
    'Purpose:   WriterRecords procedure writes all the log records currently
    '           in the global array
    'Effects:
    '           [gbGetWrittenLogCalled] becomes false
    '           The temp file is deleted if gbGetWrittenLogCalled is true
    '           [glLastAddedRecord] is set to giNO_RECORDS
    '           [gaRecords]is redimensioned to glREDIM_CHUNK_SIZE
    'Assumption:
    '           gsFileName is a valid temporary file name
    '           If gbGetWrittenLogCalled is true then all the records in
    '               the temp file have been retrieved by the manager through
    '               the GetRecords method
    '-------------------------------------------------------------------------
    
    Dim iFile As Integer        'File number
    Dim l As Long               'For...Next counter
    Dim sComponent As String    'APE Component name being written
    Dim sServiceID As String      'Service ID (Task request ID) being written
    Dim sComment As String      'Comment being written
    Dim lMilliseconds As Long   'Milliseconds being written
    
    On Error GoTo WriteRecordsError
    
    'Check to see if the contents of the temp file
    'need deleted first, the reason it is not delete
    'when the flag is flipped is to give one the chance
    'of rescueing it if the Manager fails to retreive
    'the records from it
    If gbGetWrittenLogCalled Then
        Close   'Close in case Getting log was cancelled
        Kill gsFileName
        gbGetWrittenLogCalled = False
    End If
    
    If glLastAddedRecord > giNO_RECORDS Then
        AddLogRecord LoadResString(giLOGGER_NAME), 0, LoadResString(giWRITING_TEMP_FILE), GetTickCount
        iFile = FreeFile
        Open gsFileName For Append As iFile
        'Iterate through array writing record and
        For l = 0 To glLastAddedRecord
            sComponent = gaRecords(giCOMPONENT_ELEMENT, l)
            sServiceID = gaRecords(giSERVICE_ELEMENT, l)
            sComment = gaRecords(giCOMMENT_ELEMENT, l)
            lMilliseconds = gaRecords(giMILLI_SECONDS_ELEMENT, l)
            Write #iFile, sComponent, sServiceID, sComment, lMilliseconds
            'Reset logrecord counter no after writing the first record
            'so that records are not added after the count that is being
            'written and therefore, lost.  This also protects from
            'Addlogrecord trying to write a record greater than
            'giRedimChunkSize write after gaRecords is redimensioned
            If l = 0 Then glLastAddedRecord = giNO_RECORDS
        Next
        Close iFile
        'Redimension array
        'Preserve is used because there is a potential
        'for a log record to be added after the above line
        'but before the following one
        ReDim Preserve gaRecords(giLOG_ARRAY_DIMENSION_ONE, glREDIM_CHUNK_SIZE)
    End If
    
    Exit Sub
WriteRecordsError:
    Select Case Err.Number
        Case ERR_DISK_FULL
            'Turn off logging erase array
            'leave present file for later retrieval
            DisplayStatus LoadResString(giDISK_FULL)
            Close iFile
            Erase gaRecords
            gbDiskFull = True
            Exit Sub
        Case ERR_FILE_NOT_FOUND
            'There is no temp file to kill
            Resume Next
        Case Else
            Close iFile
            Err.Raise Err.Number, Err.Source, Err.Description
            Exit Sub
    End Select
End Sub

Public Sub GetWrittenLog()
    '-------------------------------------------------------------------------
    'Purpose:   Checks to see if there is log records written to a temp file
    '           If there are it inputs it and adds it to the gaRecords array
    '           If it reaches the chunk size for passing log records it will
    '           exit the loop, leaving the file open. It is necessary to keep
    '           calling this function until no records or added.  Do not call
    '           this function more than once until the array that was filled
    '           was erased.  The external process that is calling a method that
    '           calls this procedure should be responsible for calling until
    '           all records have been attained.
    'Effects:
    '           [gbGetWrittenLogCalled] becomes true
    '           Temp file may be left open if all records are not read
    '           AddlogRecord is called for each record read
    'Assumption:
    '           If gbGetWrittenLogCalled is true then the temp file is already
    '           open, ready for the next record to be read.
    '           If the EOF is not reached before the glROWS_RETURNED_PER_GET_RECORDS
    '           is reached then the external process that called Logger.GetRecords
    '           will call it again, to get the rest of the records
    '-------------------------------------------------------------------------
    
    Static stlFile As Long      'File number of file that may be left open between calls
    Dim sComponent As String    'APE Component name that will be read from file
    Dim sServiceID As String      'Service ID that will be read from file
    Dim sComment As String      'Comment that will be read from file
    Dim lMilliseconds As Long   'Milliseconds that will be read from file
    Dim lAddedCount As Long     'Used to count how many records have been read and
                                'added to global array
    
    On Error GoTo GetWrittenLogError
    'Open file if not open yet
    If Not gbGetWrittenLogCalled Then
        'Write records in memory first to order the records
        'with any records that may have already been written
        WriteRecords
        gbGetWrittenLogCalled = True
        stlFile = FreeFile
        Open gsFileName For Input As stlFile
    End If
    
    
    Do Until EOF(stlFile)
        Input #stlFile, sComponent, sServiceID, sComment, lMilliseconds
        AddLogRecord sComponent, sServiceID, sComment, lMilliseconds
        lAddedCount = lAddedCount + 1
        'Exit here if max record size was reached
        If lAddedCount = glROWS_RETURNED_PER_GET_RECORDS Then Exit Sub
    Loop
    Close
    Exit Sub
GetWrittenLogError:
    Select Case Err.Number
        Case ERR_FILE_NOT_FOUND
            'There are no written records so exit without calling gSendLog
            Exit Sub
        Case ERR_BAD_FILE_NAME
            'We have already reached the end of the file
            'and it has been closed
            Exit Sub
        Case ERR_IPUT_PAST_EOF
            'This could occur if a temp file was artificially made that
            'had an invalid format
            Close stlFile
            Exit Sub
        Case Else
            Close stlFile
            Err.Raise Err.Number, Err.Source, Err.Description
            Exit Sub
    End Select
End Sub

Public Sub AddLogRecord(sComponent As String, sServiceID As String, sComment As String, lMilliseconds As Long)
    '-------------------------------------------------------------------------
    'Purpose:   Called to add a record to the gaRecords.
    'In:        [sComponent]    APE component name that will be added
    '           [sServiceID]    Service ID that will be added
    '           [sComment]      Comment that will be added
    '           [lMilliseconds] Milliseconds that will be added
    'Effects:   [gaRecords]     May be redimensioned (preserve) to increase
    '                           its size
    '           [glLastAddedRecord]
    '                           will be increased by one
    '-------------------------------------------------------------------------
    Dim lU As Long      'The UBound of the the 2nd dimension of gaRecords
    On Error GoTo AddLogRecordError
AddLogRecordTop:
    'If diskfull error occured immediately exit
    If gbDiskFull Then Exit Sub
    
    If glLastAddedRecord = giNO_RECORDS Then
        ReDim gaRecords(giLOG_ARRAY_DIMENSION_ONE, glREDIM_CHUNK_SIZE)
        glLastAddedRecord = 0
    Else
        lU = UBound(gaRecords, 2)
        glLastAddedRecord = glLastAddedRecord + 1
        If glLastAddedRecord > lU Then
            'Redim gaRecords to increase size
            lU = lU + glREDIM_CHUNK_SIZE
            ReDim Preserve gaRecords(giLOG_ARRAY_DIMENSION_ONE, lU)
        End If
    End If
    gaRecords(giCOMPONENT_ELEMENT, glLastAddedRecord) = sComponent
    gaRecords(giSERVICE_ELEMENT, glLastAddedRecord) = sServiceID
    gaRecords(giCOMMENT_ELEMENT, glLastAddedRecord) = sComment
    gaRecords(giMILLI_SECONDS_ELEMENT, glLastAddedRecord) = lMilliseconds
    Exit Sub
AddLogRecordError:
    Select Case Err.Number
        Case ERR_SUBSCRIPT_OUT_OF_RANGE
            'Synchronicity issues caused this
            'Got the glLastAddedRecord write before it got changed
            'but tried to put record in array right after it got redim'ed
            Dim bTried
            'If already tried raise error
            If bTried Then Err.Raise Err.Number, Err.Source, Err.Description
            bTried = True
            'Try the at the top again, getting a new glLastAddedRecord
            GoTo AddLogRecordTop
        Case Else
            Err.Raise Err.Number, Err.Source, Err.Description
    End Select
End Sub

'Puts a message in the status label
Public Sub DisplayStatus(s As String)
    '-------------------------------------------------------------------------
    'Purpose:   Displays passed string in the Logger form's status box if
    '           the form is visible.
    'Assumtions:
    '           If gbShowForm is true the form is loaded and visible
    '-------------------------------------------------------------------------
    If gbShowForm Then AlignTextToBottom frmLogger.lblStatus, s
End Sub

Public Function GetTempFile() As String
    '-------------------------------------------------------------------------
    'Purpose:   Gets a temp file name from the system
    'Return:    a valid temporary file name
    '-------------------------------------------------------------------------
    Dim lSize As Long
    Dim sPath As String
    Dim sName As String
    Dim lResult As Long
    
    sPath = Space(255)
    lResult = GetTempPath(255, sPath)
    sPath = Left$(sPath, lResult)
    sName = Space(255)
    lResult = GetTempFileName(sPath, "AEL", 0, sName)
    lResult = InStr(sName, vbNullChar)
    sName = Left$(sName, lResult - 1)
    
    GetTempFile = sName
End Function

