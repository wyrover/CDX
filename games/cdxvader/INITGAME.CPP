/**************************************************************************
 Project Name  : SPACEY VADERS
 File Name     : InitGame.cpp
 Programmer    : Paul J Lord (PLord1@aol.com)
/**************************************************************************/
#include "main.h"




/**************************************************************************
 Function: InitGame
 
 Purpose: Initalize DirectDraw Objects using the CDX Class
/**************************************************************************/
void InitGame(void)
{
    // Setup Text Color Vars
    YellowTxt = RGB(255,255,0);
    RedTxt = RGB(255,0,0);
    GreenTxt = RGB(0,255,0);
    BlueTxt = RGB(0,0,255);

    // Performance Counter
    QueryPerformanceFrequency( (LARGE_INTEGER*)&freqPerformance );
    
    // Testing, Turn off the PerformanceCounter.
    //freqPerformance = 0;

    LoadINIVars();
    // Make sure we have a INI file...
    // Need to do a check for the file.
    SaveINIVars();

    // Debug open Yes no No??
    if (DebugFlag == 1) fdebug = fopen("debug.txt","a+");
    
	// Setup the screen
	Screen = new CDXScreen;
	Screen->CreateFullScreen(hWnd, 640, 480, 8);
	Screen->LoadPalette(TITLEBMP);
	Screen->LoadBitmap(TITLEBMP);
    
	// Load the player ship
	Player = new CDXSprite;
	Player->Create(Screen, PLAYERBMP, 48, 48, 8);
	Player->SetPos(304, 430);
	Player->SetColorKey(0);
    Player->SetFrame(0);

	// Load the Space Invaders
	VadersTile = new CDXTile;
	VadersTile->Create(Screen, VADERSBMP, 48, 48, 48);

	Vaders = new CDXSprite;
	Vaders->Create(VadersTile);

	// Set the bitmap transparent color
	Vaders->SetColorKey(0);
	VadersTile->SetColorKey(0);

	SpaceTile1 = new CDXTile;
	SpaceTile1->Create(Screen, SPACEBMP, 32, 32, 99);
	SpaceTile1->SetColorKey(0);

	int Tile;
    // Create a Make for Back stars
	SpaceMap1= new CDXMap(SpaceTile1, Screen);
	SpaceMap1->Create(20, 15, 0);
	SpaceMap1->MoveTo(0,0);
	SpaceMap1->EnableScrollWrapping(TRUE);

	Tile = 1;
	for(int j = 0; j < SpaceMap1->GetMapHeight(); j++)
	{
		for(int i = 0; i < SpaceMap1->GetMapWidth(); i++)
		{
			SpaceMap1->SetTile(i, j, Tile);
			Tile++;
			if(Tile > 99) Tile = 1;
		}
	}

	// Start the music
	Music = new CDXMusic(hWnd);
    
    // Create the sound object
	Sound = new CDXSound;
	Sound->Create(hWnd);

	MISSILE.Load(Sound,  MISSILEWAV );
	XPLODE.Load(Sound,   XPLOSWAV   );
	VMISSILE.Load(Sound, VMISSILEWAV);

    // Set the Program state
	ProgramState = PS_SPLASH;
    
	// Seed the random-number generator with current time
	srand((unsigned)time(NULL));
    
    ResetGame();
    
}


