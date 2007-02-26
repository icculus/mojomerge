/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  CompareFolderPanel.cpp - Implementation of the CompareFolderPanel class
 *
 */

#include "DataTypes.h"
#include "Identifiers.h"
#include "CompareFolderPanel.h"
#include "wx/filedlg.h"
#include "wx/treectrl.h"
#include "Application.h"

using namespace MojoMerge;

CompareFolderPanel::CompareFolderPanel(wxWindow *Parent)
{
    // Set member defaults
    VerticalSizer = NULL;
    TopHorizontalSizer = NULL;
    TextBox = NULL;
    PickFolderButton = NULL;

    // Create the window where all the controls will be placed
    Create(Parent, ID_COMPARE_FILE_PANEL);

    // Create sizers for arranging the button, combobox, and text controls
    VerticalSizer = new wxBoxSizer(wxVERTICAL);
    TopHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create the file selection combo box
    TextBox = new wxStaticText(this, ID_FOLDER_TEXT_BOX, wxT(""),
        wxDefaultPosition, wxSize(20, 20), wxSUNKEN_BORDER |
        wxST_NO_AUTORESIZE);
    // Add it to the appropriate sizer
    TopHorizontalSizer->Add(TextBox, 1, wxALIGN_CENTRE, 5);

    // Create the pick file button
    PickFolderButton = new wxButton(this, ID_PICK_FOLDER_BUTTON,
        wxT("..."), wxDefaultPosition, wxSize(20, 20), 0);
    // Add it to the appropriate sizer
    TopHorizontalSizer->Add(PickFolderButton, 0, wxALIGN_CENTRE, 5);

    // Add the horizontal sizer to the top of the vertical one
    VerticalSizer->Add(TopHorizontalSizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

    // Create the text control
    TreeWindow = new wxTreeCtrl(this, ID_FOLDER_TREE_CTRL, wxDefaultPosition,
        wxDefaultSize, wxTR_NO_BUTTONS);
    // Add text control to sizer
    VerticalSizer->Add(TreeWindow, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP, 5);

    // Set parameters for sizers to they resize automagically
    SetAutoLayout(true);
    SetSizer(VerticalSizer);
    VerticalSizer->Fit(this);
    VerticalSizer->SetSizeHints(this);
}

CompareFolderPanel::~CompareFolderPanel()
{
    // All other members are wxWindow based objects and are therefore cleaned
    //  up automatically upon deletion of their container.
}

void CompareFolderPanel::SetFolder()
{
    // Prompt user for a file to open
    Folder = wxDirSelector(wxT("Choose a folder to open"));

    // Make sure the user didn't cancel
    if(!Folder.IsEmpty())
    {
        TextBox->SetLabel(Folder);
    }
}

wxString CompareFolderPanel::GetFolder()
{
    // Return the name of the file
    return Folder;
}

void CompareFolderPanel::OnPickFolderButton(wxCommandEvent& event)
{
    // Let the user choose a file
    SetFolder();
}

void CompareFolderPanel::Recompare(FolderHunk *FirstHunk,
DiffFileNumber FolderNumber)
{
    /*Hunk *p;                    // Temporary hunk pointer
    uint32 Start, End;          // Start and end lines for hunks

    // Make sure buffer is valid
    if(Buffer)
    {
        // Clear all styling
        FileTextEdit->MarkClearAll();

        // Get first hunk in the list
        p = FirstHunk;
        // Loop through all hunks and mark changes appropriately
        while(p)
        {
            // Get start and end of hunk for this file
            Start = p->GetStart(FileNumber);
            End = p->GetEnd(FileNumber);
            // If End not specified, just highlight the start line
            if(End == UNSPECIFIED)
                FileTextEdit->MarkInsertAfterLine(Start);
            // Else, set the end position appropriately
            else
                FileTextEdit->MarkChangedLine(Start, End);
            // Go to the next hunk
            p = p->GetNextHunk();
        }
        // Fixed a problem with highlighting (see function for more info)
        FileTextEdit->BlankLineStyleBugHack();

        // Set readonly back to true so user can't edit it
        //FileTextEdit->SetReadOnly(true);
    }
    // Else, report the error
    else
        wxMessageBox(wxT("File could not be open!"), wxT("Error"), wxOK | wxICON_ERROR);*/
}

wxTreeCtrl *CompareFolderPanel::GetTreeWindow()
{
    return TreeWindow;
}

// Event mappings for the file panel
BEGIN_EVENT_TABLE(CompareFolderPanel, wxPanel)
    EVT_BUTTON(ID_PICK_FOLDER_BUTTON, CompareFolderPanel::OnPickFolderButton)
END_EVENT_TABLE()

