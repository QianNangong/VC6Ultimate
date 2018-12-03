// Windows Header Files:
#include <windows.h>

#include <win/stdmessages.h>
#include <win/PaintStruct.h>
#include <win/Window.h>
#include <win/GDI.h>
#include <WinGDI.h>
#include <win/Message.h>
#include <win/VkKeys.h>
#include <win/WindowClass.h>
#include <win/Icon.h>
#include <win/CommonResources.h>
#include <win/SysColors.h>
#include <win/stdwindows_styles.h>


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Local Header Files

#include "resource.h"

//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	switch (message){
		//---------------
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			{
				RECT rc;
				GetClientRect(hWnd, &rc);

				InflateRect(&rc,-10,-10);

				FillRect(hdc,&rc,(HBRUSH)GetStockObject(WHITE_BRUSH));
			}
			EndPaint(hWnd, &ps);
			break;
		}
		//---------------
		case WM_DESTROY:{
			PostQuitMessage(0);
			break;
		}
		//---------------
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:

			switch(wParam){
				case VK_ESCAPE:
					DestroyWindow(hWnd);
					return TRUE;
			}
			break;
		//---------------
		default:
			return DefWindowProcA(hWnd, message, wParam, lParam);
   }
   return 0;
}
//-------------------------------------------------------------------------------------------------------------
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, char* lpCmdLine, int nCmdShow){

	WNDCLASSEXA wcex={sizeof(WNDCLASSEXA), CS_HREDRAW|CS_VREDRAW, WndProc, 0, 0, 0, LoadIconA(hInstance, (char const*)IDI_MEDIUM), LoadCursorA(NULL, MAKEINTRESOURCEA(IDC_ARROW)), (HBRUSH)(COLOR_3DFACE+1),0,"MAINDLG", LoadIconA(wcex.hInstance, (char const*)IDI_SMALL)};
	ATOM atom=RegisterClassExA(&wcex);

	// Perform application initialization:
  HWND hWnd = CreateWindowExA(0, (char*)atom, "Window Name", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, 0, 200, 200, NULL, NULL, hInstance, NULL);
  if (!hWnd)
		return FALSE;

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (GetMessageA(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return msg.wParam;
}
//-------------------------------------------------------------------------------------------------------------

