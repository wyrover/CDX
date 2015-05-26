/**************************************************************************
 Project Name  : SPACEY VADERS
 File Name     : States.cpp
 Programmer    : Paul J Lord (PLord1@aol.com)
/**************************************************************************/
#include "main.h"



/**************************************************************************
 Function: UpdateGame
 
 Purpose: Check the program states
/**************************************************************************/
void UpdateGame(void)
{
	switch(ProgramState)
	{

		case PS_SPLASH:
			{
                SplashScreen();
			}
			break;

		case PS_ACTIVE:
			{
 	    	   Update();
			}
			break;

		case PS_NEXTLEVEL:
			{
 	    	   NextLevel();
			}
			break;

		case PS_NEXTLIFE:
			{
 	    	   NextLife();
			}
			break;
            
		case PS_GAMEOVER:
			{
                GameOver();
			}
			break;
	}

}



/**************************************************************************
 Function: SplashScreen
 
 Purpose: Display the intro screen
/**************************************************************************/
void SplashScreen(void)
{
    _int64 diffTick;
    static _int64 lastTick, WaitTime;
    static BOOL DisplayIt; 
 
    // Display only Once and make sure the WaitTime is Cleared.
    if (DisplayIt == FALSE)
    {
    
        DisplaySplashScreen();
        DisplayIt = TRUE;
        WaitTime = 0;
        
        // Force a new setting on the lastTick
        GetTick();  
        lastTick = firstTick;

    }        

// Set program state to ACTIVE
    GetTick();  // will set the firstTick and freqAdjust
    
    diffTick = firstTick - lastTick;
    lastTick = firstTick;
    
    WaitTime += diffTick;
    
	Input.Update();

    if (WaitTime > ((15000*freqPerformance)/freqAdjust) || Input.GetKeyState(CDXKEY_SPACE) || Input.GetKeyState(CDXKEY_ENTER) || Input.GetKeyState(CDXKEY_LEFTCTRL) || Input.GetKeyState(CDXKEY_MOUSELEFT))
    {
        WaitTime = 0;
        DisplayIt = FALSE;
        
        // Fade out the screen
        Screen->FadeOut(5);

        // Black out the Screen        
      	Screen->Fill(0);
        Screen->Flip();
      	Screen->Fill(0);
        
    	Screen->LoadPalette(TITLEBMP);
        
        if (MusicFlag == 1) Music->Play(MUSIC1);

        // Ok waited now what?
        // Set the game state
        ProgramState = PS_NEXTLEVEL;
        
    }

}




/**************************************************************************
 Function: Update
 
 Purpose: Main state for game play
/**************************************************************************/
void Update(void)
{
	GetInput();

    // Check to see if the sprite delays have been meet, Yes then Move Objects    
    CheckTimeEvents();

    EraseBackBuffer();

    DrawObjects();

}





/**************************************************************************
 Function: NextLevel
 
 Purpose: State for the Next level
/**************************************************************************/
void NextLevel(void)
{

    _int64 diffTick;
    static _int64 lastTick, WaitTime;
    static BOOL DisplayIt; 
	CDXSprite *Next;
 
    // Display only Once and make sure the WaitTime is Cleared.
    if (DisplayIt == FALSE)
    {
        char tempstr[45];
        sprintf(tempstr, "L   E   V   E   L    %d", Level);

        DisplayMessage( 320, 240, tempstr);
        DisplayIt = TRUE;
        WaitTime = 0;
        
        // Force a new setting on the lastTick
        GetTick();  
        lastTick = firstTick;

        // Load the Sprites list with new structures
        int maxsprite = Sprites.GetCount();
        
		Next = Sprites.GetFirst();
	    for (int delcnt=0; delcnt < maxsprite; delcnt++)
        {
			CDXSprite *s = Next;
			Next = Sprites.GetNext(Next);

		    Sprites.DelSprite(s);
        }
        
        ResetLevel();
        SetupSprites();
        
    }        

    // Set program state to ACTIVE
    GetTick();  // will set the firstTick and freqAdjust
    
    diffTick = firstTick - lastTick;
    lastTick = firstTick;
    
    WaitTime += diffTick;
    
    if (WaitTime > ((3000*freqPerformance)/freqAdjust) )
    {
        WaitTime = 0;
        DisplayIt = FALSE;
        // Ok waited now what?
        // Set the game state
    	ProgramState = PS_ACTIVE;
                
    }


}



