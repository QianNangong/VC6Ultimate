// All files within this DLL are compiled with the $$SAFE_ROOT$$_EXPORTS symbol defined on the command line. 
// This symbol should not be defined on any project that uses this DLL. 
#ifdef $$SAFE_ROOT$$_EXPORTS
	#define $$SAFE_ROOT$$_API __declspec(dllexport)
#else
	#define $$SAFE_ROOT$$_API __declspec(dllimport)
#endif
//---------------------------------------------------------------------
struct $$SAFE_ROOT$$_API SampleStruct{		// This class is exported from the dll3.dll
	SampleStruct();
};
//---------------------------------------------------------------------
extern $$SAFE_ROOT$$_API int nSample;
//---------------------------------------------------------------------
$$SAFE_ROOT$$_API int __cdecl fnSample();
//---------------------------------------------------------------------

