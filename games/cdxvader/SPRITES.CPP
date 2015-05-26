/**************************************************************************
 Project Name  : SPACEY VADERS
 File Name     : Sprites.cpp
 Programmer    : Paul J Lord (PLord1@aol.com)
/**************************************************************************/
#include "main.h"



/**************************************************************************
 Function: SetupSprites
 
 Purpose: Load the sprite Linked list
/**************************************************************************/
void SetupSprites(void)
{
    
    // Setup the rows
    DWORD row = 0;
    DWORD maxrow = LevelInfo[Level].NumberOfRows;
    DWORD perrow = 0;
    DWORD maxperrow = LevelInfo[Level].VadersPerRow;
    
    // Starting position
    DWORD xlocation=10;
    DWORD ylocation=60;
    
    // VaderType/Frame
    DWORD vadertype = TYPEVADER1; 
    DWORD vaderframe = TYPEVADER1;

	CDXSprite *s;


    dprintf(fdebug,"Init the vaders\n"); // debug:

    vaderdirection=1;  // 1 = right, -1= left
    vaderindex = 0;

    for (row = 0; row < maxrow; row++)
    {
        ylocation += 50;  // Position of the row
        
        for (perrow = 0; perrow < maxperrow; perrow++)
        {
            xlocation += 50;     // Added 50 to each position
            
            vaderindex++;  // total number of vaders
            
            dprintf(fdebug," index=%d  x=%d, y=%d, type=%d row=%d perrow=%d\n",vaderindex,xlocation,ylocation,vadertype,row,perrow);
    
			s = new CDXSprite();
			s->Create(VadersTile);					// Tile OffScreen Buffer
			s->SetType(vadertype);					// Sprite Type
			s->SetPosX(xlocation);					// Xpos of Sprite
			s->SetPosY(ylocation);					// Ypos of Sprite
			s->SetVelX(1);							// XVel of Sprite
			s->SetVelY(0);							// YVel of Sprite
			s->SetState(ALIVE);						// IT's ALIVE!!!
			s->SetFrame(vaderframe);				// Frame to start with
			s->SetDelay(LevelInfo[Level].VaderDelay);		// Set but, value is global for Vaders. 

			Sprites.AddSprite(s);
        }
    
        // Reset the x location    
        xlocation = 10;
        
        // rotate the types
        if (vadertype == TYPEVADER1) 
        {   
            vadertype = TYPEVADER2;
            vaderframe = TYPEVADER2;
        }            
        else if (vadertype == TYPEVADER2) 
        {   
            vadertype = TYPEVADER3;
            vaderframe = TYPEVADER3;
        }            
        else if (vadertype == TYPEVADER3) 
        {   
            vadertype = TYPEVADER1;
            vaderframe = TYPEVADER1;
        }            
        
    
    }
    
    vaderspeedadjust = LevelInfo[Level].VaderSpeed/vaderindex;
    dprintf(fdebug, "vaderspeedadjust = %ld\n",vaderspeedadjust);
}




