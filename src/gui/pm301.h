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
#include "wx/toolbar.h"
////@end includes

#include "wx/vector.h"
#include "wx/spinctrl.h"
#include "wx/socket.h"
#include "wx/thread.h"
#include "wx/textfile.h"

#include "global.hpp"
#include "position.hpp"
#include "PosCtrl.h"


/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
////@end forward declarations
class PositionUpdateThread;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PM301 10000
#define ID_MENUITEM1 10020
#define ID_BUTTON3 10006
#define ID_BUTTON_SAVEXML 10003
#define ID_CHECKBOX 10004
#define ID_BUTTON 10016
#define ID_BUTTON1 10017
#define ID_TEXTCTRL6 10015
#define ID_TOOLBAR 10018
#define ID_BUTTON2 10019
#define SYMBOL_PM301_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PM301_TITLE _("MOVES - Stepper Motor Controller (PM381)")
#define SYMBOL_PM301_IDNAME ID_PM301
#define SYMBOL_PM301_SIZE wxSize(600, 500)
#define SYMBOL_PM301_POSITION wxDefaultPosition
////@end control identifiers

#define ID_AXESRADIOBOX 8999
#define ID_BITMAPBUTTONS 8899
#define SOCKET_ID 9980

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

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM1
    void OnMenuitem1Click( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON3
    void OnButtonZeroPositionClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SAVEXML
    void OnButtonSavexmlClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
    void OnCheckboxClick( wxCommandEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_BUTTON
    void LoadBatchFileDialog( wxMouseEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_BUTTON1
    void LeaveBatchModeButtonPressed( wxMouseEvent& event );

////@end PM301 event handler declarations

    void OnPosCTRLUpdated( wxCommandEvent& event );
    void OnSocketEvent( wxSocketEvent& event );
    void OnBitmapbuttonClick( wxCommandEvent& event );
    void OnRadioboxSelected( wxCommandEvent& event );

////@begin PM301 member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end PM301 member function declarations

    void ToggleBatchMode(void);

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin PM301 member variables
    wxBoxSizer* mainswitcher;
    wxBoxSizer* basiccontrol;
    wxStaticBox* poswidget;
    wxStaticBoxSizer* posSizer;
    wxBoxSizer* jogmodelayout;
    wxCheckBox* checkjog;
    wxBoxSizer* batchmodelog;
    wxTextCtrl* batchmodetextctl;
////@end PM301 member variables

    wxVector<PosCtrl*> axissc;
    wxVector<wxBitmapButton*> axisbb;
    wxVector<wxStaticText*> axisst;
    wxVector<wxBoxSizer*> axisbs;
    wxRadioBox* axesradiobox;

    Position getcurpos();
    void initaxes();
    const wxString SendandReceive(const wxString& msgstr);

    //protect critical data with mutexs
    Position get_cp() const {
        wxMutexLocker lock(m_mutex);
        if(!lock.IsOk()) {
            std::cerr << "mutex lock error" << std::endl;
            exit(1);
        }

        //if (posthread == NULL)
        //    cp_ = getcurpos();

        return cp_;
    };
    void set_cp(const Position &cp) {
        wxMutexLocker lock(m_mutex);
        if(lock.IsOk())
            cp_ = cp;
        else {
            std::cerr << "mutex lock error" << std::endl;
        }
    };

    size_t get_nraxes() const { return nraxes; };
    wxVector<wxString*> coords;
    wxVector<wxString*> units;

private:
    wxSocketClient* s;
    msg_t request, reply;
    PositionUpdateThread *posthread;
    Position cp_;
    mutable wxMutex m_mutex;
    mutable wxMutex m_tcpmutex;
    size_t nraxes;
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

private:
    PM301 *pm301;
};


#endif
    // _PM301_H_
