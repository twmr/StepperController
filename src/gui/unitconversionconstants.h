/////////////////////////////////////////////////////////////////////////////
// Name:        unitconversionconstants.h
// Purpose:     
// Author:      Thomas Hisch
// Modified by: 
// Created:     Sat 11 Jun 2011 11:27:05 CEST
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _UNITCONVERSIONCONSTANTS_H_
#define _UNITCONVERSIONCONSTANTS_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
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
#define ID_UNITCONVERSIONCONSTANTS 10011
#define ID_TEXTCTRL3 10012
#define ID_TEXTCTRL4 10013
#define ID_TEXTCTRL5 10014
#define SYMBOL_UNITCONVERSIONCONSTANTS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_UNITCONVERSIONCONSTANTS_TITLE _("Unit Conversion Constants")
#define SYMBOL_UNITCONVERSIONCONSTANTS_IDNAME ID_UNITCONVERSIONCONSTANTS
#define SYMBOL_UNITCONVERSIONCONSTANTS_SIZE wxSize(400, 300)
#define SYMBOL_UNITCONVERSIONCONSTANTS_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * UnitConversionConstants class declaration
 */

class UnitConversionConstants: public wxFrame
{    
    DECLARE_CLASS( UnitConversionConstants )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    UnitConversionConstants();
    UnitConversionConstants( wxWindow* parent, wxWindowID id = SYMBOL_UNITCONVERSIONCONSTANTS_IDNAME, const wxString& caption = SYMBOL_UNITCONVERSIONCONSTANTS_TITLE, const wxPoint& pos = SYMBOL_UNITCONVERSIONCONSTANTS_POSITION, const wxSize& size = SYMBOL_UNITCONVERSIONCONSTANTS_SIZE, long style = SYMBOL_UNITCONVERSIONCONSTANTS_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_UNITCONVERSIONCONSTANTS_IDNAME, const wxString& caption = SYMBOL_UNITCONVERSIONCONSTANTS_TITLE, const wxPoint& pos = SYMBOL_UNITCONVERSIONCONSTANTS_POSITION, const wxSize& size = SYMBOL_UNITCONVERSIONCONSTANTS_SIZE, long style = SYMBOL_UNITCONVERSIONCONSTANTS_STYLE );

    /// Destructor
    ~UnitConversionConstants();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin UnitConversionConstants event handler declarations

    /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL3
    void OnTextctrl3Enter( wxCommandEvent& event );

////@end UnitConversionConstants event handler declarations

////@begin UnitConversionConstants member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end UnitConversionConstants member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin UnitConversionConstants member variables
////@end UnitConversionConstants member variables
};

#endif
    // _UNITCONVERSIONCONSTANTS_H_
