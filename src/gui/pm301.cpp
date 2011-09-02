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
#include "wx/vector.h"
#include "wx/tokenzr.h"
#include "wx/socket.h"
#include "pm301.h"

//#include "sample.xpm"

////@begin XPM images
////@end XPM images

//#undef TEST_NETWORK
#define TEST_NETWORK

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

    EVT_BUTTON( ID_BUTTON3, PM301::OnButtonZeroPositionClick )

    EVT_CHECKBOX( ID_CHECKBOX, PM301::OnCheckboxClick )

    EVT_UPDATE_UI( ID_STATUSBAR, PM301::OnStatusbarUpdate )

////@end PM301 event table entries

    EVT_SOCKET(SOCKET_ID, PM301::OnSocketEvent )
    
    EVT_RADIOBOX( ID_AXESRADIOBOX, PM301::OnRadioboxSelected )

END_EVENT_TABLE()

/*
 * PM301 constructors
 */

PM301::PM301()
{
    Init();
}

PM301::PM301( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) :
   m_mutex()
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
    jogmodelayout = NULL;
    checkjog = NULL;
    batchmodelog = NULL;
    batchmodetextctl = NULL;
    statusbar = NULL;
////@end PM301 member initialisation
    posthread=NULL;

#ifdef TEST_NETWORK
    std::cout << "connecting to localhost" << std::endl;
    wxIPV4address addr;
    addr.Hostname(wxT("localhost"));
    addr.Service(16000);

    s = new wxSocketClient();

    s->SetEventHandler(*this, SOCKET_ID);
    s->SetNotify(wxSOCKET_INPUT_FLAG|wxSOCKET_LOST_FLAG);

    s->Notify(true);

    //Block the GUI
    s->Connect(addr, true);
    std::cout << "connected to localhost" << std::endl;
