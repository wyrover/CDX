//#include <mmsystem.h>
#include "main.h"

void Playing();
void Starting();
void Paused();
void Menu();
void Win();

#define FONT_NAME "Arial"

#define MA 10

////////////////////////////////////////////////////////////////////////////////////////////////
///  Init Game
////////////////////////////////////////////////////////////////////////////////////////////////
///  Initialize all the variables 
////////////////////////////////////////////////////////////////////////////////////////////////
void InitGame() {
	
	Engine = new CDXScreen;
	if (FAILED(Engine->CreateFullScreen(hWnd, 800, 600, 8)))
		CDXError(Engine, "Unable to open fullscreen 800x600x8");

	Engine->GetBack()->Fill(0);
	Engine->GetFront()->Fill(0);

	Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_BOLD);

	Player1 = new CDXSprite;
	Player1->Create(Engine,"Player.bmp",64,15,1);
	Player1->SetPos((Engine->GetWidth() / 2) - Player1->GetTile()->GetBlockWidth(),
		Engine->GetHeight() - 30);
	Player1->SetFrame(1);
	Player1->SetState(STANDING);
	Player1->SetColorKey(0);

	Player2 = new CDXSprite;
	Player2->Create(Engine,"Player.bmp",64,15,1);
	Player2->SetPos((Engine->GetWidth() / 2) - Player2->GetTile()->GetBlockWidth(),60);
	Player2->SetFrame(1);
	Player2->SetState(STANDING);
	Player2->SetColorKey(0);

	Stars = new CDXStarField(Engine);
	Stars->SetNumberofStars(500);
	Stars->m_StarMode = THREED;
	Stars->m_Towards = TRUE;
	Stars->m_Flashing = FALSE;
	Stars->m_Colored = FALSE;
	Stars->Create();

	Ball = new CDXSprite;
	Ball->Create(Engine,"ball.bmp",25,25,1);
	Ball->SetPos(Engine->GetWidth() / 2, Engine->GetHeight() / 2);
	Ball->SetFrame(1);
	Ball->SetColorKey(0);
	Ball->SetVel(3,3);
	Ball->SetColorKey(0);

	Player1Score = 0;
	Player2Score = 0;

	SoundSystem = new CDXSound;
	SoundSystem->Create(hWnd);

	Bounce = new CDXSoundBuffer();
	Bounce->Load(SoundSystem,"beep.wav",2);

	Point = new CDXSoundBuffer();
	Point->Load(SoundSystem,"zing.wav",1);
	
	Winner = new CDXSoundBuffer();
	Winner->Load(SoundSystem,"win.wav",1);

	GameState = MENU;
}

////////////////////////////////////////////////////////////////////////////////////////////////
///  Update Game
////////////////////////////////////////////////////////////////////////////////////////////////
///  Go through the game state switch and go to the appropriate function
////////////////////////////////////////////////////////////////////////////////////////////////
void UpdateGame() {

	switch(GameState) {

	// If the game is playing update all info...
	case PLAYING: 
		Playing();
		break;
		
	//  Game is just starting
	case STARTING: 
		Starting();
		break;

	case PAUSED: 
		Paused();
		break;

	case MENU:
		Menu();
		break;

	case WINNER:
		Win();
		break;

	} // case switch

}

