Attribute VB_Name = "modAEGlobals"
Option Explicit

'==================================================
' Routine: ReplaceString
'
' Purpose: Replaces specified string in a target
'          string with a new string
' Arguments:
'   sTarget: string to work on
'   sSearch: string to replace in sTarget
'   sNew: value to replace sSearch with
' Outputs:
'   Revised version of sTarget (Note: sTarget is
'   NOT modified.)
'==================================================
Function ReplaceString(ByVal sTarget As String, sSearch As String, sNew As String) As String
    Dim p As Integer
    Do
        p = InStr(sTarget, sSearch)
        If p Then
            sTarget = Left(sTarget, p - 1) + sNew + Mid(sTarget, p + Len(sSearch))
        End If
    Loop While p
    ReplaceString = sTarget
End Function

'==================================================
' Routine: Round
'
' Purpose: Converts the passed Single value to the
'          nearest integer value
'          In contrast to CInt or Clng which convert
'          single values to the nearest even integer
'==================================================
Public Function Round(sngIn As Single) As Long
    If (sngIn Mod 1) < 0.5 Then
        Round = Fix(sngIn)
    Else
        Round = Fix(sngIn) + 1
    End If
End Function

Public Function FormatPath(sPath As String) As String
    '-------------------------------------------------------------------------
    'Purpose:   Assures that the passed path has a "\" at the end of it
    'IN:
    '   [sPath]
    '           a valid path name
    'Return:    the same path with a "\" on the end if it did not already
    '           have one.
    '-------------------------------------------------------------------------
    If Right$(sPath, 1) <> "\" Then sPath = sPath & "\"
    FormatPath = sPath
End Function

Public Function GetArrayFromDelimited(sDelimited As String, sa() As String, Optional sDelimiter As String = ",") As Boolean
    '-------------------------------------------------------------------------
    'Purpose:   Fills the passed a single dimension string array with the
    '           values in the specified delimited string.  Leading and trailing spaces are trimmed
    '           from each substring before adding them to the array.
    'IN:
    '   [sDelimited]
    '           Delimited string
    '   [sDelimiter]
    '           Delimiter
    'Out:
    '   [sa()]  Single dimension array that will be erased and redimensioned to
    '           add values from delimited string
    'Return:    True if any items were added to array, False if array was
    '           left empty
    '-------------------------------------------------------------------------
    Dim l As Long, lCount As Long, lStart As Long, lEnd As Long, lDelimiterLength As Long
    
    lDelimiterLength = Len(sDelimiter)
    If sDelimited = "" Then
        Erase sa
        GetArrayFromDelimited = False
    Else
        lCount = 0
        lStart = 1 - lDelimiterLength
        Do
            lCount = lCount + 1
            lStart = InStr(lStart + lDelimiterLength, sDelimited, sDelimiter)
        Loop While lStart > 0
        ReDim sa(0 To lCount - 1)
        lStart = 1
        For l = LBound(sa) To UBound(sa) - 1        ' Process all but the last item in the list
            lEnd = InStr(lStart, sDelimited, sDelimiter)
            Debug.Assert lEnd <> 0
            sa(l) = Trim(Mid(sDelimited, lStart, lEnd - lStart))
            lStart = lEnd + lDelimiterLength
        Next
        sa(l) = Trim(Mid(sDelimited, lStart))     ' Final string in the list
        GetArrayFromDelimited = True
    End If
End Function

Public Function GetDelimitedFromArray(sa() As String, Optional sDelimiter As String = ",") As String
    '-------------------------------------------------------------------------
    'Purpose:   Reads all the strings in the passed array and
    '           creates a delimited string
    'IN:
    '   [sa()]
    '           A single dimension string array
    '   [sDelimiter]
    '           Delimiter
    'Returns:   a delimited string
    '-------------------------------------------------------------------------
    
    Dim sString As String
    Dim l As Long
    
    If Not ArrayHasElements(sa) Then
        GetDelimitedFromArray = ""
    Else
        sString = ""
        For l = LBound(sa) To UBound(sa)
            sString = sString & sDelimiter & sa(l)      ' Always prepend delimiter (even to the first element)
        Next
        GetDelimitedFromArray = Mid(sString, Len(sDelimiter) + 1)   ' Drop the leading delimiter
    End If
End Function

Public Function ArrayHasElements(ByVal v As Variant) As Boolean
' Returns True if the specified variant contains an array that contains any elements, else returns False.
    If Not IsArray(v) Then
        ArrayHasElements = False
    Else
        Dim l As Long
        On Error Resume Next
        l = LBound(v)
        ArrayHasElements = (Err.Number <> ERR_SUBSCRIPT_OUT_OF_RANGE)
    End If
End Function