#endif
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
    itemMenu3->Append(ID_MENUITEM, _("Change Unit-Conversion-Constants"), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Enable(ID_MENUITEM, false);
    itemMenu3->Append(ID_MENUITEM1, _("Batch Mode"), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Append(ID_MENUITEM2, _("Position Update Thread"), wxEmptyString, wxITEM_CHECK);
    menuBar->Append(itemMenu3, _("Operations"));
    itemFrame1->SetMenuBar(menuBar);

    mainswitcher = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(mainswitcher);

    basiccontrol = new wxBoxSizer(wxHORIZONTAL);
    mainswitcher->Add(basiccontrol, 0, wxGROW, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
    basiccontrol->Add(itemBoxSizer9, 1, wxALIGN_TOP|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(itemFrame1, wxID_ANY, _("Set New Stepper Position"));
    wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxVERTICAL);
    itemBoxSizer9->Add(itemStaticBoxSizer10, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton11 = new wxButton( itemFrame1, ID_BUTTON3, _("Set current position as zero position"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemButton11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticLine* itemStaticLine12 = new wxStaticLine( itemFrame1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
    basiccontrol->Add(itemStaticLine12, 0, wxGROW, 5);

    jogmodelayout = new wxBoxSizer(wxVERTICAL);
    basiccontrol->Add(jogmodelayout, 0, wxGROW|wxALL, 5);

    checkjog = new wxCheckBox( itemFrame1, ID_CHECKBOX, _("Jog Mode"), wxDefaultPosition, wxDefaultSize, 0 );
    checkjog->SetValue(false);
    jogmodelayout->Add(checkjog, 0, wxGROW, 5);

    batchmodelog = new wxBoxSizer(wxVERTICAL);
    mainswitcher->Add(batchmodelog, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    batchmodelog->Add(itemBoxSizer16, 0, wxGROW|wxALL, 0);

    itemBoxSizer16->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton18 = new wxButton( itemFrame1, ID_BUTTON, _("Load Bach File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemButton18, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton19 = new wxButton( itemFrame1, ID_BUTTON1, _("Quit Batch Mode"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemButton19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    batchmodetextctl = new wxTextCtrl( itemFrame1, ID_TEXTCTRL6, _("Batch Log"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    batchmodelog->Add(batchmodetextctl, 1, wxGROW|wxALL, 5);

    statusbar = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
    statusbar->SetFieldsCount(1);
    itemFrame1->SetStatusBar(statusbar);

    wxToolBar* itemToolBar22 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR );
    itemToolBar22->Show(false);
    wxButton* itemButton23 = new wxButton( itemToolBar22, ID_BUTTON2, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemToolBar22->AddControl(itemButton23);
    itemToolBar22->Realize();
    itemFrame1->SetToolBar(itemToolBar22);

    // Connect events and objects
    itemButton18->Connect(ID_BUTTON, wxEVT_LEFT_DOWN, wxMouseEventHandler(PM301::LoadBatchFileDialog), NULL, this);
    itemButton19->Connect(ID_BUTTON1, wxEVT_LEFT_DOWN, wxMouseEventHandler(PM301::LeaveBatchModeButtonPressed), NULL, this);
////@end PM301 content construction
//std::locale::global(locale(setlocale(LC_ALL, NULL)));
// m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT | wxLOCALE_CONV_ENCODING);

    //retreive nr. of axes and coordinate names from the server
    initaxes();

    const Position initpos = getcurpos();
    set_cp(initpos);
    wxArrayString axesradioboxStrings;

    for(size_t i=0; i < get_nraxes(); ++i) {
        axisbs.push_back(new wxBoxSizer(wxHORIZONTAL));
        axisst.push_back(new wxStaticText(itemStaticBoxSizer10->GetStaticBox(), -1,
                                          wxString::Format("%s [%s]:", *coords[i], *units[i]),
                                          wxDefaultPosition, wxDefaultSize, 0));
        axissc.push_back(new wxSpinCtrlDouble(itemStaticBoxSizer10->GetStaticBox(), ID_SPINCTRLS+i, _T("0"),
                                              wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -5000, 5000, 0));
        axisbb.push_back(new wxBitmapButton(itemStaticBoxSizer10->GetStaticBox(), ID_BITMAPBUTTONS+i,
                                            wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW));

        axissc[i]->SetValue(initpos.GetCoordinate(i+1));
        axissc[i]->SetDigits(2);
        axissc[i]->SetIncrement(0.01);

        axisbs[i]->Add(axisst[i], 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
        axisbs[i]->Add(axissc[i], 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
        axisbs[i]->Add(axisbb[i], 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
        itemStaticBoxSizer10->Add(axisbs[i], 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

        axissc[i]->Bind(wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, &PM301::OnSpinCTRLUpdated, this);
        axisbb[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &PM301::OnBitmapbuttonClick, this);

        axesradioboxStrings.Add(wxString::Format("&%s",*coords[i]));
    }

    axesradiobox = new wxRadioBox( itemFrame1, ID_AXESRADIOBOX, _("Axis"), wxDefaultPosition,
                                   wxDefaultSize, axesradioboxStrings, 1, wxRA_SPECIFY_COLS );
    axesradiobox->SetSelection(0);
    axesradiobox->Show(false);
    jogmodelayout->Add(axesradiobox, 0, wxGROW, 5);
    //itemStaticBoxSizer10Static->Disable();
    itemButton11->Disable();
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
#ifdef TEST_NETWORK
    s->Write(reinterpret_cast<char*>(&request), msglen);
#endif
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
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_AXESRADIOBOX
 */

void PM301::OnRadioboxSelected( wxCommandEvent& event )
{
    char buf[4];
    int selected = axesradiobox->GetSelection();
    sprintf(buf,"sa%d",selected+1);
    SendMessage(buf);
}


void PM301::check_and_update_position(wxSpinCtrlDouble* ctrl, wxString& axdesc, const double curval)
{
    wxString text;
    text.Printf("ma %s=%f", axdesc, curval);
    std::cout << "command to update pos: " << text.c_str() << std::endl;
    SendMessage(text);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX
 */

void PM301::OnCheckboxClick( wxCommandEvent& event )
{
    if(checkjog->IsChecked()) {
        SendMessage("1AR");
        axesradiobox->Show(true);
        SendMessage("sa1");

    } else
    {
        SendMessage("1IR");
        axesradiobox->Show(false);
    }

    jogmodelayout->Layout();
}

void PM301::initaxes()
{
    SendMessage("ga"); //get axes
#ifdef TEST_NETWORK
    s->Read(reinterpret_cast<char*>(&reply), msglen);
    wxString text(reply.msg,wxConvUTF8);
#else
    wxString text;

    text.Printf("1:x:mm\n2:y:mm\n3:z:um\n4:\u0398:deg\n5:k:m\n6:l:km\n");
#endif
    std::cout << "Command \"ga\" returned: " << text.c_str() << std::endl;
    wxVector<Position::type> vec;
    wxStringTokenizer tkz(text, wxT("\n"));
    while ( tkz.HasMoreTokens() )
    {
	wxStringTokenizer line(tkz.GetNextToken(), wxT(":"));
	unsigned long id;
        line.GetNextToken().ToULong(&id);
        wxString coordname = line.GetNextToken();
	wxString unitname = line.GetNextToken();
     
	std::cout << "GUI registering axis with id " << id << ", coordname " << coordname
		  << "and unit " << unitname
                  << std::endl;
        coords.push_back(new wxString(coordname));
        units.push_back(new wxString(unitname));
    }

    nraxes = coords.size();
}


Position PM301::getcurpos()
{
    SendMessage("pp");
#ifdef TEST_NETWORK
    s->Read(reinterpret_cast<char*>(&reply), msglen);
    wxString text(reply.msg,wxConvUTF8);
#else
    static double pos[]={3.21,91.19,324.19,-1239.09, 9234,93,-0.2};
    wxString text;
    for(size_t i=0; i < get_nraxes(); ++i) {
        text.Append(wxString::Format(" axis%d: %lf\n", i+1, pos[i]));
        pos[i] += 0.9;
    }
#endif

    wxVector<Position::type> vec;
    wxStringTokenizer tkz(text, wxT("\n"));
    while ( tkz.HasMoreTokens() )
    {
        wxString token = tkz.GetNextToken();
        Position::type v;
	double vtmp;
	//char axdesc[32];
	//char unitdesc[8];
		
        wxString axdesc  = token.BeforeFirst(':');
        token.AfterFirst(':').ToDouble(&vtmp);
	v = vtmp;

	// sscanf(token.mb_str(),"%s[%s]: %f", axdesc, unitdesc, &v);
        std::cout << "GUI_position_parser: " << axdesc.c_str()
		   << " " << v << std::endl;
        vec.push_back( v );
    }

    Position p;
    for(size_t i=0; i < vec.size(); ++i) {
        p.SetCoordinate(i+1,vec[i]);
    }
    return p;
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
        axesradiobox->Show(false);

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
                        _T("Batch Files (*.bat)|*.bat"), wxFD_OPEN);
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
    batchmodetextctl_->AppendText(_T("Finished executing batch file"));

    return NULL;
}


void PositionUpdateThread::WriteText(const wxString& text, const Position &pos, const size_t n) const
{
    wxString msg;

    // before doing any GUI calls we must ensure that this thread is the only
    // one doing it!

    wxMutexGuiEnter();

    msg << text;
    pm301->statusbar->SetStatusText(msg, n);

    //FIXME element under focus should not get updated
    //pm301->xSpinCtrl->SetValue(pos.GetCoordinate(1));
    //pm301->ySpinCtrl->SetValue(pos.GetCoordinate(2));
    //pm301->tSpinCtrl->SetValue(pos.GetCoordinate(3));

    wxMutexGuiLeave();
}

void* PositionUpdateThread::Entry()
{
    while(true) {
        if ( TestDestroy() )
            break;

        wxString text;
        Position cp = pm301->getcurpos();
        pm301->set_cp(cp); // so that the main frame can use it

        //TODO only output BarePosition to statusbar
        // or ???
        text.Printf(wxT("Position:"));
        for(size_t i = 0; i < pm301->get_nraxes(); ++i) {
	  text.Append(wxString::Format("%s: %.4f %s\t", *pm301->coords[i], cp.GetCoordinate(i+1), *pm301->units[i]));
        }
        WriteText(text,cp, 0);
        wxThread::Sleep(850);
    }
    return NULL;
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM2
 */

void PM301::OnPositionUpdateClick( wxCommandEvent& event )
{
    if(!event.IsChecked()) {
        if (posthread->Delete() != wxTHREAD_NO_ERROR )
            wxLogError(wxT("Can't delete the thread!"));
        else
            posthread = NULL;
            statusbar->SetStatusText(wxEmptyString, 0);

    }
    else
    {
        posthread = new PositionUpdateThread(this);
        if(posthread->Create() != wxTHREAD_NO_ERROR )
        {
            wxLogError(wxT("Can't create thread"));
        }else {
            posthread->SetPriority(20);
            if (posthread->Run() != wxTHREAD_NO_ERROR) {
                wxLogError(wxT("Can't run thread"));
            }
        }
    }
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON3
 */

void PM301::OnButtonZeroPositionClick( wxCommandEvent& event )
{
    wxMessageDialog dialog( NULL, wxT("Are you sure that you want to reset the stepper positions to zero?"),
                            wxT("Warning"), wxNO_DEFAULT | wxYES_NO | wxICON_INFORMATION);
        switch(dialog.ShowModal()) {
        case wxID_YES:
            // only call this if position update thread is not running
            //if(posthread == NULL) { axissc::iterator end;

            for (wxVector<wxSpinCtrlDouble*>::iterator it = axissc.begin(); it != axissc.end(); ++it)
                (*it)->SetValue(0.0);
            //}
            SendMessage("set zero");
            break;
        case wxID_NO:
            //wxLogError(wxT("no"));
            break;
        default:
            wxLogError(wxT("Unexpected wxMess Dialog return code!"));
        }
}

void PM301::OnSpinCTRLUpdated( wxCommandEvent& event ) //wxSpinDoubleEvent& event )
{
    int id = event.GetId() - ID_SPINCTRLS;
    //std::cout << "ID: " << id << std::endl;
    if(id < 0 || id >= (int)get_nraxes()) {
        wxLogError("Event ID in SpinCtrl Handler wrong");
    }

    double value = axissc[id]->GetValue();
    std::cout << value << " sc value update " << std::endl;
    // if(!entered.ToDouble(&value)){
    //     wxMessageBox(wxT("Entered string is not a number!"), wxT("Warning"), wxOK | wxICON_INFORMATION, this);
    //     //set default value ?!?!?
    //     return;
    // }

    check_and_update_position(axissc[id], *coords[id] , value);
}

void PM301::OnBitmapbuttonClick( wxCommandEvent& event )
{
    // wxBitmapButton* ptr = (wxBitmapButton*)event.GetEventObject();
    // int i = 0;
    // for (wxVector<wxBitmapButton*>::iterator it = axisbb.begin(); it != axisbb.end(); ++it, i++) {
    //     if (ptr == *it)
    //         break;
    // }

    int id = event.GetId() - ID_BITMAPBUTTONS;
    if(id < 0 || id >= (int)get_nraxes()) {
        wxLogError("Event ID in Bitmapbutton Handler wrong");
    }

    std::cout << " bitmap button " << id << " clicked " << std::endl;
    axissc[id]->SetValue(get_cp().GetCoordinate(id+1));
}
