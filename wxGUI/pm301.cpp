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


#include <vector>
#include "wx/tokenzr.h"
#include "wx/socket.h"
#include "pm301.h"
#include "unitconversionconstants.h"


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

    EVT_SPINCTRL( ID_SPINCTRL, PM301::OnSpinctrlUpdated )

    EVT_SPINCTRL( ID_SPINCTRL1, PM301::OnSpinctrl1Updated )

    EVT_SPINCTRL( ID_SPINCTRL2, PM301::OnSpinctrl2Updated )

    EVT_CHECKBOX( ID_CHECKBOX, PM301::OnCheckboxClick )

    EVT_RADIOBOX( ID_RADIOBOX, PM301::OnRadioboxSelected )

    EVT_UPDATE_UI( ID_STATUSBAR, PM301::OnStatusbarUpdate )

////@end PM301 event table entries

EVT_SOCKET(SOCKET_ID, PM301::OnSocketEvent )

END_EVENT_TABLE()

/*
 * PM301 constructors
 */

PM301::PM301()
{
    Init();
}

PM301::PM301( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
    xSpinCtrl = NULL;
    ySpinCtrl = NULL;
    tSpinCtrl = NULL;
    jogmodelayout = NULL;
    checkjog = NULL;
    axradiobox = NULL;
    batchmodelog = NULL;
    batchmodetextctl = NULL;
    statusbar = NULL;
////@end PM301 member initialisation

    wxIPV4address addr;
    addr.Hostname(wxT("localhost"));
    addr.Service(15000);

    s = new wxSocketClient();

    s->SetEventHandler(*this, SOCKET_ID);
    s->SetNotify(wxSOCKET_INPUT_FLAG|wxSOCKET_LOST_FLAG);

    s->Notify(true);

    //Block the GUI
    s->Connect(addr, true);

    double max = 5000.0;
    coord_limits[0][0] = -max;
    coord_limits[0][1] = max;
    coord_limits[1][0] = -max;
    coord_limits[1][1] = max;
    coord_limits[2][0] = -max;
    coord_limits[2][1] = max;
}

