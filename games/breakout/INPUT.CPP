/////////////////////////////////////////////////////////////////////////////////////////////
// input.cpp
/////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>

#define CDXINCLUDEALL
#include <CDX.h>

CDXInput Input;

BOOL InitInput(HINSTANCE hInst, HWND hWnd)
{
	if(FAILED(Input.Create(hInst, hWnd)))
		return FALSE;

	return TRUE;
}

void FiniInput(void)
{
}

void UpdateInput(void)
{
	Input.Update();
}