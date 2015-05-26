// ------------------------------------------------------------------------------------------
// CDX example 12 - CDXMenu example
// You must link to cdx.lib, ddraw.lib, dinput and dxguid
// ------------------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL           // include all CDX headers
#include <CDX.h>
#include "CDXMenu.h"

#define NAME		"CDXExample12"
#define TITLE		"CDX Example 12"

#define MENU_ITEM_HEIGHT		48	// height of each menu item and spacing between items

CDXScreen   * Screen        = 0;	// The screen object, every program must have one
CDXMenu     * MainMenu      = 0;
CDXMenu     * OptionsMenu   = 0;	// main and options menu. options menu is a child of main
CDXMenu     * CurrentMenu   = 0;	// pointer to the current menu to display

BOOL        bActive         = TRUE;  // Is the program running?


// ------------------------------------------------------------------------------------------
// FiniApp - Destroy the CDX objects
// ------------------------------------------------------------------------------------------
void FiniApp(void)
{
	SAFEDELETE( OptionsMenu );
	SAFEDELETE( MainMenu );
	SAFEDELETE( Screen );
}



// ------------------------------------------------------------------------------------------
// WinProc
// ------------------------------------------------------------------------------------------
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP:
			bActive = wParam;
		break;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_HOME:
					CurrentMenu->Home();
				break;

				case VK_END:
					CurrentMenu->End();
				break;

				case VK_UP:
					CurrentMenu->Up();
				break;

				case VK_DOWN:
					CurrentMenu->Down();
				break;

				case VK_ESCAPE:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;

				case VK_RETURN:
					switch(CurrentMenu->Enter()) {
						case 2:
							if(CurrentMenu == OptionsMenu) {
								CurrentMenu = MainMenu;
							}
							else {
								CurrentMenu = OptionsMenu;
							}
						break;

						case 3:
							if(CurrentMenu == MainMenu) {
								PostMessage(hWnd, WM_CLOSE, 0, 0);
							}
						break;
					}
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

    // set the video mode 800x600x8
    if( FAILED(Screen->CreateFullScreen( hWnd , 800 , 600 , 8 ) ) )
        CDXError( Screen , "could not set video mode 800x600x8" );

	// Create the CDXMenu objects in reverse order (children first)
	OptionsMenu = new CDXMenu();
	OptionsMenu->Create(Screen->GetBack(), MENU_ITEM_HEIGHT);

	// Don't wrap this menu
	OptionsMenu->SetWrap(FALSE);

	// Set the title of the menu to yellow
	OptionsMenu->SetTitle("Options", RGB(255, 255, 0));
	Screen->GetBack()->ChangeFont("Verdana", 0, MENU_ITEM_HEIGHT, FW_BOLD);

	// Add some options to the menu
	OptionsMenu->AddItem("Display", RGB(255, 255, 255), RGB(0, 0, 255));
	OptionsMenu->AddItem("Sound", RGB(255, 255, 255), RGB(0, 0, 255));
	OptionsMenu->AddItem("Quit", RGB(255, 255, 255), RGB(0, 0, 255));

	// Create the main menu
	MainMenu = new CDXMenu();
	MainMenu->Create(Screen->GetBack(), MENU_ITEM_HEIGHT);

	// Set the title of the menu to yellow
	MainMenu->SetTitle("Main Menu", RGB(255, 255, 0));

	// Set the font for the menu
	Screen->GetBack()->ChangeFont("Verdana", 0, MENU_ITEM_HEIGHT, FW_BOLD);

	// Add some options to the menu
	MainMenu->AddItem("Single Player", RGB(255, 255, 255), RGB(0, 0, 255));
	MainMenu->AddItem("Multiplayer", RGB(255, 255, 255), RGB(0, 0, 255));
	MainMenu->AddItem("Options", RGB(255, 255, 255), RGB(0, 0, 255));
	MainMenu->AddItem("Quit", RGB(255, 255, 255), RGB(0, 0, 255));

	CurrentMenu = MainMenu;
	
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
            // clear screen
			Screen->GetBack()->Fill(0);

            // draw menu
			CurrentMenu->Draw(300, 150);

            // display back buffer
			Screen->Flip();
		}
		else WaitMessage();
	}
}
