VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "clsWorkerMachines"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'-------------------------------------------------------------------------
'This class is used for storing related data
'that will be added to a collection
'Stores a Machine name that Workers are instanciated on
'-------------------------------------------------------------------------
Public MachineName As String        'Machine name
Public WorkerProvider As APEInterfaces.IWorkerProvider    'Server that can be instanciated on remote
                                    'machines to provide Worker objects
Public Remote As Boolean            'If true, this represents a remote machine
                                    'rather that the local machine.
Public WorkerKeys As Collection     'Collection of longs, representing the keys
                                    'of Workers stored in the gcWorkers collection
                                    'that are on the machine represented by this
                                    'object
Private Sub Class_Initialize()
    Set WorkerKeys = New Collection
End Sub
