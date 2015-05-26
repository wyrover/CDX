/**************************************************************************
 Project Name  : SPACEY VADERS
 File Name     : Main.h
 Programmer(s) : Paul J Lord
/**************************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <winbase.h>    // Need for the ini file read/write
#include <stdio.h>
#include <time.h>
#include <math.h>

#define CDXINCLUDEALL
#include <CDX.h>

//
// Screen Size (640x480x8)
//
#define SCREENSIZEX 640
#define SCREENSIZEY 480
#define MAXMOVEX    580  //590
#define MAXMOVEY    380  //380 /450
#define MINMOVEX    10   //2

#define ALIVE       1
#define NOTALIVE    0
#define EMPTY      -1


/**************************************************************************
 States     
/**************************************************************************/
enum
{
	PS_SPLASH,
	PS_ACTIVE,
	PS_NEXTLIFE,
	PS_NEXTLEVEL,
	PS_GAMEOVER,
};

//
// Object Types (NOTE: Because I have all sprites in one BMP will use the 
//                     starting frame of each object for the type.
#define TYPEVADER1   0
#define TYPEVADER2   4
#define TYPEVADER3   8
#define TYPEBUG1     12
#define TYPEBUG2     16
#define TYPEBUG3     20
#define TYPEBUG4     36
#define TYPEUFO      40
#define TYPEEXPLODE0 25
#define TYPEEXPLODE1 24
#define TYPEEXPLODE2 29
#define TYPEMISSILE1 44
#define TYPEMISSILE2 46
#define TYPEBLANK    28

//
// Resource files
//
#define VADERSBMP   "vaders.bmp"
#define TITLEBMP    "title642.bmp"
#define PLAYERBMP   "player.bmp"
#define SPACEBMP    "space.BMP"

#define MISSILEWAV  "missile.wav"
#define XPLOSWAV    "xploshor.wav"
#define VMISSILEWAV "sweepdow.wav"

#define MUSIC1      "mv.mid"
#define MUSIC2      "2001.mid"


/**************************************************************************
 Structures     
/**************************************************************************/

// Use this to set the level differences
struct LevelSettings 
{
	UINT NumberOfRows;
	UINT VadersPerRow;
	UINT VaderSpeed;                // Speed to move
	UINT VaderDelay;                // Delay in Sprite Frames
	UINT UfoSpeed;                  // Speed to move
	UINT UfoDelay;                  // Delay in Sprite Frames
	UINT UfoFreq;
	UINT MissileSpeed;              // Speed to Move
	UINT MissileDelay;              // Delay in Sprite frames
	UINT VMissileDelay;
	UINT VMissileFreq;
 
      
};

/**************************************************************************
 Global Variables     
/**************************************************************************/
// If its not the Main.cpp then make all the Vars EXTERN...
#ifdef MAIN_CPP
	#define DECLARE
#else
	#define DECLARE extern
#endif

// Level Array
DECLARE LevelSettings LevelInfo[4];

// Playing Flags
DECLARE UINT PlayerDelay;           // Delay between Sprite Frames
DECLARE UINT PlayerLives;
DECLARE UINT PlayerMissiles;
DECLARE UINT StarDelay;             // Delay between Space movement
DECLARE UINT MusicFlag;
DECLARE UINT SoundFlag;
DECLARE UINT DebugFlag;
DECLARE UINT FPSFlag;
DECLARE UINT GodModeFlag;

DECLARE UINT Level;

// Windows Handle
DECLARE HWND hWnd;

// Screen Object
DECLARE CDXScreen* Screen;

// Map Objects
DECLARE CDXMap *SpaceMap1;
DECLARE CDXTile *SpaceTile1;

// Input Object
DECLARE CDXInput Input;

// Sprite Images (single objects)
DECLARE CDXSprite* Player;
DECLARE CDXSprite* Missile;

// Sprite Images for LIST
DECLARE CDXTile* VadersTile;
DECLARE CDXSprite* Vaders;

DECLARE CDXSpriteList Sprites; // The sprite list object

DECLARE CDXSprite* UFO;
DECLARE CDXSprite* Explode;

DECLARE CDXMusic* Music;
DECLARE CDXSound* Sound;
DECLARE CDXSoundBuffer MISSILE;
DECLARE CDXSoundBuffer XPLODE;
DECLARE CDXSoundBuffer VMISSILE;

DECLARE int ProgramState;
DECLARE BOOL bActive;


// Movement vars
DECLARE DWORD vaderdirection;
DECLARE UINT hitbottom;
DECLARE UINT vadermindelay;
DECLARE DWORD vaderspeedadjust;
DECLARE int vaderindex;
DECLARE int vaderspeed;             // global to check for number left


// Score vars
DECLARE DWORD TotalScore, AddToScore, HighScore;

DECLARE BOOL Ufo_Active, VMissile_Active;
DECLARE UINT Missile_Active;

// Color vars
DECLARE UINT YellowTxt, RedTxt, GreenTxt, BlueTxt; 

// Debug vars
DECLARE DWORD Fps;
DECLARE _int64 freqPerformance, firstTick, freqAdjust;
DECLARE FILE *fdebug;   // Used for Debugging...


/**************************************************************************
 Function Declarations     
/**************************************************************************/

// Main.cpp
void FiniApp(void);
BOOL InitApp(HINSTANCE hInst, int nCmdShow);
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow);

// InitGame.cpp
void InitGame(void);
void ResetLevel(void);
void ResetGame(void);
void LoadINIVars(void);
void SaveINIVars(void);

// States.cpp
void UpdateGame(void);
void SplashScreen(void);
void Update();
void NextLevel(void);
void NextLife(void);
void GameOver(void);
void CheckTimeEvents(void);

// GameIO.cpp
void GetInput(void);
void EraseBackBuffer(void);
void DrawObjects(void);
void dprintf(FILE *filedebug, char *fmt, ... );
void GetFPS(void);
void DisplayFPS(void);
void DisplayScore(void);
void DisplayHighScore(void);
void DisplaySpaceyMsg(void);
void DisplayMessage(int x, int y, char *Msg);
void DisplayLives(void);
void GetTick(void);
void DisplaySplashScreen(void);

// Sprites.cpp
void SetupSprites(void);
void MoveVaderObjects(void);
void MoveUfoObjects(void);
void MoveMissileObjects(void);
BOOL CheckMissileHit(int xmissile, int ymissile);
BOOL CheckPlayerHit(int xmissile, int ymissile);
float SpriteDistance(int x1, int y1, int x2, int y2);
