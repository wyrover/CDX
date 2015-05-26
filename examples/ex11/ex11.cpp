/////////////////////////////////////////////////////////////////////////////////////////////
// CDX example 11 - Fullscreen ISO map scrolling & keyboard input
// You must link to cdx.lib, ddraw.lib, dinput and dxguid
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <time.h>
#define CDXINCLUDEALL
#include <CDX.h>

#define NAME		"CDXExample11"
#define TITLE		"CDX Example 11"

CDXScreen* Screen;  // The screen object, every program must have one
CDXTile* Tiles;     // The background tiles
CDXIsoMap* Map;        // The scrolling map object

BOOL bActive = TRUE;  // Is the program running?
char Bitmap[] = "TILES.BMP";

/////////////////////////////////////////////////////////////////////////////////////////////
// FiniApp - Destroy the CDX objects
/////////////////////////////////////////////////////////////////////////////////////////////
void FiniApp(void)
{
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

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
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

	if(!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Create the CDXSreen object
	Screen = new CDXScreen();

    // set the video mode 640x480x8
    if( FAILED(Screen->CreateFullScreen( hWnd , 640 , 480 , 8 ) ) )
        CDXError( Screen , "could not set video mode 640x480x8" );

	Screen->LoadPalette(Bitmap);

	// Load the tiles
    Tiles = new CDXTile();
    if( Tiles->Create(Screen, Bitmap, 64, 32, 0) == FALSE )
        CDXError( Screen , "could not load tiles" );

    // set the top left pixel in tiles bitmap as transparent color
	Tiles->SetColorKey();

	// Create and load the map
	Map = new CDXIsoMap(Tiles, Screen);
	Map->Create(400, 400, 1);

	srand((unsigned int)time(NULL));
	for(int row=0; row<400; row++) {
		for(int col=0; col<400; col++) {
			Map->SetTile(col, row, rand()%4+1);
		}
	}
	Map->MoveTo(0,0);

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

			if(GetAsyncKeyState(VK_UP)) Map->ScrollUp(1);
			if(GetAsyncKeyState(VK_DOWN)) Map->ScrollDown(1);
			if(GetAsyncKeyState(VK_LEFT)) Map->ScrollLeft(1);
			if(GetAsyncKeyState(VK_RIGHT)) Map->ScrollRight(1);

			Map->DrawTrans(Screen->GetBack());  // Draw the map to the back buffer
			Screen->Flip();  // Then display it
		}
		else WaitMessage();
	}
}