/**************************************************************************
 Function: MoveVaderObjects
 
 Purpose: Move the Vader objects into new positions
/**************************************************************************/
void MoveVaderObjects(void)
{

// Move the spacey vaders.

	CDXSprite* Node;
	CDXSprite* s;
    
    UINT movedown = 0;

    UINT direction = vaderdirection;
    UINT firstframe = 0;
    UINT lastframe = 0;

    UINT VaderToFire = 0;
    UINT VaderToFirePosX = 0;
    UINT VaderToFirePosY = 0;
    
    UINT VaderAliveIndex = 0;

    

	// Loop the list and update the sprites
	for(Node = Sprites.GetFirst(); Node != NULL; Node = Sprites.GetNext(Node))
	{
       // What state
        switch(Node->GetType())
        {
          
            case TYPEBUG1:    
            {
                if (Node->GetState() == ALIVE)
                {
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 3;


                    Node->SetDelay(Node->GetDelay()-1);
                    if (Node->GetDelay() <= 0)
                    {
                        Node->SetDelay(LevelInfo[Level].VaderDelay);
                         // Move the Animation
                		Node->SetFrame(Node->GetFrame()+1);
                
                        if (Node->GetFrame() > (int)lastframe)
                        {
                            Node->SetFrame(firstframe);
                        }
                    }                        


            		Node->SetPosX(Node->GetPosX() + Node->GetVelX());

            		if( (Node->GetPosX() < 0) || (Node->GetPosX() > (640 - Node->GetTile()->GetBlockWidth())) ) 
                    {
                        Node->SetFrame(TYPEBLANK);
                        Node->SetState(EMPTY);
                    }
                    

                }
                else if (Node->GetState() == EMPTY)
                {
                    // Delete the sprite from the list
			        //Sprites.DelSprite(Sprites.Next());
                    
                }
                
        
            } break;

            case TYPEBUG2:    
            {
                if (Node->GetState() == ALIVE)
                {
                    // The firstframe = the Sprite type (first frame of
                    // the sprite animation)
                    // The lastframe will be equal to the Type+3.
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 3;


                    Node->SetDelay(Node->GetDelay()-1);
                    if (Node->GetDelay() <= 0)
                    {
                        Node->SetDelay(LevelInfo[Level].VaderDelay);
                         // Move the Animation
                		Node->SetFrame(Node->GetFrame()+1);
                
                        if (Node->GetFrame() > (int)lastframe)
                        {
                            Node->SetFrame(firstframe);
                        }
                    }                        

            
            		Node->SetPosX(Node->GetPosX() + Node->GetVelX());
            		Node->SetPosY(Node->GetPosY() - Node->GetVelY());

            		if( (Node->GetPosX() < 0) || (Node->GetPosX() > (640 - Node->GetTile()->GetBlockWidth())) ) 
                    {
                        Node->SetFrame(TYPEBLANK);
                        Node->SetState(EMPTY);
                    }

            		if( (Node->GetPosY() < 0) || (Node->GetPosY() > (480 - Node->GetTile()->GetBlockHeight())) )
                    {
                        Node->SetFrame(TYPEBLANK);
                        Node->SetState(EMPTY);
                    }

            
                }
                else if (Node->GetState() == EMPTY)
                {
                    // Delete the sprite from the list
			        //Sprites.DelSprite(Sprites.Next());
                }
                
        
            } break;
        

            case TYPEEXPLODE0:    
            {
                if (Node->GetState() == ALIVE)
                {
                    // The firstframe = the Sprite type (first frame of
                    // the sprite animation)
                    // The lastframe will be equal to the Type+3.
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 4;

                     // Rotate the Animation
            		Node->SetFrame(Node->GetFrame()+1);
                
                    if (Node->GetFrame() > (int)lastframe)
                    {
                        // Change into Bug...
                        Node->SetType(TYPEBUG1);
                        Node->SetFrame(TYPEBUG1);
		                Node->SetVelX(-5);
                        
                        //Set the UFO back to Not Active
                        if (Ufo_Active == TRUE)
                        {
                            Ufo_Active = FALSE;
                        }
                        
                    }
            
                }
                
                
            } break;
        
            case TYPEEXPLODE1:    
            {
            
                if (Node->GetState() == ALIVE)
                {
                    // The firstframe = the Sprite type (first frame of
                    // the sprite animation)
                    // The lastframe will be equal to the Type+3.
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 4;

                     // Rotate the Animation
            		Node->SetFrame(Node->GetFrame()+1);
                
                    if (Node->GetFrame() > (int)lastframe)
                    {
                        // Change into Bug...
                        Node->SetType(TYPEBUG2);
                        Node->SetFrame(TYPEBUG2);
		                Node->SetVelX(rand()%4);
                		Node->SetVelY(rand()%6);
                        
                        // Make sure the X or Y are > 1
                        if (Node->GetVelX() < 2)   Node->SetVelX(2);
                        if (Node->GetVelY() < 2)   Node->SetVelY(3);
                        
                    }
            
                }
                
                
            } break;
        
        
            case TYPEEXPLODE2:    
            {
            
                if (Node->GetState() == ALIVE)
                {
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 4;

                     // Rotate the Animation
            		Node->SetFrame(Node->GetFrame()+1);
                
                    if (Node->GetFrame() > (int)lastframe)
                    {
                        Node->SetState(EMPTY);
                        Node->SetFrame(TYPEBLANK);
                    }
            
                }
                else if (Node->GetState() == EMPTY)
                {
                    // Delete the sprite from the list
			        //Sprites.DelSprite(Sprites.Next());
                }
                
            } break;
        
            case TYPEVADER1:    
            case TYPEVADER2:    
            case TYPEVADER3:    
            {

                if (Node->GetState() == ALIVE)
                {
                    // Count the number of vaders that are Left.
                    VaderAliveIndex++;
                    
                    // Increase the position
            		Node->SetPosX(Node->GetPosX() + vaderdirection);
                    
                    // The firstframe = the Sprite type (first frame of
                    // the sprite animation)
                    // The lastframe will be equal to the Type+3.
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 3;

           
                    Node->SetDelay(Node->GetDelay()-1);
                    if (Node->GetDelay() <= 0)
                    {
                        Node->SetDelay(LevelInfo[Level].VaderDelay);
                         // Move the Animation
                		Node->SetFrame(Node->GetFrame()+1);
                
                        if (Node->GetFrame() > (int)lastframe)
                        {
                            Node->SetFrame(firstframe);
                        }
                    }                        
           
           
                    // Check for max/min movements
                    if (Node->GetPosX() > MAXMOVEX)  
                    {
                        movedown=1;
                        direction = -1;  
                    }
               
                    if (Node->GetPosX() < MINMOVEX)  
                    {
                        movedown=1;
                        direction = 1;  
                    }
           
           
                    // Check to Add a Firing back Sprite.
                    if (VMissile_Active == TRUE)
                    {
                    
                        if (VaderToFirePosX == 0)
                        {
                            VaderToFirePosX = Node->GetPosX();
                    		VaderToFirePosY = Node->GetPosY();
                        }
                        
                            // Field of vision for the Vaders... I SEE YOU...
                            if (SpriteDistance(Player->GetPosX(), Player->GetPosY(), Node->GetPosX(), Node->GetPosY()) < 250)
                            {
                		        VaderToFirePosX = Node->GetPosX();
                		        VaderToFirePosY = Node->GetPosY();
                            }    
                        
                    }
           
           
                }
            
            } break;


           
        }
        
	}



    // set the new direction if found.  Do not want to change while in the loop... BAD Things!
    vaderdirection = direction;


    // Increase the Vader speed depending on the number of vaders left...
    LevelInfo[Level].VaderSpeed = (VaderAliveIndex * vaderspeedadjust) + vadermindelay ;
        
    dprintf(fdebug, "vaderspeed = %d\n",LevelInfo[Level].VaderSpeed);


    // Moved down...
    if (movedown == 1 && hitbottom != 1)
    {
	    // Loop the list and update the sprites
		for(Node = Sprites.GetFirst(); Node != NULL; Node = Sprites.GetNext(Node))
	    {
            // What state
            switch(Node->GetType())
            {
            
                 case TYPEVADER1:    
                 case TYPEVADER2:    
                 case TYPEVADER3:    
                 {
           
                     if (Node->GetState() == ALIVE)
                     {
                     
                		 Node->SetPosY(Node->GetPosY() + 20);
                         if (Node->GetPosY() > MAXMOVEY)  
                         {
                             hitbottom = 1;
                    		 //Node->GetPosY() = MAXMOVEY;
                         }
                         
                     }
        
                 } break;

            }
        }

    }



    // Vader is Firing!
    if (VMissile_Active == TRUE)
    {
		s = new CDXSprite();
		s->Create(VadersTile);					// Tile OffScreen Buffer
		s->SetType(TYPEMISSILE2);				// Sprite Type
		s->SetPosX(VaderToFirePosX);			// Xpos of Sprite
		s->SetPosY(VaderToFirePosY+24);			// Ypos of Sprite
		s->SetVelX(0);							// XVel of Sprite
		s->SetVelY(-1);							// YVel of Sprite
		s->SetState(ALIVE);						// IT's ALIVE!!!
		s->SetFrame(TYPEMISSILE2);				// Frame to start with
		s->SetDelay(LevelInfo[Level].VMissileDelay);		// Set but, value is global for Vaders. 

		Sprites.AddSprite(s);

        VMissile_Active = FALSE;
        if (SoundFlag == 1) VMISSILE.Play();
        
    }
    

    // Check to see if Vaders have Hit the Bottom.
    if (hitbottom == 1)
    {
        if (SoundFlag == 1) XPLODE.Play();
        
        PlayerLives--;
        dprintf(fdebug,"Hit BOTTOM ... PlayerLives %d\n",PlayerLives);
        Player->SetFrame(6);
        
        ProgramState = PS_GAMEOVER;
        
    }        
    // Check to see if any Vaders are left, If Not NEXT LEVEL!
    else if (VaderAliveIndex == 0)
    {
        VMissile_Active = FALSE;
	    ProgramState = PS_NEXTLEVEL;
        Level++;
        if (Level > 3)  Level = 1;
        // PS_WINNER
        // PS_SPLASH
    }




}




