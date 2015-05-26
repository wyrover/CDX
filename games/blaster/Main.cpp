/////////////////////////////////////////////////////////////////////////////////////////////
// GAME.CPP
/////////////////////////////////////////////////////////////////////////////////////////////
#define MAIN_CPP
#include "main.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// FiniApp
/////////////////////////////////////////////////////////////////////////////////////////////
void FiniApp()
{
	Music->Stop();

	delete Sound;
	delete Music;
	delete Blaster;
	delete Player;
	delete WorkSurface;
	delete Map;
	delete Tiles;
	delete Screen;
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
					//Screen->FadeOut(4);
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
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInst;
	WndClass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = NULL;
	WndClass.lpszMenuName = "Blaster";
	WndClass.lpszClassName = "Blaster";
	RegisterClass(&WndClass);

    // create the main window
	hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		"Blaster",
		"Blaster",
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

	InitGame();
	Input.Create(hInst, hWnd);

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
			UpdateGame();
		}
		else WaitMessage();
	}
}
