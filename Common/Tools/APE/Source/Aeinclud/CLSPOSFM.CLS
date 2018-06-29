VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0   'False
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
END
Attribute VB_Name = "clsPositionForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
'-------------------------------------------------------------------------
'This class must be used with modPositionForm which supplys
'declarations, and types
'This class is intended to be used with any Automation Explorer application
'for saving form positions in the registry
'and moving forms back to that position when loaded again
'If more than one form of the same name is loaded, cascading
'will occur only in relationship with each other.
'Use Move method on form_load event
'Use Save method on form_unload event
'To use this class with a application that is not
'apart of the Automation Explorer project change the
'constant msPROJECT_NAME
'-------------------------------------------------------------------------

#If UNICODE Then
    Private Declare Function GetClassName Lib "user32" Alias "GetClassNameW" (ByVal hWnd As Long, ByVal lpClassName As String, ByVal nMaxCount As Long) As Long
    Private Declare Function GetWindowText Lib "user32" Alias "GetWindowTextW" (ByVal hWnd As Long, ByVal lpString As String, ByVal cch As Long) As Long
#Else
    Private Declare Function GetClassName Lib "user32" Alias "GetClassNameA" (ByVal hWnd As Long, ByVal lpClassName As String, ByVal nMaxCount As Long) As Long
    Private Declare Function GetWindowText Lib "user32" Alias "GetWindowTextA" (ByVal hWnd As Long, ByVal lpString As String, ByVal cch As Long) As Long
#End If
Private Declare Function GetWindow Lib "user32" (ByVal hWnd As Long, ByVal wCmd As Long) As Long
Private Declare Function GetWindowRect Lib "user32" (ByVal hWnd As Long, lpRect As RECT) As Long
Private Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long

'Types
Private Type RECT
        Left As Long
        Top As Long
        Right As Long
        Bottom As Long
End Type

'Public Constants
Private Const GW_HWNDNEXT As Integer = 2
Private Const GW_HWNDFIRST As Integer = 0
Private Const SM_CYBORDER As Integer = 6
Private Const SM_CYCAPTION As Integer = 4
Private Const msSECTION_NAME As String = "Form Positions"