/**************************************************************************
 Function: NextLife
 
 Purpose: State for the Next life
/**************************************************************************/
void NextLife(void)
{

    _int64 diffTick;
    static _int64 lastTick, WaitTime;
    static BOOL DisplayIt; 

    if (PlayerLives > 0)
    {
    
        // Display only Once and make sure the WaitTime is Cleared.
        if (DisplayIt == FALSE)
        {
        
            DisplayMessage( 320, 40, "G   E   T       R   E   A   D   Y");
            DisplayIt = TRUE;
            WaitTime = 0;
        
            // Force a new setting on the lastTick
            GetTick();  
            lastTick = firstTick;
            Player->SetFrame(0);
    	    Player->SetPos(304, 430);
            
        }        

        // Set program state to ACTIVE
        GetTick();  // will set the firstTick and freqAdjust
    
        diffTick = firstTick - lastTick;
        lastTick = firstTick;
    
        WaitTime += diffTick;
     
        if (WaitTime > ((3000*freqPerformance)/freqAdjust) )
        {
            WaitTime = 0;
            DisplayIt = FALSE;
            
            // Ok waited now what?
            // Set the game state
        	ProgramState = PS_ACTIVE;
                
        }

    }
    else
    {
        // No more lives... Game Over
    	ProgramState = PS_GAMEOVER;
    }


}




/**************************************************************************
 Function: GameOver
 
 Purpose: Oh... You LOSE... State for the Game over
/**************************************************************************/
void GameOver(void)
{

    _int64 diffTick;
    static _int64 lastTick, WaitTime;
    static BOOL DisplayIt; 
	CDXSprite *Next;
 
    // Display only Once and make sure the WaitTime is Cleared.
    if (DisplayIt == FALSE)
    {
        
        // Set the new high score
        if (TotalScore > HighScore) HighScore = TotalScore;
        
        DisplayMessage( 320, 40, "G   A   M   E       O   V   E   R");
        DisplayIt = TRUE;
        WaitTime = 0;
        
        // Force a new setting on the lastTick
        GetTick();  
        lastTick = firstTick;

        // Delete all the objects 1st...
//        dprintf(fdebug,"Total sprites %d\n",Sprites.m_nSprites);
        
        int maxsprite = Sprites.GetCount();
        
		Next = Sprites.GetFirst();
	    for (int delcnt=0; delcnt < maxsprite; delcnt++)
        {
			CDXSprite *s = Next;
			Next = Sprites.GetNext(Next);

		    Sprites.DelSprite(s);
        }

    // Reset the level
        LoadINIVars();
        ResetLevel();
        ResetGame();
        
    // Load the Sprites list with new structures
        SetupSprites();
           
    }        

// Set program state to ACTIVE
    GetTick();  // will set the firstTick and freqAdjust
    
    diffTick = firstTick - lastTick;
    lastTick = firstTick;
    
    WaitTime += diffTick;
    
    // Wait 3 secs
    if (WaitTime > ( (3000*freqPerformance)/freqAdjust) )
    {
        WaitTime = 0;
        DisplayIt = FALSE;
        
        // Ok waited now what?
        // Set the game state

        // Fade out the screen
        Screen->FadeOut(2);
        // Black out the Screen        
      	Screen->Fill(0);
        Screen->Flip();
      	Screen->Fill(0);
        
    	Screen->LoadPalette(TITLEBMP);
    	Screen->LoadBitmap(TITLEBMP);

    	ProgramState = PS_SPLASH;
        
    }


}



