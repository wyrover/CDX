/////////////////////////////////////////////////////////////////////////////////////////////
// CDX example  - 
// You must link to winmm.lib, ddraw.lib and cdx.lib
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#define CDXINCLUDEALL
#include <CDX.h>

CDXScreen* Screen       = NULL;  // The screen object, every program must have one
CDXSurface* WorkSurface = NULL;
CDXSurface* Picture     = NULL;
CDXSprite* Sprite1      = NULL;

BOOL bActive = TRUE;  // Is the program running?

/////////////////////////////////////////////////////////////////////////////////////////////
// WinProc
/////////////////////////////////////////////////////////////////////////////////////////////
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP:
      bActive = wParam;
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
		break;

		case WM_DESTROY:
			bActive = FALSE;
			delete Sprite1;
			delete WorkSurface;
			delete Screen;

			PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// InitApp - Create the window and the CDX objects
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL InitApp(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WinProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInst;
	WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(0, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = "CDX Example";
	RegisterClass(&WndClass);

	hWnd = CreateWindow(
		"CDX Example",
		"CDX Example",
 		WS_OVERLAPPEDWINDOW,
		0,
		0,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);

	if(!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Create the CDXSreen object
	Screen = new CDXScreen;
	if (FAILED(Screen->CreateFullScreen(hWnd, 640, 480, 16)))
		return FALSE;

	//if (FAILED(Screen->CreateWindowed(hWnd, 640, 480)))		// Used for debugging
	//	return FALSE;

	// Create a system memory surface in which to build our frames.
	WorkSurface = new CDXSurface;
	WorkSurface->Create(Screen,640,480,CDXMEM_SYSTEMONLY);  
	
	// Create a surface to hold the claen copy of the background image.
	Picture = new CDXSurface;
	Picture->Create(Screen,"bkgrnd.bmp",CDXMEM_SYSTEMONLY); 

	// Create a Sprite.
	Sprite1 = new CDXSprite;
	Sprite1->Create(Screen,"bug1.bmp", 80, 64, 1, CDXMEM_SYSTEMONLY);
	Sprite1->SetFrame(1);
	Sprite1->SetPos(10,100);
	Sprite1->SetColorKey(0);
	Sprite1->SetAlphaValue(0);
	Sprite1->SetShadowValue(0);
	Sprite1->SetShadowOffset(0,0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// WinMain
/////////////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	CHAR str[100];
	LONG FPS;
	RECT clipRect;
	DWORD FrameCount = 0;
	DWORD time = 0, timeTotal = 0;
	int alphaValue = 0;
	int shadowValue = 0;

	clipRect.left   = 0;
	clipRect.top    = 0;
	clipRect.right  = 640;
	clipRect.bottom = 480;
	
	if(!InitApp(hInst, nCmdShow)) return FALSE;	

	// Setup the screen font.
	Screen->GetBack()->ChangeFont("Times New Roman", 4,10,200);

	// Set the clipRect for the working surface.
	WorkSurface->SetClipRect(&clipRect);

	// Inialize the time variable, used for FPS calculation.
	time = timeGetTime();


	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0 )) return msg.wParam;
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else if(bActive)
		{
			// Set the ALPHA value for the sprite.  Ranges from 0 - 256.
			Sprite1->SetAlphaValue(alphaValue);

			// Set the shadow ALPHA for the sprite.  Ranges from 0 - 256.
			Sprite1->SetShadowValue(shadowValue);

			// Set the shadoe offset for the sprite.  This offsets the
			// shadow from the sprite in order to give the illusion of
			// height.
			Sprite1->SetShadowOffset(10,-15);

			// Fill the working buffer with a background image
			Picture->DrawBlk(WorkSurface,0,0);
	
			// Draw the sprite to the WorkSurface(system memory)
			//
			// Block (BLK) blits -- These are slightly faster but do not
			//                      honor the colorkey
			Sprite1->SetPos(10,150);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_BLK);
			Sprite1->SetPos(110,150);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_BLKALPHA);
			Sprite1->SetPos(210,150);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_BLKALPHAFAST);
			Sprite1->SetPos(320,150);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_BLKSHADOW);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_BLK);
			Sprite1->SetPos(430,150);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_BLKSHADOWFAST);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_BLK);
			Sprite1->SetPos(555,150);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_BLKVFLIP);
			

			// Transparent (TRANS) blits -- These blt are a bit slower but they
			//                              honor the colorkey value.
			Sprite1->SetPos(10,300);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_TRANS);
			Sprite1->SetPos(110,300);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_TRANSALPHA);
			Sprite1->SetPos(210,300);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_TRANSALPHAFAST);
			Sprite1->SetPos(320,300);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_TRANSSHADOW);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_TRANS);
			Sprite1->SetPos(430,300);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_TRANSSHADOWFAST);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_TRANS);
			Sprite1->SetPos(555,300);
			Sprite1->Draw(WorkSurface,0, 0, CDXBLT_TRANSVFLIP);
			

			// Increment the alphaValue
			alphaValue++;
			if (alphaValue > 256)
				alphaValue = 0;

			// Increment the shadowValue
			shadowValue++;
			if (shadowValue > 256)
				shadowValue = 0;

			// Draw some text to the back buffer
			WorkSurface->GetDC();
				timeTotal += timeGetTime() - time;
				time = timeGetTime();

				if (timeTotal > 1000)
				{
					FPS = (LONG)( (float)FrameCount/((float)timeTotal / 1000.0) );
					timeTotal  = 0;
					FrameCount = 0;
				}
				
				WorkSurface->SetFont();

				sprintf(str,"Frame Rate: %d fps",FPS);
				WorkSurface->TextXY(240,20,0x00000000,str);

				// Label the CDXBLT types
				WorkSurface->TextXY(10,130,0x00000000,"Blk");
				WorkSurface->TextXY(110,130,0x00000000,"BlkAlpha");
				WorkSurface->TextXY(210,130,0x00000000,"BlkAlphaFast");
				WorkSurface->TextXY(320,130,0x00000000,"BlkShadow");
				WorkSurface->TextXY(430,130,0x00000000,"BlkShadowFast");
				WorkSurface->TextXY(555,130,0x00000000,"BlkVflip");

				WorkSurface->TextXY(10,280,0x00000000,"Trans");
				WorkSurface->TextXY(110,280,0x00000000,"TransAlpha");
				WorkSurface->TextXY(210,280,0x00000000,"TransAlphaFast");
				WorkSurface->TextXY(320,280,0x00000000,"TransShadow");
				WorkSurface->TextXY(430,280,0x00000000,"TransShadowFast");
				WorkSurface->TextXY(555,280,0x00000000,"TransVflip");	
			WorkSurface->ReleaseDC();

			// Copy Worksuface to back buffer.
			WorkSurface->DrawBlk(Screen->GetBack(),0,0);

			// Then flip it to the front buffer to display it.
			Screen->Flip();  
			
			// Increment the frame counter.
			FrameCount++;
		}
		else WaitMessage();
	}
}
