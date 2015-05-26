/////////////////////////////////////////////////////////////////////////////////////////////
// MAIN.CPP
/////////////////////////////////////////////////////////////////////////////////////////////
#define MAIN_CPP
#include "main.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// FiniApp
/////////////////////////////////////////////////////////////////////////////////////////////
void FiniApp()
{
	if(Player1) delete Player1;
	if(Player2) delete Player2;
	if(Ball) delete Ball;
	if(Stars) delete Stars;
	if(Bounce) delete Bounce;
	if(Point) delete Point;
	if(Winner) delete Winner;
	if(SoundSystem) delete SoundSystem;
	if(Engine) delete Engine;
	// Insert exit code here
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
					//PostMessage(hWnd, WM_CLOSE, 0, 0);
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
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInst;
	WndClass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = NULL;
	// Type app name here
	WndClass.lpszMenuName = "pong";
	WndClass.lpszClassName = "pong";
	RegisterClass(&WndClass);

	hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		// Here too
		"pong",
		"pong",
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

	Input.Create(hInst,hWnd);

	InitGame();

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
			if(!GetMessage(&msg, NULL, 0, 0 )) 
				return msg.wParam;
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else if(bActive)
		{
			UpdateGame();
		}
		else WaitMessage();
	}
}
