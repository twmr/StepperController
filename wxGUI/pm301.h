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
#include "wx/spinctrl.h"
#include "wx/statline.h"
#include "wx/statusbr.h"
#include "wx/toolbar.h"
////@end includes
#include "../global.hpp"
#include <boost/asio.hpp>
#include "wx/thread.h"
#include "wx/textfile.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
class wxStatusBar;
////@end forward declarations
class UnitConversionConstants;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PM301 10000
#define ID_MENUITEM 10002
#define ID_MENUITEM1 10020
#define ID_MENUITEM2 10021
#define ID_TEXTCTRL 10008
#define ID_SPINCTRL 10003
#define ID_TEXTCTRL1 10006
#define ID_SPINCTRL1 10010
#define ID_TEXTCTRL2 10007
#define ID_SPINCTRL2 10009
#define ID_CHECKBOX 10004
#define ID_RADIOBOX 10005
#define ID_BUTTON 10016
#define ID_BUTTON1 10017
#define ID_TEXTCTRL6 10015
#define ID_STATUSBAR 10001
#define ID_TOOLBAR 10018
#define ID_BUTTON2 10019
#define SYMBOL_PM301_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PM301_TITLE _("PM301")
#define SYMBOL_PM301_IDNAME ID_PM301
#define SYMBOL_PM301_SIZE wxSize(500, 500)
#define SYMBOL_PM301_POSITION wxDefaultPosition
////@end control identifiers

//FIXME mention unit of variables
#define x_min 0.9
#define x_max 3.4

/*!
 * Position class declaration
 */
 
class Position
{
public:
    Position(const int x, const int y, const int theta):
        x_(x), y_(y), theta_(theta)
        { };
    int get_x() { return x_; }
    int get_y() { return y_; }
    int get_theta() { return theta_; }
    
private:
    int x_;
    int y_;
    int theta_;
};




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

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM
    void ClickUnitConvConsts( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM1
    void OnMenuitem1Click( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM2
    void OnPositionUpdateClick( wxCommandEvent& event );

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

    /// wxEVT_LEFT_DOWN event handler for ID_BUTTON
    void LoadBatchFileDialog( wxMouseEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_BUTTON1
    void LeaveBatchModeButtonPressed( wxMouseEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_STATUSBAR
    void OnStatusbarUpdate( wxUpdateUIEvent& event );

////@end PM301 event handler declarations

////@begin PM301 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end PM301 member function declarations

    void SendMessage(const wxString&); 
    void SendMessage(const std::string&); 
    void SendMessage(const char *); 
    void send();
    void check_and_update_position(wxTextCtrl*, const std::string&, const double);
    int getIdxFromCoord(const std::string &);

    /* unit conversion functions */
    float convert_to_natural_units(const int pos, const std::string &coord) const;
    int convert_to_stepper_units(const float pos, const std::string &coord) const;
    
    //
    void ToggleBatchMode(void);

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin PM301 member variables
    wxBoxSizer* mainswitcher;
    wxBoxSizer* basiccontrol;
    wxTextCtrl* new_x_pos;
    wxTextCtrl* new_y_pos;
    wxTextCtrl* new_phi_pos;
    wxBoxSizer* jogmodelayout;
    wxCheckBox* checkjog;
    wxRadioBox* axradiobox;
    wxBoxSizer* batchmodelog;
    wxTextCtrl* batchmodetextctl;
    wxStatusBar* statusbar;
////@end PM301 member variables

    Position getcurpos();

private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::tcp::socket s;
    msg_t request, reply;
    //wxLocale m_locale;
    double coord_limits[3][2]; //max and min values of the 3 coordinates (x,y,phi)

    //FIXME insert correct values
    static const float x_unit_conv = 2.0;
    static const float y_unit_conv = 2.0;
    static const float phi_unit_conv = 2.0;
    UnitConversionConstants *uconvframe;
};

class BatchThread : public wxThread
{
public:
    BatchThread(const wxString& path, wxTextCtrl* ctl, PM301 *p) :
        batchmodetextctl_(ctl), pm301(p)
        {
            file.Open(path);
        }
    virtual void *Entry();
            
private:
    wxTextCtrl* batchmodetextctl_;
    wxTextFile file;
    PM301 *pm301;
};

class PositionUpdateThread : public wxThread
{
public:
    PositionUpdateThread(PM301 *p) :
        pm301(p)
        {};
    virtual void *Entry();
    void WriteText(const wxString& text, const size_t n) const;

private:
    PM301 *pm301;
};


#endif
    // _PM301_H_
