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


#include "wx/tokenzr.h"
#include "pm301.h"
#include "unitconversionconstants.h"
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
EVT_MENU( ID_MENUITEM, PM301::ClickUnitConvConsts )

EVT_MENU( ID_MENUITEM1, PM301::OnMenuitem1Click )

EVT_MENU( ID_MENUITEM2, PM301::OnPositionUpdateClick )

EVT_TEXT_ENTER( ID_TEXTCTRL, PM301::OnTextctrlEnter )

EVT_TEXT_ENTER( ID_TEXTCTRL1, PM301::OnTextctrl1Enter )

EVT_TEXT_ENTER( ID_TEXTCTRL2, PM301::OnTextctrl2Enter )

EVT_CHECKBOX( ID_CHECKBOX, PM301::OnCheckboxClick )

EVT_RADIOBOX( ID_RADIOBOX, PM301::OnRadioboxSelected )

EVT_UPDATE_UI( ID_STATUSBAR, PM301::OnStatusbarUpdate )

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
    mainswitcher = NULL;
    basiccontrol = NULL;
    new_x_pos = NULL;
    new_y_pos = NULL;
    new_phi_pos = NULL;
    jogmodelayout = NULL;
    checkjog = NULL;
    axradiobox = NULL;
    batchmodelog = NULL;
    batchmodetextctl = NULL;
    statusbar = NULL;
