Attribute VB_Name = "modAEConstants"
Option Explicit
'-------------------------------------------------------------------------
'This Module provides constants shared by multiple APE Components
'-------------------------------------------------------------------------

Public Const giDEFAULT_FORM_WIDTH  As Integer = 4000
Public Const giDEFAULT_FORM_HEIGHT  As Integer = 2500
Public Const giFORM_MARGIN As Integer = 75      'Used to size and position
                                                'controls in a sizable form and keep
                                                'consitency between forms
Public Const giLIST_BOX_MAX As Integer = 1000   'Used to control the length of a list box

Public Const gsLOG_FILE_EXTENSION As String = ".LOG"
'Service command constants
Public Const gsSERVICE_USE_PROCESSOR As String = "UseProcessor"
Public Const gsSERVICE_DONT_USE_PROCESSOR As String = "DontUseProcessor"
Public Const gsSERVICE_READ_DATA As String = "ReadData"
Public Const gsSERVICE_WRITE_DATA As String = "WriteData"
Public Const gsSERVICE_READWRITE_DATA As String = "ReadWriteData"
Public Const gsSERVICE_WRITE_MTS_TRANSACTIONS As String = "WriteMTSTransactions"

Public Const gsSERVICE_LIB_CLASS As String = "AEService.Service"
Public Const glSERVICE_MAX_DURATION As Long = 60000             'The longest an Service is allowed to take
Public Const giLOG_RECORD_KILOBYTES As Integer = 3  'Estimated number of log records in a KB

'Record Constants
Public Const giRECORD_NUMROWS As Integer = 0
Public Const giRECORD_ROWSIZE As Integer = 1
Public Const giRECORD_TASK_DURATION As Integer = 2
Public Const giRECORD_SLEEP_PERIOD As Integer = 3
Public Const giRECORD_CONTAINER_TYPE As Integer = 4
Public Const giRECORD_DATABASE_QUERY As Integer = 5
Public Const giRECORD_SERVICE_CONFIGURATION As Integer = 6
Public Const giRECORD_DATA_BEGIN As Integer = 7

'Return Container Constants
Public Const giCONTAINER_TYPE_NULL As Integer = 0
Public Const giCONTAINER_TYPE_VARRAY As Integer = 1
Public Const giCONTAINER_TYPE_VCOLLECTION As Integer = 2
Public Const giCONTAINER_TYPE_RECORDSET As Integer = 3

Public Const gsSEPERATOR As String = " - "

Public Const giMODEL_QUEUE As Integer = 0
Public Const giMODEL_POOL As Integer = 1
Public Const giMODEL_DIRECT As Integer = 2

'Service Task Option bit field mask values
Public Const giMASK_USE_DB_TASK As Integer = 2 ^ 0
Public Const giMASK_WRITE_MTS_TRANSACTION As Integer = 2 ^ 1    ' Toggle bit - 0 => Perform database query
Public Const giMASK_USE_CPU_TASK As Integer = 2 ^ 2

'Test Duration mode constants
Public Const giTEST_DURATION_CONTINUE As Integer = 0        'Continue the test until interupted by StopTest
Public Const giTEST_DURATION_CALLS As Integer = 1           'Continue the test for specified number of calls
Public Const giTEST_DURATION_TICKS As Integer = 2           'Continue the test for specified number of milliseconds

'Return value of clsQueueDelegator.GetServiceRequest method that instructs Worker to
'Close.  This is returned instead of Service Request Data
Public Const giCLOSE_WORKER_NOW As Integer = -1

'Log Record array elements
'Represents the element definition of the first dimension
'of a two dimensional array passed to the AEManager.clsExplorer
'by clients and the logger
Public Const giCOMPONENT_ELEMENT As Integer = 0
Public Const giSERVICE_ELEMENT  As Integer = 1
Public Const giCOMMENT_ELEMENT As Integer = 2
Public Const giMILLI_SECONDS_ELEMENT As Integer = 3
Public Const giLOG_ARRAY_DIMENSION_ONE As Integer = 3

'Worker Property array elements
'For passing properties from
'ServerMgr to Manager
Public Const giLOG_WORKER_ELEMENT As Integer = 0
Public Const giEARLYBIND_SERVICES_ELEMENT As Integer = 1
Public Const giPERSISTENT_SERVICES_ELEMENT As Integer = 2
Public Const giPRELOAD_SERVICES_ELEMENT  As Integer = 3

'Service Request Data array elements
'For passing Service data from
'QueueMgr to worker
Public Const giSERVICE_ID_ELEMENT  As Integer = 0
Public Const giCOMMAND_ELEMENT  As Integer = 1
Public Const giSERVICE_DATA_ELEMENT As Integer = 2
Public Const giDATA_PRESENT_ELEMENT As Integer = 3

'Service Results Data array elements
'For passing Service data from the
'QueueMgr to the Expediter
Public Const giRESULT_ID_ELEMENT As Integer = 0
Public Const giRESULT_CALLBACK_ELEMENT As Integer = 1
Public Const giRESULT_DATA_ELEMENT As Integer = 2
Public Const giRESULT_ERROR_ELEMENT As Integer = 3
Public Const giRESULT_CALLBACK_TYPE_ELEMENT As Integer = 4
Public Const giRESULT_DIMENSION_ONE As Integer = 4

'Performance Statistics array elements
'Array returned by GetStatistics method
'of AEClient.Client. Called by AEManager
Public Const giNUM_CALLS_ELEMENT As Integer = 0
Public Const giBEGIN_TICKS_ELEMENT  As Integer = 1
Public Const giEND_TICKS_ELEMENT  As Integer = 2
Public Const giSTAT_ARRAY_DIMENSION  As Integer = 2

'RacReg GetAutoServerSettings array elements
Public Const giREMOTE_ELEMENT As Integer = 1
Public Const giADDRESS_ELEMENT  As Integer = 2
Public Const giPROTOCOL_ELEMENT  As Integer = 3
Public Const giAUTHENTICATION_ELEMENT  As Integer = 4
Public Const giNET_OLE_ELEMENT As Integer = 5
Public Const giFIRST_RACREG_ELEMENT  As Integer = 1
Public Const giLAST_RACREG_ELEMENT  As Integer = 5

'Callback mode keys
Public Const giNO_CALLBACK As Integer = 0
Public Const giUSE_PASSED_CALLBACK As Integer = 1
Public Const giUSE_DEFAULT_CALLBACK As Integer = 2
Public Const giRETURN_BY_SYNC_EVENT As Integer = 3

'Resource String replacement tokens
Public Const gsNUMBER_TOKEN As String = "<NUMBER>"
Public Const gsNAME_TOKEN As String = "<NAME>"

'Automation errors
Public Const E_INVALIDARG = &H80070057
Public Const E_NOTIMPL = &H80004001
Public Const E_UNEXPECTED = &H8000FFFF

' Miscellaneous constants
Public Const gsODBC_INI_REG_KEY = "Software\ODBC\ODBC.INI"      ' Registry path to DSNs
Public Const gsREGISTRY_KEY As String = "Software\Microsoft\VSEE\APE"
Public Const gsNULL_SERVICE_ID As String = "-"      ' Null Service ID

'MRU server name constants
Public Const giMAX_MRU_SIZE As Integer = 8
Public Const giMAX_REG_DATA_LENGTH As Integer = 200   ' Maximum length of registry data string
Public Const glMAX_NAME_LENGTH As Long = 250   ' Max length for a server name
Public Const CB_LIMITTEXT = &H141

' Shared custom error constants
Public Const giRPC_ERROR_ACCESSING_COLLECTION As Integer = 32740

