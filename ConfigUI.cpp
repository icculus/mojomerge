/*  ConfigUI.cpp - Implementation of ConfigUI object
 *
 */

#include "Identifiers.h"
#include "ConfigUI.h"
#include "wx/treectrl.h"

using namespace MojoMerge;

ConfigUI::ConfigUI(Config *MyConfig) : wxDialog(NULL, -1, (wxString)wxT("Preferences"),
    wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    // TODO - This is just temporary to show the basic layout of the
    // preferences window.
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *item1 = new wxBoxSizer( wxHORIZONTAL );

    wxTreeCtrl *item2 = new wxTreeCtrl( this, -1, wxDefaultPosition, wxSize(120,160), wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxSUNKEN_BORDER );
    item1->Add( item2, 1, wxGROW|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    wxBoxSizer *item3 = new wxBoxSizer( wxVERTICAL );

    wxCheckBox *item4 = new wxCheckBox( this, -1, wxT("Ignore case"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item4, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxCheckBox *item5 = new wxCheckBox( this, -1, wxT("Ignore Carriage Return and Line Feed"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item5, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxCheckBox *item6 = new wxCheckBox( this, -1, wxT("Check"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item6, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    item1->Add( item3, 0, wxGROW|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    item0->Add( item1, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxBoxSizer *item7 = new wxBoxSizer( wxHORIZONTAL );

    wxButton *item8 = new wxButton( this, -1, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->Add( item8, 1, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    wxButton *item9 = new wxButton( this, -1, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->Add( item9, 1, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    wxButton *item10 = new wxButton( this, -1, wxT("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->Add( item10, 1, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    item0->Add( item7, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    //if (set_sizer)
    //{
        this->SetAutoLayout( TRUE );
        this->SetSizer( item0 );
        //if (call_fit)
        //{
            item0->Fit( this );
            item0->SetSizeHints( this );
        //}
    //}

    //item0->Show();
}