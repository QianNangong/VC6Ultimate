#pragma once
//-------------------------------------------------------------------------------------------------
#define IUnknown _IUnknown
#define IDispatch _IDispatch
#define IApplication _IApplication
#define IConfiguration _IConfiguration
#define IConfigurations _IConfigurations
#define IBuildProject _IBuildProject
#define IGenericProject _IGenericProject
#define ITextDocument _ITextDocument
#define IGenericDocument _IGenericDocument
#define ITextSelection _ITextSelection
#define IWindows _IWindows
#define IGenericWindow _IGenericWindow
#define IDocuments _IDocuments
#define IDebugger _IDebugger
#define IBreakpoints _IBreakpoints
#define IBreakpoint _IBreakpoint
//---------------------------------------------------------------------------------------------
struct IUnknown;
struct IDispatch;
struct IApplication;
struct IConfiguration;
struct IConfigurations;
struct IBuildProject;
struct IGenericProject;
struct ITextDocument;
struct IGenericDocument;
struct ITextSelection;
struct IWindows;
struct IGenericWindow;
struct IDocuments;
struct IDebugger;
struct IBreakpoints;
struct IBreakpoint;
//---------------------------------------------------------------------------------------------
#define VARIANT_INT_(Name, Value) VARIANT Name; Name.vt = VT_I4; Name.lVal = Value;
#define VARIANT_BOOL_(Name, Value) VARIANT Name; Name.vt = VT_BOOL; Name.boolVal = Value?-1:0;
#define VARIANT_BSTR_(Name, Value) VARIANT Name; Name.vt = VT_BSTR; Name.bstrVal = Value;
//---------------------------------------------------------------------------------------------
#define IFC(Struct) \
	_I##Struct* p; \
	~Struct(){ \
		if (p) \
			((IUnknown*)p)->Release(); \
	} \
	Struct(I##Struct* p=NULL):p(p){ } \
 	Struct(Struct& d):p(d.p){ \
 		if (p) \
 			((IUnknown*)p)->AddRef(); \
 	} \
	bool operator ! () { return p==NULL; }
//---------------------------------------------------------------------------------------------
struct __declspec(uuid("00000000-0000-0000-c000-000000000046")) __declspec(novtable) IUnknown{
	virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppvObj) = 0;
	virtual ULONG __stdcall AddRef() = 0;
	virtual ULONG __stdcall Release() = 0;
};
//---------------------------------------------------------------------------------------------
struct __declspec(uuid("00020400-0000-0000-C000-000000000046")) __declspec(novtable) IDispatch:IUnknown{
	virtual HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo) = 0;
	virtual HRESULT __stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) = 0;
	virtual HRESULT __stdcall GetIDsOfNames(const IID& riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) = 0;
	virtual HRESULT __stdcall Invoke(DISPID dispIdMember, const IID& riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) = 0;
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) IGenericProject:IDispatch{
  virtual HRESULT __stdcall get_Name(BSTR* Name) = 0;
  virtual HRESULT __stdcall get_FullName(BSTR* Name) = 0;
  virtual HRESULT __stdcall get_Application(IDispatch** Application) = 0;
  virtual HRESULT __stdcall get_Parent(IDispatch** Parent) = 0;
  virtual HRESULT __stdcall get_Type(BSTR* pType) = 0;
  virtual HRESULT __stdcall Reserved1() = 0;
  virtual HRESULT __stdcall Reserved2() = 0;
  virtual HRESULT __stdcall Reserved3() = 0;
  virtual HRESULT __stdcall Reserved4() = 0;
  virtual HRESULT __stdcall Reserved5() = 0;
  virtual HRESULT __stdcall Reserved6() = 0;
  virtual HRESULT __stdcall Reserved7() = 0;
  virtual HRESULT __stdcall Reserved8() = 0;
  virtual HRESULT __stdcall Reserved9() = 0;
  virtual HRESULT __stdcall Reserved10() = 0;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct GenericProject{
	IFC(GenericProject)
	//---------------
	_bstr_t GetFullName(){
		WCHAR* Str;
		if (p) p->get_FullName(OUT &Str);
		return _bstr_t(Str);
	}
	//---------------
	__declspec(property(get=GetFullName)) _bstr_t FullName;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) IBuildProject:IGenericProject{
  virtual HRESULT __stdcall get_Configurations(IConfigurations** Configurations) = 0;
  virtual HRESULT __stdcall AddFile(BSTR szFile, VARIANT Reserved) = 0;
  virtual HRESULT __stdcall AddConfiguration(BSTR szConfig, VARIANT Reserved) = 0;
};
//---------------------------------------------------------------------------------------------
enum DsWindowState{
	dsWindowStateMaximized	= 1,
	dsWindowStateMinimized	= 2,	
	dsWindowStateNormal		= 3,
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) IConfigurations:IDispatch{
	virtual HRESULT __stdcall get_Application(IDispatch** Application) = 0;
	virtual HRESULT __stdcall get_Count(long* Count) = 0;
	virtual HRESULT __stdcall get_Parent(IBuildProject** Parent) = 0;
	virtual HRESULT __stdcall get__NewEnum(IUnknown** _NewEnum) = 0;
	virtual HRESULT __stdcall Item(VARIANT Index, IConfiguration** Item) = 0;
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) IConfiguration:IDispatch{
	virtual HRESULT __stdcall get_Name(BSTR* Name) = 0;
	virtual HRESULT __stdcall get_Application(IDispatch** Application) = 0;
	virtual HRESULT __stdcall get_Parent(IDispatch** Parent) = 0;
	virtual HRESULT __stdcall AddToolSettings(BSTR szTool, BSTR szSettings, VARIANT Reserved) = 0;
	virtual HRESULT __stdcall RemoveToolSettings(BSTR szTool, BSTR szSettings, VARIANT Reserved) = 0;
	virtual HRESULT __stdcall AddCustomBuildStep(BSTR szCommand, BSTR szOutput, BSTR szDescription, VARIANT Reserved) = 0;
	virtual HRESULT __stdcall get_Configurations(IConfigurations** Configurations) = 0;
	virtual HRESULT __stdcall Reserved11() = 0;
	virtual HRESULT __stdcall MakeCurrentSettingsDefault(VARIANT Reserved) = 0;
	virtual HRESULT __stdcall AddFileSettings(BSTR szFile, BSTR szSettings, VARIANT Reserved) = 0;
	virtual HRESULT __stdcall RemoveFileSettings(BSTR szFile, BSTR szSettings, VARIANT Reserved) = 0;
	virtual HRESULT __stdcall AddCustomBuildStepToFile(BSTR szFile, BSTR szCommand, BSTR szOutput, BSTR szDescription, VARIANT Reserved) = 0;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct Configuration{
	IFC(Configuration)
	_bstr_t GetName(){
		BSTR Name;
		if (p) p->get_Name(OUT &Name);
		return _bstr_t(Name);
	}
	//---------------
 	__declspec(property(get=GetName)) _bstr_t Name;
	//---------------
};
//---------------------------------------------------------------------------------------------
enum DsSaveChanges{
	dsSaveChangesYes		= 1, 
	dsSaveChangesNo			= 2,
	dsSaveChangesPrompt	= 3,
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) __declspec(uuid("fd20fc80-a9d2-11cf-9c13-00a0c90a632c")) IGenericWindow : IDispatch{
	virtual HRESULT __stdcall get_Caption(BSTR * pbstrCaption ) = 0;
	virtual HRESULT __stdcall get_Type(BSTR * pbstrCaption ) = 0;
	virtual HRESULT __stdcall put_Active(VARIANT_BOOL pbActive ) = 0;
	virtual HRESULT __stdcall get_Active(VARIANT_BOOL * pbActive ) = 0;
	virtual HRESULT __stdcall put_Left(long plVal ) = 0;
	virtual HRESULT __stdcall get_Left(long * plVal ) = 0;
	virtual HRESULT __stdcall put_Top(long plVal ) = 0;
	virtual HRESULT __stdcall get_Top(long * plVal ) = 0;
	virtual HRESULT __stdcall put_Height(long plVal ) = 0;
	virtual HRESULT __stdcall get_Height(long * plVal ) = 0;
	virtual HRESULT __stdcall put_Width(long plVal ) = 0;
	virtual HRESULT __stdcall get_Width(long * plVal ) = 0;
	virtual HRESULT __stdcall get_Index(long * plVal ) = 0;
	virtual HRESULT __stdcall get_Next(IDispatch * * ppDispatch ) = 0;
	virtual HRESULT __stdcall get_Previous(IDispatch * * ppDispatch ) = 0;
	virtual HRESULT __stdcall put_WindowState(enum DsWindowState plVal ) = 0;
	virtual HRESULT __stdcall get_WindowState(enum DsWindowState * plVal ) = 0;
	virtual HRESULT __stdcall get_Application(IDispatch * * ppDispatch ) = 0;
	virtual HRESULT __stdcall get_Parent(IDispatch * * ppDispatch ) = 0;
	virtual HRESULT __stdcall Close(VARIANT boolSaveChanges, enum DsSaveStatus * pSaved ) = 0;
	virtual HRESULT __stdcall Reserved1( ) = 0;
	virtual HRESULT __stdcall Reserved2( ) = 0;
	virtual HRESULT __stdcall Reserved3( ) = 0;
	virtual HRESULT __stdcall Reserved4( ) = 0;
	virtual HRESULT __stdcall Reserved5( ) = 0;
	virtual HRESULT __stdcall Reserved6( ) = 0;
	virtual HRESULT __stdcall Reserved7( ) = 0;
	virtual HRESULT __stdcall Reserved8( ) = 0;
	virtual HRESULT __stdcall Reserved9( ) = 0;
	virtual HRESULT __stdcall Reserved10( ) = 0;

	//     __declspec(property(get=GetCaption)) _bstr_t Caption;
	//     __declspec(property(get=GetType)) _bstr_t Type;
	//     __declspec(property(get=GetActive,put=PutActive)) VARIANT_BOOL Active;
	//     __declspec(property(get=GetLeft,put=PutLeft)) long Left;
	//     __declspec(property(get=GetTop,put=PutTop)) long Top;
	//     __declspec(property(get=GetHeight,put=PutHeight)) long Height;
	//     __declspec(property(get=GetWidth,put=PutWidth)) long Width;
	//     __declspec(property(get=GetIndex)) long Index;
	//     __declspec(property(get=GetNext)) IDispatchPtr Next;
	//     __declspec(property(get=GetPrevious)) IDispatchPtr Previous;
	//     __declspec(property(get=GetWindowState,put=PutWindowState)) enum DsWindowState WindowState;
	//     __declspec(property(get=GetApplication)) IDispatchPtr Application;
	//     __declspec(property(get=GetParent)) IDispatchPtr Parent;
	// 
	//     _bstr_t GetCaption ( );
	//     _bstr_t GetType ( );
	//     void PutActive (VARIANT_BOOL pbActive );
	//     VARIANT_BOOL GetActive ( );
	//     void PutLeft (long plVal );
	//     long GetLeft ( );
	//     void PutTop (long plVal );
	//     long GetTop ( );
	//     void PutHeight (long plVal );
	//     long GetHeight ( );
	//     void PutWidth (long plVal );
	//     long GetWidth ( );
	//     long GetIndex ( );
	//     IDispatchPtr GetNext ( );
	//     IDispatchPtr GetPrevious ( );
	//     void PutWindowState (enum DsWindowState plVal );
	//     enum DsWindowState GetWindowState ( );
	//     IDispatchPtr GetApplication ( );
	//     IDispatchPtr GetParent ( );
	//     enum DsSaveStatus Close (const _variant_t & boolSaveChanges );
	//     HRESULT Reserved1 ( );
	//     HRESULT Reserved2 ( );
	//     HRESULT Reserved3 ( );
	//     HRESULT Reserved4 ( );
	//     HRESULT Reserved5 ( );
	//     HRESULT Reserved6 ( );
	//     HRESULT Reserved7 ( );
	//     HRESULT Reserved8 ( );
	//     HRESULT Reserved9 ( );
	//     HRESULT Reserved10 ( );
	//---------------
};
//---------------------------------------------------------------------------------------------
struct GenericWindow{
	IFC(GenericWindow)
	//---------------
	DsSaveStatus Close(DsSaveChanges SaveChanges){
		DsSaveStatus result;
		VARIANT_INT_(vSaveChanges, SaveChanges);
		if (p) p->Close(vSaveChanges, OUT &result);
		return result;
	}
	//---------------
};
//---------------------------------------------------------------------------------------------
enum DsSaveStatus{
	dsSaveSucceeded	= 1,	// The Save was successful.
	dsSaveCanceled	= 2		// The Save was canceled
};
//---------------------------------------------------------------------------------------------
struct Windows{
	IFC(Windows)
	//---------------
 	inline long GetCount();
	//---------------
 	__declspec(property(get=GetCount)) long Count;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) IGenericDocument:IDispatch{
	virtual HRESULT __stdcall get_Name( BSTR FAR* pName) = 0;
	virtual HRESULT __stdcall get_FullName( BSTR FAR* pName) = 0;
	virtual HRESULT __stdcall get_Application( IDispatch * FAR* ppApplication) = 0;
	virtual HRESULT __stdcall get_Parent( IDispatch * FAR* ppParent) = 0;
	virtual HRESULT __stdcall get_Path( BSTR FAR* pPath) = 0;
	virtual HRESULT __stdcall get_Saved( VARIANT_BOOL FAR* pSaved) = 0;
	virtual HRESULT __stdcall get_ActiveWindow( IDispatch * FAR* ppWindow) = 0;
	virtual HRESULT __stdcall get_ReadOnly( VARIANT_BOOL FAR* pReadOnly) = 0;
	virtual HRESULT __stdcall put_ReadOnly( VARIANT_BOOL ReadOnly) = 0;
	virtual HRESULT __stdcall get_Type( BSTR FAR* pType) = 0;
	virtual HRESULT __stdcall get_Windows( IDispatch * FAR* ppWindows) = 0;
	virtual HRESULT __stdcall put_Active( VARIANT_BOOL bActive) = 0;
	virtual HRESULT __stdcall get_Active( VARIANT_BOOL FAR* pbActive) = 0;
	virtual HRESULT __stdcall NewWindow( IDispatch * FAR* ppWindow) = 0;
	virtual HRESULT __stdcall Save( VARIANT vFilename, VARIANT vBoolPrompt, DsSaveStatus FAR* pSaved) = 0;
	virtual HRESULT __stdcall Undo( VARIANT_BOOL FAR* pSuccess) = 0;
	virtual HRESULT __stdcall Redo( VARIANT_BOOL FAR* pSuccess) = 0;
	virtual HRESULT __stdcall PrintOut( VARIANT_BOOL FAR* pSuccess) = 0;
	virtual HRESULT __stdcall Close( VARIANT vSaveChanges, DsSaveStatus FAR* pSaved) = 0;
	virtual HRESULT __stdcall Reserved1() = 0;
	virtual HRESULT __stdcall Reserved2() = 0;
	virtual HRESULT __stdcall Reserved3() = 0;
	virtual HRESULT __stdcall Reserved4() = 0;
	virtual HRESULT __stdcall Reserved5() = 0;
	virtual HRESULT __stdcall Reserved6() = 0;
	virtual HRESULT __stdcall Reserved7() = 0;
	virtual HRESULT __stdcall Reserved8() = 0;
	virtual HRESULT __stdcall Reserved9() = 0;
	virtual HRESULT __stdcall Reserved10() = 0;
	//---------------
	DsSaveStatus Save(WCHAR* Filename, BOOL bPrompt=false){
		DsSaveStatus Saved;
		VARIANT_BOOL_(vBoolPrompt, bPrompt);
		_bstr_t _Filename(Filename);
		VARIANT_BSTR_(vFilename, _Filename);
		Save(vFilename, vBoolPrompt, OUT &Saved);
		return Saved;
	}
	//---------------
	Windows GetWindows(){
		IWindows* pWindows;
		get_Windows(OUT (IDispatch**)&pWindows);
		return pWindows;
	}
	//---------------
	_bstr_t GetName(){
		BSTR Name;
		get_Name(OUT &Name);
		return _bstr_t(Name);
	}
	//---------------
	_bstr_t GetType(){
		BSTR Type;
		get_Type(OUT &Type);
		return _bstr_t(Type);
	}
	//---------------
 	__declspec(property(get=GetType)) _bstr_t Type;
 	__declspec(property(get=GetName)) _bstr_t Name;
	__declspec(property(get=GetWindows)) Windows Windows;
	//__declspec(property(get=ActiveWindow)) IGenericWindow& Windows;
};
//---------------------------------------------------------------------------------------------
struct __declspec(uuid("3928f551-96e6-11cf-9c00-00a0c90a632c")) __declspec(novtable) IWindows : IDispatch{
// 
// 	IDispatchPtr GetApplication( );
// 	IDispatchPtr GetParent( );
// 	IUnknownPtr Get_NewEnum( );
// 	IDispatchPtr Item(const _variant_t & Index );
// 	HRESULT Arrange(const _variant_t & vtArrangeStyle );
// 	enum DsSaveStatus CloseAll(const _variant_t & vtSaveChanges = vtMissing );

	virtual HRESULT __stdcall get_Count(long * Count ) = 0;
	virtual HRESULT __stdcall get_Application(IDispatch** ppApplication ) = 0;
	virtual HRESULT __stdcall get_Parent(IDispatch** ppParent ) = 0;
	virtual HRESULT __stdcall get__NewEnum(IUnknown** _NewEnum ) = 0;
	virtual HRESULT __stdcall raw_Item(VARIANT Index, IDispatch** Item ) = 0;
	virtual HRESULT __stdcall raw_Arrange(VARIANT vtArrangeStyle ) = 0;
	virtual HRESULT __stdcall raw_CloseAll(VARIANT vtSaveChanges, enum DsSaveStatus * pSaved ) = 0;
	//---------------
// 	__declspec(property(get=GetApplication)) IDispatchPtr Application;
// 	__declspec(property(get=GetParent)) IDispatchPtr Parent;
// 	__declspec(property(get=Get_NewEnum)) IUnknownPtr _NewEnum;
};
//---------------------------------------------------------------------------------------------
enum DsCaseOptions{
	dsLowercase		= 1,
	dsUppercase		= 2,
	dsCapitalize	= 3
};
//---------------------------------------------------------------------------------------------
// TextSelection.FindText/ReplaceText
enum DsTextSearchOptions{
	dsMatchWord				= 2,		// match whole words
	dsMatchCase				= 4,		// match is sensitive to case
	dsMatchNoRegExp		= 0,		// don't use regular expressions
	dsMatchRegExp			= 8,		// match Dev Studio regular expressions
	dsMatchRegExpB		= 16,		// match BRIEF(TM) regular expressions
	dsMatchRegExpE		= 32,		// match Epsilon(TM) regular expressions
	dsMatchRegExpCur	= 64,		// match using current reg exp setting
	dsMatchForward		= 0,		// search forward
	dsMatchBackward		= 128,	// search backwards
	dsMatchFromStart	= 256,	// do search from start or end of view
};
//---------------------------------------------------------------------------------------------
enum DsStartOfLineOptions{
	dsFirstColumn	= 0,
	dsFirstText		= 1
};
//---------------------------------------------------------------------------------------------
enum DsMovementOptions{
	dsMove		= 0,
	dsExtend	= 1
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) ITextSelection:IDispatch{
	virtual HRESULT __stdcall put_Text( BSTR newText) = 0;
	virtual HRESULT __stdcall get_Text( BSTR FAR* pText) = 0;
	virtual HRESULT __stdcall get_Application( IDispatch * FAR* ppApp) = 0;
	virtual HRESULT __stdcall get_CurrentLine( long FAR* plLine) = 0;
	virtual HRESULT __stdcall get_CurrentColumn( long FAR* plCol) = 0;
	virtual HRESULT __stdcall get_Parent( IDispatch * FAR* ppParent) = 0;
	virtual HRESULT __stdcall get_BottomLine( long FAR* plLine) = 0;
	virtual HRESULT __stdcall get_TopLine( long FAR* plLine) = 0;
	virtual HRESULT __stdcall Delete( VARIANT Count) = 0;
	virtual HRESULT __stdcall SelectLine() = 0;
	virtual HRESULT __stdcall Backspace( VARIANT Count) = 0;
	virtual HRESULT __stdcall StartOfDocument( VARIANT Extend) = 0;
	virtual HRESULT __stdcall Copy() = 0;
	virtual HRESULT __stdcall Cut() = 0;
	virtual HRESULT __stdcall Paste() = 0;
	virtual HRESULT __stdcall EndOfDocument( VARIANT Extend) = 0;
	virtual HRESULT __stdcall SelectAll() = 0;
	virtual HRESULT __stdcall Tabify() = 0;
	virtual HRESULT __stdcall Untabify() = 0;
	virtual HRESULT __stdcall Indent( VARIANT Reserved) = 0;
	virtual HRESULT __stdcall Unindent( VARIANT Reserved) = 0;
	virtual HRESULT __stdcall CharLeft( VARIANT Extend, VARIANT Count) = 0;
	virtual HRESULT __stdcall CharRight( VARIANT Extend, VARIANT Count) = 0;
	virtual HRESULT __stdcall LineUp( VARIANT Extend, VARIANT Count) = 0;
	virtual HRESULT __stdcall LineDown( VARIANT Extend, VARIANT Count) = 0;
	virtual HRESULT __stdcall PageUp( VARIANT Extend, VARIANT Count) = 0;
	virtual HRESULT __stdcall PageDown( VARIANT Extend, VARIANT Count) = 0;
	virtual HRESULT __stdcall WordLeft( VARIANT Extend, VARIANT Count) = 0;
	virtual HRESULT __stdcall WordRight( VARIANT Extend, VARIANT Count) = 0;
	virtual HRESULT __stdcall EndOfLine( VARIANT Extend) = 0;
	virtual HRESULT __stdcall StartOfLine( VARIANT MoveTo, VARIANT Extend) = 0;
	virtual HRESULT __stdcall SmartFormat() = 0;
	virtual HRESULT __stdcall ChangeCase( DsCaseOptions Type) = 0;
	virtual HRESULT __stdcall DeleteWhitespace( VARIANT Direction) = 0;
	virtual HRESULT __stdcall Cancel() = 0;
	virtual HRESULT __stdcall GoToLine( long Line, VARIANT Select) = 0;
	virtual HRESULT __stdcall MoveTo( long Line, long Column, VARIANT Extend) = 0;
	virtual HRESULT __stdcall FindText( BSTR FindString, VARIANT Flags, VARIANT_BOOL* pbRet) = 0;
	virtual HRESULT __stdcall PreviousBookmark( VARIANT_BOOL* pbRet) = 0;
	virtual HRESULT __stdcall NextBookmark( VARIANT_BOOL* pbRet) = 0;
	virtual HRESULT __stdcall SetBookmark() = 0;
	virtual HRESULT __stdcall ClearBookmark( VARIANT_BOOL* pbRet) = 0;
	virtual HRESULT __stdcall NewLine( VARIANT Reserved) = 0;
	virtual HRESULT __stdcall ReplaceText( BSTR FindText, BSTR ReplaceText, VARIANT Flags, VARIANT_BOOL* pbRetVal) = 0;
	virtual HRESULT __stdcall DestructiveInsert( BSTR szText) = 0;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct TextSelection{
	IFC(TextSelection)
	//---------------------
	BOOL FindText( BSTR FindString, DWORD Flags=dsMatchForward){
		VARIANT_BOOL bRet;
		VARIANT_INT_(vFlags, Flags);
		if (p) p->FindText(FindString, vFlags, &bRet);
		return bRet;
	}
	//---------------
	void StartOfDocument(DsMovementOptions Extend=dsMove){
		VARIANT_INT_(vExtend, Extend);
		if (p) p->StartOfDocument(vExtend);
	}
	//---------------
	void StartOfLine(DsStartOfLineOptions MoveTo=dsFirstColumn, DsMovementOptions Extend=dsMove){
		VARIANT_INT_(vMoveTo, MoveTo);
		VARIANT_INT_(vExtend, Extend);
		if (p) p->StartOfLine(vMoveTo, vExtend);
	}
	//---------------
	void EndOfLine(DsMovementOptions Extend=dsMove){
		VARIANT_INT_(vExtend, Extend);
		if (p) p->EndOfLine(vExtend);
	}
	//---------------
	void CharRight(DsMovementOptions Extend=dsMove, long Count=1){
		VARIANT_INT_(vExtend, Extend);
		VARIANT_INT_(vCount, Count);
		if (p) p->CharRight(vExtend, vCount);
	}
	//---------------
	void CharLeft(DsMovementOptions Extend=dsMove, long Count=1){
		VARIANT_INT_(vExtend, Extend);
		VARIANT_INT_(vCount, Count);
		if (p) p->CharLeft(vExtend, vCount);
	}
	//---------------
	void LineDown(DsMovementOptions Extend=dsMove, long Count=1){
		VARIANT_INT_(vExtend, Extend);
		VARIANT_INT_(vCount, Count);
		if (p) p->LineDown(vExtend, vCount);
	}
	//---------------
	void WordLeft(DsMovementOptions Extend=dsMove, long Count=1){
		VARIANT_INT_(vExtend, Extend);
		VARIANT_INT_(vCount, Count);
		if (p) p->WordLeft(vExtend, vCount);
	}
	//---------------
	void WordRight(DsMovementOptions Extend=dsMove, long Count=1){
		VARIANT_INT_(vExtend, Extend);
		VARIANT_INT_(vCount, Count);
		if (p) p->WordRight(vExtend, vCount);
	}
	//---------------
	void Delete(int Count=1){
		VARIANT_INT_(vCount, Count);
		if (p) p->Delete(vCount);
	}
	//---------------
	void NewLine(){
		VARIANT v; v.vt=VT_EMPTY; v.intVal=0;
		if (p) p->NewLine(v);
	}
	//---------------
	void MoveTo( long Line, long Column, DsMovementOptions Extend=dsMove){
		VARIANT_INT_(vExtend, Extend);
		if (p) p->MoveTo(Line, Column, vExtend);
	}
	//---------------
	BOOL ReplaceText(BSTR _FindText, BSTR _ReplaceText, DWORD Flags){
		VARIANT_INT_(vFlags, Flags);
		VARIANT_BOOL bRet;
		if (p) p->ReplaceText(_FindText, _ReplaceText, vFlags, &bRet);
		return bRet;
	}
	//---------------
	void LineUp(DsMovementOptions Extend=dsMove, long Count=1){
		VARIANT_INT_(vExtend, Extend);
		VARIANT_INT_(vCount, Count);
		if (p) p->LineUp(vExtend, vCount);
	}
	//---------------
	void Indent(){
		VARIANT v; v.vt=VT_EMPTY; v.intVal=0;
		if (p) p->Indent(v);
	}
	//---------------
	void Paste(){
		if (p) p->Paste();
	}
	//---------------
	void Cancel(){
		if (p) p->Cancel();
	}
	//---------------
	void Copy(){
		if (p) p->Copy();
	}
	//---------------
	_bstr_t GetText(){
		BSTR Text;
		if (p) p->get_Text(OUT (BSTR*)&Text);
		return _bstr_t(Text);
	}
	//---------------
	void PutText(BSTR newText){
		if (p) p->put_Text(newText);
	}
	//---------------
	long GetCurrentLine(){
		long iLine;
		if (p) p->get_CurrentLine(OUT &iLine);
		return iLine;
	}
	//---------------
	long GetCurrentColumn(){
		long iColumn;
		if (p) p->get_CurrentColumn(OUT &iColumn);
		return iColumn;
	}
	//---------------
	__declspec(property(get=GetText, put=PutText)) _bstr_t Text;
	__declspec(property(get=GetCurrentLine)) long CurrentLine;
	__declspec(property(get=GetCurrentColumn)) long CurrentColumn;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) ITextDocument:IGenericDocument{
  virtual HRESULT __stdcall get_Selection(IDispatch** ppSelection) = 0;
  virtual HRESULT __stdcall put_IndentSize(long Size) = 0;
  virtual HRESULT __stdcall get_IndentSize(long* pSize) = 0;
  virtual HRESULT __stdcall put_TabSize(long Size) = 0;
  virtual HRESULT __stdcall get_TabSize(long* pSize) = 0;
  virtual HRESULT __stdcall put_Language(BSTR Language) = 0;
  virtual HRESULT __stdcall get_Language(BSTR* pLanguage) = 0;
  virtual HRESULT __stdcall ReplaceText(BSTR FindText, BSTR ReplaceText, VARIANT Flags, VARIANT_BOOL* pbRetVal) = 0;
  virtual HRESULT __stdcall ClearBookmarks() = 0;
  virtual HRESULT __stdcall MarkText(BSTR FindText, VARIANT Flags, VARIANT_BOOL* pbRetVal) = 0;
	//---------------
  TextSelection GetSelection(){
		ITextSelection* pSelection;
		get_Selection(OUT (IDispatch**)&pSelection);
		return pSelection;
	}
	//---------------
	__declspec(property(get=GetSelection)) TextSelection Selection;

};
//---------------------------------------------------------------------------------------------
struct __declspec(uuid("fb7fdae3-89b8-11cf-9be8-00a0c90a632c")) __declspec(novtable) IDocuments : IDispatch{
// 	__declspec(property(get=GetCount)) long Count;
// 	__declspec(property(get=GetApplication)) IDispatchPtr Application;
// 	__declspec(property(get=GetParent)) IDispatchPtr Parent;
// 	__declspec(property(get=Get_NewEnum)) IUnknownPtr _NewEnum;
// 
// 	long GetCount();
// 	IDispatchPtr GetApplication();
// 	IDispatchPtr GetParent();
// 	IUnknownPtr Get_NewEnum();
// 	IDispatchPtr Item (const _variant_t & Index );
// 	enum DsSaveStatus SaveAll(const _variant_t & vtBoolPrompt = vtMissing );
// 	enum DsSaveStatus CloseAll(const _variant_t & vtSaveChanges = vtMissing );
// 	IDispatchPtr Add(_bstr_t docType, const _variant_t & vtReserved = vtMissing );
// 	IDispatchPtr Open(_bstr_t filename, const _variant_t & vtDocType = vtMissing, const _variant_t & vtBoolReadOnly = vtMissing );
	virtual HRESULT __stdcall get_Count(long* Count) = 0;
	virtual HRESULT __stdcall get_Application(IDispatch** ppApplication) = 0;
	virtual HRESULT __stdcall get_Parent(IDispatch** ppParent) = 0;
	virtual HRESULT __stdcall get__NewEnum(IUnknown** _NewEnum) = 0;
	virtual HRESULT __stdcall Item(VARIANT index, IDispatch** Item) = 0;
	virtual HRESULT __stdcall SaveAll(VARIANT vtBoolPrompt, DsSaveStatus* pSaved) = 0;
	virtual HRESULT __stdcall CloseAll(VARIANT vtSaveChanges, DsSaveStatus* pSaved) = 0;
	virtual HRESULT __stdcall Add(WCHAR* docType, VARIANT vtReserved, IDispatch** ppDocument) = 0;
	virtual HRESULT __stdcall Open(WCHAR* filename, VARIANT vtDocType, VARIANT vtBoolReadOnly, IDispatch** ppDocument) = 0;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct GenericDocument{
	IFC(GenericDocument)
	//---------------------
	DsSaveStatus Save(){
		BSTR Name;
		if (p) {
			p->get_FullName(&Name);
			return p->Save(Name, false);
		}
		return dsSaveCanceled;
	}
	//---------------
	Windows GetWindows(){
		IWindows* pWindows=NULL;
		if (p)
			p->get_Windows(OUT (IDispatch**)&pWindows);
		return pWindows;
	}
	//---------------
	_bstr_t GetName(){
		BSTR Name;
		if (p)
			p->get_Name(OUT &Name);
		return _bstr_t(Name);
	}
	//---------------
	_bstr_t GetType(){
		BSTR Type;
		if (p)
			p->get_Type(OUT &Type);
		return _bstr_t(Type);
	}
	//---------------
 	__declspec(property(get=GetType)) _bstr_t Type;
 	__declspec(property(get=GetName)) _bstr_t Name;
	__declspec(property(get=GetWindows)) Windows Windows;
};
//---------------------------------------------------------------------------------------------
struct Documents{
	IFC(Documents)
	//---------------
	GenericDocument Open(WCHAR* filename, WCHAR const* DocType=L"Auto", BOOL bReadOnly=false){
		VARIANT_BOOL_(vtBoolReadOnly, bReadOnly);
 		_bstr_t _DocType(DocType);
		VARIANT_BSTR_(vtDocType, _DocType);
		IGenericDocument* pDocument;
		if (p) p->Open(filename, vtDocType, vtBoolReadOnly, (IDispatch**)&pDocument);
		return pDocument;
	}
	//---------------
};
//---------------------------------------------------------------------------------------------
struct TextDocument:GenericDocument{
	//---------------
  TextSelection GetSelection(){
		ITextSelection* pSelection=NULL;
 		if (p)
			((ITextDocument*)p)->get_Selection(OUT (IDispatch**)&pSelection);
		return pSelection;
	}
	//---------------
	__declspec(property(get=GetSelection)) TextSelection Selection;
};
//---------------------------------------------------------------------------------------------
struct Breakpoints{
	IFC(Breakpoints)
	//---------------
	inline long GetCount();
	//---------------
 	__declspec(property(get=GetCount)) long Count;
	//---------------
	inline IBreakpoint& operator [] (long iBreakpoint);
	//---------------
};
//---------------------------------------------------------------------------------------------
struct Debugger{
	IFC(Debugger)
	//---------------
	inline Breakpoints GetBreakpoints();
	//---------------
 	__declspec(property(get=GetBreakpoints)) Breakpoints Breakpoints;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct __declspec(uuid("ec1d73a1-8cc4-11cf-9be9-00a0c90a632c")) __declspec(novtable) IApplication:IDispatch{
	virtual HRESULT __stdcall get_Height(long* Height) = 0;
	virtual HRESULT __stdcall put_Height(long Height) = 0;
	virtual HRESULT __stdcall get_Width(long* Width) = 0;
	virtual HRESULT __stdcall put_Width(long Width) = 0;
	virtual HRESULT __stdcall get_Top(long* Top) = 0;
	virtual HRESULT __stdcall put_Top(long Top) = 0;
	virtual HRESULT __stdcall get_Left(long* Left) = 0;
	virtual HRESULT __stdcall put_Left(long Left) = 0;
	virtual HRESULT __stdcall get_Name(BSTR* Name) = 0;
	virtual HRESULT __stdcall get_TextEditor(IDispatch** texteditor) = 0;
	virtual HRESULT __stdcall get_Version(BSTR* Version) = 0;
	virtual HRESULT __stdcall get_Path(BSTR* Path) = 0;
	virtual HRESULT __stdcall get_CurrentDirectory(BSTR* CurrentDirectory) = 0;
	virtual HRESULT __stdcall put_CurrentDirectory(BSTR CurrentDirectory) = 0;
	virtual HRESULT __stdcall get_FullName(BSTR* FullName) = 0;
	virtual HRESULT __stdcall get_Application(IDispatch** Application) = 0;
	virtual HRESULT __stdcall get_Parent(IDispatch** Parent) = 0;
	virtual HRESULT __stdcall get_ActiveDocument(IDispatch** ActiveDocument) = 0;
	virtual HRESULT __stdcall get_Windows(IDispatch** Windows) = 0;
	virtual HRESULT __stdcall get_Documents(IDispatch** Documents) = 0;
	virtual HRESULT __stdcall get_ActiveWindow(IDispatch** ActiveWindow) = 0;
	virtual HRESULT __stdcall get_WindowState(DsWindowState* windowstate) = 0;
	virtual HRESULT __stdcall put_WindowState(DsWindowState windowstate) = 0;
	virtual HRESULT __stdcall get_Debugger(IDispatch** ppDebugger) = 0;
	virtual HRESULT __stdcall get_Projects(IDispatch** Projects) = 0;
	virtual HRESULT __stdcall get_ActiveConfiguration(IDispatch** ActiveConfiguration) = 0;
	virtual HRESULT __stdcall put_ActiveConfiguration(IDispatch* ActiveConfiguration) = 0;
	virtual HRESULT __stdcall get_Visible(VARIANT_BOOL* Visible) = 0;
	virtual HRESULT __stdcall put_Visible(VARIANT_BOOL Visible) = 0;
	virtual HRESULT __stdcall get_ActiveProject(IDispatch** ActiveProject) = 0;
	virtual HRESULT __stdcall put_ActiveProject(IDispatch* ActiveProject) = 0;
	virtual HRESULT __stdcall put_Active(VARIANT_BOOL bActive) = 0;
	virtual HRESULT __stdcall get_Active(VARIANT_BOOL* pbActive) = 0;
	virtual HRESULT __stdcall GetPackageExtension(BSTR szExtensionName, IDispatch** pExt) = 0;
	virtual HRESULT __stdcall Quit() = 0;
	virtual HRESULT __stdcall PrintToOutputWindow(BSTR Message) = 0;
	virtual HRESULT __stdcall ExecuteCommand(BSTR szCommandName) = 0;
	virtual HRESULT __stdcall AddCommandBarButton(long nButtonType, BSTR szCmdName, long dwCookie) = 0;
	virtual HRESULT __stdcall AddKeyBinding(BSTR szKey, BSTR szCommandName, BSTR szEditor) = 0;
	virtual HRESULT __stdcall Build(VARIANT Configuration) = 0;
	virtual HRESULT __stdcall RebuildAll(VARIANT configuration) = 0;
	virtual HRESULT __stdcall ExecuteConfiguration(VARIANT Reserved) = 0;
	virtual HRESULT __stdcall SetAddInInfo(long nInstanceHandle, IDispatch* pCmdDispatch, long nIDBitmapResourceMedium, long nIDBitmapResourceLarge, long dwCookie) = 0;
	virtual HRESULT __stdcall AddCommand(BSTR szCmdName, BSTR szMethodName, long nBitmapOffset, long dwCookie, VARIANT_BOOL* pbResult) = 0;
	virtual HRESULT __stdcall EnableModeless(VARIANT_BOOL bEnable) = 0;
	virtual HRESULT __stdcall Clean(VARIANT configuration) = 0;
	virtual HRESULT __stdcall get_Errors(long* nErrors) = 0;
	virtual HRESULT __stdcall get_Warnings(long* nWarnings) = 0;
	virtual HRESULT __stdcall AddProject(BSTR szName, BSTR szPath, BSTR szType, VARIANT_BOOL bAddDefaultFolders) = 0;
	//---------------
	Configuration GetActiveConfiguration(){
		IConfiguration* pActiveConfiguration;
		get_ActiveConfiguration(OUT (IDispatch**)&pActiveConfiguration);
		return pActiveConfiguration;
	}
	//---------------
	GenericDocument GetActiveDocument(){
		IGenericDocument* pActiveDocument;
		get_ActiveDocument(OUT (IDispatch**)&pActiveDocument);
		return pActiveDocument;
	}
	//---------------
	Windows GetWindows(){
		IWindows* pWindows;
		get_Windows(OUT (IDispatch**)&pWindows);
		return pWindows;
	}
	//---------------
 	GenericWindow GetActiveWindow(){
		IGenericWindow* pActiveWindow;
		get_ActiveWindow((IDispatch**)&pActiveWindow);
		return pActiveWindow;
	}
	//---------------
 	GenericProject GetActiveProject(){
		IGenericProject* pActiveProject;
		get_ActiveProject((IDispatch**)&pActiveProject);
		return pActiveProject;
	}
	//---------------
	Documents GetDocuments(){
		IDocuments* pDocuments;
		get_Documents((IDispatch**)&pDocuments);
		return pDocuments;
	}
	//---------------
	Debugger GetDebugger(){
		IDebugger* pDebugger;
		get_Debugger((IDispatch**)&pDebugger);
		return pDebugger;
	}
	//---------------
	__declspec(property(get=GetWindows)) Windows Windows;
	__declspec(property(get=GetActiveConfiguration, put=put_ActiveConfiguration)) Configuration ActiveConfiguration;
	__declspec(property(get=GetActiveDocument)) GenericDocument ActiveDocument;
 	__declspec(property(get=GetActiveWindow)) GenericWindow ActiveWindow;
 	__declspec(property(get=GetActiveProject)) GenericProject ActiveProject;
 	__declspec(property(get=GetDocuments)) Documents Documents;
 	__declspec(property(get=GetDebugger)) Debugger Debugger;


// 	long GetHeight( );
// 	void PutHeight(long Height );
// 	long GetWidth( );
// 	void PutWidth(long Width );
// 	long GetTop( );
// 	void PutTop(long Top );
// 	long GetLeft( );
// 	void PutLeft(long Left );
// 	_bstr_t GetName( );
// 	IDispatchPtr GetTextEditor( );
// 	_bstr_t GetVersion( );
// 	_bstr_t GetPath( );
// 	_bstr_t GetCurrentDirectory( );
// 	void PutCurrentDirectory(_bstr_t CurrentDirectory );
// 	_bstr_t GetFullName( );
// 	IDispatchPtr GetApplication( );
// 	IDispatchPtr GetParent( );
// 	enum DsWindowState GetWindowState( );
// 	void PutWindowState(enum DsWindowState WindowState );
// 	IDispatchPtr GetDebugger( );
// 	IDispatchPtr GetProjects( );
// 	IDispatchPtr GetActiveConfiguration( );
// 	void PutActiveConfiguration(IDispatch * ActiveConfiguration );
// 	VARIANT_BOOL GetVisible( );
// 	void PutVisible(VARIANT_BOOL Visible );
// 	void PutActiveProject(IDispatch * ActiveProject );
// 	void PutActive(VARIANT_BOOL pbActive );
// 	VARIANT_BOOL GetActive( );
// 	IDispatchPtr GetPackageExtension(_bstr_t szExtensionName );
// 	HRESULT Quit( );
// 	HRESULT PrintToOutputWindow(_bstr_t Message );
// 	HRESULT ExecuteCommand(_bstr_t szCommandName );
// 	HRESULT AddCommandBarButton(long nButtonType, _bstr_t szCmdName, long dwCookie );
// 	HRESULT AddKeyBinding(_bstr_t szKey, _bstr_t szCommandName, _bstr_t szEditor );
// 	HRESULT Build(const _variant_t & Configuration = vtMissing );
// 	HRESULT RebuildAll(const _variant_t & Configuration = vtMissing );
// 	HRESULT ExecuteConfiguration(const _variant_t & Reserved = vtMissing );
// 	HRESULT SetAddInInfo(long nInstanceHandle, IDispatch * pCmdDispatch, long nIDBitmapResourceMedium, long nIDBitmapResourceLarge, long dwCookie );
// 	VARIANT_BOOL AddCommand(_bstr_t szCmdName, _bstr_t szMethodName, long nBitmapOffset, long dwCookie );
// 	HRESULT EnableModeless(VARIANT_BOOL bEnable );
// 	HRESULT Clean(const _variant_t & Configuration = vtMissing );
// 	long GetErrors( );
// 	long GetWarnings( );
// 	HRESULT AddProject(_bstr_t szName, _bstr_t szPath, _bstr_t szType, const _variant_t & bAddDefaultFolders );

// 	__declspec(property(get=GetName)) _bstr_t Name;
// 	__declspec(property(get=GetHeight,put=PutHeight)) long Height;
// 	__declspec(property(get=GetWidth,put=PutWidth)) long Width;
// 	__declspec(property(get=GetTop,put=PutTop)) long Top;
// 	__declspec(property(get=GetLeft,put=PutLeft)) long Left;
// 	__declspec(property(get=GetActive,put=PutActive)) VARIANT_BOOL Active;
// 	__declspec(property(get=GetDebugger)) IDispatchPtr Debugger;
// 	__declspec(property(get=GetVersion)) _bstr_t Version;
// 	__declspec(property(get=GetPath)) _bstr_t Path;
// 	__declspec(property(get=GetCurrentDirectory,put=PutCurrentDirectory)) _bstr_t CurrentDirectory;
// 	__declspec(property(get=GetFullName)) _bstr_t FullName;
// 	__declspec(property(get=GetApplication)) IDispatchPtr Application;
// 	__declspec(property(get=GetParent)) IDispatchPtr Parent;
// 	__declspec(property(get=GetActiveDocument)) IDispatchPtr ActiveDocument;
// 	__declspec(property(get=GetWindows)) IDispatchPtr Windows;
// 	__declspec(property(get=GetDocuments)) IDispatchPtr Documents;
// 	__declspec(property(get=GetActiveWindow)) IDispatchPtr ActiveWindow;
// 	__declspec(property(get=GetWindowState,put=PutWindowState)) enum DsWindowState WindowState;
// 	__declspec(property(get=GetTextEditor)) IDispatchPtr TextEditor;
// 	__declspec(property(get=GetProjects)) IDispatchPtr Projects;
// 	__declspec(property(get=GetActiveConfiguration,put=PutActiveConfiguration)) IDispatchPtr ActiveConfiguration;
// 	__declspec(property(get=GetVisible,put=PutVisible)) VARIANT_BOOL Visible;
// 	__declspec(property(get=GetActiveProject,put=PutActiveProject)) IDispatchPtr ActiveProject;
// 	__declspec(property(get=GetErrors)) long Errors;
// 	__declspec(property(get=GetWarnings)) long Warnings;
};
//---------------------------------------------------------------------------------------------
enum DsExecutionState{
	dsNoDebugee,
	dsBreak,
	dsRunning,
};
//---------------------------------------------------------------------------------------------
enum DsBreakpointType{
	dsLocation,
	dsLocationWithTrueExpression,
	dsLocationWithChangedExpression,
	dsTrueExpression,
	dsChangedExpression,
	dsMessage,
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) IDebugger:IDispatch{
	virtual HRESULT __stdcall get_Application( IDispatch** ppDispatch) = 0;
	virtual HRESULT __stdcall get_Parent( IDispatch** ppDispatch) = 0;
	virtual HRESULT __stdcall get_Breakpoints( IDispatch** ppDispatch) = 0;
	virtual HRESULT __stdcall get_DefaultRadix( long* pLong) = 0;
	virtual HRESULT __stdcall put_DefaultRadix( long l) = 0;
	virtual HRESULT __stdcall get_State( DsExecutionState* pState) = 0;
	virtual HRESULT __stdcall get_JustInTimeDebugging( VARIANT_BOOL* pBoolean) = 0;
	virtual HRESULT __stdcall put_JustInTimeDebugging( VARIANT_BOOL Bool) = 0;
	virtual HRESULT __stdcall get_RemoteProcedureCallDebugging( VARIANT_BOOL* pBoolean) = 0;
	virtual HRESULT __stdcall put_RemoteProcedureCallDebugging( VARIANT_BOOL Bool) = 0;
	virtual HRESULT __stdcall Go() = 0;
	virtual HRESULT __stdcall StepInto() = 0;
	virtual HRESULT __stdcall StepOver() = 0;
	virtual HRESULT __stdcall StepOut() = 0;
	virtual HRESULT __stdcall Restart() = 0;
	virtual HRESULT __stdcall Stop() = 0;
	virtual HRESULT __stdcall Break() = 0;
	virtual HRESULT __stdcall Evaluate( BSTR expr, BSTR* pBSTR) = 0;
	virtual HRESULT __stdcall ShowNextStatement() = 0;
	virtual HRESULT __stdcall RunToCursor() = 0;
	virtual HRESULT __stdcall SetNextStatement( VARIANT Selection) = 0;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct __declspec(uuid("34c63004-ae64-11cf-ab59-00aa00c091a1")) __declspec(novtable) IBreakpoint : IDispatch{
// __declspec(property(get=GetApplication)) IDispatchPtr Application;
// __declspec(property(get=GetParent)) IDispatchPtr Parent;
// __declspec(property(get=GetEnabled,put=PutEnabled)) VARIANT_BOOL Enabled;
// __declspec(property(get=GetLocation)) _bstr_t Location;
// __declspec(property(get=GetFunction)) _bstr_t Function;
// __declspec(property(get=GetExecutable)) _bstr_t Executable;
// __declspec(property(get=GetCondition,put=PutCondition)) _bstr_t Condition;
// __declspec(property(get=GetElements)) long Elements;
// __declspec(property(get=GetPassCount)) long PassCount;
// __declspec(property(get=GetMessage)) long Message;
// __declspec(property(get=GetWindowProcedure)) _bstr_t WindowProcedure;
// __declspec(property(get=GetType)) long Type;

//     IDispatchPtr GetApplication ( );
//     IDispatchPtr GetParent ( );
//     VARIANT_BOOL GetEnabled ( );
//     void PutEnabled (VARIANT_BOOL pBool );
//     _bstr_t GetLocation ( );
//     _bstr_t GetFunction ( );
//     _bstr_t GetExecutable ( );
//     _bstr_t GetCondition ( );
//     void PutCondition (_bstr_t pBSTR );
//     long GetElements ( );
//     long GetPassCount ( );
//     long GetMessage ( );
//     _bstr_t GetWindowProcedure ( );
//     long GetType ( );
//     HRESULT Remove ( );

	virtual HRESULT __stdcall get_Application (IDispatch * * ppDispatch ) = 0;
	virtual HRESULT __stdcall get_Parent (IDispatch * * ppDispatch ) = 0;
	virtual HRESULT __stdcall get_Enabled (VARIANT_BOOL * pBool ) = 0;
	virtual HRESULT __stdcall put_Enabled (VARIANT_BOOL pBool ) = 0;
	virtual HRESULT __stdcall get_Location (BSTR * pBSTR ) = 0;
	virtual HRESULT __stdcall get_File (BSTR * pBSTR ) = 0;
	virtual HRESULT __stdcall get_Function (BSTR * pBSTR ) = 0;
	virtual HRESULT __stdcall get_Executable (BSTR * pBSTR ) = 0;
	virtual HRESULT __stdcall get_Condition (BSTR * pBSTR ) = 0;
	virtual HRESULT __stdcall put_Condition (BSTR pBSTR ) = 0;
	virtual HRESULT __stdcall get_Elements (long * pLong ) = 0;
	virtual HRESULT __stdcall get_PassCount (long * pLong ) = 0;
	virtual HRESULT __stdcall get_Message (long * pLong ) = 0;
	virtual HRESULT __stdcall get_WindowProcedure (BSTR * pBSTR ) = 0;
	virtual HRESULT __stdcall get_Type (long * pLong ) = 0;
	virtual HRESULT __stdcall raw_Remove ( ) = 0;
	//---------------
  _bstr_t GetFile(){
		BSTR File;
		get_File(&File);
		return _bstr_t(File);
	}
	//---------------
  _bstr_t GetLocation(){
		BSTR Location;
		get_Location(&Location);
		return _bstr_t(Location);
	}
	//---------------
	__declspec(property(get=GetFile)) _bstr_t File;
	__declspec(property(get=GetLocation)) _bstr_t Location;
	//---------------
};
//---------------------------------------------------------------------------------------------
struct __declspec(novtable) IBreakpoints:IDispatch{
	virtual HRESULT __stdcall get_Count(long* Count) = 0;
	virtual HRESULT __stdcall get_Application(IDispatch** ppDispatch) = 0;
	virtual HRESULT __stdcall get_Parent(IDispatch** ppDispatch) = 0;
	virtual HRESULT __stdcall get__NewEnum(IUnknown** _NewEnum) = 0;
	virtual HRESULT __stdcall Item(VARIANT index, IDispatch** Item) = 0;
	virtual HRESULT __stdcall RemoveAllBreakpoints() = 0;	
	virtual HRESULT __stdcall RemoveBreakpointAtLine(VARIANT sel, VARIANT_BOOL* bOk) = 0;
	virtual HRESULT __stdcall AddBreakpointAtLine(VARIANT sel, IDispatch** Item) = 0;
	//---------------
};
//---------------------------------------------------------------------------------------------
Breakpoints Debugger::GetBreakpoints(){
	IBreakpoints* pBreakpoints;
	if (p) p->get_Breakpoints((IDispatch**)&pBreakpoints);
	return pBreakpoints;
}
//---------------------------------------------------------------------------------------------
long Breakpoints::GetCount(){
	long Count;
	if (p) p->get_Count(OUT &Count);
	return Count;
}
//---------------------------------------------------------------------------------------------
IBreakpoint& Breakpoints::operator [] (long iBreakpoint){
	VARIANT_INT_(index, iBreakpoint);
	IBreakpoint* pBp;
	if (p) p->Item(index, (IDispatch**)&pBp);
	return *pBp;
}
//---------------------------------------------------------------------------------------------
long Windows::GetCount(){
	long Count;
	if (p) p->get_Count(&Count);
	return Count;
}
//---------------------------------------------------------------------------------------------
#undef IFC