VERSION 5.00
Begin VB.Form frmExpediter 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Expediter"
   ClientHeight    =   2175
   ClientLeft      =   10575
   ClientTop       =   4875
   ClientWidth     =   3915
   ClipControls    =   0   'False
   Icon            =   "frmexpdt.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2175
   ScaleWidth      =   3915
   StartUpPosition =   3  'Windows Default
   Begin VB.ListBox lstLog 
      Height          =   525
      IntegralHeight  =   0   'False
      Left            =   2880
      TabIndex        =   0
      Top             =   1350
      Visible         =   0   'False
      Width           =   525
   End
   Begin VB.Timer tmrExpediter 
      Left            =   3420
      Top             =   1620
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Current Backlog"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Index           =   0
      Left            =   200
      TabIndex        =   7
      Top             =   120
      Width           =   2535
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Peak Backlog"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Index           =   1
      Left            =   200
      TabIndex        =   6
      Top             =   480
      Width           =   2535
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Total Callbacks"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Index           =   2
      Left            =   200
      TabIndex        =   5
      Top             =   840
      Width           =   2535
   End
   Begin VB.Label lblBacklog 
      BackStyle       =   0  'Transparent
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Left            =   2760
      TabIndex        =   4
      Top             =   120
      Width           =   1095
   End
   Begin VB.Label lblPeak 
      BackStyle       =   0  'Transparent
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Left            =   2760
      TabIndex        =   3
      Top             =   480
      Width           =   1095
   End
   Begin VB.Label lblCount 
      BackStyle       =   0  'Transparent
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   300
      Left            =   2760
      TabIndex        =   2
      Top             =   840
      Width           =   1095
   End
   Begin VB.Label lblStatus 
      BackStyle       =   0  'Transparent
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   800
      Left            =   200
      TabIndex        =   1
      Top             =   1200
      Width           =   3450
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "frmExpediter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Sub Form_Load()
    '-------------------------------------------------------------------------
    'Effects:
    '       Position form and load captions from string resource
    '-------------------------------------------------------------------------
    'Use clsPositionForm object to move
    'Form to settings saved in registry
    Dim oPosition As clsPositionForm
    Set oPosition = New clsPositionForm
    
    'Set the U/I values
    ApplyFontToForm Me
    Caption = LoadResString(giFORM_CAPTION)
    lblCaption(0).Caption = LoadResString(giCURRENT_BACKLOG_CAPTION)
    lblCaption(1).Caption = LoadResString(giPEAK_BACKLOG_CAPTION)
    lblCaption(2).Caption = LoadResString(giTOTAL_CALLBACK_CAPTION)
    
    'Condition compile toggles between a debug
    'mode that displays a list box with displaying
    'all loggable events.
    #If ccShowList Then
        oPosition.Move Me, True
        lstLog.Visible = True
        lblStatus.Visible = False
        lblBacklog.Visible = False
        lblPeak.Visible = False
        lblCount.Visible = False
    #Else
        oPosition.Move Me, False
        Width = giDEFAULT_FORM_WIDTH
        Height = giDEFAULT_FORM_HEIGHT
    #End If
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    'If user unloads form cancel unload
    Dim oPosition As clsPositionForm
    'Use clsPositionForm object to save
    'forms position in registry
    Set oPosition = New clsPositionForm
    If Me.Visible Then oPosition.Save Me
    If UnloadMode = vbFormControlMenu And glInstances <> 0 Then
        Cancel = True
    End If

End Sub

Private Sub Form_Resize()
    #If ccShowList Then
        Dim lX As Long
        Dim lY As Long
        If Me.ScaleHeight >= 2 * glFORM_MARGIN Then lY = (Me.ScaleHeight - (2 * glFORM_MARGIN)) Else lY = (2 * glFORM_MARGIN) - Me.ScaleHeight
        If Me.ScaleWidth >= 2 * glFORM_MARGIN Then lX = (Me.ScaleWidth - (2 * glFORM_MARGIN)) Else lX = (2 * glFORM_MARGIN) - Me.ScaleWidth
        lstLog.Move glFORM_MARGIN, glFORM_MARGIN, lX, lY
    #End If
End Sub

Private Sub tmrExpediter_Timer()
    '-------------------------------------------------------------------------
    'Effects:
    '       Polls the QueueMgr for Service Results.  If any are received the
    '       Expediter will attempt to call all the callbacks and deliver the
    '       the results
    '
    '       If gbStopTest became true during this process DestroyReferences
    '       will be called at end of procedure
    '   [gbBusy]
    '       Is true during procedure
    '-------------------------------------------------------------------------
    On Error GoTo tmrExpediter_Timer
    'Exit if already entered this procedure
    If gbBusy Or gbStopTest Then Exit Sub
    gbBusy = True
    If PollQueue Then
        tmrExpediter.Interval = 0
        DeliverResults
    End If
    If gbStopTest Then
        'References to Expediter may have
        'been destroyed while PollQueue or DeliverResults
        'was busy.  If gbBusy was true when Expediter's references
        'were destoyed, DestroyReferences needs called again to
        'make sure logger and form is destroyed
        DestroyReferences
    End If
    gbBusy = False
    Exit Sub
tmrExpediter_Timer:
    LogError Err, 0
    Exit Sub
End Sub