/**************************************************************************
 Function: CheckTimeEvents
 
 Purpose: Check all the Time events for each object movement/animation
/**************************************************************************/
void CheckTimeEvents(void)
{

    static DWORD PlayerFrame;
	CDXSprite *s;

    _int64 diffTick;
    static _int64 lastTick, VaderTime, PlayerTime, MissileTime, UfoTime, 
                UfoFreqTime, VMissileFreqTime, StarTime;

    GetTick();  // will set the firstTick and freqAdjust
    
    diffTick = firstTick - lastTick;
    lastTick = firstTick;
    
 
    // Check for the star field animation (movement)   
    StarTime += diffTick;
    if (StarTime > (StarDelay*freqPerformance)/freqAdjust )
    {
        StarTime = 0;
        SpaceMap1->ScrollUp(1);
        
    }
    

    // Check for the Player animation
    PlayerTime += diffTick;
    if (PlayerTime > (PlayerDelay*freqPerformance)/freqAdjust )
    {
        PlayerTime = 0;
        
        PlayerFrame = Player->GetFrame();
        PlayerFrame += 1;

//        dprintf(fdebug,"PlayerFrame = %d\n",PlayerFrame);

        // Player is flying in space        
        if (PlayerFrame == 3)
        {
            PlayerFrame = 0;
        }            
        
        
        // Player is Exploding...
        if (PlayerFrame > 8)
        {
//            dprintf(fdebug,"Player Exploding End\n"); // debug:
        
            PlayerFrame = 7;
            ProgramState = PS_NEXTLIFE;
        }            
        
      	Player->SetFrame(PlayerFrame);
        
    }



    // Check for Vader time to MOVE
    VaderTime += diffTick;
	if (VaderTime >= (LevelInfo[Level].VaderSpeed *freqPerformance)/freqAdjust )
	{
//        dprintf("debug.txt","Move Vaders msec=%ld \n",(VaderTime/(freq/1000)) );
        VaderTime = 0;
        MoveVaderObjects();
	}


    // Check for Missile Movement
    MissileTime += diffTick;
    if (MissileTime >= (LevelInfo[Level].MissileSpeed * freqPerformance)/freqAdjust)
    {
        MissileTime = 0;
        MoveMissileObjects();
    }
    
    
    
    // Check UFO if Active
    if (Ufo_Active)
    {
    
        UfoTime += diffTick;
    	if (UfoTime >= (LevelInfo[Level].UfoSpeed * freqPerformance)/freqAdjust)
    	{
            UfoTime = 0;
            MoveUfoObjects();
    	}
    
    }
    else
    {    
    
//       dprintf(fdebug,"Ufo NOT Active \n");

        // Check for Ufo, Time to add a UFO to system?    
        UfoFreqTime += diffTick;
        if (UfoFreqTime >= (LevelInfo[Level].UfoFreq * freqPerformance)/freqAdjust)
        {
//            dprintf(fdebug,"Ufo Add Sprite \n");
               
            UfoFreqTime = 0;
            Ufo_Active = TRUE;
// Add the UFO to the List
			s = new CDXSprite();
			s->Create(VadersTile);					// Tile OffScreen Buffer
			s->SetType(TYPEUFO);					// Sprite Type
			s->SetPosX(10);							// Xpos of Sprite
			s->SetPosY(50);							// Ypos of Sprite
			s->SetVelX(1);							// XVel of Sprite
			s->SetVelY(0);							// YVel of Sprite
			s->SetState(ALIVE);						// IT's ALIVE!!!
			s->SetFrame(TYPEUFO);					// Frame to start with
			s->SetDelay(LevelInfo[Level].UfoDelay);		// Set but, value is global for Vaders. 

			Sprites.AddSprite(s);
        }
    }
    
    
    
    // Check for time to Fire a Missile from the Vaders
    VMissileFreqTime += diffTick;
    if (VMissileFreqTime >= (LevelInfo[Level].VMissileFreq * freqPerformance)/freqAdjust)
    {
//        dprintf(fdebug,"Add VMissile Flag \n");
        VMissileFreqTime = 0;
        VMissile_Active = TRUE;
    }



}




