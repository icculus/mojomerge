/*  CompareFilePanel.cpp - Implementation of the CompareFilePanel class
 *
 */

#include "DataTypes.h"
#include "Identifiers.h"
#include "CompareFilePanel.h"
#include "wx/filedlg.h"
#include "Application.h"

using namespace MojoMerge;

CompareFilePanel::CompareFilePanel(wxWindow *Parent)
{
    // Set member defaults
    VerticalSizer = NULL;
    TopHorizontalSizer = NULL;
    FileComboBox = NULL;
    PickFileButton = NULL;
    FileTextEdit = NULL;
    Buffer = NULL;

    // TODO - Need to get color from config
    // Color for highlighting changes

    wxString dummyarray[] = 
    {
        wxT("file name test")
    };

    // Create the window where all the controls will be placed
    Create(Parent, ID_COMPARE_FILE_PANEL);

    // Create sizers for arranging the button, combobox, and text controls
    VerticalSizer = new wxBoxSizer(wxVERTICAL);
    TopHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create the file selection combo box
    wxComboBox *FileComboBox = new wxComboBox(this, ID_FILE_COMBO_BOX, wxT(""),
        wxDefaultPosition, wxSize(100,-1), 1, dummyarray, wxCB_DROPDOWN);
    // Add it to the appropriate sizer
    TopHorizontalSizer->Add(FileComboBox, 1, wxALIGN_CENTRE, 5);

    // Create the pick file button
    PickFileButton = new wxButton(this, ID_PICK_FILE_BUTTON,
        wxT("..."), wxDefaultPosition, wxSize(20,20), 0);
    // Add it to the appropriate sizer
    TopHorizontalSizer->Add(PickFileButton, 0, wxALIGN_CENTRE, 5);

    // Add the horizontal sizer to the top of the vertical one
    VerticalSizer->Add(TopHorizontalSizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

    // Create the text control
    FileTextEdit = new DiffTextEdit(this, ID_FILE_TEXT_EDIT);
    // Add text control to sizer
    VerticalSizer->Add(FileTextEdit, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP, 5);

    // Set parameters for sizers to they resize automagically
    SetAutoLayout(true);
    SetSizer(VerticalSizer);
    VerticalSizer->Fit(this);
    VerticalSizer->SetSizeHints(this);
}

CompareFilePanel::~CompareFilePanel()
{
    // Deletes the original file buffer if set
    if(Buffer)
        delete Buffer;

    // All other members are wxWindow based objects and are therefore cleaned
    //  up automatically upon deletion of their container.
}

void CompareFilePanel::SetFile()
{
    // Prompt user for a file to open
    Filename = wxFileSelector(wxT("Choose a file to open"));

    // Make sure the user didn't cancel
    if(!Filename.IsEmpty())
    {
        // Delete buffer if it was previously allocated
        if(Buffer)
            delete Buffer;

        // Read the entire file and store it
        Buffer = Application::ReadFile(Filename, true);
        // Fill the textbox with the file buffer
        FileTextEdit->DisplayText(Buffer);
    }
}

wxString CompareFilePanel::GetFile()
{
    // Return the name of the file
    return Filename;
}

void CompareFilePanel::OnPickFileButton(wxCommandEvent& event)
{
    // Let the user choose a file
    SetFile();
}

void CompareFilePanel::Recompare(Hunk *FirstHunk, DiffFileNumber FileNumber)
{
    Hunk *p;                    // Temporary hunk pointer
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

        // Set readonly back to true so user can't edit it
        //FileTextEdit->SetReadOnly(true);
    }
    // Else, report the error
    else
        wxMessageBox(wxT("File could not be open!"), wxT("Error"), wxOK | wxICON_ERROR);
}

int CompareFilePanel::GetTextCtrlOffset()
{
    // Difference in height of this panel and the DiffTextEdit control
    int TextCtrlOffset;
    // Find the difference in height of the text control and entire panel
    //  and save it.
    int dummy, height1, height2;
    FileTextEdit->GetClientSize(&dummy, &height1);
    this->GetClientSize(&dummy, &height2);
    TextCtrlOffset = height2 - height1;
    Application::Debug("Height difference = %d", TextCtrlOffset);
    return TextCtrlOffset;
}

const char *CompareFilePanel::GetBuffer()
{
    return Buffer;
}

DiffTextEdit *CompareFilePanel::GetDiffTextEdit()
{
    return FileTextEdit;
}

// Event mappings for the file panel
BEGIN_EVENT_TABLE(CompareFilePanel, wxPanel)
    EVT_BUTTON(ID_PICK_FILE_BUTTON, CompareFilePanel::OnPickFileButton)
END_EVENT_TABLE()

