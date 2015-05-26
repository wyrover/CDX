#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL
#include "CDX.h"
#include "cdxstar.h"

#ifdef MAIN_CPP
	#define DECLARE
#else
	#define DECLARE extern
#endif

DECLARE CDXScreen* Engine;
DECLARE CDXInput Input;
DECLARE CDXSprite* Player1;
DECLARE CDXSprite* Player2;
DECLARE CDXStarField* Stars;
DECLARE CDXSprite* Ball;

DECLARE CDXSound* SoundSystem;
DECLARE CDXSoundBuffer* Bounce;
DECLARE CDXSoundBuffer* Point;
DECLARE CDXSoundBuffer* Winner;

DECLARE int Player1Score;
DECLARE int Player2Score;

DECLARE DWORD GameState;

DECLARE BOOL Player2Mode;

DECLARE HWND hWnd;
DECLARE BOOL bActive;

void InitGame();
void UpdateGame();

DECLARE enum PlayerState{MOVING_RIGHT, MOVING_LEFT, STANDING};
DECLARE enum GAMESTATE{PLAYING, STARTING, PAUSED, MENU, WINNER};

#endif
	

