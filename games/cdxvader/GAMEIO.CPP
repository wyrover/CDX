/**************************************************************************
 Project Name  : SPACEY VADERS
 File Name     : GameIO.cpp
 Programmer    : Paul J Lord (PLord1@aol.com)
/**************************************************************************/
#include "main.h"



/**************************************************************************
 Function: GetInput
 
 Purpose: Get the Players Movements from the Keyboard & Mouse
/**************************************************************************/
void GetInput(void)
{

	static int Count = 0;
	long MouseX, MouseY;

	Input.Update();

	Input.GetMouseDeltas(&MouseX, &MouseY);

// Check the state, If not PS_ACTIVE then do NOT change the x,y positions
    if (ProgramState == PS_ACTIVE && Player->GetFrame() < 3)
    {

		if(MouseX < 0)
		{
			Player->SetPosX(Player->GetPosX() + MouseX);
			if(Player->GetPosX() < 0) Player->SetPosX(0);
		}
		if(Input.GetKeyState(CDXKEY_LEFTARROW))
		{
			Player->SetPosX(Player->GetPosX() - 2);
			if(Player->GetPosX() < 0) Player->SetPosX(0);
		}
	
	
	    
		if(Input.GetKeyState(CDXKEY_RIGHTARROW))
		{
			Player->SetPosX(Player->GetPosX() + 2);
			if(Player->GetPosX() > 590) Player->SetPosX(590);
		}
		if(MouseX > 0)
		{
			Player->SetPosX(Player->GetPosX() + MouseX);
			if(Player->GetPosX() > 590) Player->SetPosX(590);
		}
	
	    
		if(Input.GetKeyState(CDXKEY_SPACE) || Input.GetKeyState(CDXKEY_LEFTCTRL) || Input.GetKeyState(CDXKEY_MOUSELEFT))
		{
	    
       		if(++Count > 8)  // was 10
	    	{
                if(Missile_Active < PlayerMissiles)
	            {     
    	            int xmissilepos = Player->GetPosX();
	                int ymissilepos = Player->GetPosY() - 24;                

					CDXSprite *s;
					s = new CDXSprite();
					s->Create(VadersTile);					// Tile OffScreen Buffer
					s->SetType(TYPEMISSILE1);				// Sprite Type
					s->SetPosX(xmissilepos);				// Xpos of Sprite
					s->SetPosY(ymissilepos);				// Ypos of Sprite
					s->SetVelX(-1);							// XVel of Sprite
					s->SetVelY(0);							// YVel of Sprite
					s->SetState(ALIVE);						// IT's ALIVE!!!
					s->SetFrame(TYPEMISSILE1);				// Frame to start with
					s->SetDelay(LevelInfo[Level].MissileDelay);		// Set but, value is global for Vaders. 

					Sprites.AddSprite(s);

    	            Missile_Active++;
                    if (SoundFlag == 1) MISSILE.Play();
        			Count = 0;

	            }        
            } // If count               
		}

    } // if ProgramState

}




/**************************************************************************
 Function: EraseBackBuffer
 
 Purpose: Remove the objects from the back buffer
/**************************************************************************/
void EraseBackBuffer(void)
{
    // Fill Back buffer with black...
  	Screen->Fill(0);
}




/**************************************************************************
 Function: DrawObjects
 
 Purpose: Draw the objects on the back_buffer
/**************************************************************************/
void DrawObjects(void)
{

    // Draw the Star Field
	SpaceMap1->DrawTrans(Screen->GetBack());

    // Draw the Player
	Player->Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);
    
	// Draw all the sprites in the list
	Sprites.Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);
    
    // If debugging Display the FPS
    if (FPSFlag == 1) DisplayFPS();
    
    // Display text on the back buffer
    DisplayScore();
    DisplayHighScore();
    DisplayLives();
    DisplaySpaceyMsg();

    // Flip the Flippin screen buffers
	Screen->Flip();

    // calculate the FPS after all drawing and page flipping have been performed.  
    GetFPS();
  
}





/**************************************************************************
 Function: dprintf()
 
 Purpose: print debug info to a file.
/**************************************************************************/
void dprintf(FILE *filedebug, char *fmt, ... )
{

    char szBuffer[256];

    if (DebugFlag == 0) return;
    
    // Make sure we close each time...
//    fclose(fdebug);
//    fdebug = fopen("debug.txt","a+");
    
    va_list args;
    va_start(args, fmt);
    vsprintf(szBuffer, fmt, args);
    va_end(args);

    if (filedebug != NULL)
    {
        fprintf(filedebug,szBuffer);
    }
    
}



