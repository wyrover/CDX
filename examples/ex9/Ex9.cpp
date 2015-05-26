// ------------------------------------------------------------------------------------------
// CDXStar application
// You must link to cdd.lib, ddraw.lib and dxguid.lib
// ------------------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL           // include all CDX headers
#include <CDX.h>
#include "CDXStar.h"

CDXScreen       * Screen    = 0;    // The screen object, every program must have one
CDXStarField    * Stars     = 0;	// The CDXStarField object

BOOL            bActive     = FALSE;

#define NAME		"CDXExample9"
#define TITLE		"CDX Example 9"


// ------------------------------------------------------------------------------------------
// FiniApp - Destroy the CDX objects
// ------------------------------------------------------------------------------------------
void FiniApp(void)
{
	delete Stars;
	delete Screen;
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
				                    case VK_F2:     Stars->m_Towards = !Stars->m_Towards;
				                                    break;

				                    case VK_ADD:    Stars->SetNumberofStars(Stars->m_NumStars+10);
				                                    break;

				                    case VK_SUBTRACT:Stars->SetNumberofStars(Stars->m_NumStars-10);
				                                    break;

				                    case VK_F1: 	switch(Stars->m_StarMode)
					                                {
					                                case HORIZONTAL:
						                                Stars->m_StarMode = VERTICAL;
						                                break;

					                                case VERTICAL:
						                                Stars->m_StarMode = THREED;
						                                break;

					                                case THREED:
						                                Stars->m_StarMode = HORIZONTAL;
						                                break;
					                                }
				                                    break;

				                    case VK_ESCAPE: Screen->FadeOut(4); // first fade the screen out
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

    // change the screen font
	Screen->GetBack()->ChangeFont("Arial", 0, 14, FW_BOLD);

	// Create the CDXStar field
	Stars = new CDXStarField(Screen);
	Stars->Create();

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
			// Display some info
		    char str[200];
			char *modes[] = { "Horizontal", "Vertical", "3-D" };
			char *dir[] = { "Forward", "Backward" };

			sprintf(str, "F1 = mode (curr = %s)     F2 = direction (curr = %s)     +/- = # of stars (curr = %d)", 
			modes[Stars->m_StarMode], Stars->m_Towards ? dir[0] : dir[1], Stars->m_NumStars);

			Screen->GetBack()->Fill(0);
			Stars->Update();

	        Screen->GetBack()->GetDC();
		    Screen->GetBack()->SetFont();
			Screen->GetBack()->TextXY(5, 5, RGB(255, 255, 255), str);
	        Screen->GetBack()->ReleaseDC();

			Screen->Flip();
		}
		else 
			WaitMessage();
	}
}
