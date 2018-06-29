VERSION 5.00
Object = "{6FBA474E-43AC-11CE-9A0E-00AA0062BB4C}#1.0#0"; "SYSINFO.OCX"
Begin VB.Form frmClient 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Client"
   ClientHeight    =   2145
   ClientLeft      =   270
   ClientTop       =   1860
   ClientWidth     =   3915
   ClipControls    =   0   'False
   Icon            =   "frmclnt.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   ScaleHeight     =   2145
   ScaleWidth      =   3915
   StartUpPosition =   3  'Windows Default
   Begin SysInfoLib.SysInfo SysInfo 
      Left            =   1680
      Top             =   840
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
   End
   Begin VB.Timer tmrStartTest 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   240
      Top             =   1200
   End
   Begin VB.ListBox lstLog 
      Height          =   585
      IntegralHeight  =   0   'False
      Left            =   840
      TabIndex        =   0
      Top             =   1080
      Visible         =   0   'False
      Width           =   645
   End
   Begin VB.Label lblCallsReturnedCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Calls Returned"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   180
      TabIndex        =   5
      Top             =   480
      Width           =   2535
   End
   Begin VB.Label lblCallsReturned 
      BackStyle       =   0  'Transparent
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   2800
      TabIndex        =   4
      Top             =   480
      Width           =   1000
   End
   Begin VB.Label lblCallsMade 
      BackStyle       =   0  'Transparent
      Caption         =   "9999999999"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   2800
      TabIndex        =   3
      Top             =   150
      Width           =   1000
   End
   Begin VB.Label lblStatus 
      BackStyle       =   0  'Transparent
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   980
      Left            =   180
      TabIndex        =   2
      Top             =   1000
      Width           =   3550
      WordWrap        =   -1  'True
   End
   Begin VB.Label lblCallsCaption 
      BackStyle       =   0  'Transparent
      Caption         =   "Calls Made"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   180
      TabIndex        =   1
      Top             =   150
      Width           =   2535
   End
End
Attribute VB_Name = "frmClient"
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
    
    'Set Captions
    ApplyFontToForm Me
    Caption = LoadResString(giFORM_CAPTION)
    lblCallsCaption.Caption = LoadResString(giCALLS_MADE_CAPTION)
    lblCallsReturnedCaption.Caption = LoadResString(giCALLS_RETURNED_CAPTION)
    
    'Conditional compile toggles between release mode
    'and a debug mode which displays a list box and
    'list events as they occur in the box
    #If ccShowList Then
        lstLog.Visible = True
        lblStatus.Visible = False
        lblCallsCaption.Visible = False
        lblCallsMade.Visible = False
        lblCallsReturned.Visible = False
        lblCallsReturnedCaption.Visible = False
        oPosition.Move Me, True
    #Else
        oPosition.Move Me, False
        Width = giDEFAULT_FORM_WIDTH
        Height = giDEFAULT_FORM_HEIGHT
    #End If
End Sub


Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If UnloadMode = vbFormControlMenu And Not gbShutDown Then Cancel = True
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
End Sub

Private Sub tmrStartTest_Timer()
    '-------------------------------------------------------------------------
    'Purpose:
    '       Calls Complete test or ConfigureTest and a RunTest method, depending
    '       on gbRunCompleteProcedure flag
    '-------------------------------------------------------------------------
    Static stbInTimer As Boolean
    tmrStartTest.Enabled = False
    On Error GoTo tmrStartTestError
    
    If Not stbInTimer Then
        stbInTimer = True
        If gbRunCompleteProcedure Then
            CompleteTest
        Else
            ConfigureTest
            goTestTool.RunTest
        End If
        stbInTimer = False
    End If
    Exit Sub
tmrStartTestError:
    LogError Err
    StopOnError Err.Description
    stbInTimer = False
    Exit Sub
End Sub