void Playing() {

	char str[64];

	Input.Update();

	if(Input.GetKeyState(CDXKEY_RIGHTARROW)) {
		Player1->SetPosX(Player1->GetPosX() + MA);
		Player1->SetState(MOVING_RIGHT);
	}
	else if(Input.GetKeyState(CDXKEY_LEFTARROW)) {
		Player1->SetPosX(Player1->GetPosX() - MA);
		Player1->SetState(MOVING_LEFT);
	}
	else {
		Player1->SetState(STANDING);
	}
	
	if(Player2Mode) {
		if(Input.GetKeyState(CDXKEY_X)) {
			Player2->SetPosX(Player2->GetPosX() + MA);
			Player2->SetState(MOVING_RIGHT);
		}
		else if(Input.GetKeyState(CDXKEY_Z)) {
			Player2->SetPosX(Player2->GetPosX() - MA);
			Player2->SetState(MOVING_LEFT);
		}
		else {
			Player2->SetState(STANDING);
		}
	}
	else {
		if( (Player2->GetPosX() + Player2->GetTile()->GetBlockWidth()) < Ball->GetPosX()) {
			Player2->SetPosX(Player2->GetPosX() + MA);
			Player2->SetState(MOVING_RIGHT);
		}
		else if(Player2->GetPosX() > (Ball->GetPosX() + Ball->GetTile()->GetBlockWidth())) {
			Player2->SetPosX(Player2->GetPosX() - MA);
			Player2->SetState(MOVING_LEFT);
		}
		else {
			Player2->SetState(STANDING);
		}
	}
		
	if(Input.GetKeyState(CDXKEY_P)) {
		GameState = PAUSED;
	}

	if(Input.GetKeyState(CDXKEY_ESCAPE)) {
		GameState = MENU;
		Player1->SetPos((Engine->GetWidth() / 2) - Player1->GetTile()->GetBlockWidth(),
			Engine->GetHeight() - 30);
		Player1->SetState(STANDING);
		Player2->SetPos((Engine->GetWidth() / 2) - Player2->GetTile()->GetBlockWidth(),60);
		Player2->SetState(STANDING);
		Ball->SetVel(3,3);
		Ball->SetPos((Engine->GetWidth() / 2),(Engine->GetHeight() / 2));
	}
		
	if( (Player1->GetPosX() + Player1->GetTile()->GetBlockWidth()) > Engine->GetWidth() )
		Player1->SetPosX(Engine->GetWidth() - Player1->GetTile()->GetBlockWidth());
	else if(Player1->GetPosX() < 1) Player1->SetPosX(1);
		if( (Player2->GetPosX() + Player2->GetTile()->GetBlockWidth()) > Engine->GetWidth() )
		Player2->SetPosX(Engine->GetWidth() - Player2->GetTile()->GetBlockWidth());
	else if(Player2->GetPosX() < 1) Player2->SetPosX(1);
		Ball->SetPos(Ball->GetPosX() + Ball->GetVelX(), Ball->GetPosY() + Ball->GetVelY());
		
	if((Ball->GetPosX() + Ball->GetTile()->GetBlockWidth()) > Engine->GetWidth() ) {
		Ball->SetPosX(Engine->GetWidth() - Ball->GetTile()->GetBlockWidth());
		Ball->SetVelX(Ball->GetVelX() * -1);
		Bounce->PlayExtended(10,0);
	}
	else if(Ball->GetPosX() < 1) {
		Ball->SetPosX(1);
		Ball->SetVelX(Ball->GetVelX() * -1);
		Bounce->PlayExtended(-10,0);
	}
		
	if((Ball->GetPosY() + Ball->GetTile()->m_BlockHeight) > 
			Player1->GetPosY() + Player1->GetTile()->m_BlockHeight ) {
		Ball->SetPos(Engine->GetWidth() / 2, Engine->GetHeight() / 2);
		Ball->SetVel(3,-3);
		Player2Score++;
		Point->PlayExtended(0,0);
		if(Player2Score == 7) GameState = WINNER;
		else GameState = STARTING;
	}
	else if(Ball->GetPosY() < Player2->GetPosY()) {
		Ball->SetPos(Engine->GetWidth() / 2, Engine->GetHeight() / 2);
		Ball->SetVel(3,3);
		Player1Score++;
		Point->PlayExtended(0,0);
		if(Player1Score == 7) GameState = WINNER;
		else GameState = STARTING;
	}
		
	if(Ball->SpriteHit(Player1)) {
		Ball->SetPosY(Player1->GetPosY() - Ball->GetTile()->m_BlockHeight);
		Ball->SetVelY(Ball->GetVelY() * -1);
		if(Player1->GetState() == MOVING_RIGHT) {
			Ball->SetVelX(Ball->GetVelX() + 3);
		}
		else if(Player1->GetState() == MOVING_LEFT) {
			Ball->SetVelX(Ball->GetVelX() - 3);
		}
		Bounce->PlayExtended(0,0);
	}
		
	if(Ball->SpriteHit(Player2)) {
		Ball->SetPosY(Player2->GetPosY() + Player2->GetTile()->m_BlockHeight);
		Ball->SetVelY(Ball->GetVelY() * -1);
		if(Player2->GetState() == MOVING_RIGHT) {
			Ball->SetVelX(Ball->GetVelX() + 3);
		}
		else if(Player2->GetState() == MOVING_LEFT) {
			Ball->SetVelX(Ball->GetVelX() - 3);
		}
		Bounce->PlayExtended(0,0);
	}
	
	Engine->GetBack()->Fill(0);
	
	Stars->Update();
	Player1->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
	Player2->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
	Ball->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		
//	if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
	Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_NORMAL);

	Engine->GetBack()->GetDC();
	Engine->GetBack()->SetFont();
	sprintf(str,"Player 1: %d", Player1Score);
	Engine->GetBack()->TextXY(5,30,RGB(255,0,0),str);
	sprintf(str,"Player 2: %d", Player2Score);
	Engine->GetBack()->TextXY(650,30,RGB(255,0,0),str);
	Engine->GetBack()->ReleaseDC();
	Engine->Flip();

} // Game Playing

