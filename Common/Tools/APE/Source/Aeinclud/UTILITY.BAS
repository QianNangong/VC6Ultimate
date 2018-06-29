Attribute VB_Name = "Utility"
Option Explicit

' Registry access API
Declare Function RegCloseKey Lib "advapi32.dll" (ByVal hKey As Long) As Long
Declare Function RegEnumValue Lib "advapi32.dll" Alias "RegEnumValueA" (ByVal hKey As Long, ByVal dwIndex As Long, ByVal lpValueName As String, lpcbValueName As Long, ByVal lpReserved As Long, lpType As Long, ByVal lpData As String, lpcbData As Long) As Long
Declare Function RegSetValueEx Lib "advapi32.dll" Alias "RegSetValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, lpData As Any, ByVal cbData As Long) As Long
Declare Function RegQueryInfoKey Lib "advapi32.dll" Alias "RegQueryInfoKeyA" (ByVal hKey As Long, ByVal lpClass As String, lpcbClass As Long, ByVal lpReserved As Long, lpcSubKeys As Long, lpcbMaxSubKeyLen As Long, lpcbMaxClassLen As Long, lpcValues As Long, lpcbMaxValueNameLen As Long, lpcbMaxValueLen As Long, lpcbSecurityDescriptor As Long, ByVal lpftLastWriteTime As Long) As Long
Declare Function RegQueryValueEx Lib "advapi32.dll" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, lpType As Long, lpData As Any, lpcbData As Long) As Long
Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, ByVal lpSecurityAttributes As Long, phkResult As Long, lpdwDisposition As Long) As Long
Declare Function RegDeleteKey Lib "advapi32.dll" Alias "RegDeleteKeyA" (ByVal hKey As Long, ByVal lpSubKey As String) As Long
Declare Function RegOpenKeyEx Lib "advapi32.dll" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, phkResult As Long) As Long

Declare Function GetTempPath Lib "Kernel32" Alias "GetTempPathA" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long
Declare Function GetTempFileNameAPI Lib "Kernel32" Alias "GetTempFileNameA" (ByVal lpszPath As String, ByVal lpPrefixString As String, ByVal wUnique As Long, ByVal lpTempFileName As String) As Long
Public Const MAX_PATH = 260

' Registry constants
Public Const ERROR_SUCCESS = 0
Public Const HKEY_CURRENT_USER = &H80000001
Public Const HKEY_LOCAL_MACHINE = &H80000002
Public Const REG_OPTION_NON_VOLATILE = 0
Public Const KEY_ALL_ACCESS = &HF003F     ' ((STANDARD_RIGHTS_ALL Or KEY_QUERY_VALUE Or KEY_SET_VALUE Or KEY_CREATE_SUB_KEY Or KEY_ENUMERATE_SUB_KEYS Or KEY_NOTIFY Or KEY_CREATE_LINK) And (Not SYNCHRONIZE))
Public Const REG_SZ = 1

Public Function AddItemToList(ctrControl As Control, ByVal sItem As String) As Boolean
' Adds an item to the control's list if it is not a duplicate
' Returns true if the item was added
    Dim i As Integer
    Dim bAddItem As Boolean
    
    sItem = Trim(sItem)
    With ctrControl
        bAddItem = True
        For i = 0 To .ListCount - 1
            If StrComp(sItem, .List(i), vbTextCompare) = 0 Then
                bAddItem = False
                Exit For
            End If
        Next
        If bAddItem Then
            .AddItem sItem
        End If
        AddItemToList = bAddItem
    End With
End Function

Sub AlignTextToBottom(ctlControl As Control, sText As String)
' Sets the default property of the specified control to the specified text such that the text is aligned to
' to the bottom of the control.  Wordwrap must be set to True.
' Currently, only Label controls are supported.
    Debug.Assert TypeName(ctlControl) = "Label"
    Dim iMaxLines As Integer     ' The max number of lines the control can handle
    Dim iNumLines As Integer    ' The number of lines in the current message
    With ctlControl
        iMaxLines = Int(.Height / .Parent.TextHeight("A"))
        iNumLines = Int(.Parent.TextWidth(sText) / .Width) + 1    ' Leave an additional line for wordwrap overflow
        If iNumLines < iMaxLines Then
            ctlControl = String(iMaxLines - iNumLines, vbCrLf) & sText    ' Pad with blank lines
        Else
            ctlControl = sText
        End If
        .Refresh
    End With
