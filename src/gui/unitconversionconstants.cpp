/////////////////////////////////////////////////////////////////////////////
// Name:        unitconversionconstants.cpp
// Purpose:     
// Author:      Thomas Hisch
// Modified by: 
// Created:     Sat 11 Jun 2011 11:27:05 CEST
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

#include "unitconversionconstants.h"
#include "pm301.h"

////@begin XPM images
////@end XPM images


/*
 * UnitConversionConstants type definition
 */

IMPLEMENT_CLASS( UnitConversionConstants, wxFrame )


/*
 * UnitConversionConstants event table definition
 */

BEGIN_EVENT_TABLE( UnitConversionConstants, wxFrame )

////@begin UnitConversionConstants event table entries
    EVT_TEXT_ENTER( ID_TEXTCTRL3, UnitConversionConstants::OnTextctrl3Enter )

////@end UnitConversionConstants event table entries

END_EVENT_TABLE()


/*
 * UnitConversionConstants constructors
 */

UnitConversionConstants::UnitConversionConstants()
{
    Init();
}

UnitConversionConstants::UnitConversionConstants( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
    std::cout << "uconv ctor called" << std::endl;
}


/*
 * UnitConversionConstants creator
 */

bool UnitConversionConstants::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin UnitConversionConstants creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
////@end UnitConversionConstants creation
    return true;
}


/*
 * UnitConversionConstants destructor
 */

UnitConversionConstants::~UnitConversionConstants()
{
////@begin UnitConversionConstants destruction
////@end UnitConversionConstants destruction
    std::cout << "uconv dtor called" << std::endl;
}


/*
 * Member initialisation
 */

void UnitConversionConstants::Init()
{
////@begin UnitConversionConstants member initialisation
////@end UnitConversionConstants member initialisation
}


/*
 * Control creation for UnitConversionConstants
 */

void UnitConversionConstants::CreateControls()
{    
////@begin UnitConversionConstants content construction
    UnitConversionConstants* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl4 = new wxTextCtrl( itemFrame1, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
    itemBoxSizer3->Add(itemTextCtrl4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl6 = new wxTextCtrl( itemFrame1, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemTextCtrl6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( itemFrame1, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemTextCtrl8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end UnitConversionConstants content construction
}


/*
 * Should we show tooltips?
 */

bool UnitConversionConstants::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap UnitConversionConstants::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin UnitConversionConstants bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end UnitConversionConstants bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon UnitConversionConstants::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin UnitConversionConstants icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end UnitConversionConstants icon retrieval
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_TEXTCTRL3
 */

void UnitConversionConstants::OnTextctrl3Enter( wxCommandEvent& event )
{
    wxString msg = wxT("LALA");
    ((PM301*)GetParent())->statusbar->SetStatusText(msg, 1);
}

