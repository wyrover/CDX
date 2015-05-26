// ------------------------------------------------------------------------------------------
// CDX example 5 - Sprite lists, resources and timing
// You must link to cdd.lib, cdi.lib, ddraw.lib, dinput, dxguid and winmm.lib
// ------------------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <time.h>

#define CDXINCLUDEALL           // include all CDX headers
#include <CDX.h>

#define NAME		"CDXExample5"
#define TITLE		"CDX Example 5"

// The sprite types
enum
{
	SPR_BUG,
	SPR_MOUTH,
	SPR_CHOMPER,
	SPR_PLAYER,
};


CDXScreen   * Screen    = 0;    // The screen object, every program must have one
CDXInput    * Input     = 0;    // The input object

// The sprite bitmaps
CDXTile     * Bug       = 0;
CDXTile     * Mouth     = 0;
CDXTile     * Chomper   = 0;
CDXTile     * Player    = 0;

BOOL        bActive     = TRUE; // Is the program running?

// Frame rate counters
DWORD           LastTime        = 0;
DWORD           CurTime         = 0;
DWORD           FpsTime         = 0;
DWORD           DeltaTime       = 0;
DWORD           FramesRendered  = 0;
DWORD           Fps             = 0;

CDXSpriteList   Sprites;        // The sprite list object

// Resource bitmap files
char BUG[]      = "BUG.BMP";
char MOUTH[]    = "MOUTH.BMP";
char CHOMPER[]  = "CHOMPER.BMP";
char PLAYER[]   = "PLAYER.BMP";



// ------------------------------------------------------------------------------------------
// FiniApp - Destroy the CDX objects
// ------------------------------------------------------------------------------------------
void FiniApp(void)
{
	SAFEDELETE( Bug );
    SAFEDELETE( Mouth );
    SAFEDELETE( Chomper );
    SAFEDELETE( Player );
    SAFEDELETE( Input );
	SAFEDELETE( Screen );
}



// ------------------------------------------------------------------------------------------
// UpdateSprites, updates the sprites position 
// ------------------------------------------------------------------------------------------
void UpdateSprites(void)
{
	CDXSprite* Node;

	// Loop the list and update the sprites
	for(Node = Sprites.GetFirst(); Node != NULL; Node = Sprites.GetNext(Node))
	{
		Node->SetPosX(Node->GetPosX() + Node->GetVelX());
		Node->SetPosY(Node->GetPosY() + Node->GetVelY());

		if(Node->GetPosX() < 0)
		{
			Node->SetVelX(Node->GetVelX() - (2 * Node->GetVelX()));
            Node->SetPosX(1);
		}
		if(Node->GetPosX() > (640 - Node->GetTile()->GetBlockWidth()))
		{
			Node->SetVelX(Node->GetVelX() - (2 * Node->GetVelX()));
			Node->SetPosX(639 - Node->GetTile()->GetBlockWidth());
		}
		if(Node->GetPosY() < 0)
		{
			Node->SetVelY(Node->GetVelY() - (2 * Node->GetVelY()));
			Node->SetPosY(1);
		}
		if(Node->GetPosY() > (480 - Node->GetTile()->GetBlockHeight()))
		{
			Node->SetVelY(Node->GetVelY() - (2 * Node->GetVelY()));
			Node->SetPosY(479 - Node->GetTile()->GetBlockHeight());
		}
	}
}



