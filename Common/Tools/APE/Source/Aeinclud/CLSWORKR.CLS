VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "clsWorker"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
'-------------------------------------------------------------------------
'This class is used for storing related data
'that will be added to a collection
'Stores a Worker object and data related to managing that Worker object
'-------------------------------------------------------------------------
Public ID As Long               'ID of the Worker, it should be the same
                                'as the Workers ID property and the same
                                'as the key an object of this class is stored
                                'in gcWorkers collection with
Public Busy As Boolean          'Worker is processing a Service Request
Public Worker As APEInterfaces.IWorker    'A valid Worker class object
Public RemoveMe As Boolean      'If true the Worker is marked for removal
                                'from the PoolMgr's or QueueMgr's
                                'collection of Workers
