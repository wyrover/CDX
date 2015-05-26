/////////////////////////////////////////////////////////////////////////////////////////////
// CDX example 8 - Layers
// You must link to cdx.lib, ddraw.lib and dxguid.lib
/////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL           // include all CDX headers
#include <CDX.h>

CDXScreen   * Screen    = 0;    // The screen object, every program must have one
CDXTile     * Tiles     = 0;    // The background tiles
CDXMap      * Map       = 0;    // The scrolling map object
CDXLayer    * Layer     = 0;    // The scrolling layer bitmap
BOOL        bActive     = TRUE; // Is the program running?

#define NAME		"CDXExample8"
#define TITLE		"CDX Example 8"

// ------------------------------------------------------------------------------------------
// FiniApp - Destroy the CDX objects
// ------------------------------------------------------------------------------------------
void FiniApp(void)
{
	SAFEDELETE( Map );
	SAFEDELETE( Tiles );
    SAFEDELETE( Layer );
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
	if( FAILED(Screen->LoadPalette("LEDGES.BMP") ) ) 
        CDXError( Screen , "Could not load palette from file LEDGES.BMP" );

	// Load the tiles
    Tiles = new CDXTile();
    if( Tiles->Create(Screen, "LEDGES.BMP", 32, 32, 0) == FALSE )
        CDXError( Screen , "could not load tiles from file LEDGES.BMP" );
    
    // set the top left pixel in tiles bitmap as transparent color
	Tiles->SetColorKey();

	// Create and load the map
	Map = new CDXMap(Tiles, Screen);
	if( Map->Load("LEVEL.MAP") == FALSE )
        CDXError( Screen , "Could not load map LEVEL.MAP" );

	Map->EnableScrollWrapping(TRUE);

	// Create the bitmap layer
	Layer = new CDXLayer();
	Layer->Create(Screen, "FOREST.BMP");

	return TRUE;
}


// ------------------------------------------------------------------------------------------
// WinMain
// ------------------------------------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

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
            // check if cursor left was pressed and scroll 
			if(GetAsyncKeyState(VK_LEFT))
			{
				Map->ScrollLeft(2);
				Layer->ScrollLeft(1);
			}

            // check if cursor right was pressed and scroll 
			if(GetAsyncKeyState(VK_RIGHT))
			{
				Map->ScrollRight(2);
				Layer->ScrollRight(1);
			}

			// Draw the bitmap layer
			Layer->Draw(Screen->GetBack());

			// Draw the map to the back buffer
			Map->DrawTrans(Screen->GetBack());
	
			// Then display it
			Screen->Flip();
		}
		else WaitMessage();
	}
}