/**************************************************************************
 Function: ResetLevel
 
 Purpose: Reset the level variables
/**************************************************************************/
void ResetLevel(void)
{
    // Clear/Set counter vars
    hitbottom = 0;
    vaderdirection = 1;
    
    vadermindelay = 20;
    
    Missile_Active = 0;
    Missile_Active = FALSE;
    VMissile_Active = FALSE;
    Ufo_Active = FALSE;
    
    // Reset Player position
    Player->SetPos(304, 430);
    Player->SetFrame(0);
    
}

    
/**************************************************************************
 Function: ResetGame
 
 Purpose: Reset the world variables
/**************************************************************************/
void ResetGame(void)
{
    Level = 1;
    TotalScore = 0;
    AddToScore = 0;
    Fps = 0;
}    
    
    
/**************************************************************************
 Function: LoadINIVars
 
 Purpose: Load the variables for the game from cdxvader.ini
/**************************************************************************/
void LoadINIVars(void)
{


    //
    // Load Level #1
    //
    
    // Number Of Rows
    LevelInfo[1].NumberOfRows = 
        GetPrivateProfileInt("LEVEL1", "NumberOfRows", 3, "cdxvader.ini");   
    // Check for Valid range        
    if(LevelInfo[1].NumberOfRows > 6 || LevelInfo[1].NumberOfRows < 1)
        LevelInfo[1].NumberOfRows = 6;

    // VadersPerRow
    LevelInfo[1].VadersPerRow = 
        GetPrivateProfileInt("LEVEL1", "VadersPerRow", 10, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[1].VadersPerRow > 10 || LevelInfo[1].VadersPerRow < 1)
        LevelInfo[1].VadersPerRow = 10;

    // VaderSpeed
    LevelInfo[1].VaderSpeed = 
        GetPrivateProfileInt("LEVEL1", "VaderSpeed", 100, "cdxvader.ini"); 
    // Check for Valid range        
    if(LevelInfo[1].VaderSpeed > 300 || LevelInfo[1].VaderSpeed < 20)
        LevelInfo[1].VaderSpeed = 100;

    // VaderDelay
    LevelInfo[1].VaderDelay = 
        GetPrivateProfileInt("LEVEL1", "VaderDelay", 2, "cdxvader.ini"); 
    // Check for Valid range        
    if(LevelInfo[1].VaderDelay > 10 || LevelInfo[1].VaderDelay < 1) 
        LevelInfo[1].VaderDelay = 2;

    // UfoSpeed
    LevelInfo[1].UfoSpeed = 
        GetPrivateProfileInt("LEVEL1", "UfoSpeed", 50, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[1].UfoSpeed > 100 || LevelInfo[1].UfoSpeed < 5) 
        LevelInfo[1].UfoSpeed = 50;


    // UfoDelay = 50;
    LevelInfo[1].UfoDelay = 
        GetPrivateProfileInt("LEVEL1", "UfoDelay", 10, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[1].UfoDelay > 20 || LevelInfo[1].UfoDelay < 5) 
        LevelInfo[1].UfoDelay = 10;


    // UfoFreq = 3500;
    LevelInfo[1].UfoFreq = 
        GetPrivateProfileInt("LEVEL1", "UfoFreq", 3500, "cdxvader.ini");  
    // Check for Valid range        
    if(LevelInfo[1].UfoFreq > 10000 || LevelInfo[1].UfoFreq < 500) 
        LevelInfo[1].UfoFreq = 3500;


    // VMissileFreq = 1000;
    LevelInfo[1].VMissileFreq = 
        GetPrivateProfileInt("LEVEL1", "VMissileFreq", 1200, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[1].VMissileFreq > 5000 || LevelInfo[1].VMissileFreq < 500) 
        LevelInfo[1].VMissileFreq = 1200;


    // VMissileDelay
    LevelInfo[1].VMissileDelay = 
        GetPrivateProfileInt("LEVEL1", "VMissileDelay", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[1].VMissileDelay > 20 || LevelInfo[1].VMissileDelay < 5) 
        LevelInfo[1].VMissileDelay = 10;

    // MissileSpeed
    LevelInfo[1].MissileSpeed = 
        GetPrivateProfileInt("LEVEL1", "MissileSpeed", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[1].MissileSpeed > 50 || LevelInfo[1].MissileSpeed < 5) 
        LevelInfo[1].MissileSpeed = 10;
    
    // MissileDelay
    LevelInfo[1].MissileDelay = 
        GetPrivateProfileInt("LEVEL1", "MissileDelay", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[1].MissileDelay > 20 || LevelInfo[1].MissileDelay < 5) 
        LevelInfo[1].MissileDelay = 10;



    //
    // Load Level #2
    //

    // Number Of Rows
    LevelInfo[2].NumberOfRows = 
        GetPrivateProfileInt("LEVEL2", "NumberOfRows", 4, "cdxvader.ini");   
    // Check for Valid range        
    if(LevelInfo[2].NumberOfRows > 6 || LevelInfo[2].NumberOfRows < 1)
        LevelInfo[2].NumberOfRows = 6;

    // VadersPerRow
    LevelInfo[2].VadersPerRow = 
        GetPrivateProfileInt("LEVEL2", "VadersPerRow", 10, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[2].VadersPerRow > 10 || LevelInfo[2].VadersPerRow < 1)
        LevelInfo[2].VadersPerRow = 10;

    // VaderSpeed
    LevelInfo[2].VaderSpeed = 
        GetPrivateProfileInt("LEVEL2", "VaderSpeed", 100, "cdxvader.ini"); 
    // Check for Valid range        
    if(LevelInfo[2].VaderSpeed > 300 || LevelInfo[2].VaderSpeed < 20)
        LevelInfo[2].VaderSpeed = 100;

    // VaderDelay
    LevelInfo[2].VaderDelay = 
        GetPrivateProfileInt("LEVEL2", "VaderDelay", 2, "cdxvader.ini"); 
    // Check for Valid range        
    if(LevelInfo[2].VaderDelay > 10 || LevelInfo[2].VaderDelay < 1) 
        LevelInfo[2].VaderDelay = 2;

    // UfoSpeed
    LevelInfo[2].UfoSpeed = 
        GetPrivateProfileInt("LEVEL2", "UfoSpeed", 50, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[2].UfoSpeed > 100 || LevelInfo[2].UfoSpeed < 5) 
        LevelInfo[2].UfoSpeed = 50;


    // UfoDelay = 50;
    LevelInfo[2].UfoDelay = 
        GetPrivateProfileInt("LEVEL2", "UfoDelay", 10, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[2].UfoDelay > 20 || LevelInfo[2].UfoDelay < 5) 
        LevelInfo[2].UfoDelay = 10;


    // UfoFreq = 3500;
    LevelInfo[2].UfoFreq = 
        GetPrivateProfileInt("LEVEL2", "UfoFreq", 3500, "cdxvader.ini");  
    // Check for Valid range        
    if(LevelInfo[2].UfoFreq > 10000 || LevelInfo[2].UfoFreq < 500) 
        LevelInfo[2].UfoFreq = 3500;


    // VMissileFreq = 1000;
    LevelInfo[2].VMissileFreq = 
        GetPrivateProfileInt("LEVEL2", "VMissileFreq", 1200, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[2].VMissileFreq > 5000 || LevelInfo[2].VMissileFreq < 500) 
        LevelInfo[2].VMissileFreq = 1200;

    // VMissileDelay
    LevelInfo[2].VMissileDelay = 
        GetPrivateProfileInt("LEVEL2", "VMissileDelay", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[2].VMissileDelay > 20 || LevelInfo[2].VMissileDelay < 5) 
        LevelInfo[2].VMissileDelay = 10;

    // MissileSpeed
    LevelInfo[2].MissileSpeed = 
        GetPrivateProfileInt("LEVEL2", "MissileSpeed", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[2].MissileSpeed > 50 || LevelInfo[2].MissileSpeed < 5) 
        LevelInfo[2].MissileSpeed = 10;
    
    // MissileDelay
    LevelInfo[2].MissileDelay = 
        GetPrivateProfileInt("LEVEL2", "MissileDelay", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[2].MissileDelay > 20 || LevelInfo[2].MissileDelay < 5) 
        LevelInfo[2].MissileDelay = 10;


    //
    // Load Level #3
    //

    // Number Of Rows
    LevelInfo[3].NumberOfRows = 
        GetPrivateProfileInt("LEVEL3", "NumberOfRows", 6, "cdxvader.ini");   
    // Check for Valid range        
    if(LevelInfo[3].NumberOfRows > 6 || LevelInfo[3].NumberOfRows < 1)
        LevelInfo[3].NumberOfRows = 6;

    // VadersPerRow
    LevelInfo[3].VadersPerRow = 
        GetPrivateProfileInt("LEVEL3", "VadersPerRow", 10, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[3].VadersPerRow > 10 || LevelInfo[3].VadersPerRow < 1)
        LevelInfo[3].VadersPerRow = 10;

    // VaderSpeed
    LevelInfo[3].VaderSpeed = 
        GetPrivateProfileInt("LEVEL3", "VaderSpeed", 100, "cdxvader.ini"); 
    // Check for Valid range        
    if(LevelInfo[3].VaderSpeed > 300 || LevelInfo[3].VaderSpeed < 20)
        LevelInfo[3].VaderSpeed = 100;

    // VaderDelay
    LevelInfo[3].VaderDelay = 
        GetPrivateProfileInt("LEVEL3", "VaderDelay", 2, "cdxvader.ini"); 
    // Check for Valid range        
    if(LevelInfo[3].VaderDelay > 10 || LevelInfo[3].VaderDelay < 1) 
        LevelInfo[3].VaderDelay = 2;

    // UfoSpeed
    LevelInfo[3].UfoSpeed = 
        GetPrivateProfileInt("LEVEL3", "UfoSpeed", 50, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[3].UfoSpeed > 100 || LevelInfo[3].UfoSpeed < 5) 
        LevelInfo[3].UfoSpeed = 50;


    // UfoDelay = 50;
    LevelInfo[3].UfoDelay = 
        GetPrivateProfileInt("LEVEL3", "UfoDelay", 10, "cdxvader.ini");
    // Check for Valid range        
    if(LevelInfo[3].UfoDelay > 20 || LevelInfo[3].UfoDelay < 5) 
        LevelInfo[3].UfoDelay = 10;

    // UfoFreq = 3500;
    LevelInfo[3].UfoFreq = 
        GetPrivateProfileInt("LEVEL3", "UfoFreq", 3500, "cdxvader.ini");  
    // Check for Valid range        
    if(LevelInfo[3].UfoFreq > 10000 || LevelInfo[3].UfoFreq < 500) 
        LevelInfo[3].UfoFreq = 3500;


    // VMissileFreq = 1000;
    LevelInfo[3].VMissileFreq = 
        GetPrivateProfileInt("LEVEL3", "VMissileFreq", 1200, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[3].VMissileFreq > 5000 || LevelInfo[3].VMissileFreq < 500) 
        LevelInfo[3].VMissileFreq = 1200;

    // VMissileDelay
    LevelInfo[3].VMissileDelay = 
        GetPrivateProfileInt("LEVEL3", "VMissileDelay", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[3].VMissileDelay > 20 || LevelInfo[3].VMissileDelay < 5) 
        LevelInfo[3].VMissileDelay = 10;

    // MissileSpeed
    LevelInfo[3].MissileSpeed = 
        GetPrivateProfileInt("LEVEL3", "MissileSpeed", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[3].MissileSpeed > 50 || LevelInfo[3].MissileSpeed < 5) 
        LevelInfo[3].MissileSpeed = 10;
    
    // MissileDelay
    LevelInfo[3].MissileDelay = 
        GetPrivateProfileInt("LEVEL3", "MissileDelay", 10, "cdxvader.ini");  
    // Make sure its not out of range.
    if(LevelInfo[3].MissileDelay > 20 || LevelInfo[3].MissileDelay < 5) 
        LevelInfo[3].MissileDelay = 10;



// Read the Player information
    // For Player Animation
    // PlayerDelay
    PlayerDelay = GetPrivateProfileInt("PLAYER", "PlayerDelay", 80, "cdxvader.ini");  
    // Make sure its not out of range.
    if(PlayerDelay > 100 || PlayerDelay < 20)   PlayerDelay = 80;

    // PlayerLives
    PlayerLives = GetPrivateProfileInt("PLAYER", "PlayerLives", 3, "cdxvader.ini");  
    // Make sure its not out of range.
    if(PlayerLives > 6 || PlayerLives < 1)  PlayerLives = 3;

    // PlayerMissile
    PlayerMissiles = GetPrivateProfileInt("PLAYER", "PlayerMissiles", 2, "cdxvader.ini");  
    // Make sure its not out of range.
    if(PlayerMissiles > 10 || PlayerMissiles < 1) PlayerMissiles = 2;

    // For Star Field
    StarDelay = GetPrivateProfileInt("PLAYER", "StarDelay", 80, "cdxvader.ini");  
    // Make sure its not out of range.
    if(StarDelay > 150 || StarDelay < 80) StarDelay = 80;

    MusicFlag = GetPrivateProfileInt("PLAYER", "MusicFlag", 1, "cdxvader.ini");  
    // Make sure its not out of range.
    if(MusicFlag > 1 || MusicFlag < 0) MusicFlag = 1;

    SoundFlag = GetPrivateProfileInt("PLAYER", "SoundFlag", 1, "cdxvader.ini");  
    // Make sure its not out of range.
    if(SoundFlag > 1 || SoundFlag < 0) SoundFlag = 1;

    DebugFlag = GetPrivateProfileInt("PLAYER", "DebugFlag", 0, "cdxvader.ini");  
    // Make sure its not out of range.
    if(DebugFlag > 1 || DebugFlag < 0) DebugFlag = 1;

    FPSFlag = GetPrivateProfileInt("PLAYER", "FPSFlag", 0, "cdxvader.ini");  
    // Make sure its not out of range.
    if(FPSFlag > 1 || FPSFlag < 0) FPSFlag = 1;

    GodModeFlag = GetPrivateProfileInt("PLAYER", "GodModeFlag", 0, "cdxvader.ini");  
    // Make sure its not out of range.
    if(GodModeFlag > 1 || GodModeFlag < 0) GodModeFlag = 1;



}


