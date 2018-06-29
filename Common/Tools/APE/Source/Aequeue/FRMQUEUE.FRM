VERSION 5.00
Begin VB.Form frmQueueMgr 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Queue Manager"
   ClientHeight    =   2175
   ClientLeft      =   7170
   ClientTop       =   1785
   ClientWidth     =   3915
   ClipControls    =   0   'False
   Icon            =   "frmqueue.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   ScaleHeight     =   2175
   ScaleWidth      =   3915
   StartUpPosition =   3  'Windows Default
   Begin VB.ListBox lstLog 
      Height          =   525
      IntegralHeight  =   0   'False
      Left            =   3330
      TabIndex        =   0
      Top             =   1440
      Visible         =   0   'False
      Width           =   525
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Worker Count"
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
      Index           =   3
      Left            =   195
      TabIndex        =   9
      Top             =   1200
      Width           =   2535
   End
   Begin VB.Label lblWorkerCount 
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
      Left            =   2800
      TabIndex        =   8
      Top             =   1200
      Width           =   1000
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
      Left            =   2800
      TabIndex        =   7
      Top             =   840
      Width           =   1000
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
      Left            =   2800
      TabIndex        =   6
      Top             =   480
      Width           =   1000
   End
   Begin VB.Label lblQueue 
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
      Left            =   2800
      TabIndex        =   5
      Top             =   120
      Width           =   1000
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Total Call Count"
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
      Left            =   195
      TabIndex        =   4
      Top             =   840
      Width           =   2535
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Peak Queue Size"
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
      Left            =   195
      TabIndex        =   3
      Top             =   480
      Width           =   2535
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Current Queue Size"
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
      Left            =   195
      TabIndex        =   2
      Top             =   120
      Width           =   2535
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
      Height          =   500
      Left            =   200
      TabIndex        =   1
      Top             =   1570
      Width           =   3500
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "frmQueueMgr"
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
    
    'Set U/I captions
    ApplyFontToForm Me
    Caption = LoadResString(giFORM_CAPTI0N)
    lblCaption(0).Caption = LoadResString(giCURRENT_QUEUE_CAPTION)
    lblCaption(1).Caption = LoadResString(giPEAK_QUEUE_CAPTION)
    lblCaption(2).Caption = LoadResString(giTOTAL_CALL_CAPTION)
    lblCaption(3).Caption = LoadResString(giWORKER_COUNT_CAPTION)
    
    'Condition compile toggles between retail U/I and
    'Debug U/I that displays a list box that displays all
    'events as they occur
    #If ccShowList Then
        oPosition.Move Me, True
        lstLog.Visible = True
        lblStatus.Visible = False
        lblPeak.Visible = False
        lblCount.Visible = False
        lblQueue.Visible = False
        lblCaption(0).Visible = False
        lblCaption(1).Visible = False
        lblCaption(2).Visible = False
    #Else
        oPosition.Move Me, False
        Width = giDEFAULT_FORM_WIDTH
        Height = giDEFAULT_FORM_HEIGHT
    #End If
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    'Only unload if called by code.
    If UnloadMode = vbFormControlMenu Then Cancel = True
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

Private Sub Form_Unload(Cancel As Integer)
    'Use clsPositionForm object to save
    'forms position in registry
    Dim oPosition As clsPositionForm
    Set oPosition = New clsPositionForm
    oPosition.Save Me
    gbShow = False
End Sub

