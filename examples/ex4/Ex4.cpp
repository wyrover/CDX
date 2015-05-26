// ------------------------------------------------------------------------------------------
// CDX example 4 - Fullscreen scrolling & keyboard input
// You must link to cdx.lib, ddraw.lib, dinput and dxguid
// ------------------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL           // include all CDX headers
#include <CDX.h>

CDXScreen   * Screen    = 0;    // The screen object, every program must have one
CDXTile     * Tiles     = 0;    // The background tiles
CDXMap      * Map       = 0;    // The scrolling map object
CDXInput    * Input     = 0;    // The input object

BOOL        bActive     = TRUE;                 // Is the program running?
char        Bitmap[]    = "TILES.BMP";          // tiles filename
RECT        Window      = { 0, 0, 640, 480 };   // clip rectangle

#define NAME		"CDXExample4"
#define TITLE		"CDX Example 4"


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



/////////////////////////////////////////////////////////////////////////////////////////////
// InitApp - Create the window and the CDX objects
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

    // This is a special case because some graphics cards support 16bit, other 15bit
    // so we have to check this
    // check if the 16bit mode exists
    if( Screen->CheckIfVideoModeExists( 640 , 480 , 16 ) == FALSE )
    {   // no, set 15 bit mode and display ewrror if it fails
        if( FAILED(Screen->CreateFullScreen( hWnd , 640 , 480 , 15 )))
            CDXError( Screen , "could not set video mode 640x480x15" );
    }
    else
    {   // yes, set 16 bit mode and display error if it fails
        if( FAILED(Screen->CreateFullScreen( hWnd , 640 , 480 , 16 )))
            CDXError( Screen , "could not set video mode 640x480x16" );
    }

	// Load the tiles
    Tiles = new CDXTile();
    if( Tiles->Create( Screen , Bitmap , 64 , 64 , 0 ) == FALSE )
        CDXError( Screen , "could not load tiles from file TILES.BMP" );

	// Create and load the map
	Map = new CDXMap(Tiles, Screen);
	Map->Create(512, 512, 1);
	Map->MoveTo(0,0);

	Map->EnableScrollWrapping(TRUE);

	// Create input devices
    Input = new CDXInput();
	if( FAILED(Input->Create( hInst , hWnd )))
		CDXError( Screen , "could not create direct input object" );

	return TRUE;
}



// ------------------------------------------------------------------------------------------
// WinMain
// ------------------------------------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG     msg;
	LONG    JoyX , JoyY;

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
		else if(bActive)                            // if application has the focus, process
		{
			Input->Update();                        // Refresh the input devices
			Input->GetJoystickPos( &JoyX , &JoyY ); // get joystick position

            // check how far the joystick moved and move the map corresponding
			if(JoyY < 0 || Input->GetKeyState(CDXKEY_UPARROW)) Map->ScrollUp(1);
			if(JoyY > 0 || Input->GetKeyState(CDXKEY_DOWNARROW)) Map->ScrollDown(1);
			if(JoyX < 0 || Input->GetKeyState(CDXKEY_LEFTARROW)) Map->ScrollLeft(1);
			if(JoyX > 0 || Input->GetKeyState(CDXKEY_RIGHTARROW)) Map->ScrollRight(1);

			Map->DrawClipped(Screen->GetBack(), &Window);   // Draw the map to the back buffer
			Screen->Flip();                                 // Then display it
		}
		else WaitMessage();
	}
}