/**************************************************************************
 Function: MoveUfoObjects
 
 Purpose: Move the Ufo Objects
/**************************************************************************/
void MoveUfoObjects(void)
{

// Move the UFO

	CDXSprite* Node;
    
    UINT firstframe = 0;
    UINT lastframe = 0;

	// Loop the list and update the sprites
	for(Node = Sprites.GetFirst(); Node != NULL; Node = Sprites.GetNext(Node))
	{
       // What state
        switch(Node->GetType())
        {
        
            case TYPEUFO:    
            {

                if (Node->GetState() == ALIVE)
                {
                
            		Node->SetPosX(Node->GetPosX() + 2);
                    
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 3;




                    Node->SetDelay(Node->GetDelay()-1);
                    if (Node->GetDelay() <= 0)
                    {
                        Node->SetDelay(LevelInfo[Level].UfoDelay);
                         // Move the Animation
                		Node->SetFrame(Node->GetFrame()+1);
                
                        if (Node->GetFrame() > (int)lastframe)
                        {
                            Node->SetFrame(firstframe);
                        }
                    }                        

                      
                    if (Node->GetPosX() > 600)
                    {
                        Node->SetState(NOTALIVE);
                        Ufo_Active = FALSE;
                    }
                
                }
                else if (Node->GetState() == NOTALIVE)
                {
                    Node->SetFrame(TYPEBLANK);
                    Node->SetState(EMPTY);
                    
                }
                else if (Node->GetState() == EMPTY)
                {
                    // Delete the sprite from the list
                //    Ufo_Active = FALSE;
//			        Sprites.DelSprite(Sprites.Next());
                }
                
                
                
            }   break;   
            
           
        } // switch
        
	} // for



}