End Sub

Sub LoadResStrings(frm As Form)
    On Error Resume Next

    Dim ctl As Control
    Dim Obj As Object
    Dim iResID As Integer, i As Integer
    
    'set the form's caption
    If IsNumeric(frm.Tag) Then
        frm.Caption = LoadResString(CInt(frm.Tag))
    End If

    'set the controls' captions using the caption
    'property for menu items and the Tag property
    'for all other controls
    For Each ctl In frm.Controls
        Err.Clear
        Select Case TypeName(ctl)
            Case "Menu":
                iResID = CInt(Left$(ctl.Caption, 5))      ' Upto first 5 characters for res id, pad with spaces
                If Err = 0 Then
                    ctl.Caption = LoadResString(iResID)
                End If
            Case "SSTab":
                For i = 0 To ctl.Tabs - 1
                    iResID = CInt(Left$(ctl.TabCaption(i), 5))      ' Upto first 5 characters for res id, pad with spaces
                    If Err = 0 Then
                        ctl.TabCaption(i) = LoadResString(iResID)
                    End If
                Next
            Case "TabStrip":
                For Each Obj In ctl.Tabs
                    Err.Clear
                    If IsNumeric(Obj.Tag) Then
                        Obj.Caption = LoadResString(CInt(Obj.Tag))
                    End If
                    'check for a tooltip
                    If IsNumeric(Obj.ToolTipText) Then
                        If Err = 0 Then
                            Obj.ToolTipText = LoadResString(CInt(Obj.ToolTipText))
                        End If
                    End If
                Next
            Case "Toolbar":
                For Each Obj In ctl.Buttons
                    Err.Clear
                    If IsNumeric(Obj.Tag) Then
                        Obj.ToolTipText = LoadResString(CInt(Obj.Tag))
                    End If
                Next
            Case "ListView":
                For Each Obj In ctl.ColumnHeaders
                    Err.Clear
                    If IsNumeric(Obj.Tag) Then
                        Obj.Text = LoadResString(CInt(Obj.Tag))
                    End If
                Next
            Case Else
                If IsNumeric(ctl.Tag) Then
                    If Err = 0 Then
                        ctl.Caption = LoadResString(CInt(ctl.Tag))
                    End If
                End If
                'check for a tooltip
                If IsNumeric(ctl.ToolTipText) Then
                    If Err = 0 Then
                        ctl.ToolTipText = LoadResString(CInt(ctl.ToolTipText))
                    End If
                End If
        End Select
    Next
End Sub

Public Function RegKeyExists(hRootKey As Long, sKey As String) As Boolean
' Returns True if the specified key exists under the specified root key, else returns False
    Dim hKey As Long
    If RegOpenKeyEx(hRootKey, sKey, 0, KEY_ALL_ACCESS, hKey) = ERROR_SUCCESS Then
        RegCloseKey hKey
        RegKeyExists = True
    Else
        RegKeyExists = False
    End If
End Function

Public Function SaveRegSetting(strMainKey As String, strSubKey As String, strValueName As String, _
        strValue As String) As Boolean
' Saves the specified value in the registry under the key composed of the specified key and subkey.
' Only string values supported for now.
' Returns true if successful
    Dim hKey As Long, cbData As Long
    Dim strKey As String
    
    On Error GoTo SaveRegSettingError
    
    If strSubKey = "" Then
        strKey = strMainKey
    Else
        strKey = strMainKey & "\" & strSubKey
    End If
    If (RegCreateKeyEx(HKEY_LOCAL_MACHINE, strKey, 0, vbNullString, _
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, hKey, 0)) = ERROR_SUCCESS Then
        cbData = LenB(StrConv(strValue, vbFromUnicode))
        If RegSetValueEx(hKey, strValueName, 0, REG_SZ, ByVal strValue, cbData) <> ERROR_SUCCESS Then
            RegCloseKey hKey
            Err.Raise E_UNEXPECTED
        End If
        RegCloseKey hKey
        SaveRegSetting = True
    Else
        Err.Raise E_UNEXPECTED
    End If
    
