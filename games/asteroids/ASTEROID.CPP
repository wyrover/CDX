/////////////////////////////////////////////////////////////////////////////////////////////
// CDX Asteroids - a vector based asteroids clone
// You must link to cdx.lib, ddraw.lib, dinput.lib, dsound.lib and dxguid.lib
//
// Keys:
//
// Ctrl		= fire
// Shift	= shields
// Space	= hyperspace
//
// Things you can do to the source to make it better:
//
// -add a high score table
// -add sounds
// -add 2 player mode
// -add a menu system and the ability to enter your initials
// -add more levels, making it harder as the user progresses
// -add the alien ship and make it appear every 10,000 points or something
// -add the ability to switch between 'classic' vector graphics and sprite graphics
// -add a graphic to the back of the ship to show 'thrust'
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define CDXINCLUDEALL
#include <CDX.h>        // The CDX DirectDraw classes

#define SCREEN_WIDTH	640		// width of the screen in pixels
#define SCREEN_HEIGHT	480		// height of the screen in pixels
#define SCREEN_BPP		8		// screen color depth
#define ROTATE_STEP  3			// Number of degrees to rotate
#define SPEED        0.12		// Speed control
#define RETRO        0.06		// Retro rockets speed
#define FRICTION     0.995		// Speed reduction per frame

CDXScreen* Screen;		// The CDXScreen object, every program must have one
CDXInput* Input;		// The CDXInput object

BOOL bActive = FALSE;

int score;                           // Your score variable
int bonus_ship;                     // keeps track of bonus(extra ship)
int lives;                           // How many lives you have left
int ammo, frag=40;                  // Number of bullets - Number of Frags
int lrocks=5, mrocks=10, srocks=20; // mrocks=lrocks*2 - srocks=mrocks*2
int lvl, nor;                        // Level, Number of rocks to start with

typedef struct
{
	float x, y;        // float coordinates so we can store fractional values
} floatPOINT;

typedef struct
{
	int    NumPoints;    // Holds how many points an Object has
	floatPOINT  Point[ 15 ];  // Maximum Points Allowed For our Vector Objects
} SHAPE;

typedef struct
{
	floatPOINT  Loc;             // Location on the screen
	int    ViewAngle;       // Angle at which the object is facing
	int    FixedAngle;      // Fixed Direction which the object will move
	floatPOINT  Direction;       // Direction which the object is moving, in pixels
	SHAPE  Shape;           // The shape of the object
	SHAPE  Screen;          // Actual points it occupies on the screen
	int    id;              // Object's id(ie.. 0=Ship, 1=Asteroid, 2=Bullet..)
	int    tt;              // time tally
	int    tf;              // TRUE(1) or FALSE(0)
	int    BoxBoundry;      // Collision Detection Value
	int    Shieldup;        // Determines if shield is up or not
	int    Active;          // Determines if Object is active or not
	int    Amount;          // How many of one object? Specifically for Rocks..
} OBJECT;

struct
{
	OBJECT Ship;            // The players spaceship
	OBJECT WShip;           // The players Spaceship wrecked!
	OBJECT Ships;           // Holds Ship image for lives left..
	OBJECT Bullet[50];      // Players Bullets
} g;

struct
{
	OBJECT RockL[5];       // Large Asteroids
	OBJECT RockM[10];       // Medium Asteroids
	OBJECT RockS[20];       // Small Asteroids
} r;

struct
{
	OBJECT Frag[40];        // Fragments from explosion
} f;

// Prototypes
void InitGame(void);
void Play(void);

/////////////////////////////////////////////////////////////////////////////////////////////
// FiniApp - Destroy the CDX objects
/////////////////////////////////////////////////////////////////////////////////////////////
void FiniApp(void)
{
	SAFEDELETE(Input);
	SAFEDELETE(Screen);
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

		case WM_SETCURSOR:
			SetCursor(NULL);	// Hide the mouse cursor
		return TRUE;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					FiniApp();
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
	WndClass.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = NULL;
    WndClass.lpszMenuName = "Example 10";
	WndClass.lpszClassName = "Example 10";
	RegisterClass(&WndClass);

	hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		"Example 10",
        "Example 10",
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

	// Create the CDXSreen object and set the resoultion
	Screen = new CDXScreen();
	Screen->CreateFullScreen(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, 8);

	// Init Input device
	Input = new CDXInput();
	if (FAILED(Input->Create(hInst, hWnd)))
		return FALSE;

	// Initialize game objects, structures, etc.
	InitGame();

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
			Screen->GetBack()->Fill(0);
			Play();
			Screen->Flip();
		}
		else 
			WaitMessage();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Translates a point by rotating it (using simple trig).