/**************************************************************************
 Function: SaveINIVars
 
 Purpose: Save the variables for the game from cdxvader.ini
/**************************************************************************/
void SaveINIVars(void)
{

// Write out the INI File:

    TCHAR WriteBuffer[260], Section[25];
    LPTSTR lpTemp;


    for (int lev = 1; lev < 4; lev++)
    {

        lpTemp = WriteBuffer;
        wsprintf(Section ,"LEVEL%d",lev);
        
        wsprintf(lpTemp, "NumberOfRows=%d", LevelInfo[lev].NumberOfRows); 
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "VadersPerRow=%d", LevelInfo[lev].VadersPerRow); 
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "VaderSpeed=%d",   LevelInfo[lev].VaderSpeed);   
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "VaderDelay=%d",   LevelInfo[lev].VaderDelay);   
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "UfoSpeed=%d",     LevelInfo[lev].UfoSpeed);     
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "UfoDelay=%d",     LevelInfo[lev].UfoDelay);     
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "UfoFreq=%d",      LevelInfo[lev].UfoFreq);      
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "VMissileFreq=%d", LevelInfo[lev].VMissileFreq); 
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "VMissileDelay=%d",LevelInfo[lev].VMissileDelay);
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "MissileSpeed=%d", LevelInfo[lev].MissileSpeed); 
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "MissileDelay=%d", LevelInfo[lev].MissileDelay); 
        lpTemp += lstrlen(lpTemp) +1;                    
        wsprintf(lpTemp, "%s",     "; end of level");
        lpTemp += lstrlen(lpTemp) +1;            
        memset(lpTemp,0,1);
        
        WritePrivateProfileSection( Section, WriteBuffer, "cdxvader.ini");
        
    }


