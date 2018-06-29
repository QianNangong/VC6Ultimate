#include "MacroCPP.h"
//---------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, void* lpReserved){
	switch (ul_reason_for_call){
		case DLL_PROCESS_ATTACH:
			(void*&)GetApp = GetProcAddress(GetModuleHandleW(0), "?GetApp@@YIPAXXZ");
			SpAssert(GetApp);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
  }
  return TRUE;
}
//---------------------------------------------------------------------------------------------
TextDocument GetActiveTextDoc(){
	IApplication& Application = gApp();
	GenericDocument Doc = Application.ActiveDocument;
	if (!&Doc || Doc.Type != L"Text")
		return TextDocument();
	return (TextDocument&)Doc;
}
//---------------------------------------------------------------------------------------------
inline BOOL IsEOL(char c){ return (c=='\r' || c=='\n'); }
//---------------------------------------------------------------------------------------------
inline BOOL IsWhiteChar(char c){
	return (c==' ' || c=='\t');
}
//---------------------------------------------------------------------------------------------
struct TEXT_FILE{
	WCHAR FileName[MAX_PATH*2];
	char* Buffer;
	int		BufferSize;
	long	ContentSize;
	//-----------------
	TEXT_FILE(WCHAR const* szFileName, int ExtraSize=0):Buffer(NULL){
		FILE* f = _wfopen(szFileName, L"rb");
		if (!f)
			return;

		fseek(f, 0, SEEK_END);
		ContentSize = ftell(f);
		fseek(f, 0, SEEK_SET);
		BufferSize = ContentSize + 1 + ExtraSize;
		Buffer = new char[BufferSize];
		fread(Buffer, ContentSize, 1, f);
		fclose(f);
		Buffer[ContentSize]=0;
		//-----------------------
	}
	//-----------------
	~TEXT_FILE(){
		if (Buffer)
			delete Buffer;
	}
	//-----------------
	int SearchFrom(int Start, char const* szString, BOOL bEndOf=false) const{
		char* pStart = strstr(Buffer+Start, szString);
		if (bEndOf && pStart)
			pStart+=strlen(szString);
		return pStart ? (pStart-Buffer) : -1;
	}
	//-----------------
	int Search(char const* szString, BOOL bEndOf=false) const{
		return SearchFrom(0, szString, bEndOf);
	}
	//-----------------
	void ReplaceSelection(int SelectionStart, int SelectionLen, char const* NewSelectionContent, int NewContentLen){
		int delta = NewContentLen-SelectionLen;
		if (delta){
			if (ContentSize+delta >= BufferSize){
				char* NewBuffer = new char[ContentSize+delta+1];
				memcpy(NewBuffer, Buffer, ContentSize+1);
				delete Buffer;
				Buffer = NewBuffer;
			}
			char* pPosEnd = (Buffer+SelectionStart+SelectionLen);
			memmove(pPosEnd+delta, pPosEnd, ContentSize+1 - SelectionStart - SelectionLen);
			ContentSize+=delta;
		}
		memcpy(Buffer+SelectionStart, NewSelectionContent, NewContentLen);
	}
	//-----------------
	void Insert(int SelectionStart, char const* NewSelectionContent, int NewContentLen){
		ReplaceSelection(SelectionStart, 0, NewSelectionContent, NewContentLen);
	}
	//-----------------
	int EndOfLine(int CurrentPos) const{
		char* pStart = Buffer+CurrentPos;
		char* p = pStart;
		char c;
		while((c=*p) && !IsEOL(c))
			++p;
		return (p-Buffer);
	}
	//-----------------
	int StartOfNextLine(int CurrentPos) const{
		int EOL = EndOfLine(CurrentPos);
		char* p = Buffer + EOL;
		if (p[0]==0xD){
			if (p[1]==0xA)
				p+=2;
			else
				++p;
		} else if (p[0]==0xA){
			++p;
		}
		return p-Buffer;
	}
	//-----------------
	int StartOfLine(int CurrentPos) const{
		while(CurrentPos>0){
			if (IsEOL(Buffer[CurrentPos-1]))
				break;
			--CurrentPos;
		}
		return CurrentPos;
	}
	//-----------------
	int SkipWhites(int CurrentPos) const{
		while(IsWhiteChar(Buffer[CurrentPos]))
			++CurrentPos;
		return CurrentPos;
	}
	//-----------------
	char* GetAt(int CurrentPos){ return Buffer+CurrentPos;}
	//-----------------
};
//---------------------------------------------------------------------------------------------
#if 1
//---------------------------------------------------------------------------------------------
int GetRCFileName(WCHAR* DspName, OUT char* RCFileName){
	TEXT_FILE Content(DspName);
	//-----------------------
	int Cursor=Content.Search(".rc");
	while(Cursor>=0){
		//---------------
		int OldCursor = Cursor;
		Cursor = Content.StartOfLine(Cursor);
		Cursor = Content.SkipWhites(Cursor);
		if (!strncmp(Content.GetAt(Cursor), "SOURCE", 6)){
			Cursor+=6;
			Cursor = Content.SkipWhites(Cursor);
			if (*Content.GetAt(Cursor)=='='){
				Cursor = Content.SkipWhites(Cursor+1);
				int EOL = Content.EndOfLine(Cursor);
				int Len = EOL-Cursor;
				if (*Content.GetAt(Cursor)=='"'){ /// On vire les guillemets
					++Cursor;
					Len-=2;
				}
				memcpy(RCFileName, Content.GetAt(Cursor), Len);
				RCFileName[Len]=0;
				return Len;
			}
 		}
		//---------------
		Cursor = Content.SearchFrom(OldCursor+1, ".rc");
	}
	gApp().PrintToOutputWindow(L"ERROR : RC filename not found (have you saved your .DSP ?)");
	return 0;
}
//---------------------------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------------------------
_bstr_t GetRCFileName(WCHAR* DspName){
	IApplication& Application = gApp();

	Application.Documents.Open(DspName, L"Text");
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return L"";

	BOOL bIsFind = ActiveDocument.Selection.FindText(L".rc", dsMatchFromStart);
	while(bIsFind){
		ActiveDocument.Selection.StartOfLine(dsFirstText);
		ActiveDocument.Selection.CharRight(dsExtend);
		if (ActiveDocument.Selection.Text!=L"#"){
			ActiveDocument.Selection.StartOfLine();
			ActiveDocument.Selection.CharRight(dsMove, 7);
			ActiveDocument.Selection.EndOfLine(dsExtend);
			_bstr_t RCName = ActiveDocument.Selection.Text;
			if (Left(RCName, 2) == L".\\") { 
				RCName = Mid(RCName, 3);
			}
	 		Application.ActiveWindow.Close(dsSaveChangesPrompt);
			return RCName;
		}
		ActiveDocument.Selection.StartOfLine(dsFirstText);
		ActiveDocument.Selection.LineDown();
		ActiveDocument.Selection.FindText(L".rc");
	}
	Application.ActiveWindow.Close(dsSaveChangesPrompt);
	Application.PrintToOutputWindow(L"ERROR : RC filename not found (have you saved your .DSP ?)");
	return L"";
}
#endif
//-------------------------------------------------------------------------
inline int isspace(char c){
	return isspace((int)(unsigned char)c);
}
//-------------------------------------------------------------------------------------------------------
// Renvoie la fin du nombre dans 'len'
inline long atoi(char const* _ptr, OUT int& len){
	typedef unsigned char uchar;
	uchar const* ptr=(uchar const*)_ptr;

	uchar const* start=ptr;

  while (isspace(*ptr))
		++ptr;

  int c = (int)*ptr++;
  int sign = c;									// if '-', then negative, otherwise positive
  if (c == '-' || c == '+')
    c = (int)*ptr++;    // skip sign

  long total = 0;

  while ( (c = ((c >= '0' && c <= '9') ? (c - '0') : -1)) != -1 ) {
    total = 10 * total + c;			// accumulate digit
    c = *ptr++;					// get next char
  }

	len=int((ptr-start)-1);

  return (sign == '-') ? -total : total;
}
//-------------------------------------------------------------------------------------------------
// ############################################# IncreaseVersionInFile (Filename) #################################################################################################
void IncreaseVersionInFile(WCHAR* Filename){
	//DESCRIPTION: Build active project and increase version counters in rc script.
	// (c)'98 Th. Mahler and parts by LJP.
	// Modified by N.E. Powroz -- Dec 1999
	// W. Koehne -- Dec 1999

	//------------SAVE BACK-UP OF FILE----------------------
	//Documents.Open (Filename), "Text"
	//Windows(Filename).Active = True;
	//ActiveDocument.Save (Filename+"~")
	//ActiveDocument.Close()
	//-----------------------------------------------------
	IApplication& Application = gApp();

#if 1
	TEXT_FILE Content(Filename, 256);
	if (!Content.Buffer)
		return;
	//-----------------------
#if 0
    FILEVERSION     6,0,7,56658
#endif
	{
		int Start = Content.Search("FILEVERSION", true);
		if (Start>=0){
			char* p=Content.Buffer+Start;
			char c;
			int CommaCount=0;
			while((c=*p) && !IsEOL(c)){
				++p;
				if (c==','){
					++CommaCount;
					if (CommaCount==3){
						while((*p) && *p==' ')
							++p;
						{
							int SelectionStart = p-Content.Buffer;
							int len;
							DWORD BuildVersion = atoi(p, OUT len);
							++BuildVersion;
							char Number[64];
							int len2=sprintf(Number, "%d", BuildVersion);
							Content.ReplaceSelection(SelectionStart, len, Number, len2);
						}
						break;
					}
				}
			}
		}
	}
	//-----------------------
#if 0
    VALUE "FileVersion", "6, 0, 7, 56658\0"
#endif
	{
		int Start = Content.Search("VALUE \"FileVersion\"", true);
		if (Start>=0){
			char* p=Content.Buffer+Start;
			char c;
			int CommaCount=0;
			while((c=*p) && !IsEOL(c)){
				++p;
				if (c==','){
					++CommaCount;
					if (CommaCount==4){
						while((*p) && *p==' ')
							++p;
						{
							int SelectionStart = p-Content.Buffer;
							int len;
							DWORD BuildVersion = atoi(p, OUT len);
							char* pPosEnd = p+len;
							++BuildVersion;
							char Number[64];
							int len2=sprintf(Number, "%d", BuildVersion);
							Content.ReplaceSelection(SelectionStart, len, Number, len2);
							//-----------------------
#if 0
		VALUE "SpecialBuild", "11/11/2015 20:43:41"
#endif
							//-----------------------
							int SpecialBuildStart = Content.SearchFrom(SelectionStart+len2, "VALUE \"SpecialBuild\"");
							if (SpecialBuildStart>=0){
								int EOL = Content.EndOfLine(SpecialBuildStart);
								int len = EOL-SpecialBuildStart;

								char szDate[256];
								SYSTEMTIME Date = Now();
								int len2=sprintf(szDate, "VALUE \"SpecialBuild\", \"%02d/%02d/%04d %02d:%02d:%02d\"", Date.wDay, Date.wMonth, Date.wYear, Date.wHour, Date.wMinute, Date.wSecond);
								Content.ReplaceSelection(SpecialBuildStart, len, szDate, len2);

							}else{
								int ProductVersionStart = Content.SearchFrom(SelectionStart+len2, "VALUE \"ProductVersion\"");
								if (ProductVersionStart>=0){
									int SOL = Content.StartOfNextLine(ProductVersionStart);
									char szDate[256];
									SYSTEMTIME Date = Now();
									int len2=sprintf(szDate, "            VALUE \"SpecialBuild\", \"%02d/%02d/%04d %02d:%02d:%02d\"\r\n", Date.wDay, Date.wMonth, Date.wYear, Date.wHour, Date.wMinute, Date.wSecond);
									Content.Insert(SOL, szDate, len2);
								}
							}
							//-----------------------
						}
						break;
					}
				}
			}
		}
	}
	//-----------------------
	{
		FILE* f = _wfopen(Filename, L"wb");
		if (f){
			fwrite(Content.Buffer, Content.ContentSize, 1, f);
			fclose(f);
		}
	}
#else
	BOOL bDir;
	if (!FileExistsW(Filename, OUT bDir) || bDir)
		return; //do nothing (or add code here to search *.rc file etc.)

	//open *.rc file as text
	GenericDocument Doc = Application.Documents.Open(Filename, L"Text");
	if (!&Doc || Doc.Type != L"Text")
		return;
	TextDocument& oDoc = (TextDocument&)Doc;

	//-----------------------------------------------------
	//Go to 2nd line in rc file, delete it, and get current time/date
	//Dim Sel
	//Set Sel=ActiveDocument.Selection
	//Sel.GoToLine 2
	//Sel.SelectLine

	//if Left(Sel, 3) == "//#" Then 'only delete our own comment
	//	Sel.Delete
	//} else {
	//	Sel.StartOfLine 'insert our comment at this place
	//}

	//write current date/time and add newline - default formatted
	//Sel.Text ="//# Last version auto update " + CStr(now) + vbCrLf

	//---------------------------------------------------------------------------------------
	BOOL ret = oDoc.Selection.FindText(L"FILEVERSION", dsMatchForward | dsMatchFromStart | dsMatchCase | dsMatchWord);
	if (ret && wcslen(oDoc.Selection.Text)>0) {
		// move to eol and { to end of build number:
		oDoc.Selection.EndOfLine(dsExtend);
		_bstr_t strOldLine = oDoc.Selection.Text;
		oDoc.Selection.EndOfLine();

		// mark Build Number and store in strSelection
		oDoc.Selection.WordLeft(dsExtend, 1);

		TextSelection strSelection = oDoc.Selection;
		int strOldFileVersion = _wtoi(strSelection.Text);
		WCHAR strNewFileVersion[20];
		swprintf(strNewFileVersion, L"%d", strOldFileVersion + 1);

		// Replace file entry
		strSelection.Text = strNewFileVersion;
		oDoc.Selection.StartOfLine(); 
		oDoc.Selection.EndOfLine();
		WCHAR Buffer[1024];
		swprintf(Buffer, L"\"%s\" => \"%s\"", (WCHAR const*)strOldLine, (WCHAR const*)oDoc.Selection.Text);
		Application.PrintToOutputWindow(Buffer);
	} 

	//---------------------------------------------------------------------------------------

	ret = oDoc.Selection.FindText(L"VALUE \"FileVersion\",", dsMatchForward | dsMatchFromStart | dsMatchCase);
	if (ret && Len(oDoc.Selection.Text)>0) {
		// move to eol and { to end of build number:
		oDoc.Selection.EndOfLine(dsExtend);
		_bstr_t strOldLine = oDoc.Selection.Text;
		oDoc.Selection.EndOfLine();
		oDoc.Selection.CharLeft(dsMove, 3);

		// mark Build Number and store in strVersion
		oDoc.Selection.WordLeft(dsExtend, 1);

		TextSelection strSelection = oDoc.Selection;
		int strOldFileVersion = _wtoi(strSelection.Text);
		WCHAR strNewFileVersion[20];
		swprintf(strNewFileVersion, L"%d", strOldFileVersion + 1);

		oDoc.Selection.Text = strNewFileVersion;
		oDoc.Selection.StartOfLine(); 
		oDoc.Selection.EndOfLine(dsExtend);
		WCHAR Buffer[1024];
		swprintf(Buffer, L"%s => %s", (WCHAR const*)strOldLine, (WCHAR const*)oDoc.Selection.Text);
		Application.PrintToOutputWindow(Buffer);
	} 

	//---------------------------------------------------------------------------------------

	ret = oDoc.Selection.FindText(L"VALUE \"SpecialBuild\", ", dsMatchForward | dsMatchFromStart | dsMatchCase);
	if (ret && Len(oDoc.Selection.Text)>0) {
		WCHAR Buffer[1024];
		swprintf(Buffer, L"<%s>\r\n", (WCHAR const*)oDoc.Selection.Text);
		Application.PrintToOutputWindow(Buffer);
		oDoc.Selection.CharRight();
		oDoc.Selection.EndOfLine(dsExtend);
		oDoc.Selection.Delete();
		swprintf(Buffer, L"\"%s\"", (WCHAR const*)CStr(Now()));
		oDoc.Selection.Text = Buffer;
	} else {
		ret = oDoc.Selection.FindText(L"VALUE \"ProductVersion\", ", dsMatchForward | dsMatchFromStart | dsMatchCase);
		if (ret && Len(oDoc.Selection.Text)>0) {
			oDoc.Selection.EndOfLine();
			oDoc.Selection.NewLine();
			WCHAR Buffer[1024];
			swprintf(Buffer, L"VALUE \"SpecialBuild\", \"%s\"", (WCHAR const*)CStr(Now()));
		}
	} 
	//---------------------------------------------------------------------------------------
	oDoc.Save();
#endif

}
//-------------------------------------------------------------------------
EXPORT IncResVersion(){
	//DESCRIPTION: increase version counter in rc script.
	//Modified by : Dr Hoiby (hoibian@mail.dotcom.fr)
	// W. Koehne -- Dec 1999
	IApplication& Application = gApp();
	Application.PrintToOutputWindow(L"---------------- IncResVersion (C++) -----------------");

	int WindowCount = Application.Windows.Count;

	GenericWindow OldActiveWindow = WindowCount ? Application.ActiveWindow : NULL;

	_bstr_t DspName = Left(Application.ActiveProject.FullName, Len(Application.ActiveProject.FullName)-3)+L"dsp";
	Application.PrintToOutputWindow(L"Opening : "+DspName);

	BOOL bDir;
	if (!FileExistsW(DspName, OUT bDir) || bDir)
		return; ///-- do nothing (or add code here to search *.rc file etc.)

	char rcFileName[MAX_PATH*2];
	int rcFileNameLen = GetRCFileName(DspName, OUT rcFileName);
	if (!rcFileNameLen){
		//MsgBox "No .RC file found in project - terminating"
		return;
	}

	WCHAR Msg[MAX_PATH*2];
	swprintf(Msg, L"rcFileName :%S", rcFileName);

	Application.PrintToOutputWindow(Msg);

	///-- open *.rc file as text
	//-------------------------------
	WCHAR RCName[MAX_PATH*2];
	WCHAR* FilePart;
	GetFullPathNameW(DspName, MAX_PATH*2, OUT RCName, &FilePart);
	swprintf(FilePart, L"%S", rcFileName);

//	Application.PrintToOutputWindow(L"RCName : " + RCName);
// 	if (WindowCount==0) {
// 		ActiveWindow.Close dsSaveChangesPrompt
// 	else if (ActiveWindow!=OldActiveWindow) {
// 		ActiveWindow.Close dsSaveChangesPrompt
// 	}
	//-------------------------------
	IncreaseVersionInFile(RCName);
	//-------------------------------

// 	if (WindowCount==0) {
// 		if (Windows.Count!=0) 
// 			ActiveWindow.Close(dsSaveChangesPrompt);
// 	else if (ActiveWindow!=OldActiveWindow) {
// 		ActiveWindow.Close(dsSaveChangesPrompt);
// 		Windows(OldActiveWindow).Active=True;
// 	}

	Application.PrintToOutputWindow(L"------------------------------------------------");
}
//-------------------------------------------------------------------------
EXPORT ShowAllBreakpoints(){
	IApplication& Application = gApp();
	Application.PrintToOutputWindow(L"--------- All Breakpoints -----------------------");
	Breakpoints Breakpoints = Application.Debugger.Breakpoints;
	int Count = Breakpoints.Count;
	for (int iBreakpoint=0; iBreakpoint<Count; ++iBreakpoint){
		IBreakpoint& bp = Breakpoints[iBreakpoint];
		WCHAR Buffer[1024];
		swprintf(Buffer, L"%s(%s): ", (WCHAR const*)bp.File, (WCHAR const*)Right(bp.Location, Len(bp.Location)-1));
		Application.PrintToOutputWindow(Buffer);
	}
}
//-------------------------------------------------------------------------
//Allows the user to make sure the current header file is included only once. 
EXPORT OneTimeInclude(){
//DESCRIPTION: Adds code to the current header file so it is included only once per c/cpp file.
	IApplication& Application = gApp();

	TextDocument TextDoc = GetActiveTextDoc();
	if (!TextDoc)
		return;

	_bstr_t ext = TextDoc.Name;
	if (ext.isEmpty()){
		//If MsgBox("The file you are working with does not have a file extension." + vbLF + "Are you sure this is a C/C++ header file?", 4) == vbCancel Then
		//	return;
		//End If
		//ext = "nofilenamegiven.h"
		return;
	}
	//DocName = UCase(ext)
	_bstr_t DocName = ext;
	int pos = InStr(ext, L".");
	while(pos != 1){
		ext = Mid(ext, pos, (Len(ext) - pos + 1));
		pos = InStr(ext, L".");
	}
	ext = LCase(ext);
	pos = InStr(DocName, L".");
	if (!wcscmp(ext, L".h") || !wcscmp(ext,L".hpp") || !wcscmp(ext,L".hxx")){
		//Warn user that this will not work with a compiler other than VC++.
		//If MsgBox("This macro uses the Visual C++ dependant #pragma once" + vbLf + "Is the source to be portable across compilers?", 4) == 6 Then

			TextDoc.Selection.StartOfDocument();

			//Examp = "__" + UCase(Left(DocName, pos - 1)) + "__INCLUDE__" + UCase(Right(ext, Len(ext) - 1)) + "__";
			//ControlVarName = Examp;
			//ControlVarName = InputBox("What should the control variable be?" + vbLf + vbLf + "Example: #ifdef " + Examp, "One time header include protection", Examp)
			//If ValidId(ControlVarName) == True Then
				TextDoc.Selection.Text = 
					L"#pragma once\r\n"
					L"//-------------------------------------------------------------------------------------------------\r\n";
				//ActiveDocument.Selection = "#ifndef " + ControlVarName + vbLf + "#define " + ControlVarName + vbLf + "//-------------------------------------------------------------------------------------------------" + vbLf

				//ActiveDocument.Selection.EndOfDocument(False)
				//ActiveDocument.Selection = vbLf + "//-------------------------------------------------------------------------------------------------" + vbLf + "#endif //" + ControlVarName + vbLf
			//Else
			//	MsgBox(ControlVarName + " is not a valid c identifier." + vbLf + "please re-run the macro with a valid C identifier")
			//End If
		//Else
		//	ActiveDocument.Selection.StartOfDocument(False)
		//	ActiveDocument.Selection = "#pragma once" + vbLf + vbLf
		//End If
	//Else
	//	MsgBox("This macro can only be run on .h or .hpp files")
	} else if (!wcscmp(ext,L".cpp") || !wcscmp(ext,L".c") || !wcscmp(ext,L".cxx")){

		TextDoc.Selection.StartOfDocument();
		WCHAR Buffer[1024];
		swprintf(Buffer,
			L"#include \"%s.h\"\r\n"
			L"//-------------------------------------------------------------------------------------------------\r\n"
			L"//-------------------------------------------------------------------------------------------------\r\n", (WCHAR const*)Left(DocName, pos - 1)
		);
		TextDoc.Selection.Text = Buffer;
		TextDoc.Selection.StartOfDocument();
	}
}
//-------------------------------------------------------------------------
EXPORT DeclareM(){
//DESCRIPTION: Declare m pour le this->
	TextDocument TextDoc = GetActiveTextDoc();
	if (!TextDoc)
		return;
	TextDoc.Selection.Text = L"THIS_CLASS& m=*this;";
	TextDoc.Selection.NewLine();
}
//-------------------------------------------------------------------------
EXPORT DoNotCommitTag(){
//DESCRIPTION: Ajoute une section de code avec un warning pour ne pas la committer
	TextDocument TextDoc = GetActiveTextDoc();
	if (!TextDoc)
		return;
	TextDoc.Selection.Text = L"{ /////////////////////////////////////////////// DO NOT COMMIT";
	TextDoc.Selection.NewLine();
	TextDoc.Selection.Text = L"} /////////////////////////////////////////////// DO NOT COMMIT";
	TextDoc.Selection.NewLine();
}
//-------------------------------------------------------------------------
EXPORT DoNotCommitDebugTag(){
//DESCRIPTION: Ajoute une section de Debug avec un warning pour ne pas la committer
	TextDocument TextDoc = GetActiveTextDoc();
	if (!TextDoc)
		return;
	TextDoc.Selection.Text = L"#ifndef NDEBUG	/////////////////////////////////////////////// DO NOT COMMIT";
	TextDoc.Selection.NewLine();
	TextDoc.Selection.Text = L"#endif					/////////////////////////////////////////////// DO NOT COMMIT";
	TextDoc.Selection.NewLine();
}
//-------------------------------------------------------------------------
EXPORT QuickFind(){
//DESCRIPTION: finds current selection in the opened document or pops up a regular DevStudio's find dialog if current selection is empty.
	TextDocument doc = GetActiveTextDoc();
	if (!doc)
		return;

	IApplication& Application = gApp();

	_bstr_t lookFor = doc.Selection.Text;
	long curLine = doc.Selection.CurrentLine;
	long curCol = doc.Selection.CurrentColumn;

	if (!Len(lookFor)) {
		Application.ExecuteCommand(L"FindNext");
		if (!Len(doc.Selection.Text))
			Application.ExecuteCommand(L"Find");
	} else {
		doc.Selection.Cancel();
		doc.Selection.MoveTo(curLine, curCol);
		doc.Selection.FindText(lookFor);
	}
}
//-------------------------------------------------------------------------
EXPORT DoNothing(){
//DESCRIPTION: do nothing (used to disable keyboard shortcuts)
}
//-------------------------------------------------------------------------
EXPORT CloseProgram64(){
//DESCRIPTION: KILL EXECUTED PROGRAM ON 64bits Platforms
	IApplication& Application = gApp();
	Application.ExecuteCommand(L"ActivateWorkspaceWindow");
	Application.Documents.Open(L"Q:\\@Projets\\DevTools\\Stop64.cpp", L"Text");
	TextDocument doc = GetActiveTextDoc();
	if (!doc)
		return;
	doc.Selection.StartOfDocument();
	doc.Selection.LineDown(dsMove, 3);
	Application.ExecuteCommand(L"DebugSetNextStatement");
	Application.ExecuteCommand(L"DebugStepOver");
	Application.ActiveWindow.Close(dsSaveChangesPrompt);
}
//-------------------------------------------------------------------------
EXPORT Crc(){
	IApplication& Application = gApp();
	Application.ExecuteCommand(L"UserTool9");

	TextDocument doc = GetActiveTextDoc();
	if (!doc)
		return;

	Sleep(100);

	doc.Selection.Paste();
}
//-------------------------------------------------------------------------
EXPORT CleanComments(){
	TextDocument doc = GetActiveTextDoc();
	if (!doc)
		return;

	doc.Selection.FindText(L"/*");
	doc.Selection.EndOfLine();
	doc.Selection.CharLeft(dsExtend, 2);
	if (doc.Selection.Text == L"*/") { 
		doc.Selection.Delete();
		doc.Selection.FindText(L"/*", dsMatchBackward);
		doc.Selection.Text = L"//";
	}
}
//-------------------------------------------------------------------------
void SetTmpCopyFile(int i, OUT WCHAR* szFileName){
	int len = GetEnvironmentVariableW(L"TEMP", OUT szFileName, MAX_PATH*2);
	swprintf(szFileName+len, L"\\Copy%d.txt", i);
}
//-------------------------------------------------------------------------
EXPORT CopyN(int i){
//DESCRIPTION: Copy the selection to file
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;

	if (Len(ActiveDocument.Selection.Text) == 0)
		return;

	//Set Tmp = WshSysEnv("TEMP")
	//WshShell.SendKeys "^V"
	//WshShell.Run("Calc.exe")

	//Set oNet = CreateObject( "WScript.Network" )
	//oNet.ComputerName

	//Set d = fso.GetDrive(fso.GetDriveName("C:\MVS10.0\readme.htm"))
	WCHAR szTempFileName[MAX_PATH*2];
	SetTmpCopyFile(i, OUT szTempFileName);
	//ActiveDocument.Selection = tmp
//	Set WshNamed = CreateObject("WScript.Named")
	//ActiveDocument.Selection = WshShell.ProcessID
	//WshShell.ActiveDocument.Selection = WshNamed.Arguments.Count

	FILE* f=_wfopen(szTempFileName, L"wb");
	if (f){
		fwprintf(f, L"%s\r\n", (WCHAR const*)ActiveDocument.Selection.Text);
		fclose(f);
	}
}
//-------------------------------------------------------------------------
EXPORT PasteN(int i){
//DESCRIPTION: Paste a file to selection
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;

//	Set fso = CreateObject("Scripting.FileSystemObject")
	//Set d = fso.GetDrive(fso.GetDriveName("C:\MVS10.0\readme.htm"))
// 	Set WshShell = CreateObject("WScript.Shell")
// 	Set WshSysEnv = WshShell.Environment("Process")
// 	tmp = WshSysEnv("TEMP")
	WCHAR szTempFileName[MAX_PATH*2];
	SetTmpCopyFile(i, OUT szTempFileName);

	BOOL bDir;
	if (FileExistsW(szTempFileName, OUT bDir) && !bDir) {
		FILE* f=_wfopen(szTempFileName, L"rb");
		if (f){
			fseek(f, 0, SEEK_END);
			size_t size = ftell(f);
			_bstr_t szContent(size);

			fseek(f, 0, SEEK_SET);
			fread((WCHAR*)szContent, size, 1, f);
			ActiveDocument.Selection.Text = szContent;
			fclose(f);
		}else{
			ActiveDocument.Selection.Text = L"";
		}
	}
}
//-------------------------------------------------------------------------
EXPORT Copy1(){
	CopyN(1);
}
//-------------------------------------------------------------------------
EXPORT Paste1(){
	PasteN(1);
}
//-------------------------------------------------------------------------
EXPORT Copy2(){
	CopyN(2);
}
//-------------------------------------------------------------------------
EXPORT Paste2(){
	PasteN(2);
}
//-------------------------------------------------------------------------
EXPORT Copy3(){
	CopyN(3);
}
//-------------------------------------------------------------------------
EXPORT Paste3(){
	PasteN(3);
}
//-------------------------------------------------------------------------
EXPORT Copy4(){
	CopyN(4);
}
//-------------------------------------------------------------------------
EXPORT Paste4(){
	PasteN(4);
}
//-------------------------------------------------------------------------
EXPORT Crc2(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;
  _bstr_t sel = ActiveDocument.Selection.Text;
	int l = Len(sel);
	_bstr_t txt(L"_CRC32<");
	for (int iChar=1; iChar<=l; iChar+=4){
		WCHAR sz[16];
		swprintf(sz, L"'%s',", (WCHAR const*)Mid(sel, iChar, 4));
	  txt += sz;
	}
	txt = Left(txt, Len(txt)-1)+L">::v";
 	//Application.PrintToOutputWindow(txt);
	ActiveDocument.Selection.Text = txt; //+" // "+ActiveDocument.Selection.Text
}
//-------------------------------------------------------------------------
EXPORT RemoveTab(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;
	ActiveDocument.Selection.ReplaceText(L"	", L"", dsMatchCase);
	ActiveDocument.Selection.ReplaceText(L" ", L"", dsMatchCase);
}
//-------------------------------------------------------------------------
EXPORT RemoveA(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;
	ActiveDocument.Selection.ReplaceText(L"a_", L"", dsMatchCase);
}
//-------------------------------------------------------------------------
EXPORT SpirOpsEditorGoTo(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;
	ActiveDocument.Selection.Copy();
	IApplication& Application = gApp();
	Application.ExecuteCommand(L"UserTool10");
}
//-------------------------------------------------------------------------
typedef DWORD CRC;
//-------------------------------------------------------------------------
// Polynom : X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X+1
static unsigned int Crc32Table[256] = {
	0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,
	0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
	0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,
	0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
	0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,
	0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
	0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
	0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
	0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,
	0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
	0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,
	0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
	0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,
	0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
	0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,
	0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
	0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,
	0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
	0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,
	0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
	0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
	0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
	0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,
	0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
	0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,
	0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
	0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,
	0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
	0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,
	0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,
	0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,
	0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D
};
//-------------------------------------------------------------------------
CRC UnicodeCrc32(const unsigned short* Datas){
	CRC CrcInit=0;
	register CRC crcbin=~CrcInit;
	const unsigned short* p=Datas;
	int c;
	while((c=((*(p++))&0xFF))!=0){
	  crcbin=((crcbin>>8)&0xFFFFFF)^Crc32Table[(crcbin^c)&0x000000FF];
	}
	return(~crcbin);
}
//-------------------------------------------------------------------------
EXPORT CmpConvert(){
//DESCRIPTION: convert cmp en pseudo code
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;
	ActiveDocument.Selection.WordRight(dsExtend);
	_bstr_t Test = Trim(ActiveDocument.Selection.Text);
	ActiveDocument.Selection.Delete();
	if (Test==L"	") {
		ActiveDocument.Selection.WordRight(dsExtend);
		Test = Trim(ActiveDocument.Selection.Text);
		ActiveDocument.Selection.Delete();
	}
	ActiveDocument.Selection.FindText(L",");
	ActiveDocument.Selection.CharLeft();
	ActiveDocument.Selection.StartOfLine(dsFirstText, dsExtend);
	_bstr_t LeftP = Trim(ActiveDocument.Selection.Text);
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.EndOfLine(dsExtend);
	_bstr_t RightP = Trim(ActiveDocument.Selection.Text);
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.Text = L"	if ( ";
	if (Test==L"test" && LeftP!=RightP) {
		ActiveDocument.Selection.Text = L"(";
	}
	ActiveDocument.Selection.Text = LeftP;
	ActiveDocument.Selection.Text = L" ";
	ActiveDocument.Selection.LineDown();
	ActiveDocument.Selection.StartOfLine(dsFirstText);
	ActiveDocument.Selection.WordRight(dsExtend);
	_bstr_t Jmp = RTrim(ActiveDocument.Selection.Text);
	ActiveDocument.Selection.EndOfLine(dsExtend);
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.LineUp();
	ActiveDocument.Selection.EndOfLine();
	if (Test==L"test" && LeftP!=RightP)
		ActiveDocument.Selection.Text = L" & "+RightP+L" )";

	CRC JmpCrc=UnicodeCrc32(Jmp);
	switch(JmpCrc){
		case 0x6E1B09F0:	ActiveDocument.Selection.Text = L"=="; break; // "jz"
		case 0xF1AF49EF:	ActiveDocument.Selection.Text = L"!="; break; // "jnz"
		case 0xE47EC01C:	ActiveDocument.Selection.Text = L">"; break; // "ja"
		case 0x7D7791A6:	ActiveDocument.Selection.Text = L"<"; break; // "jb"
		case 0x17C7B154:	ActiveDocument.Selection.Text = L"<"; break; // "js"
		case 0x9ACFBCA1:	ActiveDocument.Selection.Text = L"<"; break; // "jl"
		case 0xAD65FF53:	ActiveDocument.Selection.Text = L">="; break; // "jge"
		case 0x0D1D6529:	ActiveDocument.Selection.Text = L">"; break; // "jg"
		case 0x4E912698:	ActiveDocument.Selection.Text = L"<="; break; // "jle"
		case 0xD0120B16:	ActiveDocument.Selection.Text = L"<="; break; // "jbe"
		case 0xE2C3D1B9:	ActiveDocument.Selection.Text = L">="; break; // "jnb"
		case 0x8873F14B:	ActiveDocument.Selection.Text = L">="; break; // "jns"
		default:
			ActiveDocument.Selection.Text = L"\"";
			ActiveDocument.Selection.Text = Jmp;
			ActiveDocument.Selection.Text = L"\"";
	}

	if (Test==L"test") {
		ActiveDocument.Selection.Text = L" 0";
	} else {
		ActiveDocument.Selection.Text = RightP;
	}
	ActiveDocument.Selection.Text = L" ){";
	ActiveDocument.Selection.NewLine();
	ActiveDocument.Selection.NewLine();
	ActiveDocument.Selection.Text = L"}} else {{";
	ActiveDocument.Selection.NewLine();
	ActiveDocument.Selection.NewLine();
	ActiveDocument.Selection.Text = L"}";
	ActiveDocument.Selection.LineUp(dsMove, 3);
	ActiveDocument.Selection.Indent();

//	ActiveDocument.Selection.LineDown
//	ActiveDocument.Selection.WordRight dsExtend
//	Jmp = RTrim(ActiveDocument.Selection.Text)
//	ActiveDocument.Selection.Copy
//	ActiveDocument.Selection.EndOfLine dsExtend
//	ActiveDocument.Selection.Delete
//	ActiveDocument.Selection.LineUp
//	ActiveDocument.Selection.EndOfLine

//	ActiveDocument.Selection = "{"
//	ActiveDocument.Selection.NewLine
//	ActiveDocument.Selection = "	}} else {{"
//	ActiveDocument.Selection.NewLine
//	ActiveDocument.Selection = "}"
//	ActiveDocument.Selection.LineUp dsMove, 4
//	ActiveDocument.Selection.StartOfLine dsFirstText
//	ActiveDocument.Selection.Indent
//	ActiveDocument.Selection = "if ("
//	ActiveDocument.Selection.EndOfLine
//	ActiveDocument.Selection = " "
//	ActiveDocument.Selection = " "
//	ActiveDocument.Selection.Delete
//	ActiveDocument.Selection.EndOfLine'
	//ActiveDocument.Selection = ")"
	//ActiveDocument.Selection.Delete
	//ActiveDocument.Selection.EndOfLine
	//ActiveDocument.Selection.NewLine
	//ActiveDocument.Selection.Indent
}
//-------------------------------------------------------------------------
EXPORT CmpConvertJump(){
//DESCRIPTION: convert cmp en pseudo code
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;

	ActiveDocument.Selection.WordRight(dsExtend);
	_bstr_t Test = Trim(ActiveDocument.Selection.Text);
	ActiveDocument.Selection.Delete();
	if (Test==L"\t") {
		ActiveDocument.Selection.WordRight(dsExtend);
		Test = Trim(ActiveDocument.Selection.Text);
		ActiveDocument.Selection.Delete();
	}
	ActiveDocument.Selection.FindText(L",");
	ActiveDocument.Selection.CharLeft();
	ActiveDocument.Selection.StartOfLine(dsFirstText, dsExtend);
	_bstr_t LeftP = Trim(ActiveDocument.Selection.Text);
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.EndOfLine(dsExtend);
	_bstr_t RightP = Trim(ActiveDocument.Selection.Text);
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.Text = L"	if ( ";
	if (Test==L"test" && LeftP!=RightP) {
		ActiveDocument.Selection.Text = L"(";
	}
	ActiveDocument.Selection.Text = LeftP;
	ActiveDocument.Selection.Text = L" ";
	ActiveDocument.Selection.LineDown();
	ActiveDocument.Selection.StartOfLine(dsFirstText);
	ActiveDocument.Selection.WordRight(dsExtend);
	_bstr_t Jmp = RTrim(ActiveDocument.Selection.Text);
	ActiveDocument.Selection.EndOfLine(dsExtend);
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.LineUp();
	ActiveDocument.Selection.EndOfLine();
	if (Test==L"test" && LeftP!=RightP) {
		ActiveDocument.Selection.Text = L" & "+RightP+L" )";
	}

	CRC JmpCrc=UnicodeCrc32(Jmp);
	switch(JmpCrc){
		case 0x6E1B09F0:	ActiveDocument.Selection.Text = L"!="; break; // "jz"
		case 0xF1AF49EF:	ActiveDocument.Selection.Text = L"=="; break; // "jnz"
		case 0xE47EC01C:	ActiveDocument.Selection.Text = L"<="; break; // "ja"
		case 0x7D7791A6:	ActiveDocument.Selection.Text = L">="; // "jb"
		case 0x17C7B154:	ActiveDocument.Selection.Text = L">="; // "js"
		case 0x9ACFBCA1:	ActiveDocument.Selection.Text = L">="; // "jl"
		case 0xAD65FF53:	ActiveDocument.Selection.Text = L"<"; // "jge"
		case 0x0D1D6529:	ActiveDocument.Selection.Text = L"<="; // "jg"
		case 0x4E912698:	ActiveDocument.Selection.Text = L">"; // "jle"
		case 0xD0120B16:	ActiveDocument.Selection.Text = L">"; // "jbe"
		case 0xE2C3D1B9:	ActiveDocument.Selection.Text = L"<"; // "jnb"
		case 0x8873F14B:	ActiveDocument.Selection.Text = L"<"; // "jns"
		default:
			ActiveDocument.Selection.Text = L"\"";
			ActiveDocument.Selection.Text = Jmp;
			ActiveDocument.Selection.Text = L"\"";
	}

	if (Test==L"test") {
		ActiveDocument.Selection.Text = L" 0";
	} else {
		ActiveDocument.Selection.Text = RightP;
	}
	ActiveDocument.Selection.Text = L" ){";
	ActiveDocument.Selection.NewLine();
	ActiveDocument.Selection.NewLine();
	ActiveDocument.Selection.Text = L"}";
	ActiveDocument.Selection.LineUp(dsMove, 1);
	ActiveDocument.Selection.Indent();

//	ActiveDocument.Selection.LineDown
//	ActiveDocument.Selection.WordRight dsExtend
//	Jmp = RTrim(ActiveDocument.Selection.Text)
//	ActiveDocument.Selection.Copy
//	ActiveDocument.Selection.EndOfLine dsExtend
//	ActiveDocument.Selection.Delete
//	ActiveDocument.Selection.LineUp
//	ActiveDocument.Selection.EndOfLine

//	ActiveDocument.Selection = "{"
//	ActiveDocument.Selection.NewLine
//	ActiveDocument.Selection = "	}} else {{"
//	ActiveDocument.Selection.NewLine
//	ActiveDocument.Selection = "}"
//	ActiveDocument.Selection.LineUp dsMove, 4
//	ActiveDocument.Selection.StartOfLine dsFirstText
//	ActiveDocument.Selection.Indent
//	ActiveDocument.Selection = "if ("
//	ActiveDocument.Selection.EndOfLine
//	ActiveDocument.Selection = " "
//	ActiveDocument.Selection = " "
//	ActiveDocument.Selection.Delete
//	ActiveDocument.Selection.EndOfLine'
	//ActiveDocument.Selection = ")"
	//ActiveDocument.Selection.Delete
	//ActiveDocument.Selection.EndOfLine
	//ActiveDocument.Selection.NewLine
	//ActiveDocument.Selection.Indent
}
//-------------------------------------------------------------------------
EXPORT FindNextNonFunctionExport(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;
	ActiveDocument.Selection.StartOfLine();
	int i = 1;
	while(i){
		ActiveDocument.Selection.EndOfLine(dsExtend);
		i = InStr(ActiveDocument.Selection.Text, L"(");
		if (i) {
			ActiveDocument.Selection.LineDown();
			ActiveDocument.Selection.StartOfLine();
		}
	//while bIsFind
	//	ActiveDocument.Selection.LineDown
	//	ActiveDocument.Selection.StartOfLine
	//	ActiveDocument.Selection.EndOfLine dsExtend
	//	bIsFind=ActiveDocument.Selection.FindText("(", dsMatchFromStart)
	}
	ActiveDocument.Selection.StartOfLine();
}
//-------------------------------------------------------------------------
EXPORT SmartCapitalize(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;

	_bstr_t txt;
	_bstr_t sel = ActiveDocument.Selection.Text;
	int l = Len(sel);
	WCHAR* p = sel;
	BOOL b = true;
	for (int I=0; I<l; ++I){
	  WCHAR c = p[I];
		if (c>=L'a' && c<=L'z') {
			c += (L'A'-L'a');
			b = false;
		} else  if (c>='A' && c<='Z') {
			if (!b)
				txt += L'_';
			b = true;
		} else {
			b = (c == L'_');
		} 
		txt += c;
	}
	 
	ActiveDocument.Selection.Text = txt;

}
//-------------------------------------------------------------------------
EXPORT AntiSlashToSlash(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;

	_bstr_t sel = ActiveDocument.Selection.Text;
	int l = Len(sel);
	WCHAR* p=sel;
	for (int I = 0; I<l; ++I){
		if (p[I]=='\\')
			p[I]='/';
	}
	ActiveDocument.Selection.Text = sel;
}
//-------------------------------------------------------------------------
EXPORT RemoveComments(){
//DESCRIPTION: A description was not provided.
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;

	ActiveDocument.Selection.FindText(L"/**", dsMatchCase);
	ActiveDocument.Selection.FindText(L"*/", dsMatchCase | dsExtend);
	//ActiveDocument.Selection.Delete
}
//-------------------------------------------------------------------------
EXPORT HookFctBytes(){
//DESCRIPTION: Convertit la chaine copiée de IDA en 7 Bytes en Hexa.
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;

	ActiveDocument.Selection.Text = L"0x";
	ActiveDocument.Selection.CharRight(dsMove, 2);
	ActiveDocument.Selection.Text = L",";
	ActiveDocument.Selection.CharRight();

	ActiveDocument.Selection.Text = L"0x";
	ActiveDocument.Selection.CharRight(dsMove, 2);
	ActiveDocument.Selection.Text = L",";
	ActiveDocument.Selection.CharRight();

	ActiveDocument.Selection.Text = L"0x";
	ActiveDocument.Selection.CharRight(dsMove, 2);
	ActiveDocument.Selection.Text = L",";
	ActiveDocument.Selection.CharRight();

	ActiveDocument.Selection.Text = L"0x";
	ActiveDocument.Selection.CharRight(dsMove, 2);
	ActiveDocument.Selection.Text = L",";
	ActiveDocument.Selection.CharRight();

	ActiveDocument.Selection.Text = L"0x";
	ActiveDocument.Selection.CharRight(dsMove, 2);
	ActiveDocument.Selection.Text = L",";
	ActiveDocument.Selection.CharRight();

	ActiveDocument.Selection.Text = L"0x";
	ActiveDocument.Selection.CharRight(dsMove, 2);
	ActiveDocument.Selection.Text = L",";
	ActiveDocument.Selection.CharRight();

	ActiveDocument.Selection.Text = L"0x";
	ActiveDocument.Selection.CharRight(dsMove, 2);

	ActiveDocument.Selection.Delete(27);

}
//-------------------------------------------------------------------------
EXPORT CreateDefLine(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;
	ActiveDocument.Selection.FindText(L"\"");
	ActiveDocument.Selection.CharRight();
	ActiveDocument.Selection.StartOfLine(dsFirstText, dsExtend);
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.FindText(L"\"");
	ActiveDocument.Selection.CharLeft();
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.EndOfLine(dsExtend);
	ActiveDocument.Selection.CharLeft(dsExtend);
	ActiveDocument.Selection.Copy();
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.Delete();
	ActiveDocument.Selection.StartOfLine(dsFirstText);
	ActiveDocument.Selection.Indent();
	ActiveDocument.Selection.Paste();

	IApplication& Application = gApp();
	Application.ExecuteCommand(L"IndentToPrev");

	ActiveDocument.Selection.Text = L"@XXXX";
	ActiveDocument.Selection.Text = L" NONAME ; ";
	ActiveDocument.Selection.StartOfLine();
	ActiveDocument.Selection.LineDown();
}
//---------------------------------------------------------------------------------------------
// EXPORT jjj(){
// 	//ExecuteCommand "VisualUltimateTmpAction dsMove"
// 	Application.ExecuteCommand("VisualUltimateTmpAction", 666, "prout", 444);
// 	//ExecuteCommand2 "VisualUltimateTmpAction"
// }
// //-------------------------------------------------------------------------
// EXPORT MAttachToProcess(){
// 	AttachToProcess("Notepad.exe");
// }
//-------------------------------------------------------------------------
struct Application:IApplication{
	EXPORT BeforeBuildStart();
};
//---------------------------------------------------------------------------------------------
EXPORT Application::BeforeBuildStart(){
	// Insert code to handle the event here

	if (InStr(ActiveConfiguration.Name, L"Release"))
		IncResVersion();
}
//---------------------------------------------------------------------------------------------
extern "C" {
	__declspec(dllimport) BOOL __stdcall OpenClipboard(HWND hWndNewOwner);
	__declspec(dllimport) HANDLE __stdcall GetClipboardData(UINT uFormat);
	__declspec(dllimport) void* __stdcall GlobalLock(HGLOBAL hMem);
	__declspec(dllimport) BOOL __stdcall CloseClipboard();
	__declspec(dllimport) BOOL		__stdcall GlobalUnlock(HGLOBAL hMem);
}
//-------------------------------------------------------------------------------------------------------------
#define CF_TEXT 1
#define CF_UNICODETEXT 13
//-------------------------------------------------------------------------------------------------------------
EXPORT MoveSplashscreenToTooHot(){
	TextDocument ActiveDocument = GetActiveTextDoc();
	if (!ActiveDocument)
		return;
	IApplication& Application = gApp();
	Application.ExecuteCommand(L"VisualUltimateCopyLastSplashScreenFileNameToClipboard");

	if (!::OpenClipboard(0))
		return;

	BOOL bOk = false;
	WCHAR FileName[MAX_PATH*2];
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		if (hData){
			WCHAR* pStart = (WCHAR*)GlobalLock(hData);
			wcscpy(FileName, pStart);
			bOk = true;
			GlobalUnlock(hData);
		}
		CloseClipboard();
	}
	if (!bOk){
		return;
	}

	ActiveDocument.Selection.Text = FileName;
}
//---------------------------------------------------------------------------------------------


