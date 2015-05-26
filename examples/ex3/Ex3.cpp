// ------------------------------------------------------------------------------------------
// CDX example 3 - Windowed scrolling
// You must link to cdx.lib, ddraw.lib and dxguid.lib
// fixed and reworked for CDX 2.3 7/1999 by Ioannis Karagiorgos, Ioannis.Karagiorgos@gmx.de
// ------------------------------------------------------------------------------------------

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL           // include all CDX headers
#include <CDX.h>

CDXScreen   * Screen    = 0;    // The screen object, every program must have one
CDXTile     * Tiles     = 0;    // The background tiles
CDXMap      * Map       = 0;    // The scrolling map object

BOOL        bActive     = TRUE; // Is the program running?

#define NAME		"CDXExample3"
#define TITLE		"CDX Example 3"


// ------------------------------------------------------------------------------------------
// FiniApp - Destroy the CDX objects
// ------------------------------------------------------------------------------------------
void FiniApp(void)
{
    SAFEDELETE( Screen );       // delete the screen object
}



// ------------------------------------------------------------------------------------------
// WinProc, this handles all windows messages
// ------------------------------------------------------------------------------------------
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP:    bActive = wParam;   // set if app is active or not
			                    break;

		case WM_KEYDOWN:		switch(wParam)      // if ESC key is hit, quit program
			                    {
				                    case VK_ESCAPE:
					                PostMessage(hWnd, WM_CLOSE, 0, 0);
				                    break;
			                    }
		                        break;

		case WM_DESTROY:		FiniApp();			// destroy all objects
								PostQuitMessage(0); // terminate program
		                        break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



// ------------------------------------------------------------------------------------------
// InitApp - Create the window and the CDX objects
// ------------------------------------------------------------------------------------------
BOOL InitApp(HINSTANCE hInst, int nCmdShow)
{
	HWND        hWnd;
	WNDCLASS    WndClass;
   	RECT        rc;
	DWORD       dwStyle;


	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInst;
	WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(0, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = NAME;
	RegisterClass(&WndClass);

    // create the main window
	hWnd = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		NAME,
		TITLE,
		WS_VISIBLE|WS_SYSMENU,
		0,0,
		448,
		320,
		NULL,
		NULL,
		hInst,
		NULL);

    // when hWnd = -1 there was an error creating the main window
    // CDXError needs a CDXScreen object, if there is none at this early
    // program stage, pass it NULL
	if(!hWnd) 
        CDXError( NULL , "could not create the main window" );

    // set the window style to a normal window
	dwStyle = GetWindowStyle(hWnd);
	dwStyle &= ~WS_POPUP;
	dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);

    // set the client area size to 448 , 320
	SetRect(&rc, 0, 0, 448, 320);
	AdjustWindowRectEx(&rc,
		GetWindowStyle(hWnd),
		GetMenu(hWnd) != NULL,
		GetWindowExStyle(hWnd));

    // set windows coordinates to 0,0
	SetWindowPos(hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

    // show the main window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Create the CDXSreen object
	Screen = new CDXScreen();
	if( FAILED(Screen->CreateWindowed(hWnd, 448, 320)))
        CDXError( NULL , "could not set video mode 448x320 windowed" );

	// Load the tiles
    Tiles = new CDXTile();
    if( Tiles->Create( Screen , "TILES.BMP" , 64 , 64 , 0 )==FALSE)
        CDXError( Screen , "could not load tiles from file TILES.BMP" );

	// Create and load the map
	Map = new CDXMap(Tiles, Screen);
	Map->Create(64, 64, 1);
	Map->MoveTo(0,0);

	Map->EnableScrollWrapping(TRUE);

	return TRUE;
}


// ------------------------------------------------------------------------------------------
// WinMain
// ------------------------------------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG 	msg;

	if(!InitApp(hInst, nCmdShow))
        CDXError( NULL , "could not initialize CDX application" );

	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0 )) return msg.wParam;
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else if(bActive)    // if application has the focus, process
		{
            // Check key states for arrow up,down,left and right
            // and scroll the map in the corresponding direction
			if(GetAsyncKeyState(VK_UP)) Map->ScrollUp(2);
			if(GetAsyncKeyState(VK_DOWN)) Map->ScrollDown(2);
			if(GetAsyncKeyState(VK_LEFT))	Map->ScrollLeft(2);
			if(GetAsyncKeyState(VK_RIGHT)) Map->ScrollRight(2);

			Map->Draw(Screen->GetBack());  // Draw the map to the back buffer
			Screen->Flip();  // Then display it
		}
		else WaitMessage();
	}
}
