Attribute VB_Name = "basWrkPd"
Option Explicit

'Constants
Public Const giRACREG_ERROR_CODE_OFFSET = 200       'Add this offset to a RacReg.RegClas error code
                                                    'to convert to corresponding resource
                                                    'string key
Public Const giRACREG_ERROR = 32750                 'Error value to raise if RacReg error code
                                                    'is returned by RacReg.
                                                    'Value is also a String Resource key.