/**************************************************************************
 Function: GetFPS
 
 Purpose: get the frame rate
/**************************************************************************/
void GetFPS(void)
{
    _int64 diffTick;
    static _int64 FpsTime, lastTick;
    static DWORD FramesRendered;

    GetTick();  // will set the firstTick and freqAdjust
    
    diffTick = firstTick - lastTick;
    lastTick = firstTick;
    
    FpsTime += diffTick;
    FramesRendered++;
    
    // Every 1 sec (1000 ms) reset the counters
    if (FpsTime > ((1000*freqPerformance)/freqAdjust) )
    {
		Fps = FramesRendered;
		FramesRendered  = 0;
		FpsTime = 0;
    }


}




/**************************************************************************
 Function: DisplayFPS
 
 Purpose: Write the FPS variable to the back buffer
/**************************************************************************/
void DisplayFPS(void)
{

	char str[25];
	sprintf(str, "Fps:%d", Fps);

    if (Fps > 0)
    {
    	// Print the frame rate to the screen
	    Screen->GetBack()->GetDC();
    	Screen->GetBack()->TextXY(2, 2, RedTxt, str);
	    Screen->GetBack()->ReleaseDC();
    }
    
}



/**************************************************************************
 Function: DisplayScore
 
 Purpose: Write the Score to the back buffer
/**************************************************************************/
void DisplayScore(void)
{

	static char str1[25];
	static char str2[25];
	static char str3[25];
    
	TotalScore += AddToScore;
    
    if (AddToScore != 0 || TotalScore == 0)
    {   
        sprintf(str1, "%ld", AddToScore);
        strcpy(str2, "Score:");
        sprintf(str3, "%ld", TotalScore);
    }        
    
    
    Screen->GetBack()->GetDC();
    
    // Display the current item score    
    Screen->GetBack()->TextXY(60, 2, GreenTxt, str1);
    
    Screen->GetBack()->TextXY(120, 2, BlueTxt, str2);
    Screen->GetBack()->TextXY(120, 20, GreenTxt, str3);
    Screen->GetBack()->ReleaseDC();
    
    AddToScore = 0;
    
}



/**************************************************************************
 Function: DisplayHighScore
 
 Purpose: Write the High Score to the back buffer
/**************************************************************************/
void DisplayHighScore(void)
{

	static char str1[25];
	static char str2[25];
    
    strcpy(str1, "High Score:");
    sprintf(str2, "%ld", HighScore);
    
    Screen->GetBack()->GetDC();
    Screen->GetBack()->TextXY(455, 2, BlueTxt, str1);
    Screen->GetBack()->TextXY(455, 20, GreenTxt, str2);
    Screen->GetBack()->ReleaseDC();
    
    AddToScore = 0;
    
}

/**************************************************************************
 Function: DisplaySpaceyMsg(void)
 
 Purpose: Display Message
/**************************************************************************/
void DisplaySpaceyMsg(void)
{

	UINT fMode;
	HDC dc;
    
	dc = Screen->GetBack()->GetDC();
    
	fMode = GetTextAlign(dc);
	SetTextAlign(dc, TA_CENTER | VTA_CENTER);
    
    Screen->GetBack()->TextXY(320, 2, GreenTxt, "SPACEY VADERS");
    Screen->GetBack()->TextXY(320, 20, GreenTxt, "by Paul Lord '97");

	SetTextAlign(dc, fMode);
	Screen->GetBack()->ReleaseDC();

}



/**************************************************************************
 Function: DisplayMessage(x,y,String)
 
 Purpose: Write the Score to the back buffer
/**************************************************************************/
void DisplayMessage(int x, int y, char *Msg)
{

	UINT fMode;
	HDC dc;
    
	dc = Screen->GetFront()->GetDC();
    
	fMode = GetTextAlign(dc);
	SetTextAlign(dc, TA_CENTER | VTA_CENTER);
    
    Screen->GetFront()->TextXY(x, y, GreenTxt, Msg);

	SetTextAlign(dc, fMode);
	Screen->GetFront()->ReleaseDC();

}