Public Sub Move(frmNew As Form, bSize As Boolean, Optional sComparableCharacters As String = "", Optional sngDefaultWidth As Single = 0, Optional sngDefaultHeight As Single = 0)
    '-------------------------------------------------------------------------
    'Purpose:   This method moves the passed form to the position saved
    '           in the registry.  It also cascades the forms position from
    '           the first form it finds with the same caption or that contains
    '           vComparableCharachters at the beginning of the caption.
    'IN:
    '   [frmNew]
    '           Form to position
    '   [bSize] If true also size the passed form
    '   [sComparableCharacters]
    '           String to compare to other form captions for cascading instead
    '           of passed forms captions.  If "Client" was passed, forms with
    '           captions "Client - 1", "Client - 2", "Client - N" would be compared
    '-------------------------------------------------------------------------
    Dim sWinName As String  'Window caption
    Dim sWinClass As String 'Window class
    Dim sDefault As String  'Default position of form in string format
    Dim sReturn As String   'Saved positon of form in string format
    Dim lResult As Long
    Dim lHwnd As Long, hWndNew As Long
    Dim tRect As RECT
    Dim lFactor As Long     'Factor for cascading form
    Dim iPos1 As Integer    'Position one in string
    Dim iPos2 As Integer    'Position two in string
    Dim lState As Long      'Window state
    Dim sngLeft As Single
    Dim sngTop As Single
    Dim sngWidth As Single
    Dim sngHeight As Single
    Dim lDefaultX As Long
    Dim lDefaultY As Long
    Dim sngScreenWidth As Single
    
    On Error Resume Next
    If sComparableCharacters = "" Then sComparableCharacters = frmNew.Caption
    'Create the default string
    If Not (sngDefaultWidth = 0) Then lDefaultX = sngDefaultWidth Else lDefaultX = giDEFAULT_FORM_WIDTH
    If Not (sngDefaultHeight = 0) Then lDefaultY = sngDefaultHeight Else lDefaultY = giDEFAULT_FORM_HEIGHT
    sDefault = CStr(-1) & "," & CStr(-1) & "," & CStr(lDefaultX) & "," & CStr(lDefaultY) & "," & CStr(vbNormal) & ",1"
    sReturn = GetRegSetting(gsREGISTRY_KEY, msSECTION_NAME, frmNew.Name, sDefault)
    'Parse values from returned string "left, top, width, height, state"
    iPos1 = InStr(sReturn, ",")
    sngLeft = CSng(Left$(sReturn, (iPos1 - 1)))
    iPos2 = InStr((iPos1 + 1), sReturn, ",")
    sngTop = CSng(Mid$(sReturn, (iPos1 + 1), (iPos2 - 1 - iPos1)))
    iPos1 = iPos2
    iPos2 = InStr((iPos1 + 1), sReturn, ",")
    sngWidth = CSng(Mid$(sReturn, (iPos1 + 1), (iPos2 - 1 - iPos1)))
    iPos1 = iPos2
    iPos2 = InStr((iPos1 + 1), sReturn, ",")
    sngHeight = CSng(Mid$(sReturn, (iPos1 + 1), (iPos2 - 1 - iPos1)))
    iPos1 = iPos2
    iPos2 = InStr((iPos1 + 1), sReturn, ",")
    lState = CSng(Mid$(sReturn, (iPos1 + 1), (iPos2 - 1 - iPos1)))
    sngScreenWidth = CLng(Right$(sReturn, Len(sReturn) - iPos2))
    'If this is not the first instance or if more than one form
    'is loaded find a handle to the next window
    'in the z-order with the same class name and window text
    'move the change the coordinates to one's that represent
    'a cascaded position in relation
    'ship to the next window
    sWinName = frmNew.Caption
    hWndNew = frmNew.hWnd
    sWinClass = Space$(255)
    lResult = GetClassName(hWndNew, sWinClass, 255)
    sWinClass = Left$(sWinClass, lResult)
    'Perform a loop checking previous windows in z-order
    'until window with same title and class name is found
    'or hwnd = 0
    lHwnd = GetWindow(hWndNew, GW_HWNDFIRST)
    Do Until lHwnd = 0
        If lHwnd <> hWndNew Then
            'check the window's class name
            sReturn = Space$(255)
            lResult = GetClassName(lHwnd, sReturn, 255)
            sReturn = Left$(sReturn, lResult)
            If sReturn = sWinClass Then
                'check the window's title
                sReturn = Space$(255)
                lResult = GetWindowText(lHwnd, sReturn, 255)
                sReturn = Left$(sReturn, lResult)
                If Left$(sReturn, Len(sComparableCharacters)) = Left$(sWinName, Len(sComparableCharacters)) Then
                    'Get the windows position and calculate
                    'the position for the new window
                    lResult = GetWindowRect(lHwnd, tRect)
                    'Get the system size of title bar and border
                    lFactor = GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYCAPTION)
                    'If cascaded position will not put the form
                    'off the screen change the left and top position
                    'to represent a cascaded position
                    'else leave the coordinates equal to what
                    'was retrieved from the registry
                    If Not ((tRect.Left + lFactor) * Screen.TwipsPerPixelX) + sngWidth > Screen.Width Then sngLeft = (tRect.Left + lFactor) * Screen.TwipsPerPixelX
                    If Not ((tRect.Top + lFactor) * Screen.TwipsPerPixelY) + sngHeight > Screen.Height Then sngTop = (tRect.Top + lFactor) * Screen.TwipsPerPixelY
                    Exit Do
                End If
            End If
        End If
        ' Get the next window in the z-order for the next loop
        lHwnd = GetWindow(lHwnd, GW_HWNDNEXT)
    Loop
    'If the screen width is less than
    'when form position was saved, do not
    'position form according to saved position,
    'because the saved position and size may be off
    'the screen.  Instead, let form be positioned to windows
    'default.
    If sngScreenWidth <= Screen.Width Then
        'If the passed bSize flag is true
        'size and move, else just move
        If sngTop <> -1 Then frmNew.Top = sngTop
        If sngLeft <> -1 Then frmNew.Left = sngLeft
        If bSize Then
            frmNew.Width = sngWidth
            frmNew.Height = sngHeight
        End If
    Else
        'Apply default width and height
        If bSize Then
            If sngDefaultWidth <> 0 Then frmNew.Width = sngDefaultWidth
            If sngDefaultHeight <> 0 Then frmNew.Height = sngDefaultHeight
        End If
    End If
    frmNew.WindowState = lState
