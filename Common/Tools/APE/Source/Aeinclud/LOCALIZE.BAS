Attribute VB_Name = "Localize"
Option Explicit
'------------------------------------------------------------
'- Localization Declares...
'------------------------------------------------------------
Private Declare Function GetSystemDefaultLCID Lib "Kernel32" () As Long

'------------------------------------------------------------
'- Localization Fonts Charicter sets...
'------------------------------------------------------------
Public Const CHARSET_DEFAULT = 1
Public Const CHARSET_SHIFTJIS = 128
Public Const CHARSET_HANGEUL = 129
Public Const CHARSET_CHINESESIMPLIFIED = 134
Public Const CHARSET_CHINESEBIG5 = 136
Public Const CHARSET_HEBREW = 177
Public Const CHARSET_ARABIC = 178

'------------------------------------------------------------
'  Primary language IDs.
'------------------------------------------------------------
Public Const LANG_ARABIC = &H1              ' added 10-14-97
Public Const LANG_CHINESE = &H4
Public Const LANG_HEBREW = &HD              ' added 10-14-97
Public Const LANG_JAPANESE = &H11
Public Const LANG_KOREAN = &H12

'------------------------------------------------------------
'  Sublanguage IDs.
'------------------------------------------------------------
'  The name immediately following SUBLANG_ dictates which primary
'  language ID that sublanguage ID can be combined with to form a
'  valid language ID.
'------------------------------------------------------------
Public Const SUBLANG_CHINESE_TRADITIONAL = &H1           ' Chinese (Taiwan)
Public Const SUBLANG_CHINESE_SIMPLIFIED = &H2            ' Chinese (PR China)
Public Const SUBLANG_KOREAN = &H1                        ' Korean (Extended Wansung) ' added 10-14-97
Public Const SUBLANG_KOREAN_JOHAB = &H2                  ' Korean (Johab)            ' added 10-14-97

Public Sub ApplyFontToForm(frmForm As Form)
' Applies the appropriate font for the locale to all the controls of the specified form.
    Dim ctl As Control
    Dim sFont As String
    Dim nFont As Integer, nCharset As Integer
    
    On Error Resume Next
    GetFontInfo sFont, nFont, nCharset
    For Each ctl In frmForm.Controls
        ' If the control does not have Font property,
        ' this line will be skipped.
        With ctl.Font
            .Name = sFont
            .Size = nFont
            .Charset = nCharset
        End With
    Next
    With frmForm.Font
        .Name = sFont
        .Size = nFont
        .Charset = nCharset
    End With
End Sub

'-------------------------------------------------------
Public Sub GetFontInfo(sFont As String, nFont As Integer, nCharset As Integer)
'-------------------------------------------------------
    Static ssFont As String                   ' the cached name of the font
    Static snFont As Integer                  ' the cached size of the font
    Static snCharset As Integer               ' the cached charset of the font

    ' if font is set, used the cached values
    If ssFont <> "" Then
        sFont = ssFont
        nFont = snFont
        nCharset = snCharset
        Exit Sub
    End If
'-------------------------------------------------------
    Dim LCID    As Integer
    Dim PLangId As Integer
    Dim sLangId As Integer
'-------------------------------------------------------
    LCID = GetSystemDefaultLCID                 ' get current system LCID
    PLangId = (LCID And &H3FF)               ' LCID's Primary language id
    sLangId = (LCID / (2 ^ 10))                   ' LCID's Sub language id
    
    Select Case PLangId                         ' determine primary language id
    Case LANG_CHINESE
        If (sLangId = SUBLANG_CHINESE_TRADITIONAL) Then
            sFont = ChrW$(&H65B0) & ChrW$(&H7D30) & ChrW$(&H660E) & ChrW$(&H9AD4)   ' New Ming-Li
            nFont = 9
            nCharset = CHARSET_CHINESEBIG5
        ElseIf (sLangId = SUBLANG_CHINESE_SIMPLIFIED) Then
            sFont = ChrW$(&H5B8B) & ChrW$(&H4F53)
            nFont = 9
            nCharset = CHARSET_CHINESESIMPLIFIED
        End If
    Case LANG_JAPANESE
        sFont = ChrW$(&HFF2D) & ChrW$(&HFF33) & ChrW$(&H20) & ChrW$(&HFF30) & _
                ChrW$(&H30B4) & ChrW$(&H30B7) & ChrW$(&H30C3) & ChrW$(&H30AF)
        nFont = 9
        nCharset = CHARSET_SHIFTJIS
    Case LANG_KOREAN
        If (sLangId = SUBLANG_KOREAN) Then
            sFont = ChrW$(&HAD74) & ChrW$(&HB9BC)
        ElseIf (sLangId = SUBLANG_KOREAN_JOHAB) Then
            sFont = ChrW$(&HAD74) & ChrW$(&HB9BC)
        End If
        nFont = 9
        nCharset = CHARSET_HANGEUL
    Case LANG_ARABIC
        sFont = "Tahoma"
        nFont = 8
        nCharset = CHARSET_ARABIC
    Case LANG_HEBREW
        sFont = "Tahoma"
        nFont = 8
        nCharset = CHARSET_HEBREW
    Case Else
        sFont = "Tahoma"
        nFont = 8
        nCharset = CHARSET_DEFAULT
    End Select
    ssFont = sFont
    snFont = nFont
    snCharset = nCharset
'-------------------------------------------------------
End Sub
'-------------------------------------------------------
