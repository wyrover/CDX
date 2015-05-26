/////////////////////////////////////////////////////////////////////////////////////////////
// GAME.H
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL
#include <CDX.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////

// Program states
enum
{
	PS_MENU,
	PS_ACTIVE,
};

// Sprite types
enum
{
	SPR_MOUTH,
	SPR_CHOMP,
	SPR_BUG,
	SPR_SPYDER1,
	SPR_SPYDER2,
	SPR_NIPPER,
	SRP_CRAB,
	SPR_BLASTER,
	SPR_WAVE,
	SPR_ZAPP,
	SPR_LAZER,
	POW_BLASTER,
	POW_WAVE,
	POW_ZAPP,
	POW_LAZER,
	POW_BOOST,
	POW_HEALTH,
};

/////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MAIN_CPP
	#define DECLARE
#else
	#define DECLARE extern
#endif

DECLARE CDXScreen* Screen;
DECLARE CDXInput Input;

DECLARE CDXSurface* WorkSurface;
// Tile objects
DECLARE CDXTile* Tiles;

// Map objects
DECLARE CDXMap* Map;

// Sprite objects
DECLARE CDXSprite* Player;
DECLARE CDXTile* Blaster;
DECLARE CDXTile* Mouth;
DECLARE CDXTile* Chomp;
DECLARE CDXTile* Bug;
DECLARE CDXTile* Spyder1;
DECLARE CDXTile* Spyder2;
DECLARE CDXTile* Nipper;
DECLARE CDXTile* Crab;

// The sprite list
DECLARE CDXSpriteList Sprites;

// Sound objects
DECLARE CDXMusic* Music;
DECLARE CDXSound* Sound;
DECLARE CDXSoundBuffer* SND_BLASTER;

DECLARE	HWND hWnd;
DECLARE RECT Window;

DECLARE int ProgramState;
DECLARE BOOL bActive;

/////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION DECLARATIONS
/////////////////////////////////////////////////////////////////////////////////////////////

// ACTIVE.CPP
void Update(void);
void UpdateObjects(void);
void UpdateInput(void);

// GAME.CPP
void InitGame(void);
void UpdateGame(void);

#endif