End Sub


Public Sub Save(frmSave As Form)
    '-------------------------------------------------------------------------
    'Purpose:   This method saves the forms size and position in the registry
    '           using the form name as the label and string format
    '           "left, top, width, height
    'IN:
    '   [frmSave]
    '           Form to save position of
    'Effects:   The Forms position is saved to the registry
    '-------------------------------------------------------------------------
    Dim iPos1 As Integer    'Position one in string
    Dim iPos2 As Integer    'Position two in string
    Dim sngLeft As Single
    Dim sngTop As Single
    Dim sngWidth As Single
    Dim sngHeight As Single
    Dim sDefault As String  'Default position of form in string format
    Dim sReturn As String   'Saved positon of form in string format
    Dim lState As Long
    Dim sngScreenWidth As Single
    If frmSave.WindowState = vbNormal Then
        sReturn = CStr(frmSave.Left) & "," & CStr(frmSave.Top) & "," & CStr(frmSave.Width) & "," & CStr(frmSave.Height) & "," & CStr(frmSave.WindowState) & "," & CStr(Screen.Width)
    Else
        'Read the current settings and then only change the Widowstate value
        'and the screen width
        'Create the default string
        sDefault = CStr(-1) & "," & CStr(-1) & "," & CStr(giDEFAULT_FORM_WIDTH) & "," & CStr(giDEFAULT_FORM_HEIGHT) & "," & CStr(vbNormal) & ",1"
        sReturn = GetRegSetting(gsREGISTRY_KEY, msSECTION_NAME, frmSave.Name, sDefault)
        'Parse values from returned string "left, top, width, height, state"
        iPos1 = InStr(sReturn, ",")
        sngLeft = CSng(Left$(sReturn, (iPos1 - 1)))
        iPos2 = InStr((iPos1 + 1), sReturn, ",")
        sngTop = CSng(Mid$(sReturn, (iPos1 + 1), (iPos2 - 1 - iPos1)))
        iPos1 = iPos2
        iPos2 = InStr((iPos1 + 1), sReturn, ",")
        sngWidth = CSng(Mid$(sReturn, (iPos1 + 1), (iPos2 - 1 - iPos1)))
        iPos1 = iPos2
        iPos2 = InStr((iPos1 + 1), sReturn, ",")
        sngHeight = CSng(Mid$(sReturn, (iPos1 + 1), (iPos2 - 1 - iPos1)))
        iPos1 = iPos2
        iPos2 = InStr((iPos1 + 1), sReturn, ",")
        lState = CSng(Mid$(sReturn, (iPos1 + 1), (iPos2 - 1 - iPos1)))
        sngScreenWidth = CLng(Right$(sReturn, Len(sReturn) - iPos2))
        sReturn = CStr(sngLeft) & "," & CStr(sngTop) & "," & CStr(sngWidth) & "," & CStr(sngHeight) & "," & CStr(frmSave.WindowState) & "," & CStr(sngScreenWidth)
    End If
    SaveRegSetting gsREGISTRY_KEY, msSECTION_NAME, frmSave.Name, sReturn
    
End Sub

