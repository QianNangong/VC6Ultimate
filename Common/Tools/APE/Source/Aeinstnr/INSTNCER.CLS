VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "Instancer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Description = "APE Instance Manager"
Option Explicit

Implements APEInterfaces.IInstancer

Private Function IInstancer_Object(ByVal sProgID As String) As Object
    '-------------------------------------------------------------------------
    'Purpose:
    '   This public class is a work around for error
    '   -2147221166 (80040152) which occurrs every time a client
    '   object creates an instance of a remote server,
    '   destroys it, registers it local, and tries to
    '   create a local instance.  The client can not
    '   create an object registered locally after it created
    '   an instance while it was registered remotely
    '   until it shuts down and restart.  Therefore,
    '   it works to call another process to create the
    '   local instance and pass it back.
    'In:
    '   [sProgID]
    '       ProgID of needed object
    'Return:
    '   Object created using the passed progId
    '-------------------------------------------------------------------------
    Set IInstancer_Object = CreateObject(sProgID)
End Function