/**************************************************************************
 Function: DisplayLives
 
 Purpose: Display the player Lives on the screen
/**************************************************************************/
void DisplayLives(void)
{

// Draw small scaled down ships
//    Vaders->SetFrame(0);
//    Vaders->SetPos(200,330);
//    Vaders->DrawScaled(2, Screen->GetBack());
  
	static char str1[25];
	static char str2[25];
    
    strcpy(str1, "Lives:");
    
    LPTSTR lpTemp;
    lpTemp = str2;

    if (PlayerLives > 0)
    {
        for (int lev = 0; lev < (int)PlayerLives; lev++)
        {
            wsprintf(lpTemp, "%c", 'I'); 
            lpTemp++;  
        }        
        lpTemp++;  
        memset(lpTemp,0,1);
    }
    else
    {
        strcpy(str2, "    ");
    }            
    
    Screen->GetBack()->GetDC();
    Screen->GetBack()->TextXY(580, 2, BlueTxt, str1);
    Screen->GetBack()->TextXY(580, 20, BlueTxt, str2);
    Screen->GetBack()->ReleaseDC();
    
}



/**************************************************************************
 Function: GetTick
 
 Purpose: Get the current timer tick (PerformanceCounter or timeGetTime)
          Store in global Vars  - freqAdjust, firsttick
          Requires global var freqPerformance
/**************************************************************************/
void GetTick(void)
{

    if (freqPerformance > 1)
    {    
        QueryPerformanceCounter((LARGE_INTEGER*)&firstTick);    
        freqAdjust = 1000;
    }
    else
    {
        dprintf(fdebug,"using timeGetTime, QueryPerformanceCounter Failed \n");
        firstTick = timeGetTime();
        // QueryPerformanceCounter did not work, so use the timeGettime
        freqAdjust = 1;
        freqPerformance = 1;
    }
    
}
 



/**************************************************************************
 Function: Splash_Screen
 
 Purpose: Display a intro screen
/**************************************************************************/
void DisplaySplashScreen(void)
{

    static BOOL DrawScreen;
	HDC dc;

    // Print the Text... Testing
    char TitleText1[16][80]=
    {
    	{" "},
    	{" "},
    	{" "},
    	{"          =    25 POINTS"},
    	{" "},
    	{" "},
    	{"          =    50 POINTS"},
    	{" "},
    	{" "},
    	{"          =    75 POINTS"},
    	{" "},
    	{" "},
    	{"          =   100 POINTS"},
    	{" "},
    	{" "},
    	{" "},
    };


     char TitleText2[5][80]=
    {
    	{"          =   - 100 POINTS"},
    	{" "},
    	{" "},
    	{"          =   - 100 POINTS"},
    	{" "},
    };
        
      char TitleText3[3][80]=
    {
    	{"PRESS [RETURN] TO START THE GAME, ESC TO EXIT"},
    	{" "},
    	{"'Spacey Vaders' - by Paul Lord - PLord1@aol.com"},
    };
    
    
    
    //Load Back image
    Screen->LoadBitmap(TITLEBMP);
    
    UINT fMode;
    
    // Display the text message
    dc = Screen->GetBack()->GetDC();
    
    fMode = GetTextAlign(dc);
    SetTextAlign(dc, TA_CENTER | VTA_CENTER);
    
    int index;
        
    for (index=0; index< 16; index++)
    {
        Screen->GetBack()->TextXY(320, 100+(index*15), GreenTxt, TitleText1[index]);
    }
    
    for (index=0; index< 5; index++)
    {
        Screen->GetBack()->TextXY(320, 100+((index+16)*15), RedTxt, TitleText2[index]);
    }
    
    for (index=0; index< 3; index++)
    {
        Screen->GetBack()->TextXY(320, 100+((index+21)*15), GreenTxt, TitleText3[index]);
    }
    
    
    SetTextAlign(dc, fMode);
    
    Screen->GetBack()->ReleaseDC();
    
    // Display the Vader Sprites    
    Vaders->SetFrame(0);
    Vaders->SetPos(220,120);
	Vaders->Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);
    
    Vaders->SetFrame(4);
    Vaders->SetPos(220,170);
	Vaders->Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);
    
    Vaders->SetFrame(8);
    Vaders->SetPos(220,218);
	Vaders->Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);
    
    Vaders->SetFrame(41);
    Vaders->SetPos(220,265);
	Vaders->Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);
    
    Vaders->SetFrame(14);
    Vaders->SetPos(220,320);
	Vaders->Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);
    
    Vaders->SetFrame(16);
    Vaders->SetPos(220,370);
	Vaders->Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);
    
    Screen->Flip();
    
}