Attribute VB_Name = "ODBCAPI"
Option Explicit

Public Declare Function SQLAllocHandle Lib "odbc32.dll" (ByVal iHandleType As Integer, ByVal lInputHandle As Long, lOutputHandlePtr As Long) As Integer
Public Declare Function SQLFreeHandle Lib "odbc32.dll" (ByVal iHandleType As Integer, ByVal lHandle As Long) As Integer

Public Declare Function SQLDriverConnect Lib "odbc32.dll" (ByVal hConnection As Long, ByVal hWnd As Long, ByVal sInConnectionString As String, ByVal iStringLength1 As Integer, ByVal sOutConnectionString As String, ByVal iBufferLength As Integer, iStringLength2Ptr As Integer, ByVal iDriverCompletion As Integer) As Integer
Public Declare Function SQLDisconnect Lib "odbc32.dll" (ByVal hdbc As Long) As Integer

Public Declare Function SQLSetEnvAttr Lib "odbc32.dll" (ByVal hEnv As Long, ByVal lAttribute As Long, ByVal sValuePtr As String, ByVal lStringLength As Long) As Integer
Public Declare Function SQLSetEnvAttrLong Lib "odbc32.dll" Alias "SQLSetEnvAttr" (ByVal hEnv As Long, ByVal lAttribute As Long, ByVal lValue As Long, ByVal lStringLength As Long) As Integer
Public Declare Function SQLExecDirect Lib "odbc32.dll" (ByVal hstmt As Long, ByVal szSqlStr As String, ByVal cbSqlStr As Long) As Integer
Public Declare Function SQLEndTran Lib "odbc32.dll" (ByVal iHandleType As Integer, ByVal hConnection As Long, ByVal iCompletionType As Integer) As Integer
Public Declare Function SQLFetch Lib "odbc32.dll" (ByVal hstmt As Long) As Integer
Public Declare Function SQLFetchScroll Lib "odbc32.dll" (ByVal hStatement As Long, ByVal iFetchOrientation As Integer, ByVal FetchOffset As Long) As Integer
Public Declare Function SQLSetStmtAttrLong Lib "odbc32.dll" Alias "SQLSetStmtAttr" (ByVal hStatement As Long, ByVal lAttribute As Long, ByVal lValue As Long, ByVal lStringLength As Long) As Integer
Public Declare Function SQLCloseCursor Lib "odbc32.dll" (ByVal hStatement As Long) As Integer
Public Declare Function SQLGetDataLong Lib "odbc32.dll" Alias "SQLGetData" (ByVal hStatement As Long, ByVal iColumn As Integer, ByVal iTargetType As Integer, lValue As Long, ByVal lValueLength As Long, lActualLen As Long) As Integer
Public Declare Function SQLGetDiagRec Lib "odbc32.dll" (ByVal iHandleType As Integer, ByVal hHandle As Long, ByVal iRecNumber As Integer, ByVal sSQLState As String, lNativeErrorPtr As Long, ByVal sMessageText As String, ByVal iBufferLength As Integer, iTextLengthPtr As Integer) As Integer

' Options for SQLAllocHandle
Public Const SQL_HANDLE_ENV = 1
Public Const SQL_HANDLE_DBC = 2
Public Const SQL_HANDLE_STMT = 3
Public Const SQL_HANDLE_DESC = 4
Public Const SQL_NULL_HANDLE = 0&

Public Const SQL_ATTR_ODBC_VERSION = 200
Public Const SQL_OV_ODBC3 = 3&

' Options for SQLDriverConnect
Public Const SQL_DRIVER_NOPROMPT As Long = 0
Public Const SQL_DRIVER_COMPLETE As Long = 1
Public Const SQL_DRIVER_PROMPT As Long = 2
Public Const SQL_DRIVER_COMPLETE_REQUIRED As Long = 3

' Options for SQLEndTran
Public Const SQL_COMMIT = 0
Public Const SQL_ROLLBACK = 1

' Options for SQLFetchScroll
Public Const SQL_FETCH_NEXT = 1
Public Const SQL_FETCH_FIRST = 2
Public Const SQL_FETCH_LAST = 3
Public Const SQL_FETCH_PRIOR = 4
Public Const SQL_FETCH_ABSOLUTE = 5
Public Const SQL_FETCH_RELATIVE = 6

'  RETCODEs
Public Const SQL_SUCCESS As Long = 0
Public Const SQL_SUCCESS_WITH_INFO As Long = 1
Public Const SQL_ERROR As Long = -1
Public Const SQL_INVALID_HANDLE As Long = -2
Public Const SQL_NO_DATA  As Long = 100

' Statement attributes
Public Const SQL_ATTR_CURSOR_SCROLLABLE = -1
Public Const SQL_ATTR_CURSOR_SENSITIVITY = -2
Public Const SQL_CURSOR_TYPE = 6

' SQL_ATTR_CURSOR_SCROLLABLE values
Public Const SQL_NONSCROLLABLE = 0
Public Const SQL_SCROLLABLE = 1

' SQL_CURSOR_TYPE options
Public Const SQL_CURSOR_FORWARD_ONLY = 0
Public Const SQL_CURSOR_KEYSET_DRIVEN = 1
Public Const SQL_CURSOR_DYNAMIC = 2
Public Const SQL_CURSOR_STATIC = 3
Public Const SQL_CURSOR_TYPE_DEFAULT = SQL_CURSOR_FORWARD_ONLY          ' Default value

' SQL data type codes
Public Const SQL_UNKNOWN_TYPE = 0
Public Const SQL_CHAR = 1
Public Const SQL_NUMERIC = 2
Public Const SQL_DECIMAL = 3
Public Const SQL_INTEGER = 4
Public Const SQL_SMALLINT = 5
Public Const SQL_FLOAT = 6
Public Const SQL_REAL = 7
Public Const SQL_DOUBLE = 8
Public Const SQL_DATETIME = 9
Public Const SQL_VARCHAR = 12

' Error numbers raised when a call fails - These are also resource IDs for the corresponding error description.
Public Enum ODBCAPIErrors
    ErrorAllocateHandle = 20000
    ErrorSetAttribute = 20001
    ErrorConnectDriver = 20002
    ErrorExecuteQuery = 20003
    ErrorFetchRecord = 20004
    ErrorCloseCursor = 20005
    ErrorFreeHandle = 20006
    ErrorDisconnectDriver = 20007
    ErrorGetData = 20008
    ErrorEndTransaction = 20009
    ErrorResourceDeadlock = 20010
End Enum

Public Function ODBCAPICallSuccessful(lReturnCode As Long) As Boolean
' Returns True if the specified return code from an ODBC API call indicates that the operation was successful, else
' returns False
    Select Case lReturnCode
        Case SQL_SUCCESS, SQL_SUCCESS_WITH_INFO
            ODBCAPICallSuccessful = True
        Case Else
            ODBCAPICallSuccessful = False
    End Select
End Function
