/////////////////////////////////////////////////////////////////////////////
// Name:        pm301.cpp
// Purpose:     
// Author:      Thomas Hisch
// Modified by: 
// Created:     Mon 21 Mar 2011 16:38:47 CET
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

#include "pm301.h"
#include <boost/asio.hpp>
#include "../IAP_server.hpp"

////@begin XPM images
////@end XPM images


/*
 * PM301 type definition
 */

IMPLEMENT_CLASS( PM301, wxFrame )


/*
 * PM301 event table definition
 */

BEGIN_EVENT_TABLE( PM301, wxFrame )

////@begin PM301 event table entries
    EVT_TEXT_ENTER( ID_TEXTCTRL, PM301::OnTextctrlEnter )

    EVT_TEXT_ENTER( ID_TEXTCTRL1, PM301::OnTextctrl1Enter )

    EVT_TEXT_ENTER( ID_TEXTCTRL2, PM301::OnTextctrl2Enter )

    EVT_CHECKBOX( ID_CHECKBOX, PM301::OnCheckboxClick )

    EVT_RADIOBOX( ID_RADIOBOX, PM301::OnRadioboxSelected )

////@end PM301 event table entries

END_EVENT_TABLE()


/*
 * PM301 constructors
 */

PM301::PM301()
 : s(io_service)
{
    Init();
}

PM301::PM301( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
  : s(io_service)
{
    Init();
    Create( parent, id, caption, pos, size, style );
}


/*
 * PM301 creator
 */

bool PM301::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin PM301 creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end PM301 creation
    return true;
}


/*
 * PM301 destructor
 */

PM301::~PM301()
{
////@begin PM301 destruction
////@end PM301 destruction
}


/*
 * Member initialisation
 */

void PM301::Init()
{
////@begin PM301 member initialisation
    new_x_pos = NULL;
    new_y_pos = NULL;
    new_phi_pos = NULL;
    jogmodelayout = NULL;
    checkjog = NULL;
    axradiobox = NULL;
////@end PM301 member initialisation

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), "localhost", "15000");
    tcp::resolver::iterator iterator = resolver.resolve(query);
    s.connect(*iterator);

    coord_limits[0][0] = -10.0;
    coord_limits[0][1] = 10.0;
    coord_limits[1][0] = -10.0;
    coord_limits[1][1] = 10.0;
    coord_limits[2][0] = -10.0;
    coord_limits[2][1] = 10.0;
    

}


/*
 * Control creation for PM301
 */

