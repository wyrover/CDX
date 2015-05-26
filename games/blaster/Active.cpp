/////////////////////////////////////////////////////////////////////////////////////////////
// ACTIVE.CPP
/////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"

/////////////////////////////////////////////////////////////////////////////////////////////
// UpdateInput
/////////////////////////////////////////////////////////////////////////////////////////////
void UpdateInput(void)
{
	static int Count = 0;

	Input.Update();
	Player->SetFrame(0);

	if(Input.GetKeyState(CDXKEY_UPARROW))
	{
		Player->SetFrame(0);

		Player->SetPosY(Player->GetPosY()-4);
		if(Player->GetPosY() < 0) Player->SetPosY(0);
	}

	if(Input.GetKeyState(CDXKEY_DOWNARROW))
	{
		Player->SetFrame(0);

		Player->SetPosY(Player->GetPosY()+4);
		//if(Player->GetPosY() > 444) Player->GetPosY() = 444;
	}

	if(Input.GetKeyState(CDXKEY_LEFTARROW))
	{
		Player->SetFrame(1);

		if(Player->GetPosX() < 180 && Map->GetPosX() > 0)
		{
			Map->ScrollLeft(4);
			Player->SetPosX(Player->GetPosX()-4);
		}
		else if(Map->GetPosX() == 0)
		{
			Player->SetPosX(Player->GetPosX()-4);
			if(Player->GetPosX() < 0) Player->SetPosX(0);
		}
		else
		{
			Player->SetPosX(Player->GetPosX()-4);
		}
	}

	if(Input.GetKeyState(CDXKEY_RIGHTARROW))
	{
		Player->SetFrame(2);

		if(Player->GetPosX() > 500 && Map->GetPosX() < 128)
		{
			Map->ScrollRight(4);
			Player->SetPosX(Player->GetPosX()+4);
		}
		else if(Map->GetPosX() >= 128)
		{
			Player->SetPosX(Player->GetPosX()+4);
			if(Player->GetPosX() > 715) Player->SetPosX(715);
		}
		else
		{
			Player->SetPosX(Player->GetPosX()+4);
		}
	}

	if(Input.GetKeyState(CDXKEY_LEFTCTRL) || Input.GetKeyState(CDXKEY_RIGHTCTRL))
	{
		if(--Count < 0)
		{
			SND_BLASTER->Play();

			CDXSprite *s;
			s = new CDXSprite();
			s->Create(Blaster);
			s->SetType(SPR_BLASTER);
			s->SetPosX(Player->GetPosX()+4);
			s->SetPosY(Player->GetPosY()-48);
			s->SetVelX(0);
			s->SetVelY(0);
			s->SetAlphaValue(120);
			s->SetShadowValue(220);
			s->SetShadowOffset(20,-35);

			Sprites.AddSprite(s);
			
			Count = 15;
		}
	}

	if(!(Input.GetKeyState(CDXKEY_LEFTCTRL) || Input.GetKeyState(CDXKEY_RIGHTCTRL))) Count = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// UpdateObjects
/////////////////////////////////////////////////////////////////////////////////////////////
void UpdateObjects(void)
{
	CDXSprite* Node;

	//Update the Player
	Player->SetPosY(Player->GetPosY()-1);
	// Loop the list and update the sprites
	for(Node = Sprites.GetFirst(); Node != NULL; Node = Sprites.GetNext(Node))
	{
		switch(Node->GetType())
		{
			case SPR_BLASTER:
			{
				Node->SetPosY(Node->GetPosY() - 8);
				if(Node->GetPosY() < 5) Sprites.DelSprite(Node);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Update
/////////////////////////////////////////////////////////////////////////////////////////////
void Update(void)
{
	UpdateInput();
	UpdateObjects();

	// Scroll the background
	Map->ScrollUp(1);

	// Draw the background
	//Map->Draw(Screen->GetBack());
	Map->Draw(WorkSurface);

	// Draw the player
	Player->Draw(WorkSurface, Map->GetPosX(), Map->GetPosY(), CDXBLT_TRANSSHADOW);
	Player->Draw(WorkSurface, Map->GetPosX(), Map->GetPosY(), CDXBLT_TRANS);
	//Player->Draw(Screen->GetBack(), Map->GetPosX(), Map->GetPosX(), CDXBLT_TRANSSHADOW);
	//Player->Draw(Screen->GetBack(), Map->GetPosX(), Map->GetPosX(), CDXBLT_TRANS);

	// Loop the list and draw the sprites
	Sprites.Draw(WorkSurface, Map->GetPosX(), Map->GetPosY(), CDXBLT_TRANSSHADOW);
	Sprites.Draw(WorkSurface, Map->GetPosX(), Map->GetPosY(), CDXBLT_TRANSALPHA);
	//Sprites.Draw(Screen->GetBack(), Map->GetPosX(), Map->GetPosX(), CDXBLT_TRANSSHADOW);
	//Sprites.Draw(Screen->GetBack(), Map->GetPosX(), Map->GetPosX(), CDXBLT_TRANSALPHA);

	// Send the finished WorkSurface to the Back Buffer
	//WorkSurface->CDXBLT_Blk(Screen->GetBack(),0,0,WorkSurface->m_ClipRect);
	WorkSurface->DrawBlk(Screen->GetBack(),0,0);

	// Flip the video memory buffers
	Screen->Flip();
}