void Starting() {
	char str[64];
	DWORD InitTime, CurrTime, dTime;
	
	InitTime = timeGetTime();
	do{

	Input.Update();

	if(Input.GetKeyState(CDXKEY_RIGHTARROW)) {
		Player1->SetPosX(Player1->GetPosX() + MA);
		Player1->SetState(MOVING_RIGHT);
	}
	else if(Input.GetKeyState(CDXKEY_LEFTARROW)) {
		Player1->SetPosX(Player1->GetPosX() - MA);
		Player1->SetState(MOVING_LEFT);
	}
	else {
		Player1->SetState(STANDING);
	}

	if(Player2Mode) {
		if(Input.GetKeyState(CDXKEY_X)) {
			Player2->SetPosX(Player2->GetPosX() + MA);
			Player2->SetState(MOVING_RIGHT);
		}
		else if(Input.GetKeyState(CDXKEY_Z)) {
			Player2->SetPosX(Player2->GetPosX() - MA);
			Player2->SetState(MOVING_LEFT);
		}
		else {
			Player2->SetState(STANDING);
		}
	}

	if( (Player1->GetPosX() + Player1->GetTile()->GetBlockWidth()) > Engine->GetWidth() )
		Player1->SetPosX(Engine->GetWidth() - Player1->GetTile()->GetBlockWidth());
	else if(Player1->GetPosX() < 1) Player1->SetPosX(1);

	if( (Player2->GetPosX() + Player2->GetTile()->GetBlockWidth()) > Engine->GetWidth() )
		Player2->SetPosX(Engine->GetWidth() - Player2->GetTile()->GetBlockWidth());
	else if(Player2->GetPosX() < 1) Player2->SetPosX(1);

	Engine->GetBack()->Fill(0);

	Stars->Update();
	Player1->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
	Player2->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
	Ball->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		
//	if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
	Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_NORMAL);

	Engine->GetBack()->GetDC();
	Engine->GetBack()->SetFont();
	sprintf(str,"Player 1: %d",Player1Score);
	Engine->GetBack()->TextXY(5,30,RGB(255,0,0),str);
	sprintf(str,"Player 2: %d",Player2Score);
	Engine->GetBack()->TextXY(650,30,RGB(255,0,0),str);
	Engine->GetBack()->ReleaseDC();

//	if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
	Engine->GetBack()->ChangeFont(FONT_NAME,30,75,FW_BOLD);

	Engine->GetBack()->GetDC();
	Engine->GetBack()->SetFont();
	Engine->GetBack()->TextXY(Engine->GetWidth()/3,Engine->GetHeight()/3,
		RGB(255,255,255),"Get");
	Engine->GetBack()->TextXY(Engine->GetWidth()/3,Engine->GetHeight()/2,
		RGB(255,255,255),"Ready!");
	Engine->GetBack()->ReleaseDC();

	Engine->Flip();
	CurrTime = timeGetTime();
	dTime = CurrTime - InitTime;
	}
	while(dTime < 3000);

	GameState = PLAYING;

} // Starting

