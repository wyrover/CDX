/////////////////////////////////////////////////////////////////////////////////////////////
// graphics.cpp
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <CDX.h>
#include "breakout.h"

CDXScreen *Screen;
CDXSurface *Foreground, *Background;

extern OBJp Player;
extern OBJp Ball;
extern OBJECT Blocks;

BOOL InitGraphics(HWND hWnd)
{
	Screen = new CDXScreen();
	if(FAILED(Screen->CreateFullScreen(hWnd, 640, 480, 8)))
		return FALSE;
	Foreground = new CDXSurface;
	Foreground->Create(Screen, "FORE.BMP");
	Foreground->SetColorKey(255);

	Background = new CDXSurface;
	Background->Create(Screen, "BACK.BMP");
	Background->SetColorKey(255);
	
	return TRUE;
}

void FiniGraphics(void)
{
	delete Background;
	delete Foreground;
	delete Screen;
}

void UpdateGraphics(void)
{
	RECT clipRect;

	// Clear the background buffer
	Screen->GetBack()->Fill(0);
	
	// Draw the background screen
	Background->DrawBlk(Screen->GetBack(), 0, 0);
	
	// Draw the player
	clipRect.left = 0;
	clipRect.top = 0;
	clipRect.right = 60;
	clipRect.bottom = 15;

	Foreground->SetClipRect(&clipRect);
	Foreground->DrawTrans(Screen->GetBack(), Player->PosX, Player->PosY);

	// Draw the ball
	clipRect.left = 0;
	clipRect.top = 15;
	clipRect.right = 10;
	clipRect.bottom = 25;

	Foreground->SetClipRect(&clipRect);
	Foreground->DrawTrans(Screen->GetBack(), Ball->PosX, Ball->PosY);

	// Draw the blocks
	OBJp node;

	clipRect.left = 0;
	clipRect.top = 25;
	clipRect.right = 31;
	clipRect.bottom = 45;

	Foreground->SetClipRect(&clipRect);
	for(node = Blocks.Next; node != &Blocks; node = node->Next)
	{
		Foreground->DrawTrans(Screen->GetBack(), node->PosX, node->PosY);
	}

	Screen->Flip();
}