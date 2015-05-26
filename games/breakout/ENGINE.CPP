/////////////////////////////////////////////////////////////////////////////////////////////
// engine.cpp
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>

#define CDXINCLUDEALL
#include <CDX.h>
#include "breakout.h"

extern void UpdateInput(void);
extern void UpdateGraphics(void);
extern CDXInput Input;
extern CDXSoundBuffer *Sounds[SOUND_MAX];

OBJECT Blocks;
OBJp Player;
OBJp Ball;

int nBlocks;
int ProgramState;

int Level[20][3] =
{
        { 0, 100, 100 },
        { 0, 140, 100 },
        { 0, 180, 100 },
        { 0, 220, 100 },
        { 0, 260, 100 },
        { 0, 300, 100 },
        { 0, 340, 100 },
        { 0, 380, 100 },
        { 0, 420, 100 },
        { 0, 460, 100 },
        { 0, 100, 125 },
        { 0, 140, 125 },
        { 0, 180, 125 },
        { 0, 220, 125 },
        { 0, 260, 125 },
        { 0, 300, 125 },
        { 0, 340, 125 },
        { 0, 380, 125 },
        { 0, 420, 125 },
        { 0, 460, 125 },
};

void AddBlock(int type, int x, int y)
{
	OBJp node;

	node = new OBJECT;
	if(!node)
		return;

	node->Width = 31;
	node->Height = 20;
	node->Type = type;
	node->PosX = x;
	node->PosY = y;

	node->Next = Blocks.Next;
	node->Prev = &Blocks;
	Blocks.Next->Prev = node;
	Blocks.Next = node;

	nBlocks++;
}

void DelBlock(OBJp node)
{
	nBlocks--;
	node->Next->Prev = node->Prev;
	node->Prev->Next = node->Next;
	delete node;
}

void InitLevel()
{
	nBlocks = 0;

	Blocks.Next = Blocks.Prev = &Blocks;
	Blocks.PosX = Blocks.PosY = 0;

	for(int i=0; i<20; i++)
	{
		AddBlock(Level[i][0], Level[i][1], Level[i][2]);
	}

	Player->PosX = 300;
	Player->PosY = 450;

	Ball->PosX = 310;
	Ball->PosY = 60;
	Ball->VelX = 4;
	Ball->VelY = 4;
}

void InitEngine(void)
{
	ProgramState = PS_ACTIVE;

	Player = new OBJECT;
	Player->Width = 60;
	Player->Height = 15;

	Ball = new OBJECT;
	Ball->Width = 10;
	Ball->Height = 10;

	InitLevel();
}

void HitPlayer(OBJp Obj1, OBJp Obj2)
{
	RECT Dest;
	RECT Src1 = { Obj1->PosX, Obj1->PosY, Obj1->PosX + Obj1->Width, Obj1->PosY + Obj1->Height };
	RECT Src2 = { Obj2->PosX, Obj2->PosY, Obj2->PosX + Obj2->Width, Obj2->PosY + Obj2->Height };

	if(!IntersectRect(&Dest, &Src1, &Src2))
		return;

	Sounds[SOUND_BALLHIT]->Play();

	if((Dest.right - Dest.left) > (Dest.bottom - Dest.top))
	{
		Ball->VelY = -Ball->VelY;
		
		if(Src2.left < Src1.left)
		{
			Ball->VelX -= 4;
		}
		if(Src2.left < Src1.left + 10)
		{
			Ball->VelX -= 2;
		}
		else if(Src2.right > Src1.right)
		{
			Ball->VelX += 4;
		}
		else if(Src2.right > Src1.right - 10)
		{
			Ball->VelX += 2;
		}
	}
	else 
		Ball->VelX = -Ball->VelX;
}

HITS HitBlock(OBJp Obj1, OBJp Obj2)
{
	RECT dest;

	RECT src1 = { Obj1->PosX, Obj1->PosY, Obj1->PosX+Obj1->Width, Obj1->PosY+Obj1->Height };
	RECT src2 = { Obj2->PosX, Obj2->PosY, Obj2->PosX+Obj2->Width, Obj2->PosY+Obj2->Height };

	if(!IntersectRect(&dest, &src1, &src2))
		return NO_HIT;

	if(dest.right - dest.left > dest.bottom - dest.top)
		return HIT_Y;
	else
		return HIT_X;
}

void UpdateFrame(void)
{
	HITS rval;
	OBJp node;
	OBJp save;
	BOOL BlockHit;
	long MouseX, MouseY;

	switch(ProgramState)
	{
	case PS_EDITOR:
		break;

	case PS_SPLASH:
		break;

	case PS_WON:
		InitLevel();
		break;

	case PS_DEAD:
		break;

	case PS_ACTIVE:
		
		// check for input and adjust position
		UpdateInput();
		
		Input.GetMouseDeltas(&MouseX, &MouseY);

		if(Input.GetKeyState(CDXKEY_LEFTARROW))
			Player->PosX -= 8;
		if(Input.GetKeyState(CDXKEY_RIGHTARROW))
			Player->PosX += 8;
		if(Input.HasMouse()) Player->PosX += MouseX;
		if(Player->PosX < 10) Player->PosX = 10;
		if(Player->PosX > 570) Player->PosX = 570;

		// player/ball collision detection
		HitPlayer(Player, Ball);

		node = Blocks.Next;
		do {
			BlockHit = FALSE;

			rval = HitBlock(node, Ball);

			if(rval == HIT_Y) {
				Ball->VelY = -Ball->VelY;
				save = node;
				BlockHit = TRUE;
			}
			else if(rval == HIT_X) {
				Ball->VelX = -Ball->VelX;
				save = node;
				BlockHit = TRUE;
			}

			node = node->Next;
			if(BlockHit) {
				Sounds[SOUND_BLOCKHIT]->Play();
				DelBlock(save);
			}
		} while(node != &Blocks);
		
		// update ball position
		Ball->PosX += Ball->VelX;
		Ball->PosY += Ball->VelY;

		if(Ball->PosX < 10) {
			Ball->VelX = -Ball->VelX;
			Sounds[SOUND_WALLHIT]->Play();
		}
		if(Ball->PosX > 620) {
			Ball->VelX = -Ball->VelX;
			Sounds[SOUND_WALLHIT]->Play();
		}
		if(Ball->PosY < 10) {
			Ball->VelY = -Ball->VelY;
			Sounds[SOUND_DEAD]->Play();
		}
		if(Ball->PosY > 470) {
			Ball->VelY = -Ball->VelY;
			Sounds[SOUND_WALLHIT]->Play();
		}

		if(nBlocks <= 0) {
			Sounds[SOUND_WON]->Play();
			InitLevel();
		}

		// display everything
		UpdateGraphics();

		break;
	}
}