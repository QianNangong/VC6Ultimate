// Unreal Script helper classes

#define VARARG_DECL( FuncRet, StaticFuncRet, Return, FuncName, Pure, FmtType, ExtraDecl, ExtraCall )	\
	FuncRet FuncName( ExtraDecl FmtType Fmt, ... );

#define IMPLEMENT_CLASS(TClass) \
	UClass* TClass::PrivateStaticClass = NULL; \
	UClass* TClass::GetPrivateStaticClass##TClass( TCHAR* Package );\
	UBOOL TClass::HasUniqueStaticConfigName() const;\
	UBOOL TClass::HasParentClassChanged() const ;

#define DECLARE_CLASS( TClass, TSuperClass, TStaticFlags, TPackage ) \
	DECLARE_BASE_CLASS( TClass, TSuperClass, TStaticFlags, TPackage ) \
	friend FArchive &operator<<( FArchive& Ar, TClass*& Res );\
		virtual ~TClass();\
		static void InternalConstructor( void* X );

template<class classname>
class array
{
	int length;
	classname operator[](int index);
};
typedef FString String;
