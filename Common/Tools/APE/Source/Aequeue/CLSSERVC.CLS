VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "clsService"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
'-------------------------------------------------------------------------
'This class is used a structure for holding queued
'Service requests.  Objects of this class are
'added to the gcQueue collection
'-------------------------------------------------------------------------
Public ID As String             'Service Request ID
Public Command As String        'Service Command used passed to Worker
Public Data As Variant          'Service Data passed to Worker
Public CallBack As APEInterfaces.IClientCallback  'Callback object Expediter will use to call
                                'Client application back
Public CallBackMode As Integer  'Defines if and how data should be returned
                                'to client.  See "Callback mode keys" in modAEConstants
Public EventObject As Object 'sync object to return results through
                                'back to calling client
Public Status As Integer        'Status flag, see global constants
Public DataPresent As Boolean   'Flag, if true, data needs to goto to Service object
                                'Pass Data to Service object
Public ReturnData As Variant    'Data to be returned to Client application.  The
                                'results of a Service Request processed by a
                                'worker.  The Expediter will get this.
Public ReturnError As String    'Error description to return to client.  Description
                                'of error that occured while processing service
                                'request.
