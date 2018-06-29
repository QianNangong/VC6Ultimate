VERSION 5.00
Begin VB.Form frmLogger 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Logger"
   ClientHeight    =   2100
   ClientLeft      =   1380
   ClientTop       =   1980
   ClientWidth     =   4200
   ClipControls    =   0   'False
   Icon            =   "frmloggr.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2100
   ScaleWidth      =   4200
   StartUpPosition =   3  'Windows Default
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
      Height          =   1170
      Left            =   70
      TabIndex        =   0
      Top             =   900
      Width           =   3835
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "frmLogger"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
    'Use clsPositionForm object to move
    'Form to settings saved in registry
    Dim oPosition As clsPositionForm
    Set oPosition = New clsPositionForm
    oPosition.Move Me, False
    Width = giDEFAULT_FORM_WIDTH
    Height = giDEFAULT_FORM_HEIGHT
    'Set Form Caption
    ApplyFontToForm Me
    Caption = LoadResString(giFORM_CAPTION)

End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    'Don't unload unless called from code.
    If UnloadMode = vbFormControlMenu Then Cancel = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    'Use clsPositionForm object to save
    'forms position in registry
    Dim oPosition As clsPositionForm
    Set oPosition = New clsPositionForm
    oPosition.Save Me
    gbShowForm = False
End Sub
