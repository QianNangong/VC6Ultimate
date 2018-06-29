// Windows Header Files:
#include <windows.h>
#include <win/stdmessages.h>
#include <win/Dialog.h>
#include <win/Window.h>
#include <win/Message.h>
#include <win/DrawText.h>
#include <win/WindowClass.h>
#include <win/Icon.h>
#include <win/CommonResources.h>
#include <win/SysColors.h>
#include <win/stdwindows_styles.h>
#include <win/Accel.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Local Header Files

#include "resource.h"

// Global Variables:
HINSTANCE ghAppInstance;									// current instance
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
		case WM_INITDIALOG:{
			return TRUE;
		}
		case WM_COMMAND:{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
		}
	}
  return FALSE;
}
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	switch (message){
		//---------------
		case WM_COMMAND:{
			int wmId    = LOWORD(wParam); 
			int wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId){
				case IDM_ABOUT:
				   DialogBoxParamA(ghAppInstance, (char const*)IDD_ABOUTBOX, hWnd, (DLGPROC)About, 0);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProcA(hWnd, message, wParam, lParam);
			}
			break;
		}
		//---------------
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			RECT rt;
			GetClientRect(hWnd, &rt);

			char szHello[]="Hello World!";

			DrawTextA(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		}
		//---------------
		case WM_DESTROY:{
			PostQuitMessage(0);
			break;
		}
		//---------------
		default:
			return DefWindowProcA(hWnd, message, wParam, lParam);
   }
   return 0;
}
//-------------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance){
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEXA); 

	wcex.style					= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc		= (WNDPROC)WndProc;
	wcex.cbClsExtra			= 0;
	wcex.cbWndExtra			= 0;
	wcex.hInstance			= hInstance;
	wcex.hIcon					= LoadIconA(hInstance, (char const*)IDI_MEDIUM);
	wcex.hCursor				= LoadCursorA(NULL, MAKEINTRESOURCEA(IDC_ARROW));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName		= (char const*)IDC_$$SAFE_ROOT$$;
	wcex.lpszClassName	= "$$SAFE_ROOT$$";
	wcex.hIconSm				= LoadIconA(wcex.hInstance, (char const*)IDI_SMALL);

	return RegisterClassExA_(&wcex);
}
//-------------------------------------------------------------------------------------------------------------
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, char* lpCmdLine, int nCmdShow){

	MyRegisterClass(hInstance);

	// Perform application initialization:
  ghAppInstance = hInstance; // Store instance handle in our global variable
  HWND hWnd = CreateWindowExA(0, "$$SAFE_ROOT$$", "$$root$$", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
  if (!hWnd)
		return FALSE;

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

	HACCEL hAccelTable = LoadAcceleratorsA(hInstance, (char const*)IDC_$$SAFE_ROOT$$);

	// Main message loop:
	MSG msg;
	while (GetMessageA(&msg, NULL, 0, 0)){
		if (!TranslateAcceleratorA(msg.hwnd, hAccelTable, &msg)){
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	return msg.wParam;
}
//-------------------------------------------------------------------------------------------------------------

