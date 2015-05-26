// ------------------------------------------------------------------------------------------
// CDX example 7 - Clipped scrolling
// You must link to cdx.lib, ddraw.lib and dxguid.lib
// ------------------------------------------------------------------------------------------


#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL           // include all CDX headers
#include <CDX.h>

CDXScreen   * Screen    = 0;    // The screen object, every program must have one
CDXTile     * Tiles     = 0;    // The background tiles
CDXMap      * Map1      = 0;    // The scrolling maps
CDXMap      * Map2      = 0;
CDXMap      * Map3      = 0; 
CDXMap      * Map4      = 0;

BOOL        bActive     = TRUE; // Is the program running?

#define NAME		"CDXExample7"
#define TITLE		"CDX Example 7"


// ------------------------------------------------------------------------------------------
// FiniApp - Destroy the CDX objects
// ------------------------------------------------------------------------------------------
void FiniApp(void)
{
	SAFEDELETE( Map1 );
    SAFEDELETE( Map2 );
    SAFEDELETE( Map3 );
    SAFEDELETE( Map4 );
	SAFEDELETE( Tiles );
	SAFEDELETE( Screen );
}




// ------------------------------------------------------------------------------------------
// WinProc, this handles all windows messages
// ------------------------------------------------------------------------------------------
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_SETCURSOR:		SetCursor(NULL);    // disable the cursor
			                    return TRUE;

		case WM_ACTIVATEAPP:    bActive = wParam;   // set if app is active or not
			                    break;

		case WM_KEYDOWN:		switch(wParam)      // if ESC key is hit, quit program
			                    {
				                    case VK_ESCAPE:
					                Screen->FadeTo(255, 255, 255, 0);
                                    Screen->FadeOut(4); // first fade the screen out
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
		WS_EX_TOPMOST,
		NAME,
		TITLE,
		WS_POPUP,
		0,0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hInst,
		NULL);

    // when hWnd = -1 there was an error creating the main window
    // CDXError needs a CDXScreen object, if there is none at this early
    // program stage, pass it NULL
	if(!hWnd) 
        CDXError( NULL , "could not create the main window" );

    // show the main window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Create the CDXSreen object
	Screen = new CDXScreen();

    // set the video mode 640x480x8
    if( FAILED(Screen->CreateFullScreen( hWnd , 640 , 480 , 8 ) ) )
        CDXError( Screen , "could not set video mode 640x480x8" );

    // Load the palette from the tiles bitmap
	if( FAILED(Screen->LoadPalette("ANIM.BMP") ) ) 
        CDXError( Screen , "Could not load palette from file ANIM.BMP" );

	// Load the tiles
    Tiles = new CDXTile();
    if( Tiles->Create( Screen , "ANIM.BMP" , 64 , 64 , 0 ) == FALSE )
        CDXError( Screen , "could not load tiles from file ANIM.BMP" );


	// Create and load the maps
	Map1 = new CDXMap(Tiles, Screen);
	Map1->Create(64, 64, 1);
	Map1->EnableScrollWrapping(TRUE);

	Map2 = new CDXMap(Tiles, Screen);
	Map2->Create(64, 64, 2);
	Map2->EnableScrollWrapping(TRUE);

	Map3 = new CDXMap(Tiles, Screen);
	Map3->Create(64, 64, 3);
	Map3->EnableScrollWrapping(TRUE);
	
	Map4 = new CDXMap(Tiles, Screen);
	Map4->Create(64, 64, 4);
	Map4->EnableScrollWrapping(TRUE);

	return TRUE;
}




// ------------------------------------------------------------------------------------------
// WinMain
// ------------------------------------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG     msg;
	int     Delay = 0;
	RECT Window1 = { 0, 0, 320, 240 };
	RECT Window2 = { 320, 0, 640, 240 };
	RECT Window3 = { 0, 240, 640, 480 };
	RECT Window4 = { 320, 240, 640, 480 };

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
		else if(bActive)
		{
			// Clear the back buffer
			Screen->Fill(0);

            // check if cursor up was pressed and move the maps
			if(GetAsyncKeyState(VK_UP))
			{
				Map1->ScrollUp(2);
				Map2->ScrollUp(2);
				Map3->ScrollUp(2);
				Map4->ScrollUp(2);
			}

            // check if cursor down was pressed and move the maps
			if(GetAsyncKeyState(VK_DOWN))
			{
				Map1->ScrollDown(2);
				Map2->ScrollDown(2);
				Map3->ScrollDown(2);
				Map4->ScrollDown(2);
			}

            // check if cursor left was pressed and move the maps
			if(GetAsyncKeyState(VK_LEFT))
			{
				Map1->ScrollLeft(2);
				Map2->ScrollLeft(2);
				Map3->ScrollLeft(2);
				Map4->ScrollLeft(2);
			}

            // check if cursor right was pressed and move the maps
			if(GetAsyncKeyState(VK_RIGHT))
			{
				Map1->ScrollRight(2);
				Map2->ScrollRight(2);
				Map3->ScrollRight(2);
				Map4->ScrollRight(2);
			}

			// Draw the maps to the back buffer
            // each map is clipped individually
			Map1->DrawClipped(Screen->GetBack(), &Window1);
			Map2->DrawClipped(Screen->GetBack(), &Window2);
			Map3->DrawClipped(Screen->GetBack(), &Window3);
			Map4->DrawClipped(Screen->GetBack(), &Window4);
	
			// Then display the back buffer
			Screen->Flip();
		}
		else WaitMessage();
	}
}
