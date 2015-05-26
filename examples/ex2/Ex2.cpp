// ------------------------------------------------------------------------------------------
// CDX example 2 - Windowed applications
// You must link to cdx.lib, ddraw.lib and dxguid.lib
// fixed and reworked for CDX 2.3 7/1999 by Ioannis Karagiorgos, Ioannis.Karagiorgos@gmx.de
// ------------------------------------------------------------------------------------------

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL       // include all headers
#include <CDX.h>


CDXScreen   * Screen = 0;   // The screen object, every program must have one
                            // remember to set all CDX objects to 0 when you declare them!
int           Toggle = 0;   // flag for the screen color

#define TIMER_ID	1       // variables used for the timer
#define TIMER_RATE	500

#define NAME		"CDXExample2"
#define TITLE		"CDX Example 2"




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
		case WM_TIMER:  if( Toggle )        // fill the screen depending on toggle flag
                        { 
				            Screen->Fill(0);
				            Toggle = 0;
			            }
			            else
			            {
				            Screen->Fill(255);
				            Toggle = 1;
			            }
			            Screen->Flip();     // flip back and front buffers
		                break;

		case WM_KEYDOWN:switch(wParam)
			            {
				            case VK_ESCAPE: // if ESC key was hit, quit program
					        PostMessage(hWnd, WM_CLOSE, 0, 0);
				            break;
			            }
		                break;

		case WM_DESTROY:    FiniApp();          // destroy all objects
			                PostQuitMessage(0); // terminate the program
		                    break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



// ------------------------------------------------------------------------------------------
// InitApp - Create the window and the CDX objects
// ------------------------------------------------------------------------------------------
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

	hWnd = CreateWindowEx(
		0,
		NAME,
		TITLE,
 		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
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
	if( FAILED(Screen->CreateWindowed( hWnd , 640 , 480 ) ) )
        CDXError( NULL , "could not set video mode 640x480 windowed" );

	// Create our timer for flipping the screen display
	SetTimer(hWnd, TIMER_ID, TIMER_RATE, NULL);

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
		else WaitMessage();
	}
}
