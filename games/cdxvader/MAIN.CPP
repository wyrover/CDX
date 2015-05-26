/**************************************************************************
 Project Name  : SPACEY VADERS
 Version       : 1.00
 File Name     : Main.cpp
 
 Programmer    : Paul J Lord (PLord1@aol.com)
 
 Creation Date : 06/03/97
 Update Date   : 07/14/97
 
 Special Notes : Create a simple Space Invader Clone that uses
                 DirectDraw, DirectSound, DirectInput.
                 
                 Graphics images from 'The SpriteLib' by Ari Feldman.
                 http://www.walrus.com/~ari/
                 ari@walrus.com
                 SpriteLib License number: 152.172.91.155-868871971
                 

 Compiling Notes:
    Requires DirectX 3.0
    You must link to winmm.lib, ddraw.lib, dinput, dxguid and cdx.lib.

 Things to do:
    1.  Create a 2nd sprite linked list for the missiles,
        so I have display them over the other sprites.
    2.  Create smarter Vaders that shoot better.
    3.  Restore the buffers of application is switched.
 
 
 Thanks:       
   Julie, Ciarra, Savanna, Stevie Lord for testing the game.
   Nelson Grandjean for getting me started in high school.
   
   Special Thanks to Danny Farley ( danny@jags.co.uk) for the CDX.LIB Classes.
   Check out his web site: http://www.maidex.demon.co.uk   
  

/**************************************************************************/
#define MAIN_CPP
#include "main.h"



/**************************************************************************
 Function: FiniApp()
 
 Purpose: Clean and release any objects.
/**************************************************************************/
void FiniApp()
{

	if (MusicFlag == 1) Music->Stop();
    
    // Close the Debug file
    if (DebugFlag == 1) fclose(fdebug);
    
}


/**************************************************************************
 Function: WinProc
 
 Purpose: Process windows events
/**************************************************************************/
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP:
            bActive = wParam;
            // Setup the Screen again...
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



/**************************************************************************
 Function: InitApp
 
 Purpose: Setup a Windows Class.
/**************************************************************************/
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
	WndClass.lpszMenuName = "CDXVader";
	WndClass.lpszClassName = "CDXVader";
	RegisterClass(&WndClass);

	hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		"CDXVader",
		"CDXVader",
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

    SetCursor(NULL);

	return TRUE;
}


/**************************************************************************
 Function: WinMain
 
 Purpose: Main function.
/**************************************************************************/
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if(!InitApp(hInst, nCmdShow)) return FALSE;
// Print Error Message Box HERE

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


