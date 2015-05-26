/////////////////////////////////////////////////////////////////////////////////////////////
// CDX Breakout - a simple breakout game
// You must link to cdx.lib, ddraw.lib, dinput.lib, dsound.lib and dxguid.lib
//
// Keys:
//
// Left, Right	= move
//
// Things you can do to the source to make it better:
//
// -add a high score table
// -add 2 player mode
// -add a front-end menu system with options like play, options, etc.
// -add more levels and read them from a file instead of the hard coded one here now
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>

extern BOOL InitGraphics(HWND);
extern void FiniGraphics(void);

extern BOOL InitInput(HINSTANCE, HWND);
extern void FiniInput(void);

extern BOOL InitSound(HWND);
extern void FiniSound(void);

extern void InitEngine(void);
extern void UpdateFrame(void);

BOOL bActive;
// extern int ProgramState;

/////////////////////////////////////////////////////////////////////////////////////////////
// FiniApp
/////////////////////////////////////////////////////////////////////////////////////////////
void FiniApp()
{
	FiniSound();
	FiniInput();
	FiniGraphics();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// WinProc
/////////////////////////////////////////////////////////////////////////////////////////////
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP:
			bActive = wParam;
			break;

		case WM_CREATE:
			break;

		case WM_SETCURSOR:
			SetCursor(NULL);
			return TRUE;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
			}
			break;

		case WM_DESTROY:
			FiniApp();
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// InitApp
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL InitApp(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInst;
	WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(0, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = "CDXBreakout";
	RegisterClass(&WndClass);

	hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		"CDXBreakout",
		"CDXBreakout",
		WS_POPUP,
		0,0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hInst,
		NULL);

	if(!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if(!InitGraphics(hWnd)) return FALSE;
	if(!InitInput(hInst, hWnd)) return FALSE;
	if(!InitSound(hWnd)) return FALSE;
	InitEngine();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// WinMain
/////////////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if(!InitApp(hInst, nCmdShow)) return FALSE;

	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0 )) return msg.wParam;
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else if(bActive)
		{
			UpdateFrame();
		}
		else WaitMessage();
	}
}

