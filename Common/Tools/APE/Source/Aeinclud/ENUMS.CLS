VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "Enums"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

' APE Component Types
Public Enum ape_ComponentTypes
    ape_ctManager
    ape_ctClient
    ape_ctWorker
    ape_ctService
    ape_ctMTSService
    ape_ctQueueManager
    ape_ctPoolManager
    ape_ctLogger
    ape_ctServerManager
End Enum

'ClientOptions Dialog
Public Enum ape_CliTestDurationOptions
    ape_ictdUntilStopped = 0
    ape_ictdNumCalls = 1
    ape_ictdNumMinutes = 2
End Enum

Public Enum ape_CliLocalComponentOptions
    ape_iclrLocalActiveX = 0
    ape_iclrLocalJava = 1
End Enum

Public Enum ape_CliClientTypeOptions
    ape_icatWinClient = 0
    ape_icatWebClient = 1
End Enum

Public Enum ape_CliDataOptions
    ape_icdtActiveResultSet = 0
    ape_icdtUdtArray = 1
    ape_icdtVarArray = 2
    ape_icdtVarCollection = 3
End Enum

Public Enum ape_CliCallbackOptions
    ape_icctOnlyOnce = 0
    ape_icctEveryRequest = 1
    ape_icctUseEventSinking = 2
End Enum

'Service Connection Options Dialog
Public Enum ape_SvcConnOptions
    ape_iscDCOM = 0
    ape_iscASP = 1
    ape_iscRA = 2
End Enum

Public Enum ape_SvcRaConnOptions
    ape_iscrNetBIOS_TCP = 0
    ape_iscrNetBIOS_SPX = 1
    ape_iscrNetBIOS_NetBEUI = 2
    ape_iscrTCPIP = 3
    ape_iscrSPX = 4
    ape_iscrNamedPipes = 5
    ape_iscrDECnetTransport = 6
    ape_iscrDatagram_UDP = 7
    ape_iscrDatagram_IPX = 8
End Enum

'ServiceOptions Dialog
Public Enum ape_SvcDbTaskOptions
    ape_isdtMTS = 0
    ape_isdtOLAP = 1
    ape_isdtQuery = 2
End Enum

Public Enum ape_SvcPoolResOptions
    ape_isprJobMngr = 0
    ape_isprPoolMngr = 1
End Enum

Public Enum ape_SvcLanguageOptions
    ape_islLangVB = 0
    ape_islLangVC = 1
    ape_islLangVJ = 2
End Enum

'Database Connection Options Dialog
Public Enum ape_DbConnectionOptions
    ape_idcADO = 0
    ape_idcRDO = 1
    ape_idcDAO = 2
    ape_idcODBC = 3
    ape_idcOracle = 4
End Enum

'DatabaseServerOptions Dialog
Public Enum ape_DbServerOptions
    ape_idsJet = 0
    ape_idsSqlServer = 1
    ape_idsOracle = 2
    ape_idsOther = 3
End Enum

'AdminOptions Dialog
Public Enum ape_AdmWhenToWriteLogOptions
    ape_iawlEndOfTest = 0
    ape_iawlLogSizeLimit = 1
End Enum

Public Enum ape_UpdateDisplayOptions
    ape_udoAll = 0
    ape_udoClient = 1
    ape_udoServiceConnection = 2
    ape_udoService = 3
    ape_udoDatabaseConnection = 4
    ape_udoDatabase = 5
End Enum

Public Enum ape_CallResultCodes
    ape_retSuccess = 0
    ape_retFailure
    ape_retBadSendDataValues
    ape_retBadReturnDataValues
    ape_retInvalidParameter
    ape_retPrevTestInProgress
    ape_retInvalidCallbackObject
End Enum

' Error Codes
Public Enum MTSSvcErrors
    errAccountCreateFailed = vbObjectError + 1
    errAccountTransactionFailed
    errInsufficientFunds
    errInvalidAccount
    errDatabaseOperationFailed
End Enum

'Service component configuration
Public Enum ServiceConfigurationInfomation
    FirstMember
    ape_conConnectionString
    ape_conConnectionOption
    ape_conLogMTSTransactions
    ape_conShowMTSTransactions
    ape_conLogDatabaseEvents
    LastMember
End Enum