////@end PM301 member initialisation

    // tcp::resolver resolver(io_service);
    // tcp::resolver::query query(tcp::v4(), "localhost", "15000");
    // tcp::resolver::iterator iterator = resolver.resolve(query);
    // s.connect(*iterator);

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
    wxMenu* itemMenu3 = new wxMenu;
    menuBar->Append(itemMenu3, _("FIle"));
    wxMenu* itemMenu4 = new wxMenu;
    itemMenu4->Append(ID_MENUITEM, _("Change Unit-Conversion-Constants"), wxEmptyString, wxITEM_NORMAL);
    itemMenu4->Append(ID_MENUITEM1, _("Batch Mode"), wxEmptyString, wxITEM_NORMAL);
    itemMenu4->Append(ID_MENUITEM2, _("Start Position Update Thread"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu4, _("Operations"));
    itemFrame1->SetMenuBar(menuBar);

    mainswitcher = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(mainswitcher);

    basiccontrol = new wxBoxSizer(wxHORIZONTAL);
    mainswitcher->Add(basiccontrol, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    basiccontrol->Add(itemBoxSizer10, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Static"));
    wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
    itemBoxSizer10->Add(itemStaticBoxSizer11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemFrame1, wxID_STATIC, _("x:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    new_x_pos = new wxTextCtrl( itemFrame1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemBoxSizer12->Add(new_x_pos, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSpinCtrl* itemSpinCtrl15 = new wxSpinCtrl( itemFrame1, ID_SPINCTRL, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemBoxSizer12->Add(itemSpinCtrl15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer16, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemFrame1, wxID_STATIC, _("y:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemStaticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    new_y_pos = new wxTextCtrl( itemFrame1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemBoxSizer16->Add(new_y_pos, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSpinCtrl* itemSpinCtrl19 = new wxSpinCtrl( itemFrame1, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemBoxSizer16->Add(itemSpinCtrl19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer20, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText21 = new wxStaticText( itemFrame1, wxID_STATIC, _("t:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    new_phi_pos = new wxTextCtrl( itemFrame1, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemBoxSizer20->Add(new_phi_pos, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSpinCtrl* itemSpinCtrl23 = new wxSpinCtrl( itemFrame1, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemBoxSizer20->Add(itemSpinCtrl23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine24 = new wxStaticLine( itemFrame1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    basiccontrol->Add(itemStaticLine24, 0, wxGROW, 5);

    jogmodelayout = new wxBoxSizer(wxVERTICAL);
    basiccontrol->Add(jogmodelayout, 0, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

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

    batchmodelog = new wxBoxSizer(wxVERTICAL);
    mainswitcher->Add(batchmodelog, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer29 = new wxBoxSizer(wxHORIZONTAL);
    batchmodelog->Add(itemBoxSizer29, 0, wxGROW|wxALL, 0);

    itemBoxSizer29->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton31 = new wxButton( itemFrame1, ID_BUTTON, _("Load Bach File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer29->Add(itemButton31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton32 = new wxButton( itemFrame1, ID_BUTTON1, _("Quit Batch Mode"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer29->Add(itemButton32, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    batchmodetextctl = new wxTextCtrl( itemFrame1, ID_TEXTCTRL6, _("Batch Log"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    batchmodelog->Add(batchmodetextctl, 1, wxGROW|wxALL, 5);

    statusbar = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    statusbar->SetFieldsCount(2);
    statusbar->SetStatusText(_("hello"), 0);
    statusbar->SetStatusText(_("abc"), 1);
    itemFrame1->SetStatusBar(statusbar);

    wxToolBar* itemToolBar35 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR );
    wxButton* itemButton36 = new wxButton( itemToolBar35, ID_BUTTON2, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemToolBar35->AddControl(itemButton36);
    itemToolBar35->Realize();
    itemFrame1->SetToolBar(itemToolBar35);

    // Connect events and objects
    itemButton31->Connect(ID_BUTTON, wxEVT_LEFT_DOWN, wxMouseEventHandler(PM301::LoadBatchFileDialog), NULL, this);
    itemButton32->Connect(ID_BUTTON1, wxEVT_LEFT_DOWN, wxMouseEventHandler(PM301::LeaveBatchModeButtonPressed), NULL, this);
////@end PM301 content construction
//std::locale::global(locale(setlocale(LC_ALL, NULL)));
// m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT | wxLOCALE_CONV_ENCODING);

    //SendMessage("gp1");
    //FIXME unit conversion
    //new_x_pos->SetValue(wxString::Format(wxT("%f"), atof(reply.msg));
    batchmodelog->Show(false);
    mainswitcher->Layout();


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

void PM301::send(void)
{
    boost::asio::write(s, boost::asio::buffer(reinterpret_cast<char*>(&request), msglen));
    boost::asio::read(s, boost::asio::buffer(reinterpret_cast<char*>(&reply), msglen));
}


void PM301::SendMessage(const wxString& msgstr)
{
    request.type = MSG_REQUEST;
    strcpy(request.msg, msgstr.mb_str());
    send();
}

void PM301::SendMessage(const std::string& msgstr)
{
    request.type = MSG_REQUEST;
    strcpy(request.msg, msgstr.c_str());
    send();
}

void PM301::SendMessage(const char *msgstr)
{
    request.type = MSG_REQUEST;
    strcpy(request.msg, msgstr);
    send();
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



int PM301::convert_to_stepper_units(const float pos, const std::string &coord) const
{
    if(coord == "x")
        return (int)pos/x_unit_conv;
    else if(coord == "y")
        return (int)pos/y_unit_conv;
    else if(coord == "phi")
        return (int)pos/phi_unit_conv;
    else
        return -1;
}

float PM301::convert_to_natural_units(const int pos, const std::string &coord) const
{
    if(coord == "x")
        return pos*x_unit_conv;
    else if(coord == "y")
        return pos*y_unit_conv;
    else if(coord == "phi")
        return pos*phi_unit_conv;
    else
        return -1.0;
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

Position PM301::getcurpos()
{
    static int a=10;
    static int b=20;
    static int c=30;
    wxString text;

    //SendMessage("pbp");

    text.Printf(_T(" axis1: %d\n axis2: %d\n axis3:\n"), a,b,c);

//    wxRegEx rePos(_T("[[:digit:]]+\n"));
//    wxString num = rePos.GetMatch(text, 1);

    std::vector<int> vec;
    wxStringTokenizer tkz(text, wxT("\n"));
    while ( tkz.HasMoreTokens() )
    {
        wxString token = tkz.GetNextToken();
        int v;
        int d;
        sscanf(token.mb_str()," axis%d: %d", &d, &v);
        vec.push_back( v );
    }

    //{

    //}
    // if ( reEmail.Matches(text) )
    // {
    //     wxString text = reEmail.GetMatch(email);
    //     wxString username = reEmail.GetMatch(email, 1);
    // }

    // or we could do this to hide the email address
//    size_t count = reEmail.ReplaceAll(text, "HIDDEN@\\2\\3");
//    printf("text now contains %u hidden addresses", count);


    //std::string b = wxT("abc");
    //wxString a = b.c_str();
    //wxLogError(wxT("bca"));
    a++;
    b++;
    c++;
    return Position(vec[0],vec[1],vec[2]);
    //return Position(a,b,c);
}

/*
 * wxEVT_UPDATE_UI event handler for ID_STATUSBAR
 */

void PM301::OnStatusbarUpdate( wxUpdateUIEvent& event )
{
////@begin wxEVT_UPDATE_UI event handler for ID_STATUSBAR in PM301.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_UPDATE_UI event handler for ID_STATUSBAR in PM301.
}


void PM301::ToggleBatchMode(void)
{
    static bool batchstate = false;
    basiccontrol->Show(batchstate);
    if(batchstate && !checkjog->IsChecked())
        axradiobox->Show(false);

    batchmodelog->Show(!batchstate);
    batchstate = !batchstate;
    mainswitcher->Layout();

}

/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM
 */

void PM301::ClickUnitConvConsts( wxCommandEvent& event )
{
    ToggleBatchMode();
}


/*
 * wxEVT_LEFT_DOWN event handler for ID_BUTTON1
 */

void PM301::LeaveBatchModeButtonPressed( wxMouseEvent& event )
{
    ToggleBatchMode();
}


/*
 * wxEVT_LEFT_DOWN event handler for ID_BUTTON
 */

void PM301::LoadBatchFileDialog( wxMouseEvent& event )
{
    wxFileDialog dialog(GetParent(), _T("choose a file"), wxEmptyString, wxEmptyString,
                        _T("Batch Files (*.bat)|*.bat"), wxOPEN);
    if(dialog.ShowModal() == wxID_OK)
    {
        BatchThread *thread = new BatchThread(dialog.GetPath(),batchmodetextctl, this);
        if(thread->Create() != wxTHREAD_NO_ERROR )
        {
            wxLogError(wxT("Can't create thread"));
        }else {
            thread->Run();
        }
    }
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM1
 */

void PM301::OnMenuitem1Click( wxCommandEvent& event )
{
    ToggleBatchMode();
}

void *BatchThread::Entry()
{
    for(size_t i = 0; i < file.GetLineCount(); i++)
    {
        batchmodetextctl_->AppendText(file[i] + _T("\n"));
        if(file[i].length() > 5 && !file[i].substr(0,5).compare(_T("sleep"))) {
            wxString tmp(file[i].substr(6));
            sleep(wxAtoi(tmp));
        }

        pm301->SendMessage(file[i]);
    }
    return NULL;
}


void PositionUpdateThread::WriteText(const wxString& text, const size_t n) const
{
    wxString msg;

    // before doing any GUI calls we must ensure that this thread is the only
    // one doing it!

    wxMutexGuiEnter();

    msg << text;
    pm301->statusbar->SetStatusText(msg, n);

    wxMutexGuiLeave();
}

void* PositionUpdateThread::Entry()
{
    while(true) {
        if ( TestDestroy() )
            break;

        wxString text;
        Position cp = pm301->getcurpos();
        text.Printf(wxT("Position: x: %d,  y: %d,  \u0398: %d"),
                    cp.get_x(), cp.get_y(), cp.get_theta());
        WriteText(text,0);
        wxThread::Sleep(800);
    }
    return NULL;
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM2
 */

void PM301::OnPositionUpdateClick( wxCommandEvent& event )
{
    PositionUpdateThread *thread = new PositionUpdateThread(this);
    if(thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread"));
    }else {
        thread->SetPriority(20);
        if(thread->Run() != wxTHREAD_NO_ERROR)
            wxLogError(wxT("Can't run thread"));
    }
}