/**************************************************************************
 Function: MoveMissileObjects
 
 Purpose: Move the Vader/Player Missiles
/**************************************************************************/
void MoveMissileObjects(void)
{

// Move the Missiles.

	CDXSprite* Node;
    
    UINT firstframe = 0;
    UINT lastframe = 0;

	// Loop the list and update the sprites
	for(Node = Sprites.GetFirst(); Node != NULL; Node = Sprites.GetNext(Node))
	{
       // What state
        switch(Node->GetType())
        {
        
            case TYPEMISSILE1:    
            {

                if (Node->GetState() == ALIVE)
                {
            		Node->SetPosY(Node->GetPosY() - 2);
                    
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 1;


                    Node->SetDelay(Node->GetDelay()-1);
                    if (Node->GetDelay() <= 0)
                    {
                        Node->SetDelay(LevelInfo[Level].MissileDelay);
                         // Move the Animation
                		Node->SetFrame(Node->GetFrame()+1);
                
                        if (Node->GetFrame() > (int)lastframe)
                        {
                            Node->SetFrame(firstframe);
                        }
                    }                        
                      
                      
                    // Check for offscreen  
                    if (Node->GetPosY() < 10)
                    {
                        Node->SetState(NOTALIVE);
                        Missile_Active--;
                    }
                
                
                    // Check for player hit by own missile
                    if (CheckPlayerHit(Node->GetPosX(), Node->GetPosY()) == TRUE)
                    {
                        Node->SetState(NOTALIVE);
                    }
                    else
                    if (CheckMissileHit(Node->GetPosX(), Node->GetPosY()) == TRUE)
                    {
                        Node->SetState(NOTALIVE);
                        Missile_Active--;
                    }
                    
                }
                else if (Node->GetState() == NOTALIVE)
                {
                    Node->SetFrame(TYPEBLANK);
                    Node->SetState(EMPTY);
                    
                }
                else if (Node->GetState() == EMPTY)
                {
                    // Delete the sprite from the list
                    // make sure this is the last thing you do...
			        Sprites.DelSprite(Node);
                }
                
                
                
            }   break;   
            
            case TYPEMISSILE2:    
            {

                if (Node->GetState() == ALIVE)
                {
            		Node->SetPosY(Node->GetPosY() + 2);
                    
                    firstframe = Node->GetType();
                    lastframe = Node->GetType() + 1;

                    Node->SetDelay(Node->GetDelay()-1);
                    if (Node->GetDelay() <= 0)
                    {
                        Node->SetDelay(LevelInfo[Level].VMissileDelay);
                         // Move the Animation
                		Node->SetFrame(Node->GetFrame()+1);
                
                        if (Node->GetFrame() > (int)lastframe)
                        {
                            Node->SetFrame(firstframe);
                        }
                    }                        
                      
                    // Check for offscreen  
                    if (Node->GetPosY() > MAXMOVEY+48)
                    {
                        Node->SetState(NOTALIVE);
                    }
                
                    // Check for hit
                    BOOL HitIt = CheckPlayerHit(Node->GetPosX(), Node->GetPosY());
                    if (HitIt == TRUE)
                    {
                        Node->SetState(NOTALIVE);
                    }
                    
                }
                else if (Node->GetState() == NOTALIVE)
                {
                    Node->SetFrame(TYPEBLANK);
                    Node->SetState(EMPTY);
                    
                }
                else if (Node->GetState() == EMPTY)
                {
                    // Delete the sprite from the list
                }
               
               
            }   break;   
                
           
        } // switch
        
	} // for



}



