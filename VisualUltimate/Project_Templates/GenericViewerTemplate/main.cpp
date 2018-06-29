#include <windows.h>
#include <genericviewer.h>
#define INCLUDE_SKYDOME() 0
#include <SkyDome.inl>
//-------------------------------------------------------------------------------------------------------------
struct VIEWER:GENERICVIEWER{
};
//-------------------------------------------------------------------------------------------------------------
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, char* lpCmdLine, int nCmdShow){
	VIEWER v;
	v.AutoRun(L"My Viewer");
	return 0;
}
//-------------------------------------------------------------------------------------------------------------



