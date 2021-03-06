//////////////////////////////////////////////////////////////////////////////////
// $Source: /cvsroot/cdx/cdx3.0/src/cdx3d/light.cpp,v $
// $Author: hebertjo $
//
// $Log: light.cpp,v $
// Revision 1.1.1.1  2000/04/22 16:08:42  hebertjo
// Initial checkin of v3.0 to SourceForge CVS.
//
// Revision 2.0  1999/05/01 13:51:18  bsimser
// Updated revision number to 2.0
//
// Revision 1.1.1.1  1999/05/01 04:10:56  bsimser
// Initial revision to cvs
//
// $Revision: 1.1.1.1 $
//////////////////////////////////////////////////////////////////////////////////
#ifdef SAVE_RCSID
static char rcsid = "@(#) $Id: light.cpp,v 1.1.1.1 2000/04/22 16:08:42 hebertjo Exp $";
#endif

#include "CDX3D.h"

//////////////////////////////////////////////////////////////////////////////////
// CRMLight Constructor
//////////////////////////////////////////////////////////////////////////////////
CRMLight::CRMLight(CRMEngine* pEngine)
{
	m_Light = NULL;
	m_Engine = pEngine;
}

//////////////////////////////////////////////////////////////////////////////////
// CRMLight Destructor
//////////////////////////////////////////////////////////////////////////////////
CRMLight::~CRMLight(void)
{
	RELEASE(m_Light);
}

//////////////////////////////////////////////////////////////////////////////////
// CRMLight Create
//////////////////////////////////////////////////////////////////////////////////
BOOL CRMLight::Create(D3DRMLIGHTTYPE type, double r, double g, double b)
{
	HRESULT rval;

	m_Engine->CreateFrame(NULL, &m_Frame);
	m_Engine->CreateLight(type, r, g, b, &m_Light);

	rval = m_Frame->AddLight(m_Light);
	if(rval != D3DRM_OK) return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// CRMLight CreateAmbient
//////////////////////////////////////////////////////////////////////////////////
BOOL CRMLight::CreateAmbient(double r, double g, double b)
{
	return Create(D3DRMLIGHT_AMBIENT, r, g, b);
}

//////////////////////////////////////////////////////////////////////////////////
// CRMLight CreateDirectional
//////////////////////////////////////////////////////////////////////////////////
BOOL CRMLight::CreateDirectional(double r, double g, double b)
{
	return Create(D3DRMLIGHT_DIRECTIONAL, r, g, b);
}

//////////////////////////////////////////////////////////////////////////////////
// CRMLight CreatePoint
//////////////////////////////////////////////////////////////////////////////////
BOOL CRMLight::CreatePoint(double r, double g, double b)
{
	return Create(D3DRMLIGHT_POINT, r, g, b);
}

//////////////////////////////////////////////////////////////////////////////////
// CRMLight CreateParallelPoint
//////////////////////////////////////////////////////////////////////////////////
BOOL CRMLight::CreateParallelPoint(double r, double g, double b)
{
	return Create(D3DRMLIGHT_PARALLELPOINT, r, g, b);
}

//////////////////////////////////////////////////////////////////////////////////
// CRMLight CreateSpotlight
//////////////////////////////////////////////////////////////////////////////////
BOOL CRMLight::CreateSpot(double r, double g, double b)
{
	return Create(D3DRMLIGHT_SPOT, r, g, b);
}

//////////////////////////////////////////////////////////////////////////////////
// CRMLight SetColor
//////////////////////////////////////////////////////////////////////////////////
void CRMLight::SetColor(double r, double g, double b)
{
	m_Light->SetColorRGB(r, g, b);
}
