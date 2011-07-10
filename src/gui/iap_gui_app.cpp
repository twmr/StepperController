/////////////////////////////////////////////////////////////////////////////
// Name:        iap_gui_app.cpp
// Purpose:     
// Author:      Thomas Hisch
// Modified by: 
// Created:     Mon 21 Mar 2011 16:36:29 CET
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "iap_gui_app.hpp"

////@begin XPM images
////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( iap_gui )
////@end implement app


/*
 * iap_gui type definition
 */

IMPLEMENT_CLASS( iap_gui, wxApp )


/*
 * iap_gui event table definition
 */

BEGIN_EVENT_TABLE( iap_gui, wxApp )

////@begin iap_gui event table entries
////@end iap_gui event table entries

END_EVENT_TABLE()


/*
 * Constructor for iap_gui
 */

iap_gui::iap_gui()
{
    Init();
}


/*
 * Member initialisation
 */

void iap_gui::Init()
{
////@begin iap_gui member initialisation
////@end iap_gui member initialisation
}

/*
 * Initialisation for iap_gui
 */

bool iap_gui::OnInit()
{    
////@begin iap_gui initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	PM301* mainWindow = new PM301( NULL );
	mainWindow->Show(true);
////@end iap_gui initialisation

    return true;
}


/*
 * Cleanup for iap_gui
 */

int iap_gui::OnExit()
{    
////@begin iap_gui cleanup
	return wxApp::OnExit();
////@end iap_gui cleanup
}