/////////////////////////////////////////////////////////////////////////////////////////////
void TranslatePoint( floatPOINT *Dest, floatPOINT *Src, int Angle )
{
	double Radians;

	Radians = Angle * 0.01745;
	Dest->x = Src->x * cos( Radians ) - Src->y * sin( Radians );
	Dest->y = Src->x * sin( Radians ) + Src->y * cos( Radians );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Translates an object's relative coordinates to actual
// screen coordinates.
/////////////////////////////////////////////////////////////////////////////////////////////
void TranslateObject( OBJECT *Object )
{
	int i;

	for( i = 0; i < Object->Shape.NumPoints; i++ )
	{
		// First, account for rotation
		TranslatePoint( &Object->Screen.Point[ i ], &Object->Shape.Point[ i ], Object->ViewAngle );

		// Now make it relative to the screen location
		Object->Screen.Point[ i ].x += Object->Loc.x;
		Object->Screen.Point[ i ].y += Object->Loc.y;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Goes through the list of points in an object
// and connects them all with lines.
/////////////////////////////////////////////////////////////////////////////////////////////
void DrawObject( OBJECT *Object )
{
	Screen->GetBack()->Lock();
	if(Object->id == 2)	// bullet
	{
		Screen->GetBack()->Circle(Object->Loc.x, Object->Loc.y, 1, 255);
	}
	else
	{
		for(int i=1; i<Object->Shape.NumPoints; i++)
		{
			Screen->GetBack()->Line(Object->Screen.Point[i-1].x,
				Object->Screen.Point[i-1].y,
				Object->Screen.Point[i].x,
				Object->Screen.Point[i].y,
				255);
		}

		Screen->GetBack()->Line(Object->Screen.Point[i-1].x,
			Object->Screen.Point[i-1].y,
			Object->Screen.Point[0].x,
			Object->Screen.Point[0].y,
			255);
	}

	if((Object->Shieldup == 1) && (Object->id == 0))
	{
		Screen->GetBack()->Circle(Object->Loc.x, Object->Loc.y, Object->BoxBoundry+1, 255);
	}

	Screen->GetBack()->UnLock();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Moves an object along its direction.
/////////////////////////////////////////////////////////////////////////////////////////////
void MoveObject( OBJECT *Object )
{
	Object->Loc.x += Object->Direction.x;
	Object->Loc.y += Object->Direction.y;

	// Handle screen wrap-around
	if( Object->Loc.x < 0 )    Object->Loc.x = Object->Loc.x + SCREEN_WIDTH;
	if( Object->Loc.x > SCREEN_HEIGHT-1 )  Object->Loc.x = Object->Loc.x - SCREEN_WIDTH;
	if( Object->Loc.y < 0 )    Object->Loc.y = Object->Loc.y + SCREEN_HEIGHT;
	if( Object->Loc.y > SCREEN_WIDTH-1 )  Object->Loc.y = Object->Loc.y - SCREEN_HEIGHT;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Fragment Control
/////////////////////////////////////////////////////////////////////////////////////////////
void Fragment( int NUMBER, double FX, double FY, double SFX, double SFY)
{
	int i,zzz;
	double Radians;

	for(i=0;i<NUMBER;i++)
	{
		for(zzz=0;zzz<frag;zzz++)
		{
			if(f.Frag[zzz].Active==0)
			{
				f.Frag[zzz].Active=1;
				f.Frag[zzz].FixedAngle = (rand()%359);
				f.Frag[zzz].ViewAngle = (rand()%359);
				f.Frag[zzz].Loc.x = FX;
				f.Frag[zzz].Loc.y = FY;
				Radians = f.Frag[zzz].ViewAngle * 0.01745;
				f.Frag[zzz].Direction.x = SFX;
				f.Frag[zzz].Direction.y = SFY;
				f.Frag[zzz].Direction.x += (sin( Radians ) * (((rand()%150)/100)+.2));
				f.Frag[zzz].Direction.y -= (cos( Radians ) * (((rand()%150)/100)+.2));
				f.Frag[zzz].tt=20+(rand()%50);
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Collision Detection
/////////////////////////////////////////////////////////////////////////////////////////////
int CheckHit( OBJECT *Object1, OBJECT *Object2)
{
	if(((Object1->Loc.x+Object1->BoxBoundry)>(Object2->Loc.x-Object2->BoxBoundry))&&
		((Object1->Loc.x-Object1->BoxBoundry)<(Object2->Loc.x+Object2->BoxBoundry))&&
		((Object1->Loc.y+Object1->BoxBoundry)>(Object2->Loc.y-Object2->BoxBoundry))&&
		((Object1->Loc.y-Object1->BoxBoundry)<(Object2->Loc.y+Object2->BoxBoundry)))
	{
		return(TRUE);
	}
    return(FALSE);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Main game loop
/////////////////////////////////////////////////////////////////////////////////////////////
void Play( void )
{
	double Radians;
	int z, i, zz, zzz;

    // Simulate friction.
    if(g.Ship.Active==1)
    {
	    g.Ship.Direction.x *= (float)FRICTION;
		g.Ship.Direction.y *= (float)FRICTION;
    }

	// Check for input and move ship
	Input->Update();

	// If g.WShip.Active=1 then player has no control of ship
    if(g.WShip.Active==0)
    {
	    // If g.Ship.Shieldup=1 then player has no maneuvering and shooting abilities
		if(g.Ship.Shieldup==0)
		{
			// rotate ship left
			if(Input->GetKeyState(CDXKEY_LEFTARROW))
			{
				g.Ship.ViewAngle -= ROTATE_STEP;
				if( g.Ship.ViewAngle < 0 )  
					g.Ship.ViewAngle = 360 - ROTATE_STEP;
			}

			// rotate ship right
			if(Input->GetKeyState(CDXKEY_RIGHTARROW))
			{
				g.Ship.ViewAngle += ROTATE_STEP;
				if( g.Ship.ViewAngle > 359 )  
					g.Ship.ViewAngle = 0 + ROTATE_STEP;
			}

			// thrust (forward)
			if(Input->GetKeyState(CDXKEY_UPARROW))
			{
				Radians = g.Ship.ViewAngle * 0.01745;

				g.Ship.Direction.x += sin( Radians ) * SPEED;
				g.Ship.Direction.y -= cos( Radians ) * SPEED;
			}

			// Retro rockets (backward)
			if(Input->GetKeyState(CDXKEY_DOWNARROW))
			{
				Radians = g.Ship.ViewAngle * 0.01745;

				g.Ship.Direction.x -= sin( Radians ) * RETRO;
				g.Ship.Direction.y += cos( Radians ) * RETRO;
			}

			// hyperspace
			if(Input->GetKeyState(CDXKEY_SPACE))
			{
				g.Ship.Loc.x = rand()%SCREEN_WIDTH;
				g.Ship.Loc.y = rand()%SCREEN_HEIGHT;
				g.Ship.Direction.x = 0;
				g.Ship.Direction.y = 0;
			}

			// fire bullets
			if(Input->GetKeyState(CDXKEY_LEFTCTRL) || Input->GetKeyState(CDXKEY_RIGHTCTRL))
			{
				for(z=0; z<ammo; z++)
				{
					if (g.Bullet[z].tf==0)
					{
						g.Bullet[z].tf=1;
						g.Bullet[z].tt=55;
						g.Bullet[z].Loc.x=g.Ship.Loc.x;
						g.Bullet[z].Loc.y=g.Ship.Loc.y;
						g.Bullet[z].Direction.x = g.Ship.Direction.x;
						g.Bullet[z].Direction.y = g.Ship.Direction.y;
						g.Bullet[z].ViewAngle=g.Ship.ViewAngle;
						Radians = g.Bullet[z].ViewAngle * 0.01745;
						g.Bullet[z].Direction.x +=(sin(Radians)*3.5) ;
						g.Bullet[z].Direction.y -=(cos(Radians)*3.5) ;
						break;
					}
				}
			}
		}

		// shields
		if(Input->GetKeyState(CDXKEY_LEFTSHIFT) || Input->GetKeyState(CDXKEY_RIGHTSHIFT))
		{
			if(g.Ship.Shieldup == 0) {
				g.Ship.Shieldup=1;
			}
			else {
				g.Ship.Shieldup=0;
			}
		}
	}
    
	// Draw your ship
    if(g.Ship.Active==1)
    {
		TranslateObject( &g.Ship );
		DrawObject( &g.Ship );
		MoveObject( &g.Ship );
    }

    // Astroid's turn to animate...
    // Large Rocks
    for(z = 0 ; z<lrocks ; z++)
    {
     if (r.RockL[z].Active==1)
      {
      TranslateObject( &r.RockL[z] );
      DrawObject( &r.RockL[z] );
      MoveObject( &r.RockL[z] );
      Radians = r.RockL[z].FixedAngle * 0.01745;

      r.RockL[z].Direction.x = sin( Radians ) * 0.75;
      r.RockL[z].Direction.y = cos( Radians ) * 0.75;
      r.RockL[z].ViewAngle += 1;
      if( r.RockL[z].ViewAngle > 359 )  r.RockL[z].ViewAngle = 0 + 1;
         // check if there is a collision between ship and asteroid
         if ((g.Ship.Shieldup==0)&&(g.Ship.Active==1)&&(CheckHit( &g.Ship, &r.RockL[z])))
         {
          lives-=1;            // subtract 1 from lives
          r.RockL[z].Active=0; // deactivates the unlucky asteroid
          g.Ship.Active=0;     // do not draw ship anymore
          g.WShip.Active=1;    // set to draw wrecked ship
          g.WShip.tt=100;  // set how long the wrecked ship is to be animated
          g.WShip.Loc.x=g.Ship.Loc.x;  // setup WShip were your ship
          g.WShip.Loc.y=g.Ship.Loc.y;  // was last positioned
          g.WShip.Direction.x = g.Ship.Direction.x; // makes sure that WShip
          g.WShip.Direction.y = g.Ship.Direction.y; // moves the same as Ship
          g.WShip.ViewAngle = g.Ship.ViewAngle; // set WShip with the same ViewAngle as your ship
          // lets break the asteroid up into two smaller asteroids
          for(i=0;i<2;i++)
          {
           for(zzz=0;zzz<mrocks;zzz++)
           {
            if(r.RockM[zzz].Active==0)
             {
              r.RockM[zzz].Active=1;
              r.RockM[zzz].FixedAngle = (rand()%359);
              r.RockM[zzz].ViewAngle = (rand()%359);

              r.RockM[zzz].Loc.x = r.RockL[z].Loc.x;
              r.RockM[zzz].Loc.y = r.RockL[z].Loc.y;

              r.RockM[zzz].Direction.x = 0;
              r.RockM[zzz].Direction.y = 0;
			  break;
             }
           }
          }
         }
      }
    }

    // Medium Rocks
    for(z = 0 ; z<mrocks ; z++)
    {
     if (r.RockM[z].Active==1)
      {
      TranslateObject( &r.RockM[z] );
      DrawObject( &r.RockM[z] );
      MoveObject( &r.RockM[z] );
      Radians = r.RockM[z].FixedAngle * 0.01745;

      r.RockM[z].Direction.x = sin( Radians ) * 1.5;
      r.RockM[z].Direction.y = cos( Radians ) * 1.5;
      r.RockM[z].ViewAngle += 5;
      if( r.RockM[z].ViewAngle > 359 )  r.RockM[z].ViewAngle = 0 + 1;
         if ((g.Ship.Shieldup==0)&&(g.Ship.Active==1)&&(CheckHit( &g.Ship, &r.RockM[z])))
         {
          lives-=1;
          r.RockM[z].Active=0;
          g.Ship.Active=0;
          g.WShip.Active=1;
          g.WShip.tt=100;
          g.WShip.Loc.x=g.Ship.Loc.x;
          g.WShip.Loc.y=g.Ship.Loc.y;
          g.WShip.Direction.x = g.Ship.Direction.x;
          g.WShip.Direction.y = g.Ship.Direction.y;
          g.WShip.ViewAngle=g.Ship.ViewAngle;


          for(i=0;i<2;i++)
          {
           for(zzz=0;zzz<srocks;zzz++)
           {
            if(r.RockS[zzz].Active==0)
             {
              r.RockS[zzz].Active=1;
              r.RockS[zzz].FixedAngle = (rand()%359);
              r.RockS[zzz].ViewAngle = (rand()%359);

              r.RockS[zzz].Loc.x = r.RockM[z].Loc.x;
              r.RockS[zzz].Loc.y = r.RockM[z].Loc.y;

              r.RockS[zzz].Direction.x = 0;
              r.RockS[zzz].Direction.y = 0;
			  break;
             }
           }
          }
         }

      }
    }

    // Small Rocks
    for(z = 0 ; z<srocks ; z++)
    {
     if (r.RockS[z].Active==1)
      {
      TranslateObject( &r.RockS[z] );
      DrawObject( &r.RockS[z] );
      MoveObject( &r.RockS[z] );
      Radians = r.RockS[z].FixedAngle * 0.01745;

      r.RockS[z].Direction.x = sin( Radians ) * 2.9;
      r.RockS[z].Direction.y = cos( Radians ) * 2.9;
      r.RockS[z].ViewAngle += 9;
      if( r.RockS[z].ViewAngle > 359 )  r.RockS[z].ViewAngle = 0 + 1;
         if ((g.Ship.Shieldup==0)&&(g.Ship.Active==1)&&(CheckHit( &g.Ship, &r.RockS[z])))
         {
          lives-=1;
          r.RockS[z].Active=0;
          g.Ship.Active=0;
          g.WShip.Active=1;
          g.WShip.tt=100;
          g.WShip.Loc.x=g.Ship.Loc.x;
          g.WShip.Loc.y=g.Ship.Loc.y;
          g.WShip.Direction.x = g.Ship.Direction.x;
          g.WShip.Direction.y = g.Ship.Direction.y;
          g.WShip.ViewAngle=g.Ship.ViewAngle;
          // For the smallest asteroid, lets animate temporary fragments
          Fragment( 5 , r.RockS[z].Loc.x , r.RockS[z].Loc.y,
                        r.RockS[z].Direction.x , r.RockS[z].Direction.y);
         }

      }
    }

    // Move bullets
    for(z=0; z<ammo ; z++)
    {
     if(g.Bullet[z].tf==1)
     {
      if(g.Bullet[z].tt>0)
      {
        g.Bullet[z].tt-=1;
        DrawObject( &g.Bullet[z] );
        MoveObject( &g.Bullet[z] );
        for(zz = 0 ; zz<lrocks ; zz++)
        {
         // lets see if there is a collision with an asteroid
         if ((r.RockL[zz].Active==1)&&(CheckHit( &g.Bullet[z], &r.RockL[zz])))
         {
          r.RockL[zz].Active=0; // Deactivates the unlucky asteroid
          score+=50;            // Add points to your score.
          bonus_ship+=50;      // Add points yo your bonus for extra ship

          // lets break up the asteroid into two smaller asteroids
          for(i=0;i<2;i++)
          {
           for(zzz=0;zzz<mrocks;zzz++)
           {
            if(r.RockM[zzz].Active==0)
             {
              r.RockM[zzz].Active=1;
              r.RockM[zzz].FixedAngle = (rand()%359);
              r.RockM[zzz].ViewAngle = (rand()%359);

              r.RockM[zzz].Loc.x = r.RockL[zz].Loc.x;
              r.RockM[zzz].Loc.y = r.RockL[zz].Loc.y;

              r.RockM[zzz].Direction.x = 0;
              r.RockM[zzz].Direction.y = 0;
              g.Bullet[z].tt=0;
			  break;
             }
           }
          }
         }
        }
        for(zz = 0 ; zz<mrocks ; zz++)
        {
         if ((g.Bullet[z].tt>0)&&(r.RockM[zz].Active==1)&&(CheckHit( &g.Bullet[z], &r.RockM[zz])))
         {
          r.RockM[zz].Active=0;
          score+=75;
          bonus_ship+=75;
          for(i=0;i<2;i++)
          {
           for(zzz=0;zzz<srocks;zzz++)
           {
            if(r.RockS[zzz].Active==0)
             {
              r.RockS[zzz].Active=1;
              r.RockS[zzz].FixedAngle = (rand()%359);
              r.RockS[zzz].ViewAngle = (rand()%359);

              r.RockS[zzz].Loc.x = r.RockM[zz].Loc.x;
              r.RockS[zzz].Loc.y = r.RockM[zz].Loc.y;

              r.RockS[zzz].Direction.x = 0;
              r.RockS[zzz].Direction.y = 0;
              g.Bullet[z].tt=0;
			  break;
             }
           }
          }
         }
        }
        for(zz = 0 ; zz<srocks ; zz++)
        {
         if ((g.Bullet[z].tt>0)&&(r.RockS[zz].Active==1)&&(CheckHit( &g.Bullet[z], &r.RockS[zz])))
            {
          r.RockS[zz].Active=0;
          score+=100;
          bonus_ship+=100;
          Fragment( 5 , r.RockS[zz].Loc.x , r.RockS[zz].Loc.y,
                        r.RockS[zz].Direction.x , r.RockS[zz].Direction.y);
          g.Bullet[z].tt=0;
         }
        }
        if (g.Bullet[z].tt<1)
        {
         g.Bullet[z].tt=0;
         g.Bullet[z].tf=0;
        }
      }
     }
    }

    // Move Fragments
    for(z=0; z<frag ; z++)
    {
     if(f.Frag[z].Active==1)
     {
      if(f.Frag[z].tt>0)
      {
        f.Frag[z].tt-=1;
        f.Frag[z].ViewAngle += 7;
        if( f.Frag[z].ViewAngle > 359 )  f.Frag[z].ViewAngle = 0 + 1;
        TranslateObject( &f.Frag[z] );
        DrawObject( &f.Frag[z] );
        MoveObject( &f.Frag[z] );
        if (f.Frag[z].tt<1)
        {
         f.Frag[z].tt=0;
         f.Frag[z].Active=0;
        }
      }
     }
    }

    // Move Wrecked Ship...
    if(g.WShip.Active==1)
    {
     // Make a big looking explosion.. Just need to call Fragment() once..
     if(g.WShip.tf==0)
     {
          Fragment( 15 , g.WShip.Loc.x , g.WShip.Loc.y,
                         g.WShip.Direction.x , g.WShip.Direction.y);
          g.WShip.tf=1;
     }

     // Turn the wrecked ship
     g.WShip.tt-=1;
     g.WShip.ViewAngle += 15;
     if( g.WShip.ViewAngle > 359 )  g.WShip.ViewAngle -= 360;
     if( g.WShip.tt>25)
     {
        TranslateObject( &g.WShip );
        DrawObject( &g.WShip );
        MoveObject( &g.WShip );
     }

     // Turn the remaining lives just for the fun of it...
     g.Ships.ViewAngle += 15;
     if( g.Ships.ViewAngle > 359 )  g.Ships.ViewAngle -= 360;

     // Is it time to give you another ship or are you out of lives?
     if(g.WShip.tt<1)
     {
      g.WShip.tt = 0;
      g.WShip.Active = 0;
      g.WShip.tf = 0;
      g.Ship.Active = 1;
      g.Ship.ViewAngle = 0;
      g.Ship.Loc.x = 320;
      g.Ship.Loc.y = 240;
      g.Ship.Direction.x = 0;
      g.Ship.Direction.y = 0;
      g.Ships.ViewAngle = 0;
     }
    }

    // Draw Remaining Ships
    if (lives>0)
    {
     for(i=0;i<lives;i++)
     {
         g.Ships.Loc.x=(100+(i*16));
         g.Ships.Loc.y=50;
         TranslateObject( &g.Ships );
         DrawObject( &g.Ships );
         MoveObject( &g.Ships );

     }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Game Initialization code.
/////////////////////////////////////////////////////////////////////////////////////////////
void InitGame( void )
{
	int i,z;

	// initialize random seed
	srand((unsigned)time(NULL));
	
	// intialize game characteristics
	score=0;		// reset score
	bonus_ship=0;	// reset bonus_ship
	lives=5;		// lives you start with
	lvl=1;			// level you start on
	nor=1;			// number of rocks(+1)(ex:nor=1 then you see two rocks)
	ammo=5;			// round of bullets in one burst...

	// player's ship...
	g.Ship.BoxBoundry = 8;
	g.Ship.id = 0;
	g.Ship.ViewAngle = 0;
	g.Ship.Loc.x = 320;
	g.Ship.Loc.y = 240;
	g.Ship.Direction.x = 0;
	g.Ship.Direction.y = 0;
	g.Ship.Active = 1;
	g.Ship.Shape.NumPoints = 3;

	g.Ship.Shape.Point[ 0 ].x =   0;
	g.Ship.Shape.Point[ 0 ].y =   -8;
	g.Ship.Shape.Point[ 1 ].x =   -4;
	g.Ship.Shape.Point[ 1 ].y =   4;
	g.Ship.Shape.Point[ 2 ].x =   4;
	g.Ship.Shape.Point[ 2 ].y =   4;

	// Image for ships remaining
	g.Ships.Shape.NumPoints = g.Ship.Shape.NumPoints;
	for (i=0;i<g.Ships.Shape.NumPoints;i++)
	{
		g.Ships.Shape.Point[ i ].x =g.Ship.Shape.Point[ i ].x;
		g.Ships.Shape.Point[ i ].y =g.Ship.Shape.Point[ i ].y;
	}
	g.Ships.id = 5;
	g.Ships.ViewAngle = 0;

	// player's ship wrecked...
	g.WShip.BoxBoundry = 6;
	g.WShip.id = 4;
	g.WShip.Active = 0;
	g.WShip.Shape.NumPoints = 8;

	g.WShip.Shape.Point[ 0 ].x =   0;
	g.WShip.Shape.Point[ 0 ].y =  -4;
	g.WShip.Shape.Point[ 1 ].x =  -2;
	g.WShip.Shape.Point[ 1 ].y =   0;
	g.WShip.Shape.Point[ 2 ].x =  -4;
	g.WShip.Shape.Point[ 2 ].y =   0;
	g.WShip.Shape.Point[ 3 ].x =  -3;
	g.WShip.Shape.Point[ 3 ].y =   6;
	g.WShip.Shape.Point[ 4 ].x =  -1;
	g.WShip.Shape.Point[ 4 ].y =   3;
	g.WShip.Shape.Point[ 5 ].x =   1;
	g.WShip.Shape.Point[ 5 ].y =   3;
	g.WShip.Shape.Point[ 6 ].x =   4;
	g.WShip.Shape.Point[ 6 ].y =   0;
	g.WShip.Shape.Point[ 7 ].x =   2;
	g.WShip.Shape.Point[ 7 ].y =   0;
	g.WShip.Shape.Point[ 8 ].x =   1;
	g.WShip.Shape.Point[ 8 ].y =  -4;

	// Asteroids!!
	// Large Rock...
	for(z=0;z<lrocks;z++)
	{
		r.RockL[z].BoxBoundry = 34;
		r.RockL[z].Active = 0;
		r.RockL[z].id = 1;
		r.RockL[z].FixedAngle = (rand()%359);
		r.RockL[z].ViewAngle = (rand()%359);
		r.RockL[z].Loc.x = (rand()%(SCREEN_HEIGHT-1));
		r.RockL[z].Loc.y = (rand()%(SCREEN_WIDTH-1));
		r.RockL[z].Direction.x = 0;
		r.RockL[z].Direction.y = 0;
		r.RockL[z].Amount = lrocks;
		r.RockL[z].Shape.NumPoints = 10;

		r.RockL[z].Shape.Point[ 0 ].x =  0;
		r.RockL[z].Shape.Point[ 0 ].y =  20;
		r.RockL[z].Shape.Point[ 1 ].x =  20;
		r.RockL[z].Shape.Point[ 1 ].y =  0;
		r.RockL[z].Shape.Point[ 2 ].x =  20;
		r.RockL[z].Shape.Point[ 2 ].y =   -10;
		r.RockL[z].Shape.Point[ 3 ].x =  0;
		r.RockL[z].Shape.Point[ 3 ].y =   -30;
		r.RockL[z].Shape.Point[ 4 ].x =  -10;
		r.RockL[z].Shape.Point[ 4 ].y =   -30;
		r.RockL[z].Shape.Point[ 5 ].x =   0;
		r.RockL[z].Shape.Point[ 5 ].y =   0;
		r.RockL[z].Shape.Point[ 6 ].x =   -20;
		r.RockL[z].Shape.Point[ 6 ].y =   -30;
		r.RockL[z].Shape.Point[ 7 ].x =   -30;
		r.RockL[z].Shape.Point[ 7 ].y =   -10;
		r.RockL[z].Shape.Point[ 8 ].x =   -10;
		r.RockL[z].Shape.Point[ 8 ].y =  0;
		r.RockL[z].Shape.Point[ 9 ].x =   -30;
		r.RockL[z].Shape.Point[ 9 ].y =  0;
		r.RockL[z].Shape.Point[ 10].x =   -20;
		r.RockL[z].Shape.Point[ 10].y =  20;

		if(z<(lvl+nor))
		{
			r.RockL[z].Active=1;
		}
	}

	// Medium Rock...
	for(z=0;z<mrocks;z++)
	{
		r.RockM[z].BoxBoundry = 13;
		r.RockM[z].Active = 0;
		r.RockM[z].id = 1;
		r.RockM[z].Amount = mrocks;
		r.RockM[z].Shape.NumPoints = 11;

		r.RockM[z].Shape.Point[ 0 ].x =  -4;
		r.RockM[z].Shape.Point[ 0 ].y =  -14;
		r.RockM[z].Shape.Point[ 1 ].x =  -14;
		r.RockM[z].Shape.Point[ 1 ].y =  -10;
		r.RockM[z].Shape.Point[ 2 ].x =  -10;
		r.RockM[z].Shape.Point[ 2 ].y =   4;
		r.RockM[z].Shape.Point[ 3 ].x =  -14;
		r.RockM[z].Shape.Point[ 3 ].y =   10;
		r.RockM[z].Shape.Point[ 4 ].x =  -2;
		r.RockM[z].Shape.Point[ 4 ].y =   14;
		r.RockM[z].Shape.Point[ 5 ].x =   4;
		r.RockM[z].Shape.Point[ 5 ].y =   12;
		r.RockM[z].Shape.Point[ 6 ].x =   12;
		r.RockM[z].Shape.Point[ 6 ].y =   14;
		r.RockM[z].Shape.Point[ 7 ].x =   14;
		r.RockM[z].Shape.Point[ 7 ].y =   6;
		r.RockM[z].Shape.Point[ 8 ].x =   10;
		r.RockM[z].Shape.Point[ 8 ].y =  -2;
		r.RockM[z].Shape.Point[ 9 ].x =   14;
		r.RockM[z].Shape.Point[ 9 ].y =  -6;
		r.RockM[z].Shape.Point[ 10].x =   10;
		r.RockM[z].Shape.Point[ 10].y =  -14;
		r.RockM[z].Shape.Point[ 11].x =   2;
		r.RockM[z].Shape.Point[ 11].y =  -10;
	}

	// Small Rock...
	for(z=0;z<srocks;z++)
	{
		r.RockS[z].BoxBoundry = 6;
		r.RockS[z].Active = 0;
		r.RockS[z].id = 1;
		r.RockS[z].Amount = srocks;
		r.RockS[z].Shape.NumPoints = 11;

		r.RockS[z].Shape.Point[ 0 ].x =  -2;
		r.RockS[z].Shape.Point[ 0 ].y =  -7;
		r.RockS[z].Shape.Point[ 1 ].x =  -7;
		r.RockS[z].Shape.Point[ 1 ].y =  -5;
		r.RockS[z].Shape.Point[ 2 ].x =  -5;
		r.RockS[z].Shape.Point[ 2 ].y =   2;
		r.RockS[z].Shape.Point[ 3 ].x =  -7;
		r.RockS[z].Shape.Point[ 3 ].y =   5;
		r.RockS[z].Shape.Point[ 4 ].x =  -1;
		r.RockS[z].Shape.Point[ 4 ].y =   7;
		r.RockS[z].Shape.Point[ 5 ].x =   2;
		r.RockS[z].Shape.Point[ 5 ].y =   6;
		r.RockS[z].Shape.Point[ 6 ].x =   6;
		r.RockS[z].Shape.Point[ 6 ].y =   7;
		r.RockS[z].Shape.Point[ 7 ].x =   7;
		r.RockS[z].Shape.Point[ 7 ].y =   3;
		r.RockS[z].Shape.Point[ 8 ].x =   5;
		r.RockS[z].Shape.Point[ 8 ].y =  -1;
		r.RockS[z].Shape.Point[ 9 ].x =   7;
		r.RockS[z].Shape.Point[ 9 ].y =  -3;
		r.RockS[z].Shape.Point[ 10].x =   5;
		r.RockS[z].Shape.Point[ 10].y =  -7;
		r.RockS[z].Shape.Point[ 11].x =   1;
		r.RockS[z].Shape.Point[ 11].y =  -5;
	}

	// Fragment...
	for(z=0;z<frag;z++)
	{
		f.Frag[z].BoxBoundry = 3;
		f.Frag[z].Active=0;
		f.Frag[z].id = 3;
		f.Frag[z].tf = 0;
		f.Frag[z].tt = 0;
		f.Frag[z].Shape.NumPoints = 6;

		f.Frag[z].Shape.Point[ 0 ].x =  -1;
		f.Frag[z].Shape.Point[ 0 ].y =  -1;
		f.Frag[z].Shape.Point[ 1 ].x =  0;
		f.Frag[z].Shape.Point[ 1 ].y =  0;
		f.Frag[z].Shape.Point[ 2 ].x =  -1;
		f.Frag[z].Shape.Point[ 2 ].y =  2;
		f.Frag[z].Shape.Point[ 3 ].x =  1;
		f.Frag[z].Shape.Point[ 3 ].y =  3;
		f.Frag[z].Shape.Point[ 4 ].x =  2;
		f.Frag[z].Shape.Point[ 4 ].y =  1;
		f.Frag[z].Shape.Point[ 5 ].x =  1;
		f.Frag[z].Shape.Point[ 5 ].y =  0;
		f.Frag[z].Shape.Point[ 6 ].x =  1;
		f.Frag[z].Shape.Point[ 6 ].y =  -2;
	}

	// bullet
	for(z=0; z<ammo; z++)
	{
		g.Bullet[z].id=2;
		g.Bullet[z].tf=0;
		g.Bullet[z].tt=0;
	}
}
