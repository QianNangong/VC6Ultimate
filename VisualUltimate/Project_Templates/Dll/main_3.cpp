#include <windows.h>
#include "$$root$$.h"
//---------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, void* lpReserved){
	switch (ul_reason_for_call){
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
  }
  return TRUE;
}
//---------------------------------------------------------------------------------------------
$$SAFE_ROOT$$_API int nSample=0;						// This is an example of an exported variable
//---------------------------------------------------------------------------------------------
$$SAFE_ROOT$$_API int __cdecl fnSample(){		// This is an example of an exported function.
	return 42;
}
//---------------------------------------------------------------------------------------------
SampleStruct::SampleStruct(){										// This is the constructor of a class that has been exported.
	return; 
}
//---------------------------------------------------------------------------------------------