void Paused() {

	char str[64];
	BOOL done = FALSE;
	DWORD InitTime, CurrTime, dTime;

	InitTime = timeGetTime();
	CurrTime = dTime = 0;
	do{
		Stars->Update();
		Player1->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Player2->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Ball->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,40,100,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		Engine->GetBack()->TextXY(250,Engine->GetHeight() / 2, RGB(255,255,0), "P");
		Engine->GetBack()->ReleaseDC();

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		sprintf(str,"Player 1: %d", Player1Score);
		Engine->GetBack()->TextXY(5,30,RGB(255,0,0),str);
		sprintf(str,"Player 2: %d", Player2Score);
		Engine->GetBack()->TextXY(650,30,RGB(255,0,0),str);
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		CurrTime = timeGetTime();
		dTime = CurrTime - InitTime;
	} while(dTime < 100);

	InitTime = timeGetTime();
	do{
		Stars->Update();
		Player1->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Player2->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Ball->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,40,100,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		Engine->GetBack()->TextXY(250,Engine->GetHeight() / 2, RGB(255,255,0), "PA");
		Engine->GetBack()->ReleaseDC();

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		sprintf(str,"Player 1: %d", Player1Score);
		Engine->GetBack()->TextXY(5,30,RGB(255,0,0),str);
		sprintf(str,"Player 2: %d", Player2Score);
		Engine->GetBack()->TextXY(650,30,RGB(255,0,0),str);
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		CurrTime = timeGetTime();
		dTime = CurrTime - InitTime;
	} while(dTime < 100);

	InitTime = timeGetTime();
	do{
		Stars->Update();
		Player1->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Player2->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Ball->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,40,100,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		Engine->GetBack()->TextXY(250,Engine->GetHeight() / 2, RGB(255,255,0), "PAU");
		Engine->GetBack()->ReleaseDC();

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		sprintf(str,"Player 1: %d", Player1Score);
		Engine->GetBack()->TextXY(5,30,RGB(255,0,0),str);
		sprintf(str,"Player 2: %d", Player2Score);
		Engine->GetBack()->TextXY(650,30,RGB(255,0,0),str);
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		CurrTime = timeGetTime();
		dTime = CurrTime - InitTime;
	} while(dTime < 100);

	InitTime = timeGetTime();
	do{
		Stars->Update();
		Player1->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Player2->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Ball->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,40,100,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		Engine->GetBack()->TextXY(250,Engine->GetHeight() / 2, RGB(255,255,0), "PAUS");
		Engine->GetBack()->ReleaseDC();

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		sprintf(str,"Player 1: %d", Player1Score);
		Engine->GetBack()->TextXY(5,30,RGB(255,0,0),str);
		sprintf(str,"Player 2: %d", Player2Score);
		Engine->GetBack()->TextXY(650,30,RGB(255,0,0),str);
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		CurrTime = timeGetTime();
		dTime = CurrTime - InitTime;
	} while(dTime < 100);

	InitTime = timeGetTime();
	do{
		Stars->Update();
		Player1->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Player2->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Ball->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,40,100,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		Engine->GetBack()->TextXY(250,Engine->GetHeight() / 2, RGB(255,255,0), "PAUSE");
		Engine->GetBack()->ReleaseDC();

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		sprintf(str,"Player 1: %d", Player1Score);
		Engine->GetBack()->TextXY(5,30,RGB(255,0,0),str);
		sprintf(str,"Player 2: %d", Player2Score);
		Engine->GetBack()->TextXY(650,30,RGB(255,0,0),str);
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		CurrTime = timeGetTime();
		dTime = CurrTime - InitTime;
	} while(dTime < 100);

	do{
		Input.Update();
		Stars->Update();
		Player1->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Player2->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);
		Ball->Draw(Engine->GetBack(), 0, 0, CDXBLT_BLK);

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,40,100,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		Engine->GetBack()->TextXY(250,Engine->GetHeight() / 2, RGB(255,255,0), "PAUSED");
		Engine->GetBack()->ReleaseDC();

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,10,25,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		sprintf(str,"Player 1: %d", Player1Score);
		Engine->GetBack()->TextXY(5,30,RGB(255,0,0),str);
		sprintf(str,"Player 2: %d", Player2Score);
		Engine->GetBack()->TextXY(650,30,RGB(255,0,0),str);
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		if(Input.GetKeyState(CDXKEY_P)) done = TRUE;
	} while(!done);

	InitTime = timeGetTime();
	CurrTime = dTime = 0;
	do{
		CurrTime = timeGetTime();
		dTime = CurrTime - InitTime;
	} while(dTime < 200);

	GameState = PLAYING;

}