SaveRegSettingResume:
    Exit Function
    
SaveRegSettingError:
    SaveRegSetting = False
    Resume SaveRegSettingResume
End Function

Public Sub SelectListItem(ctlList As Control, strValue As String)
'   Selects the item that matches strValue in ctlList (ListBox or Combo)

    Dim i As Integer, iNewIndex As Integer
    
    Debug.Assert TypeName(ctlList) = "ComboBox" Or TypeName(ctlList) = "ListBox"
    
    With ctlList
        iNewIndex = -1
        For i = 0 To .ListCount - 1
            If .List(i) = strValue Then iNewIndex = i
        Next
        .ListIndex = iNewIndex
    End With
End Sub

Public Function GetRegSetting(strMainKey As String, strSubKey As String, strValueName As String, _
        Optional strDefault As String = "", Optional hRootKey As Long = HKEY_LOCAL_MACHINE) As String
' Gets the specified value from the registry key composed of the specified key and subkey.
' Only string values supported for now.
    Dim hKey As Long, cbData As Long, lType As Long
    Dim strKey As String, strData As String * glMAX_NAME_LENGTH
    
    If strSubKey = "" Then
        strKey = strMainKey
    Else
        strKey = strMainKey & "\" & strSubKey
    End If
    
    If RegOpenKeyEx(hRootKey, strKey, 0, KEY_ALL_ACCESS, hKey) = ERROR_SUCCESS Then
        cbData = LenB(StrConv(strData, vbFromUnicode))
        Dim lReserved As Long
        If RegQueryValueEx(hKey, strValueName, 0, lType, ByVal strData, cbData) = ERROR_SUCCESS Then
            GetRegSetting = TruncateAtNull(strData)
        Else
            GetRegSetting = strDefault
        End If
        RegCloseKey hKey
    Else
        GetRegSetting = strDefault
    End If
End Function

Public Function GetTempDir() As String
' Returns the temp directory path
    Dim TmpDirLen As Long
    Dim TmpDir As String
    Dim TmpDirLenActual As Long
    
    TmpDirLen = GetTempPath(0, TmpDir)  ' Get the length needed for the temp directory
    TmpDir = Space(TmpDirLen)           ' Create enough space for it then get it
    TmpDirLenActual = GetTempPath(TmpDirLen, TmpDir)
    ' Strip off any extra stuff
    If TmpDirLen > TmpDirLenActual Then
        GetTempDir = Left(TmpDir, TmpDirLenActual)
    Else
        GetTempDir = App.Path & "\"   ' This code should never get executed - but, just in case
    End If
End Function

Public Function GetTempFileName() As String
' Returns a unique filename.
    Dim sTempDir As String, sFileName As String
            
    sTempDir = GetTempDir
    sFileName = Space$(MAX_PATH)
    If GetTempFileNameAPI(sTempDir, "ape", 0, sFileName) <> 0 Then
        GetTempFileName = TruncateAtNull(sFileName)
    Else
        GetTempFileName = ""
    End If
End Function

Function Substitute(sString As String, sFind As String, sReplace As String) As String
' Substitutes string sReplace in place of string sFind in sString
    Dim lStart As Long, lEnd As Long, lFindLength As Long
    Dim sNewString As String
    
    sNewString = ""
    lFindLength = Len(sFind)
    lStart = 1
    lEnd = InStr(lStart, sString, sFind)
    Do While lEnd > 0
        sNewString = sNewString & Mid(sString, lStart, lEnd - lStart) & sReplace
        lStart = lEnd + lFindLength
        lEnd = InStr(lStart, sString, sFind)
    Loop
    Substitute = sNewString & Mid(sString, lStart)
End Function

