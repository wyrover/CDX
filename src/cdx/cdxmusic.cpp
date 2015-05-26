//////////////////////////////////////////////////////////////////////////////////
// $Source: /cvsroot/cdx/cdx3.0/src/cdx/cdxmusic.cpp,v $
// $Author: hebertjo $
//
// $Log: cdxmusic.cpp,v $
// Revision 1.1.1.1  2000/04/22 16:10:44  hebertjo
// Initial checkin of v3.0 to SourceForge CVS.
//
// Revision 2.2  1999/05/27 05:33:56  janni
// added destructor which stops the music when the object is destroyed
//
// Revision 2.1  1999/05/20 15:29:03  pietro
// Multiple changes:
// * fixed #include bugs in all .cpp and various .h files
// * fixed all rcsid[] bugs
// * added conditional compile variable CDXINCLUDEALL - when defined,
//   all #include files are included in cdx.h to keep backward compatibility
// * All the libraries are created in ..\..\lib\vc\ directory, library names are
//   cdx.lib/cdxd.lib/cdxdx3.lib/cdxdx3d.lib/cdxadx3.lib/cdxadx3d.lib
//
// Revision 2.0  1999/05/01 13:51:16  bsimser
// Updated revision number to 2.0
//
// Revision 1.1.1.1  1999/05/01 04:10:56  bsimser
// Initial revision to cvs
//
// $Revision: 1.1.1.1 $
//////////////////////////////////////////////////////////////////////////////////
#ifdef SAVE_RCSID
    static char rcsid[] = "@(#) $Id: cdxmusic.cpp,v 1.1.1.1 2000/04/22 16:10:44 hebertjo Exp $";
#endif

#include "CDX.h"
#include "cdxmusic.h"

//////////////////////////////////////////////////////////////////////////////////
// Default constructor.
//////////////////////////////////////////////////////////////////////////////////
CDXMusic::CDXMusic(void *hWnd)
{
    m_hWnd = ((HWND)hWnd);
}


//////////////////////////////////////////////////////////////////////////////////
// Default constructor.
//////////////////////////////////////////////////////////////////////////////////
CDXMusic::~CDXMusic()
{
    Stop();
}

//////////////////////////////////////////////////////////////////////////////////
// Plays the specified MIDI file.
//////////////////////////////////////////////////////////////////////////////////
BOOL CDXMusic::Play(const char *Filename)
{
    if (Filename == NULL) return FALSE;

    char buffer[256];
    sprintf(buffer, "open %s type sequencer alias MUSIC", Filename);

    if (mciSendString("close all", NULL, 0, NULL) != 0) {
        return (FALSE);
    }

    if (mciSendString(buffer, NULL, 0, NULL) != 0) {
        return (FALSE);
    }

    if (mciSendString("play MUSIC from 0 notify", NULL, 0, m_hWnd) != 0) {
        return (FALSE);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Stops the currently playing MIDI file.
//////////////////////////////////////////////////////////////////////////////////
CDXMusic::Stop()
{
    if (mciSendString("close all", NULL, 0, NULL) != 0) {
        return (FALSE);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Pauses all currently playing MIDI files.
//////////////////////////////////////////////////////////////////////////////////
BOOL CDXMusic::Pause(void)
{
    // Pause if we're not already paused
    if (mciSendString("stop MUSIC", NULL, 0, NULL) != 0) {
        return (FALSE);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Resumes currently paused MIDI files.
//////////////////////////////////////////////////////////////////////////////////
BOOL CDXMusic::Resume(void)
{
    if (mciSendString("play MUSIC notify", NULL, 0, m_hWnd) != 0) {
        return (FALSE);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Restarts the MIDI file from the beginning.
//////////////////////////////////////////////////////////////////////////////////
BOOL CDXMusic::Restart(void)
{
    if (mciSendString("play MUSIC from 0 notify", NULL, 0, m_hWnd) != 0) {
        return (FALSE);
    }

    return TRUE;
}