void Menu() {
	BOOL done = FALSE;
	BOOL Changing = FALSE;
	DWORD InitTime, CurrTime, dTime;

	DWORD CurrSelection = 1;
	
	do {
		Input.Update();
		Stars->Update();

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,40,100,FW_BOLD);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		Engine->GetBack()->TextXY(10,0,RGB(0,0,255),"Pong 2000");
		Engine->GetBack()->ReleaseDC();

//		if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
		Engine->GetBack()->ChangeFont(FONT_NAME,20,50,FW_NORMAL);

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		if(CurrSelection == 1) {
			Engine->GetBack()->TextXY(200,200,RGB(255,0,0),"1 Player Game");
			Engine->GetBack()->TextXY(200,300,RGB(64,64,0),"2 Player Game");
			Engine->GetBack()->TextXY(200,400,RGB(64,64,0),"Exit");
		}
		else if(CurrSelection == 2) {
			Engine->GetBack()->TextXY(200,200,RGB(64,64,0),"1 Player Game");
			Engine->GetBack()->TextXY(200,300,RGB(255,0,0),"2 Player Game");
			Engine->GetBack()->TextXY(200,400,RGB(64,64,0),"Exit");
		}
		else if(CurrSelection == 3) {
			Engine->GetBack()->TextXY(200,200,RGB(64,64,0),"1 Player Game");
			Engine->GetBack()->TextXY(200,300,RGB(64,64,0),"2 Player Game");
			Engine->GetBack()->TextXY(200,400,RGB(255,0,0),"Exit");
		}
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		if(Input.GetKeyState(CDXKEY_UPARROW)) {
			if(Changing) {
				CurrTime = timeGetTime();
				dTime = CurrTime - InitTime;
				if(dTime > 350) Changing = FALSE;
			}
			else {
				InitTime = timeGetTime();
				CurrSelection--;
				Changing = TRUE;
			}
		}
		if(Input.GetKeyState(CDXKEY_DOWNARROW)) {
			if(Changing) {
				CurrTime = timeGetTime();
				dTime = CurrTime - InitTime;
				if(dTime > 350) Changing = FALSE;
			}
			else {
				InitTime = timeGetTime();
				CurrSelection++;
				Changing = TRUE;
			}
		}
		if(CurrSelection < 1) CurrSelection = 1;
		if(CurrSelection > 3) CurrSelection = 3;

		if(Input.GetKeyState(CDXKEY_ENTER)) {
			done = TRUE;
		}
		

	} while(!done);

	if(CurrSelection == 1) {
		GameState = STARTING;
		Player2Mode = FALSE;
	}
	if(CurrSelection == 2) {
		GameState = STARTING;
		Player2Mode = TRUE;
	}
	if(CurrSelection == 3) {
		PostMessage(hWnd,WM_CLOSE,0,0);
	}

	Player1Score = 0;
	Player2Score = 0;
	
}

void Win() {
	DWORD InitTime, CurrTime, dTime;
	BOOL response;
	BOOL done = FALSE;
	DWORD r = 0, g = 50, b = 200;
	DWORD rDir = 1, gDir = 0, bDir = 1;

	Winner->PlayExtended(0,0);

//	if(Engine->GetBack()->m_Font) DeleteObject(Engine->GetBack()->m_Font);
	Engine->GetBack()->ChangeFont(FONT_NAME,30,75,FW_NORMAL);

	InitTime = timeGetTime();
	CurrTime = dTime = 0;
	do {
		Stars->Update();

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		if(Player1Score == 7)
		Engine->GetBack()->TextXY(200,150,RGB(255,0,0),"Player 1");
		else Engine->GetBack()->TextXY(200,150,RGB(255,0,0),"Player 2");
		Engine->GetBack()->TextXY(300,200,RGB(255,0,0),"Wins!");
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		CurrTime = timeGetTime();
		dTime = CurrTime - InitTime;
	} while(dTime < 3000);

	do{
		Input.Update();
		Stars->Update();
		r += rDir;
		g += gDir;
		b += bDir;
		if((r > 254) || (r < 1)) rDir *= -1;
		if((g > 254) || (g < 1)) gDir *= -1;
		if((b > 254) || (b < 1)) bDir *= -1;

		Engine->GetBack()->GetDC();
		Engine->GetBack()->SetFont();
		Engine->GetBack()->TextXY(100,200,RGB(r,g,b),"Play again? (Y/N)");
		Engine->GetBack()->ReleaseDC();

		Engine->Flip();

		if(Input.GetKeyState(CDXKEY_Y)) {
			response = TRUE;
			done = TRUE;
		}
		else if(Input.GetKeyState(CDXKEY_N)){
			response = FALSE;
			done = TRUE;
		}
	}while(!done);

	if(response == TRUE) {
		Player1Score = 0;
		Player2Score = 0;
		
		Player1->SetPos((Engine->GetWidth() / 2) - Player1->GetTile()->GetBlockWidth(),
			Engine->GetHeight() - 30);
		Player1->SetState(STANDING);

		Player2->SetPos((Engine->GetWidth() / 2) - Player2->GetTile()->GetBlockWidth(),60);
		Player2->SetState(STANDING);

		Ball->SetVel(3,3);
		Ball->SetPos((Engine->GetWidth() / 2),(Engine->GetHeight() / 2));

		GameState = STARTING;
	}
	else {
		Player1Score = 0;
		Player2Score = 0;
		
		Player1->SetPos((Engine->GetWidth() / 2) - Player1->GetTile()->GetBlockWidth(),
			Engine->GetHeight() - 30);
		Player1->SetState(STANDING);

		Player2->SetPos((Engine->GetWidth() / 2) - Player2->GetTile()->GetBlockWidth(),60);
		Player2->SetState(STANDING);

		Ball->SetVel(3,3);
		Ball->SetPos((Engine->GetWidth() / 2), (Engine->GetHeight() / 2));

		GameState = MENU;
	}
}
