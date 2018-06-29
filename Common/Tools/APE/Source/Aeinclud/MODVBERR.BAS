Attribute VB_Name = "modVBErrors"
Option Explicit
'-------------------------------------------------------------------------
'This Module provides VB4 Error constants
'-------------------------------------------------------------------------

'VB4 Errors
Public Const ERR_RETURN_WITHOUT_GOSUB As Integer = 3                'Return without GoSub
Public Const ERR_INVALID_PROCEDURE_CALL  As Integer = 5             'Invalid procedure call
Public Const ERR_OVER_FLOW  As Integer = 6                          'Overflow
Public Const ERR_OUT_OF_MEMORY  As Integer = 7                      'Out of memory
Public Const ERR_SUBSCRIPT_OUT_OF_RANGE  As Integer = 9             'Subscript out of range
Public Const ERR_ARRAY_FIXED_OR_LOCKED  As Integer = 10             'This array is fixed or temporarily locked
Public Const ERR_DIVISION_BY_ZERO  As Integer = 11                  'Division by zero
Public Const ERR_TYPE_MISMATCH  As Integer = 13                     'Type mismatch
Public Const ERR_OUT_OF_STRING_SPACE  As Integer = 14               'Out of string space
Public Const ERR_EXPRESSION_TOO_COMPLEX As Integer = 16             'Expression Too Complex
Public Const ERR_CANT_PERFORM_OPERATION As Integer = 17             'Can 't perform requested operation
Public Const ERR_USER_INTERRUPT  As Integer = 18                    'User interrupt occurred
Public Const ERR_RESUME_WITHOUT_ERROR  As Integer = 20              'Resume without error
Public Const ERR_OUT_OF_STACK_SPACE  As Integer = 28                'Out of stack space
Public Const ERR_PROCEDURE_NOT_DEFINED  As Integer = 35             'Sub, Function, or Property not defined
Public Const ERR_TOO_MANY_DLL_CLIENTS As Integer = 47               'Too many DLL application clients
Public Const ERR_ERROR_LOADING_DLL As Integer = 48                  'Error in loading DLL
Public Const ERR_BAD_DLL_CALL  As Integer = 49                      'Bad DLL calling convention
Public Const ERR_INTERNAL_ERROR  As Integer = 51                    'Internal Error
Public Const ERR_BAD_FILE_NAME  As Integer = 52                     'Bad file name or number
Public Const ERR_FILE_NOT_FOUND  As Integer = 53                    'File Not found
Public Const ERR_BAD_FILE_MODE  As Integer = 54                     'Bad file mode
Public Const ERR_FILE_ALREADY_OPEN  As Integer = 55                 'File already open
Public Const ERR_DEVICE_IO_ERROR  As Integer = 57                   'Device I/O error
Public Const ERR_FILE_ALREADY_EXISTS  As Integer = 58               'File already exists
Public Const ERR_BAD_RECORD_LENGTH  As Integer = 59                 'Bad record length
Public Const ERR_DISK_FULL  As Integer = 61                         'Disk full
Public Const ERR_IPUT_PAST_EOF  As Integer = 62                     'Input past end of file
Public Const ERR_BAD_RECORD_NUMBER  As Integer = 63                 'Bad record number
Public Const ERR_TOO_MANY_FILES  As Integer = 67                    'Too many files
Public Const ERR_DEVICE_UNAVAILABLE  As Integer = 68                'Device unavailable
Public Const ERR_PERMISSION_DENIED  As Integer = 70                 'Permission denied
Public Const ERR_DISK_NOT_READY  As Integer = 71                    'Disk Not ready
Public Const ERR_CANT_RENAME_WITH_DIFFERENT_DRIVE  As Integer = 74  'Can 't rename with different drive
Public Const ERR_PATH_OR_FILE_ACCESS_ERROR  As Integer = 75         'Path/File access error
Public Const ERR_PATH_NOT_FOUND  As Integer = 76                    'Path Not found
Public Const ERR_OBJECT_VARIABLE_NOT_SET  As Integer = 91           'Object variable or With block variable not set
Public Const ERR_FOR_LOOP_NOT_INITIALIZED As Integer = 92           'For loop not initialized
Public Const ERR_INVALID_PATTERN_STRING As Integer = 93             'Invalid pattern string
Public Const ERR_INVALID_USE_OF_NULL  As Integer = 94               'Invalid use of Null
Public Const ERR_CONTROL_ARRAY_ELEMENT_DOESNOT_EXIST = 340
Public Const ERR_INVALID_PROPERTY_VALUE  As Integer = 380           'Invalid property value
Public Const ERR_INVALID_PROPERTY_ARRAY_INDEX  As Integer = 381
Public Const ERR_PROPERTY_IS_READ_ONLY  As Integer = 383
Public Const ERR_CANT_CREATE_OBJECT  As Integer = 429               'OLE Automation server can't create object
Public Const ERR_METHOD_NOT_APPLICABLE  As Integer = 444            'Method not applicable in this context
Public Const ERR_INVALID_ORDINAL As Integer = 452                   'Invalid ordinal
Public Const ERR_DLL_FUNCITON_NOT_FOUND  As Integer = 453           'Specified DLL function not found
Public Const ERR_DUPLICATE_KEY  As Integer = 457                    'Duplicate Key
Public Const ERR_INVALID_CLIPBOARD_FORMAT  As Integer = 460         'Invalid Clipboard format
Public Const ERR_FORMAT_DOESNT_MATCH_DATA As Integer = 461          'Specified format doesn't match format of data
Public Const ERR_CANT_CREATE_AUTOREDRAW  As Integer = 480           'Can 't create AutoRedraw image
Public Const ERR_INVALID_PICTURE  As Integer = 481                  'Invalid Picture
Public Const ERR_PRINTER_ERROR  As Integer = 482                    'Printer Error
Public Const ERR_PRINTER_DRIVE_RPROPERTY_INVALID  As Integer = 483  'Printer driver does not support specified property
Public Const ERR_PRINTER_SYSTEM_INFO_PROBLEM  As Integer = 484      'Problem getting printer information from the system. Make sure the printer is set up correctly
Public Const ERR_INVALID_PICTURE_TYPE  As Integer = 485             'Invalid picture type
Public Const ERR_CANT_EMPTY_CLIPBOARD As Integer = 520              'Can 't empty Clipboard
Public Const ERR_CANT_OPEN_CLIPBOARD  As Integer = 521              'Can 't open Clipboard