// ------------------------------------------------------------------------------------------
// DoUpdate, checks for keyboard input and updates the sprites
// ------------------------------------------------------------------------------------------
void DoUpdate(void)
{
	// Update the input device states
	Input->Update();

    // if INSERT key was pressed, add a random sprite to the list
	if( Input->GetKeyState(CDXKEY_INS) )
	{
		int Type = rand()%3;
		int PosX = rand()%556;
		int PosY = rand()%400;
		int VelX = rand()%3;
		int VelY = rand()%3;

		switch(Type)
		{
			case SPR_BUG:
				{
					CDXSprite *s;
					s = new CDXSprite();
					s->Create(Bug);
					s->SetPosX(PosX);
					s->SetPosY(PosY);
					s->SetVelX(VelX);
					s->SetVelY(VelY);
					Sprites.AddSprite(s);
					//Bug,Type,PosX,PosY,VelX,VelY,0,0,0);
				}
			case SPR_MOUTH: //Sprites.AddSprite(Mouth,Type,PosX,PosY,-VelX,VelY,0,0,0);
				{
					CDXSprite *s;
					s = new CDXSprite();
					s->Create(Mouth);
					s->SetPosX(PosX);
					s->SetPosY(PosY);
					s->SetVelX(-VelX);
					s->SetVelY(VelY);
					Sprites.AddSprite(s);
				}
			case SPR_CHOMPER: //Sprites.AddSprite(Chomper,Type,PosX,PosY,VelX,-VelY,0,0,0);
				{
					CDXSprite *s;
					s = new CDXSprite();
					s->Create(Chomper);
					s->SetPosX(PosX);
					s->SetPosY(PosY);
					s->SetVelX(VelX);
					s->SetVelY(-VelY);
					Sprites.AddSprite(s);
				}
			case SPR_PLAYER: //Sprites.AddSprite(Player,Type,PosX,PosY,-VelX,-VelY,0,0,0);
				{
					CDXSprite *s;
					s = new CDXSprite();
					s->Create(Player);
					s->SetPosX(PosX);
					s->SetPosY(PosY);
					s->SetVelX(-VelX);
					s->SetVelY(-VelY);
					Sprites.AddSprite(s);
				}
		}
	}

    // if DEL key was pressed, delete a sprite from the list
	if( Input->GetKeyState(CDXKEY_DEL) ) 
		Sprites.DelSprite(Sprites.GetFirst());

	// Loop the sprite list and update the sprites
	UpdateSprites();

	// Clear the back buffer
	Screen->Fill(0);

	// Draw all the sprites in the list
	Sprites.Draw(Screen->GetBack() , 0 , 0 , CDXBLT_TRANS );

	// Frame rate calculations
	CurTime   = timeGetTime();
	DeltaTime = CurTime - LastTime;
	LastTime  = CurTime;
	FpsTime  += DeltaTime;

	FramesRendered++;

	if (FpsTime > 1000)
	{
		Fps = FramesRendered;
		FramesRendered  = 0;
		FpsTime = 0;
	}

	char str[12];
	sprintf(str, "FPS: %d", Fps);

	// Print the frame rate to the screen
	Screen->GetBack()->GetDC();
	Screen->GetBack()->SetFont();
	Screen->GetBack()->TextXY(5, 5, 255, str);
	Screen->GetBack()->TextXY(5, 20, 255, "INSERT - ADD SPRITE");
	Screen->GetBack()->TextXY(5, 35, 255, "DELETE - DEL SPRITE");

	sprintf(str, "COUNT: %d", Sprites.GetCount());
	Screen->GetBack()->TextXY(5, 50, 255, str);

	Screen->GetBack()->ReleaseDC();

	// Display the back buffer
	Screen->Flip();
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

    // set video mode 640x480x8
    if( FAILED(Screen->CreateFullScreen( hWnd , 640 , 480 , 8 ) ) )
        CDXError( Screen , "could not set video mode 640x480x8" );

    // load and set the palette for this videomode from the BUG bitmap file
    if( FAILED(Screen->LoadPalette(BUG) ) )
        CDXError( Screen , "Could not load palette from file BUG.BMP" );

   	// Create the input device
	Input = new CDXInput;
    if( Input == NULL )
        CDXError( Screen , "Could not create CDXInput object!" );

	if( FAILED(Input->Create(hInst, hWnd) ) )
        CDXError( Screen , "Could not create CDXInput object!" );

	// Load the bitmaps
    Bug = new CDXTile();
    if( Bug->Create( Screen , BUG , 84 , 80 , 0 ) == FALSE )
        CDXError( Screen , "could not load tiles from file BUG.BMP" );

    Mouth = new CDXTile();
    if( Mouth->Create( Screen , MOUTH , 39 , 36 , 0 ) == FALSE )
        CDXError( Screen , "could not load tiles from file MOUTH.BMP" );

    Chomper = new CDXTile();
    if( Chomper->Create( Screen , CHOMPER , 38 , 34 , 0 ) == FALSE )
        CDXError( Screen , "could not load tiles from file CHOMPER.BMP" );

    Player = new CDXTile();
    if( Player->Create( Screen , PLAYER , 38 , 36 , 0 ) == FALSE )
        CDXError( Screen , "could not load tiles from file PLAYER.BMP" );

	// Set the bitmaps transparent color
	Bug->SetColorKey(8);
	Mouth->SetColorKey(8);
	Chomper->SetColorKey(8);
	Player->SetColorKey(8);
	
	// Seed the random-number generator with current time
	srand((unsigned)time(NULL));

	// Change the default font
	Screen->GetBack()->ChangeFont("Comic Sans MS", 0, 18);

	return TRUE;
}



// ------------------------------------------------------------------------------------------
// WinMain
// ------------------------------------------------------------------------------------------
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG     msg;

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
			DoUpdate();
		}
		else WaitMessage();
	}
}
