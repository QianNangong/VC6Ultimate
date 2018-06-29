VERSION 5.00
Begin VB.Form frmPoolMgr 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Pool Manager"
   ClientHeight    =   2175
   ClientLeft      =   6780
   ClientTop       =   1620
   ClientWidth     =   3915
   ClipControls    =   0   'False
   Icon            =   "frmpool.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2175
   ScaleWidth      =   3915
   StartUpPosition =   3  'Windows Default
   Begin VB.ListBox lstLog 
      Height          =   645
      IntegralHeight  =   0   'False
      Left            =   3270
      TabIndex        =   0
      Top             =   540
      Visible         =   0   'False
      Width           =   855
   End
   Begin VB.Label lblSatisfiedCap 
      BackStyle       =   0  'Transparent
      Caption         =   "Requests Satisfied"
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
      Left            =   200
      TabIndex        =   7
      Top             =   120
      Width           =   2535
   End
   Begin VB.Label lblRejectedCap 
      BackStyle       =   0  'Transparent
      Caption         =   "Requests Rejected"
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
      Left            =   200
      TabIndex        =   6
      Top             =   480
      Width           =   2535
   End
   Begin VB.Label lblWorkersCap 
      BackStyle       =   0  'Transparent
      Caption         =   "Number of Workers"
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
      Left            =   200
      TabIndex        =   5
      Top             =   840
      Width           =   2535
   End
   Begin VB.Label lblSatisfied 
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
      Left            =   2820
      TabIndex        =   4
      Top             =   120
      Width           =   1000
   End
   Begin VB.Label lblRejected 
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
      Left            =   2820
      TabIndex        =   3
      Top             =   480
      Width           =   1000
   End
   Begin VB.Label lblWorkers 
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
      Left            =   2820
      TabIndex        =   2
      Top             =   840
      Width           =   1000
   End
   Begin VB.Label lblStatus 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   780
      Left            =   195
      TabIndex        =   1
      Top             =   1200
      Width           =   3570
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "frmPoolMgr"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private Sub Form_Load()
    'Use clsPositionForm object to move
    'Form to settings saved in registry
    Dim oPosition As clsPositionForm
    Set oPosition = New clsPositionForm
    
    'Set U/I captions
    ApplyFontToForm Me
    lblRejectedCap.Caption = LoadResString(giLBL_REJECTED)
    lblSatisfiedCap.Caption = LoadResString(giLBL_SATISFIED)
    lblWorkersCap.Caption = LoadResString(giLBL_NUM_WORKERS)
    Caption = LoadResString(giPOOLMGR_CAPTION)
    
    #If ccShowList Then
        oPosition.Move Me, True
        lstLog.Visible = True
        lblStatus.Visible = False
    #Else
        oPosition.Move Me, False
        Width = giDEFAULT_FORM_WIDTH
        Height = giDEFAULT_FORM_HEIGHT
    #End If
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    'Don't unload unless called from code.
    If UnloadMode = vbFormControlMenu Then Cancel = False
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


