/////////////////////////////////////////////////////////////////////////////////////////////
// GAME.CPP
/////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// InitGame
/////////////////////////////////////////////////////////////////////////////////////////////
void InitGame()
{
	// Setup the screen
	Screen =  new CDXScreen;
	Screen->CreateFullScreen(hWnd, 640, 480, 16);

	// do a quick display to display a "Loading menu"
	Screen->GetBack()->Fill(0);
	Screen->GetFront()->Fill(0);
	Screen->GetBack()->TextXY(245,232,RGB(255,255,255),"Loading Please Wait...");
	Screen->Flip();

	// Load the tiles
	Tiles = new CDXTile;
	Tiles->Create(Screen, "TILES.BMP", 64, 64, 42, CDXMEM_SYSTEMONLY);

	// Load the map and set the position
	Map = new CDXMap(Tiles, Screen);
	Map->Load("LEVEL.MAP");
	Map->MoveTo(64, 7712);

	// Create a working surface in system memory
	WorkSurface = new CDXSurface;
	WorkSurface->Create(Screen, 640,480,CDXMEM_SYSTEMONLY);

	// Load the player
	Player = new CDXSprite;
	Player->Create(Screen, "PLAYERWB.BMP", 38, 36, 3, CDXMEM_SYSTEMONLY);
	Player->SetPos(384,7900);
	Player->SetColorKey(65535);
	Player->SetShadowOffset(25,-30);
	Player->SetShadowValue(160);

	// Load the blaster
	Blaster = new CDXTile;
	Blaster->Create(Screen, "BLASTERWB.BMP", 30, 42, 1, CDXMEM_SYSTEMONLY);
	Blaster->SetColorKey(65535);

	// Start the music
	Music = new CDXMusic(hWnd);
	Music->Play("ROCKEM.MID");

	// Create the sound object
	Sound = new CDXSound;
	Sound->Create(hWnd);

	// Load the sounds
	SND_BLASTER = new CDXSoundBuffer;
	SND_BLASTER->Load(Sound, "BLASTER.WAV", 4);

	SetRect(&Window, 0, 0, 640, 480);
	ProgramState = PS_ACTIVE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// UpdateFrame
/////////////////////////////////////////////////////////////////////////////////////////////
void UpdateGame()
{
	switch(ProgramState)
	{
		case PS_ACTIVE:
		{
			Update();
		}
		break;
	}
}