Public Function RemoveAmpersands(strString As String) As String
' Removes the ampersands in the specified string.
    RemoveAmpersands = Substitute(strString, "&", "")
End Function

Public Function DSNExists(sDSN As String) As Boolean
' Returns True if the specified DSN exists, else returns False.
    If sDSN = "" Then
        DSNExists = False
    Else
        DSNExists = RegKeyExists(HKEY_CURRENT_USER, gsODBC_INI_REG_KEY & "\" & sDSN)
    End If
End Function

Public Function GetDSNValue(sDSN As String, sValueName As String) As String
' Returns the specified value (sValueName) of the specified DSN (sDSN)
    GetDSNValue = GetRegSetting(gsODBC_INI_REG_KEY, sDSN, sValueName, "", HKEY_CURRENT_USER)
End Function

Public Function GetAllRegSettings(strMainKey As String, strSubKey As String) As Variant
' Returns an array of all values under the registry key composed of the specified key and subkey.
' Only string values supported for now.
    Dim hKey As Long, cbData As Long, cbValueName As Long, lType As Long
    Dim strKey As String, strData As String * giMAX_REG_DATA_LENGTH, strValueName As String * glMAX_NAME_LENGTH
    Dim aValues() As String
    
    If strSubKey = "" Then
        strKey = strMainKey
    Else
        strKey = strMainKey & "\" & strSubKey
    End If
    If (RegCreateKeyEx(HKEY_LOCAL_MACHINE, strKey, 0, vbNullString, _
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, hKey, 0)) = ERROR_SUCCESS Then
        ' Iterate over all the values in this key
        Dim strClass As String * glMAX_NAME_LENGTH
        Dim cbClass As Long, cSubKeys As Long, cbMaxSubKeyLen As Long, cbMaxClassLen As Long, lReserved As Long
        Dim cValues As Long, cbMaxValueNameLen As Long, cbMaxValueLen As Long, cbSecurityDescriptor As Long
        
        cbClass = LenB(StrConv(strClass, vbFromUnicode))
        If RegQueryInfoKey(hKey, strClass, cbClass, lReserved, cSubKeys, cbMaxSubKeyLen, cbMaxClassLen, cValues, cbMaxValueNameLen, _
                cbMaxValueLen, cbSecurityDescriptor, 0) = ERROR_SUCCESS Then
            If cValues > 0 Then
                ReDim aValues(0 To cValues - 1, 0 To 1)
                Dim i As Long
                For i = 0 To cValues - 1
                    cbValueName = LenB(StrConv(strValueName, vbFromUnicode))
                    cbData = LenB(StrConv(strData, vbFromUnicode))
                    If RegEnumValue(hKey, i, strValueName, cbValueName, 0, lType, strData, cbData) = ERROR_SUCCESS Then
                        aValues(i, 0) = TruncateAtNull(strValueName)
                        aValues(i, 1) = TruncateAtNull(strData)
                    End If
                Next
                GetAllRegSettings = aValues
            End If
        End If
        RegCloseKey hKey
    End If
End Function

Public Function DeleteRegSettings(strMainKey As String, strSubKey As String) As Boolean
' Deletes the specified key and all its sub keys
' Returns true is successful
    Dim strKey As String
    
    If strSubKey = "" Then
        strKey = strMainKey
    Else
        strKey = strMainKey & "\" & strSubKey
    End If
    DeleteRegSettings = (RegDeleteKey(HKEY_LOCAL_MACHINE, strKey) = ERROR_SUCCESS)
End Function

Public Function Min(a, b)
    If a < b Then
        Min = a
    Else
        Min = b
    End If
End Function

Public Function Max(a, b)
    If b > a Then
        Max = b
    Else
        Max = a
    End If
End Function

Public Function TruncateAtNull(ByVal strText As String) As String
' Returns the specified string truncated at the first null character
    Dim lLen As Long
    
    lLen = InStr(strText, Chr(0))
    If lLen < 1 Then
        TruncateAtNull = strText
    Else
        TruncateAtNull = Left(strText, lLen - 1)
    End If
End Function

Public Function TruncateFile(sFileName As String, dSize As Double)
' Truncates the specified file to lSize bytes.  Truncation occurs at the beginning of the file.
    Const CHUNK_SIZE = 65535        ' Size of chunk for copying file contents
    
    Dim dOverflow As Double
    dOverflow = CDbl(FileLen(sFileName)) - dSize
    If dOverflow > 0 Then
        Dim sTempFile As String, baChunk(CHUNK_SIZE) As Byte
        Dim lFileNum As Long, lTempFileNum As Long
        sTempFile = GetTempFileName
        lFileNum = FreeFile
        Open sFileName For Binary Access Read As lFileNum
        lTempFileNum = FreeFile
        Open sTempFile For Binary Access Write As lTempFileNum
        Seek lFileNum, dOverflow + 1
        ' Start copying file from first complete line - fill the very first (incomplete) line with "." characters.
        Get lFileNum, , baChunk
        Dim l As Long
        For l = LBound(baChunk) To UBound(baChunk)
            If baChunk(l) <> 13 Then         ' = vbCr
                baChunk(l) = 46      ' = Asc(".")
            Else
                Put lTempFileNum, , baChunk
                Exit For
            End If
        Next
        Do While Not EOF(lFileNum)
            Get lFileNum, , baChunk
            Put lTempFileNum, , baChunk
        Loop
        Close lFileNum
        Close lTempFileNum
        FileCopy sTempFile, sFileName
        Kill sTempFile
    End If
End Function

Public Function StripPath(strFileName As String, Optional bStripExtension As Boolean = False) As String
' Strips the path off the specified fully qualified filename.  If bStripExtension is True, the file extension is stripped as well.
    Dim iStart As Integer, iNext As Integer, iActualStart As Integer
    
    ' First, find the beginning of the actual filename
    iStart = 0
    Do
        iNext = InStr(iStart + 1, strFileName, "\")
        If iNext = 0 Then
            Exit Do
        End If
        iStart = iNext
    Loop
    iActualStart = iStart + 1     ' Point to beginning of actual filename
    ' Next, find the beginning of the extension (which immediately follows the last period in the filename)
    If bStripExtension Then
        Do
            iNext = InStr(iStart + 1, strFileName, ".")
            If iNext = 0 Then
                Exit Do
            End If
            iStart = iNext
        Loop
    End If
    If iStart = iActualStart - 1 Then   ' If no extension
        StripPath = Mid(strFileName, iActualStart)
    Else
        StripPath = Mid(strFileName, iActualStart, iStart - iActualStart)
    End If
End Function

Public Sub SizeToFit(ctl As Control)
' Resizes the control to fit the displayed text
    With ctl
        Select Case TypeName(ctl)
            Case "CheckBox", "OptionButton"
                .Width = .Parent.TextWidth(.Caption) + .Height      ' Compensate for non-text area
            Case Else
                Debug.Assert False
        End Select
    End With
End Sub

Public Function SubstituteParams(bstrString As String, ParamArray aParams()) As String
' Substitutes the parameters in the paramarray for placeholders in the string.  The placeholders are of the format
' '%n' where 'n' represents the index of the parameter in the paramarray.
' A maximum of 10 parameters (0 - 9) are supported.

    Dim iStart As Integer, iPtr As Integer, iParamNum As Integer
    Dim cChar As String * 1
    Dim bstrReturn As String
    
    iStart = 1
    iPtr = InStr(bstrString, "%")
    Do While iPtr > 0 And iPtr < Len(bstrString)
        bstrReturn = bstrReturn & Mid$(bstrString, iStart, iPtr - iStart)
        cChar = Mid(bstrString, iPtr + 1, 1)
        If IsNumeric(cChar) Then
            iParamNum = Val(cChar)
            If iParamNum <= UBound(aParams) Then
                bstrReturn = bstrReturn & aParams(iParamNum)
            End If
        End If
        iStart = iPtr + 2
        iPtr = InStr(iStart, bstrString, "%")
    Loop
    SubstituteParams = bstrReturn & Mid(bstrString, iStart)
End Function


