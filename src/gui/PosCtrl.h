#ifndef __POSCTRL__
#define __POSCTRL__

#include "wx/textctrl.h"
#include "wx/valtext.h"

//this is used inside the PM301 main frame for the #axes Position widgets
#define ID_POSCTRLS 9000

class PM301;

//----------------------------------------------------------------------------
// ComTextCtrl
//----------------------------------------------------------------------------
class ComTextCtrl: public wxTextCtrl
{
public:
    ComTextCtrl( wxWindow* parent, wxWindowID id, const wxString& value = "",
                 const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                 long style = wxTE_PROCESS_ENTER, const wxValidator& validator = wxDefaultValidator,
                 const wxString& name = wxTextCtrlNameStr ) :
        wxTextCtrl( parent, id, value, pos, size, style, validator , name )
        {
        }
    bool CheckValue( void )  { return save_value != GetValue(); };
    void SetValue( const wxString& value )  { save_value = value; wxTextCtrl::SetValue( value ); };

private:
    wxString save_value; //!< old value gets saved if command is issued

    DECLARE_EVENT_TABLE()
};


//----------------------------------------------------------------------------
// PosCtrl
//----------------------------------------------------------------------------

class PosCtrl: public ComTextCtrl
{
public:
    PosCtrl( wxWindow* parent, PM301& ref, wxWindowID id, const wxString& value = "",
             const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
             long style = wxTE_PROCESS_ENTER, const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxTextCtrlNameStr);
    void SetValidatorValue( const wxString& value )  { dummy_value = value; ComTextCtrl::SetValue( value ); };
    void SetDoubleValue( double value );
    double GetDoubleValue() const;
    void SetIncrement( double inc ) { incr_ = inc; };
    double GetIncrement() const { return incr_; };
    void Increment() { SetDoubleValue(GetDoubleValue() + GetIncrement()); };
    void Decrement() { SetDoubleValue(GetDoubleValue() - GetIncrement()); };
    unsigned int GetAxisIdx() const { return  GetId() - ID_POSCTRLS; };


private:
    void OnKeyUp( wxKeyEvent& event );
    void OnKeyDown( wxKeyEvent& event );
    void OnEnter( wxCommandEvent & event);

    //Ask Werner Smekal
    wxString dummy_value; //!< value for wxTextValidato
    PM301& ref_;
    double incr_;

    DECLARE_EVENT_TABLE()
};


#endif // __POSCTRL__
