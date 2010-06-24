// base.cpp
#include <wx/wx.h>
#include "base.h"

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
               wxSize(width, height)) { }

BasicFrame::~BasicFrame() { }

