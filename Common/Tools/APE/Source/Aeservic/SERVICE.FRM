VERSION 5.00
Begin VB.Form frmService 
   BorderStyle     =   1  'Fixed Single
   Caption         =   ".MTS Transaction Status"
   ClientHeight    =   2175
   ClientLeft      =   10575
   ClientTop       =   4875
   ClientWidth     =   3915
   ClipControls    =   0   'False
   Icon            =   "Service.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2175
   ScaleWidth      =   3915
   StartUpPosition =   3  'Windows Default
   Tag             =   "6"
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   ".Successful Transactions:"
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
      TabIndex        =   3
      Tag             =   "7"
      Top             =   120
      Width           =   2775
   End
   Begin VB.Label lblCaption 
      BackStyle       =   0  'Transparent
      Caption         =   ".Aborted Transactions:"
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
      TabIndex        =   2
      Tag             =   "8"
      Top             =   480
      Width           =   2775
   End
   Begin VB.Label lblSuccessCount 
      BackStyle       =   0  'Transparent
      Caption         =   "0"
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
      Left            =   3120
      TabIndex        =   1
      Top             =   120
      Width           =   800
   End
   Begin VB.Label lblAbortedCount 
      BackStyle       =   0  'Transparent
      Caption         =   "0"
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
      Left            =   3120
      TabIndex        =   0
      Top             =   480
      Width           =   800
   End
End
Attribute VB_Name = "frmService"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private mcMTSSucceeded As Long
Private mcMTSFailed As Long

Public Sub UpdateUI()
    lblSuccessCount.Caption = mcMTSSucceeded
    lblAbortedCount.Caption = mcMTSFailed
    lblSuccessCount.Refresh
    lblAbortedCount.Refresh
End Sub

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
    Caption = LoadResString(giMTS_FORM_CAPTION)
    lblCaption(0).Caption = LoadResString(giSUCCEEDED_TRANSACTIONS_CAPTION)
    lblCaption(1).Caption = LoadResString(giABORTED_TRANSACTIONS_CAPTION)
    oPosition.Move Me, False
    Width = giDEFAULT_FORM_WIDTH
    Height = giDEFAULT_FORM_HEIGHT
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    'If user unloads form cancel unload
    Dim oPosition As clsPositionForm
    'Use clsPositionForm object to save
    'forms position in registry
    Set oPosition = New clsPositionForm
    If Me.Visible Then oPosition.Save Me
    If UnloadMode = vbFormControlMenu Then
        Cancel = True
    End If
End Sub

Public Sub MTSResults(bSucceeded As Boolean)
    If bSucceeded Then
        mcMTSSucceeded = mcMTSSucceeded + 1
    Else
        mcMTSFailed = mcMTSFailed + 1
    End If
    UpdateUI
End Sub

Public Sub Reset()
    mcMTSFailed = 0
    mcMTSSucceeded = 0
    UpdateUI
End Sub