void PM301::CreateControls()
{    
////@begin PM301 content construction
    PM301* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu24 = new wxMenu;
    menuBar->Append(itemMenu24, _("FIle"));
    wxMenu* itemMenu25 = new wxMenu;
    itemMenu25->Append(ID_MENUITEM, _("Item"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu25, _("Operations"));
    itemFrame1->SetMenuBar(menuBar);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemFrame1, wxID_ANY, _("myposition"));
    wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxHORIZONTAL);
    itemBoxSizer3->Add(itemStaticBoxSizer4, 1, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemFrame1, wxID_STATIC, _("x:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    new_x_pos = new wxTextCtrl( itemFrame1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemStaticBoxSizer4->Add(new_x_pos, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemFrame1, wxID_STATIC, _("y:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    new_y_pos = new wxTextCtrl( itemFrame1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemStaticBoxSizer4->Add(new_y_pos, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemFrame1, wxID_STATIC, _("phi:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer4->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    new_phi_pos = new wxTextCtrl( itemFrame1, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemStaticBoxSizer4->Add(new_phi_pos, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(itemFrame1, wxID_ANY, _("current position"));
    wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxHORIZONTAL);
    itemBoxSizer3->Add(itemStaticBoxSizer11, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemFrame1, wxID_STATIC, _("x:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemFrame1, wxID_STATIC, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemFrame1, wxID_STATIC, _("y:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemFrame1, wxID_STATIC, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemFrame1, wxID_STATIC, _("phi:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText18 = new wxStaticText( itemFrame1, wxID_STATIC, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine19 = new wxStaticLine( itemFrame1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    itemBoxSizer2->Add(itemStaticLine19, 0, wxGROW, 5);

    jogmodelayout = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(jogmodelayout, 0, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

    checkjog = new wxCheckBox( itemFrame1, ID_CHECKBOX, _("Jog Mode"), wxDefaultPosition, wxDefaultSize, 0 );
    checkjog->SetValue(false);
    jogmodelayout->Add(checkjog, 0, wxALIGN_LEFT, 5);

    wxArrayString axradioboxStrings;
    axradioboxStrings.Add(_("&Axis 1"));
    axradioboxStrings.Add(_("&Axis 2"));
    axradioboxStrings.Add(_("&Axis 3"));
    axradiobox = new wxRadioBox( itemFrame1, ID_RADIOBOX, _("Axes"), wxDefaultPosition, wxDefaultSize, axradioboxStrings, 1, wxRA_SPECIFY_COLS );
    axradiobox->SetSelection(0);
    axradiobox->Show(false);
    jogmodelayout->Add(axradiobox, 0, wxGROW, 5);

////@end PM301 content construction
//std::locale::global(locale(setlocale(LC_ALL, NULL)));
// m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT | wxLOCALE_CONV_ENCODING);

    //SendMessage("gp1");
    //FIXME unit conversion
    //new_x_pos->SetValue(wxString::Format(wxT("%f"), atof(reply.msg));

}


/*
 * Should we show tooltips?
 */

bool PM301::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap PM301::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin PM301 bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end PM301 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon PM301::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin PM301 icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end PM301 icon retrieval
}


void PM301::SendMessage(const std::string &msgstr)
{
        request.type = MSG_REQUEST;
        strcpy(request.msg, msgstr.c_str());
        boost::asio::write(s, boost::asio::buffer(reinterpret_cast<char*>(&request), msglen));
        boost::asio::read(s, boost::asio::buffer(reinterpret_cast<char*>(&reply), msglen));
}


/*
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_RADIOBOX
 */

void PM301::OnRadioboxSelected( wxCommandEvent& event )
{
    char buf[4];
    int selected = axradiobox->GetSelection();
    sprintf(buf,"sa%d",selected);
    SendMessage(buf);
}





int PM301::getIdxFromCoord(const std::string &coord)
{
    if(coord == "x")
        return 0;
    else if(coord == "y")
        return 1;
    else if(coord == "phi")
        return 2;
    else
        return -1;
}


void PM301::check_and_update_position(wxTextCtrl* ctrl, const std::string& coord, const double curval)
{
    int idx = getIdxFromCoord(coord);
    bool outofregion = false;
    bool greatermax = false;
    
    if(idx < 0) {
        wxLogError(wxT("yes"));
        return;
    }
    
    if(curval > coord_limits[idx][1]) {
        outofregion = true;
        greatermax = true;
    }
    else if (curval < coord_limits[idx][0]) {
        outofregion = true;
        greatermax = false;
    }

    if(outofregion) {
        wxString wxcord = wxString::FromAscii(coord.c_str());
        wxString tmp = wxT("Entered ") + wxcord + wxT(" position (%f) is out of range. Move motor to ") + wxcord + wxT("%s=%f ?");
        wxString msg = wxString::Format(tmp, curval,
                                        greatermax ? wxT("max") : wxT("min"),
                                        greatermax ? coord_limits[idx][1] : coord_limits[idx][0]);
        
        wxMessageDialog dialog( NULL, msg,
                                wxT("Warning"), wxNO_DEFAULT | wxYES_NO | wxICON_INFORMATION);
        switch(dialog.ShowModal()) {
        case wxID_YES:
            //wxLogError(wxT("yes"));
            //FIXME sendmessage
            ctrl->SetValue(wxString::Format(wxT("%f"),
                                            greatermax ? coord_limits[idx][1] : coord_limits[idx][0]));
            break;
        case wxID_NO:
            //wxLogError(wxT("no"));
            break;
        default:
            wxLogError(wxT("Unexpected wxMess Dialog return code!"));
        }
    }
    else {
        //
    }
    
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL
 */

void PM301::OnTextctrlEnter( wxCommandEvent& event )
{
    wxString entered = new_x_pos->GetValue();

    double value;
    if(!entered.ToDouble(&value)){
        wxMessageBox(wxT("Entered string is not a number!"), wxT("Warning"), wxOK | wxICON_INFORMATION, this);
        //set default value ?!?!?
        return;
    }

    check_and_update_position(new_x_pos, std::string("x"), value);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

void PM301::OnCheckboxClick( wxCommandEvent& event )
{
    if(checkjog->IsChecked()) {
        SendMessage("1AR");
        axradiobox->Show(true);
    } else
    {
        SendMessage("1IR");
        axradiobox->Show(false);
    }
        
    jogmodelayout->Layout();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL1
 */

void PM301::OnTextctrl1Enter( wxCommandEvent& event )
{
    wxString entered = new_y_pos->GetValue();

    double value;
    if(!entered.ToDouble(&value)){
        wxMessageBox(wxT("Entered string is not a number!"), wxT("Warning"), wxOK | wxICON_INFORMATION, this);
        //set default value ?!?!?
        return;
    }

    check_and_update_position(new_y_pos, std::string("y"), value);
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL2
 */

void PM301::OnTextctrl2Enter( wxCommandEvent& event )
{
    wxString entered = new_phi_pos->GetValue();

    double value;
    if(!entered.ToDouble(&value)){
        wxMessageBox(wxT("Entered string is not a number!"), wxT("Warning"), wxOK | wxICON_INFORMATION, this);
        //set default value ?!?!?
        return;
    }

    check_and_update_position(new_y_pos, std::string("phi"), value);
}





