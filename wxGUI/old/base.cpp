// base.cpp
#include <boost/asio.hpp>
#include <wx/wx.h>
#include "base.hpp"

IMPLEMENT_APP(HalloWeltApp)

bool HalloWeltApp::OnInit() {
   BasicFrame *frame =
      new BasicFrame( wxT("Hallo Welt"), 50, 50, 450, 300);
   frame->Show(TRUE);
   SetTopWindow(frame);
   return TRUE;
}

BasicFrame::BasicFrame (
   const wxChar *title,
   int xpos, int ypos,
   int width, int height)
   : wxFrame ( (wxFrame *) NULL,
               -1, title,
               wxPoint(xpos, ypos),
               wxSize(width, height)) {
    wxMenu *fileMenu = new wxMenu;
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
                     wxT("Show about dialog"));
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),
                     wxT("Quit this program"));

    wxMenuBar *menubar = new wxMenuBar();
    menubar->Append(fileMenu, wxT("&File"));
    menubar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(menubar);


}

BasicFrame::~BasicFrame() { }
