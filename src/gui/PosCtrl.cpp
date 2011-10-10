#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "pm301.h"
#include "PosCtrl.h"

//----------------------------------------------------------------------------
// ComTextCtrl
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE( ComTextCtrl, wxTextCtrl )
END_EVENT_TABLE( )

//----------------------------------------------------------------------------
// PosTextCtrl
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE( PosCtrl, wxTextCtrl )
  EVT_KEY_DOWN(PosCtrl::OnKeyDown)
  EVT_KEY_UP(PosCtrl::OnKeyUp)
  EVT_TEXT_ENTER(-1,PosCtrl::OnEnter)
END_EVENT_TABLE( )

PosCtrl::PosCtrl( wxWindow* parent, PM301& ref, wxWindowID id, const wxString& value,
                  const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator,
                  const wxString& name ) :
      ComTextCtrl( parent, id, value, pos, size, style, validator , name ),
      ref_(ref),
      incr_(0.1)
{
  // initialize dummy_value since content of dummy_value is transfered to TextCtrl
  dummy_value = value;

  // set validator - only numeric input is allowed
  // the next line crashes so we use the workaround below
  //wxStringList IncludeList( "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
  //                          "+", "-", "e", "." );
  // set validator - only numeric input is allowed
  // the next line crashes so we use the workaround below
  //wxStringList IncludeList( "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
  //                          "+", "-", "e", "." );
  wxArrayString Includes;
  for(size_t i=0; i < 10; i++){
      Includes.Add(wxString::Format("%u",(unsigned int)i));
  }

  Includes.Add("+");
  Includes.Add("-");
  Includes.Add("e");
  Includes.Add(".");

  wxTextValidator Validator( wxFILTER_INCLUDE_CHAR_LIST , &dummy_value );
  Validator.SetIncludes( Includes );
  this->SetValidator( Validator );
}

void PosCtrl::SetDoubleValue( double value )
{
    ComTextCtrl::SetValue( wxString::Format("%.04f",value) );
}

double PosCtrl::GetDoubleValue() const
{
    wxString text = ComTextCtrl::GetValue();
    double retval;
    text.ToDouble(&retval);
    return retval;
}

void PosCtrl::OnKeyUp( wxKeyEvent& event )
{
    int kc = event.GetKeyCode();

    if ( kc != WXK_UP && kc != WXK_DOWN ) {
        // std::cout << "skip" << std::endl;
        event.Skip();
        return;
    }

    PositionUpdate();
    //SetInsertionPointEnd( );
}

void PosCtrl::OnKeyDown( wxKeyEvent& event )
{
    int kc = event.GetKeyCode();
    // std::cout << "idx:" << event.GetId() << std::endl;

    if ( event.GetKeyCode() != WXK_UP && event.GetKeyCode() != WXK_DOWN ) {
        // std::cout << "skip" << std::endl;
        event.Skip();
        return;
    }

    switch ( kc )
    {
    case WXK_UP:
        Increment();
        break;
    case WXK_DOWN:
        Decrement();
        break;
    }
}

void PosCtrl::PositionUpdate()
{
    wxString text;
    text.Printf("ma %s=%f", *ref_.coords[GetAxisIdx()], GetDoubleValue());
    std::cout << "command to update pos: " << text.c_str() << std::endl;
    text = ref_.SendandReceive(text);
    // std::cout  << "PosCtrl: UpdatePos returned: |"
    //            << text.c_str()  << "|" << std::endl;

    if(!text.StartsWith("OK")) {
        wxMessageBox(text, wxT("Warning"),
                     wxOK | wxICON_INFORMATION, &ref_);

    }

    // always update position after a "ma" call
    Position cp = ref_.getcurpos();
    SetDoubleValue(cp.GetCoordinate(GetAxisIdx()+1));
}


void PosCtrl::OnEnter( wxCommandEvent& event )
{
    PositionUpdate();
}