void PM301::OnSocketEvent(wxSocketEvent& event)
{
    wxSocketBase* sock = event.GetSocket();

    switch(event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        std::cout << "INPUT event received" << std::endl;
        sock->Read(reinterpret_cast<char*>(&reply), msglen);
        break;
    case wxSOCKET_LOST:
    default:
        std::cerr << "lost network connection to server" << std::endl;
        sock->Destroy();
        break;

    }


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

    xSpinCtrl = new wxSpinCtrl( itemFrame1, ID_SPINCTRL, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 5000, 0 );
    itemBoxSizer12->Add(xSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemFrame1, wxID_STATIC, _("y:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemStaticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    ySpinCtrl = new wxSpinCtrl( itemFrame1, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 5000, 0 );
    itemBoxSizer15->Add(ySpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText19 = new wxStaticText( itemFrame1, wxID_STATIC, _("t:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemStaticText19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    tSpinCtrl = new wxSpinCtrl( itemFrame1, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 5000, 0 );
    itemBoxSizer18->Add(tSpinCtrl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine21 = new wxStaticLine( itemFrame1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    basiccontrol->Add(itemStaticLine21, 0, wxGROW, 5);

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

    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    batchmodelog->Add(itemBoxSizer26, 0, wxGROW|wxALL, 0);

    itemBoxSizer26->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton28 = new wxButton( itemFrame1, ID_BUTTON, _("Load Bach File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer26->Add(itemButton28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton29 = new wxButton( itemFrame1, ID_BUTTON1, _("Quit Batch Mode"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer26->Add(itemButton29, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    batchmodetextctl = new wxTextCtrl( itemFrame1, ID_TEXTCTRL6, _("Batch Log"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    batchmodelog->Add(batchmodetextctl, 1, wxGROW|wxALL, 5);

    statusbar = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    statusbar->SetFieldsCount(2);
    statusbar->SetStatusText(_("hello"), 0);
    statusbar->SetStatusText(_("abc"), 1);
    itemFrame1->SetStatusBar(statusbar);

    wxToolBar* itemToolBar32 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR );
    itemToolBar32->Show(false);
    wxButton* itemButton33 = new wxButton( itemToolBar32, ID_BUTTON2, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemToolBar32->AddControl(itemButton33);
    itemToolBar32->Realize();
    itemFrame1->SetToolBar(itemToolBar32);

    // Connect events and objects
    itemButton28->Connect(ID_BUTTON, wxEVT_LEFT_DOWN, wxMouseEventHandler(PM301::LoadBatchFileDialog), NULL, this);
    itemButton29->Connect(ID_BUTTON1, wxEVT_LEFT_DOWN, wxMouseEventHandler(PM301::LeaveBatchModeButtonPressed), NULL, this);
////@end PM301 content construction
//std::locale::global(locale(setlocale(LC_ALL, NULL)));
// m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT | wxLOCALE_CONV_ENCODING);

    //SendMessage("gp1");
    //FIXME unit conversion
    //new_x_pos->SetValue(wxString::Format(wxT("%f"), atof(reply.msg));
    batchmodelog->Show(false);
    mainswitcher->Layout();

    Position cp = getcurpos();
    xSpinCtrl->SetValue((int)cp.get_x());
    ySpinCtrl->SetValue((int)cp.get_y());
    tSpinCtrl->SetValue((int)cp.get_theta());

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
    s->Write(reinterpret_cast<char*>(&request), msglen);
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



int PM301::convert_to_stepper_units(const float pos, const wxString &coord) const
{
    if(coord == wxT("x"))
        return (int)pos/x_unit_conv;
    else if(coord == wxT("y"))
        return (int)pos/y_unit_conv;
    else if(coord == wxT("phi"))
        return (int)pos/phi_unit_conv;
    else
        return -1;
}

float PM301::convert_to_natural_units(const int pos, const wxString &coord) const
{
    if(coord == wxT("x"))
        return pos*x_unit_conv;
    else if(coord == wxT("y"))
        return pos*y_unit_conv;
    else if(coord == wxT("phi"))
        return pos*phi_unit_conv;
    else
        return -1.0;
}


int PM301::getIdxFromCoord(const wxString &coord)
{
    if(coord == wxT("x"))
        return 0;
    else if(coord == wxT("y"))
        return 1;
    else if(coord == wxT("phi"))
        return 2;
    else
        return -1;
}


void PM301::check_and_update_position(wxSpinCtrl* ctrl, const wxString& coord, const double curval)
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
        wxString tmp = wxT("Entered ") + coord + wxT(" position (%f) is out of range. Move motor to ") + coord + wxT("%s=%f ?");
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
            
            tmp.Printf(wxT("sa%d"),idx);
            SendMessage(tmp);
            tmp.Printf(wxT("1MA%d"),  (int)(greatermax ? coord_limits[idx][1] : coord_limits[idx][0]));
            SendMessage(tmp);
            break;
        case wxID_NO:
            //wxLogError(wxT("no"));
            break;
        default:
            wxLogError(wxT("Unexpected wxMess Dialog return code!"));
        }
    }
    else {
        wxString tmp;
            tmp.Printf(wxT("sa%d"),idx);
            SendMessage(tmp);
            tmp.Printf(wxT("1MA%d"), (int)curval);
            SendMessage(tmp);
    }

}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

void PM301::OnCheckboxClick( wxCommandEvent& event )
{
    if(checkjog->IsChecked()) {
        SendMessage("1AR");
        axradiobox->Show(true);
        SendMessage("sa0");

    } else
    {
        SendMessage("1IR");
        axradiobox->Show(false);
    }

    jogmodelayout->Layout();
}


Position PM301::getcurpos()
{
    static int a=10;
    static int b=20;
    static int c=30;

    SendMessage("pbp");
    s->Read(reinterpret_cast<char*>(&reply), msglen);
    wxString text(reply.msg,wxConvUTF8);

    //text.Printf(_T(" axis1: %d\n axis2: %d\n axis3:\n"), a,b,c);

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
        wxThread::Sleep(900);
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


void PM301::GeneralSpinCtrlUpdate(const wxString& coord)
{
    wxSpinCtrl *cur = NULL;
    //wxSpinCtrlDouble *tmp = NULL;
    if(coord == wxT("x"))
        cur = xSpinCtrl;
    else if(coord == wxT("y"))
        cur = ySpinCtrl;
    else if(coord == wxT("phi"))
        cur = tSpinCtrl;
    else
        return;

    int entered = cur->GetValue();
    double value = entered;
    //if(!entered.ToDouble(&value)){
    //    wxMessageBox(wxT("Entered string is not a number!"), wxT("Warning"), wxOK | wxICON_INFORMATION, this);
        //set default value ?!?!?
    //    return;
    //}

    check_and_update_position(cur, coord, value);

}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SPINCTRL
 */

//void PM301::OnxSpinCtrlTextUpdated( wxCommandEvent& event )
//{
//    GeneralSpinCtrlUpdate(wxT("x"));
//}





/*
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL
 */

void PM301::OnSpinctrlUpdated( wxSpinEvent& event )
{
    GeneralSpinCtrlUpdate(wxT("x"));
}


/*
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL1
 */

void PM301::OnSpinctrl1Updated( wxSpinEvent& event )
{
    GeneralSpinCtrlUpdate(wxT("y")); 
}


/*
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SPINCTRL2
 */

void PM301::OnSpinctrl2Updated( wxSpinEvent& event )
{
    GeneralSpinCtrlUpdate(wxT("phi"));
}

