/////////////////////////////////////////////////////////////////////////////
// Name:        iap_gui_app.hpp
// Purpose:     
// Author:      Thomas Hisch
// Modified by: 
// Created:     Mon 21 Mar 2011 16:36:29 CET
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _IAP_GUI_APP_H_
#define _IAP_GUI_APP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "pm301.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * iap_gui class declaration
 */

class iap_gui: public wxApp
{    
    DECLARE_CLASS( iap_gui )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    iap_gui();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin iap_gui event handler declarations

////@end iap_gui event handler declarations

////@begin iap_gui member function declarations

////@end iap_gui member function declarations

////@begin iap_gui member variables
////@end iap_gui member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(iap_gui)
////@end declare app

#endif
    // _IAP_GUI_APP_H_