/**************************************************************************
 Function: CheckMissileHit
 
 Purpose: Check to see if the Missile has hit the sprite.  Return TRUE/FALSE
/**************************************************************************/
BOOL CheckMissileHit(int xmissile, int ymissile)
{

	CDXSprite* Node;

    // top,left,bottom,right
    RECT Rect1;
    RECT Rect2;

    // Make the box of the missile smaller make is a 28x28
    Rect1.top   =  xmissile+20; 
    Rect1.left   = ymissile+20;
    Rect1.bottom = xmissile + 28;
    Rect1.right  = ymissile + 28;
    
	// Loop the list and update the sprites
	for(Node = Sprites.GetFirst(); Node != NULL; Node = Sprites.GetNext(Node))
	{
        if (Node->GetState() == ALIVE)
        {

// Make the box 43x43
            Rect2.top   =  Node->GetPosX()+5; 
            Rect2.left   = Node->GetPosY()+5;
            Rect2.bottom = Node->GetPosX() + 43;
            Rect2.right  = Node->GetPosY() + 43;

            if (Rect1.top < Rect2.bottom &&
                Rect1.bottom > Rect2.top &&
                Rect1.right > Rect2.left &&
                Rect1.left < Rect2.right)
            
            {
            
                           
                 
                 // What state
                    switch(Node->GetType())
                    {
                    
                 // If BUGTYPE1/2 then - score! Do not hit them...
                        case TYPEBUG1:
                        case TYPEBUG2:
                        {
                            AddToScore -= 100;
                            Node->SetType( TYPEEXPLODE2);
                            Node->SetFrame(TYPEEXPLODE2);
                    	    if (SoundFlag == 1) XPLODE.Play();
                            return TRUE;
                        } break;  
                          
                        case TYPEUFO:
                        {
                            AddToScore += 100;
                            Node->SetType(TYPEEXPLODE0);
                            Node->SetFrame(TYPEEXPLODE0);
                            if (SoundFlag == 1) XPLODE.Play();
                            return TRUE;
                        } break;    
                        
                        case TYPEVADER1:
                        {
                            vaderindex--; 
                            AddToScore += 75;
                            Node->SetType(TYPEEXPLODE1);
                            Node->SetFrame(TYPEEXPLODE1);
                            if (SoundFlag == 1) XPLODE.Play();
                            return TRUE;
                        } break;
                        case TYPEVADER2:
                        {
                            vaderindex--; 
                            AddToScore += 50;
                            Node->SetType(TYPEEXPLODE1);
                            Node->SetFrame(TYPEEXPLODE1);
                            if (SoundFlag == 1) XPLODE.Play();
                            return TRUE;
                        } break;
                        case TYPEVADER3:
                        {
                            vaderindex--; 
                            AddToScore += 25;
                            Node->SetType(TYPEEXPLODE1);
                            Node->SetFrame(TYPEEXPLODE1);
                            if (SoundFlag == 1) XPLODE.Play();
                            return TRUE;
                        } break;
                        
                    } // switch
                            
            }
        
        }    
        
    } // for
    return FALSE;

}



