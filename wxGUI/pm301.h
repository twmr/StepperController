/////////////////////////////////////////////////////////////////////////////
// Name:        pm301.h
// Purpose:     
// Author:      Thomas Hisch
// Modified by: 
// Created:     Mon 21 Mar 2011 16:38:47 CET
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _PM301_H_
#define _PM301_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/statline.h"
////@end includes
#include "../global.hpp"
#include <boost/asio.hpp>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PM301 10000
#define ID_TEXTCTRL 10008
#define ID_TEXTCTRL1 10006
#define ID_TEXTCTRL2 10007
#define ID_CHECKBOX 10004
#define ID_RADIOBOX 10005
#define ID_MENUITEM 10002
#define SYMBOL_PM301_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PM301_TITLE _("PM301")
#define SYMBOL_PM301_IDNAME ID_PM301
#define SYMBOL_PM301_SIZE wxSize(400, 140)
#define SYMBOL_PM301_POSITION wxDefaultPosition
////@end control identifiers

//FIXME mention unit of variables
#define x_min 0.9
#define x_max 3.4

/*!
 * PM301 class declaration
 */

class PM301: public wxFrame
{    
    DECLARE_CLASS( PM301 )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    PM301();
    PM301( wxWindow* parent, wxWindowID id = SYMBOL_PM301_IDNAME, const wxString& caption = SYMBOL_PM301_TITLE, const wxPoint& pos = SYMBOL_PM301_POSITION, const wxSize& size = SYMBOL_PM301_SIZE, long style = SYMBOL_PM301_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PM301_IDNAME, const wxString& caption = SYMBOL_PM301_TITLE, const wxPoint& pos = SYMBOL_PM301_POSITION, const wxSize& size = SYMBOL_PM301_SIZE, long style = SYMBOL_PM301_STYLE );

    /// Destructor
    ~PM301();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin PM301 event handler declarations

    /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL
    void OnTextctrlEnter( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL1
    void OnTextctrl1Enter( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL2
    void OnTextctrl2Enter( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
    void OnCheckboxClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_RADIOBOX
    void OnRadioboxSelected( wxCommandEvent& event );

////@end PM301 event handler declarations

////@begin PM301 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end PM301 member function declarations

    void SendMessage(const std::string&); 
    void check_and_update_position(wxTextCtrl*, const std::string&, const double);
    int getIdxFromCoord(const std::string &);

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin PM301 member variables
    wxTextCtrl* new_x_pos;
    wxTextCtrl* new_y_pos;
    wxTextCtrl* new_phi_pos;
    wxBoxSizer* jogmodelayout;
    wxCheckBox* checkjog;
    wxRadioBox* axradiobox;
////@end PM301 member variables

private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::tcp::socket s;
    msg_t request, reply;
    //wxLocale m_locale;
    double coord_limits[3][2]; //max and min values of the 3 coordinates (x,y,phi)
};

#endif
    // _PM301_H_