// Write the Player Information:

    lpTemp = WriteBuffer;
    wsprintf(lpTemp, "PlayerDelay=%d",   PlayerDelay);  
    lpTemp += lstrlen(lpTemp) +1;                    
    wsprintf(lpTemp, "PlayerLives=%d",   PlayerLives);  
    lpTemp += lstrlen(lpTemp) +1;                    
    wsprintf(lpTemp, "PlayerMissiles=%d",PlayerMissiles);
    lpTemp += lstrlen(lpTemp) +1;                    
    wsprintf(lpTemp, "StarDelay=%d",     StarDelay);
    lpTemp += lstrlen(lpTemp) +1;            

    wsprintf(lpTemp, "MusicFlag=%d",  MusicFlag);
    lpTemp += lstrlen(lpTemp) +1;            
    wsprintf(lpTemp, "SoundFlag=%d",  SoundFlag);
    lpTemp += lstrlen(lpTemp) +1;            
    wsprintf(lpTemp, "DebugFlag=%d",  DebugFlag);
    lpTemp += lstrlen(lpTemp) +1;            
    wsprintf(lpTemp, "FPSFlag=%d",    FPSFlag);
    lpTemp += lstrlen(lpTemp) +1;            
    wsprintf(lpTemp, "GodModeFlag=%d",GodModeFlag);
    lpTemp += lstrlen(lpTemp) +1;            
    
    wsprintf(lpTemp, "%s",     "; end of Player Info");
    lpTemp += lstrlen(lpTemp) +1;            
    memset(lpTemp,0,1);
 
    WritePrivateProfileSection( "PLAYER", WriteBuffer, "cdxvader.ini");

}