/**************************************************************************
 Function: CheckPlayerHit
 
 Purpose: Check to see if the player has been hit.  Return TRUE/FALSE
/**************************************************************************/
BOOL CheckPlayerHit(int xcheck, int ycheck)
{

// top,left,bottom,right
    RECT Rect1;
    RECT Rect2;

    if (GodModeFlag == 1 || Player->GetFrame() > 2) return FALSE;

// Make the box of the missile smaller make is a 28x28
    Rect1.top   =  xcheck+20; 
    Rect1.left   = ycheck+20;
    Rect1.bottom = xcheck + 28;
    Rect1.right  = ycheck + 28;
    

// Make the Player Sprite 43x33
    Rect2.top   =  Player->GetPosX()+15; 
    Rect2.left   = Player->GetPosY()+5;
    Rect2.bottom = Player->GetPosX() + 33;
    Rect2.right  = Player->GetPosY() + 43;

    if (Rect1.top < Rect2.bottom &&
        Rect1.bottom > Rect2.top &&
        Rect1.right > Rect2.left &&
        Rect1.left < Rect2.right)
    
    {

        if (SoundFlag == 1) XPLODE.Play();
        
        PlayerLives--;
        dprintf(fdebug,"PlayerLives %d\n",PlayerLives);
        Player->SetFrame(3);
        
        // Modify the speed of the explosion
        PlayerDelay = 100;

        return TRUE;

    }
    return FALSE;

}




/**************************************************************************
 Function: SpriteDistance
 
 Purpose: Get the Distance between sprites
/**************************************************************************/
float SpriteDistance(int x1, int y1, int x2, int y2)
{

    float distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
    
    dprintf(fdebug,"distance = %fd\n", distance);
    
    return distance;
}
