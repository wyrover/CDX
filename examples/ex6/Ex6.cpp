// ------------------------------------------------------------------------------------------
// CDX example 6 - Animated Tiles
// You must link to cdx.lib, ddraw.lib and dxguid.lib
// ------------------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL           // include all CDX headers
#include <CDX.h>

CDXScreen   * Screen    = 0;    // The screen object, every program must have one
CDXTile     * Tiles     = 0;    // The background tiles
CDXMap      * Map       = 0;    // The scrolling map object

BOOL        bActive     = TRUE; // Is the program running?

#define NAME		"CDXExample6"
#define TITLE		"CDX Example 6"


// ------------------------------------------------------------------------------------------
// FiniApp - Destroy the CDX objects
// ------------------------------------------------------------------------------------------
void FiniApp(void)
{
	SAFEDELETE( Map );
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
	int         Tile = 1;
	int         i , j = 0;


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

	// Create and load the map
	Map = new CDXMap(Tiles, Screen);
	Map->Create(63, 63, 0);
	Map->MoveTo(0,0);

	Map->EnableScrollWrapping(TRUE);

    // initialize the tiles in the map
	for( i=0; i<63; i++ )
		for( j=0; j<63; j++ )
		{
			Map->SetTile( i , j , Tile );
			if( ++Tile > 4) 
                Tile = 1;
		}

	return TRUE;
}


// ------------------------------------------------------------------------------------------
// WinMain
// ------------------------------------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG     msg;
	int     Delay  = 0;
	RECT    ClipWindow = { 0, 0, 640, 480 };
    int     i , j, Tile;

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

            // wait 40 frames until the next map update
			if(++Delay > 40)
			{
                Tile = 1;
				for( i=0; i<63; i++ )
					for( j=0; j<63; j++ )
					{
						Tile = Map->GetTile( i , j );
						++ Tile;
						if( Tile > 4 ) Tile = 1;
						Map->SetTile( i , j , Tile );
					}
					Delay = 0;
			}
            
            // check cursor keys and scroll the map into the corresponding direction
			if(GetAsyncKeyState(VK_UP))    Map->ScrollUp(2);
			if(GetAsyncKeyState(VK_DOWN))  Map->ScrollDown(2);
			if(GetAsyncKeyState(VK_LEFT))  Map->ScrollLeft(2);
			if(GetAsyncKeyState(VK_RIGHT)) Map->ScrollRight(2);

			// Draw the map to the back buffer, clipped at rectangle Window
			Map->DrawClipped(Screen->GetBack(), &ClipWindow);

	
			// Then display it
			Screen->Flip();
		}
		else WaitMessage();
	}
}

