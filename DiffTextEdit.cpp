/*  DiffTextEdit.cpp - Implementation of DiffTextEdit class
 *
 */

#include "DiffTextEdit.h"
#include "Application.h"

using namespace MojoMerge;

#define STYLE_CHANGE    1
//#define STYLE_INSERT    2

DiffTextEdit::DiffTextEdit(wxWindow *Parent, wxWindowID id) :
wxStyledTextCtrl(Parent, id, wxDefaultPosition, wxSize(80,40), wxSUNKEN_BORDER)
{
    // Default font to use for text
    wxFont DefaultFont(8, wxMODERN, wxNORMAL, wxNORMAL);
    wxColour DefaultChangeColor(138, 170, 189);

    SetScrollWidth(200);
    StyleSetFont(wxSTC_STYLE_DEFAULT, DefaultFont);
    // Set styles for the change highlighting    
    StyleSetBackground(STYLE_CHANGE, DefaultChangeColor);
    StyleSetEOLFilled(STYLE_CHANGE, true);
    SetMarginWidth(0, 32);
    SetMarginWidth(1, 0);
    // Set styles for the insert after highlighting
    //StyleSetUnderline(STYLE_INSERT, true);
    //StyleSetEOLFilled(STYLE_INSERT, true);
    //SetViewEOL(true);
}

void DiffTextEdit::MarkChangedLine(int Start, int End)
{
    int StartPos, EndPos;       // Start and end position to highlight
    int LineLength;             // Length of line in text control

    // Get actual positions in editor window of lines
    StartPos = PositionFromLine(Start - 1);
    // Get length of end line
    LineLength = this->LineLength(End - 1);
    // Get last position of end line
    EndPos = PositionFromLine(End - 1) + LineLength;
    // Highlight the change text appropriately
    StartStyling(StartPos, 0x1F);
    SetStyling(EndPos - StartPos, STYLE_CHANGE);
    // If we styled to the end of file
    if(End == GetLineCount())
        EOFStyleBugHack();
}

void DiffTextEdit::MarkInsertAfterLine(int Line)
{
/*    EndPos = FileTextEdit->GetLineEndPosition(Start - 1);
    // Highlight the change text appropriately
    FileTextEdit->StartStyling(StartPos, 0x1F);
    FileTextEdit->SetStyling(EndPos - StartPos + 2, STYLE_INSERT);*/
}

void DiffTextEdit::MarkClearAll()
{
    int Pos;    // Temporary position variable

    // Get last position in file
    Pos = GetLineEndPosition(GetLineCount() - 1);
    // Start styling with default style
    StartStyling(0, 0x1F);
    // Style the whole file
    SetStyling(Pos, 0);
    // We're styling to EOF, call the bug fix function
    EOFStyleBugHack();
}

void DiffTextEdit::EOFStyleBugHack()
{
    // TODO - The BlankLineStyleBugHack() seems to fix this problem
    // TODO - Should fix scintilla with this bug
    // Add text and remove text from the last line to get
    //  around a seemingly strange bug in scintilla.  If we
    //  don't do this, the style does not get applied to the
    //  last line (i.e. anything without an EOL char).  But
    //  this is only true if the cursor is located on the
    //  last line as well.  Moving the cursor programmatically
    //  doesn't seem to fix it, but moving the cursor manually does
    //  fix the problem.
    /*int Pos;

    // Get last position in entire file
    Pos = GetLineEndPosition(GetLineCount() - 1);
    // Add an "x" character to the end
    InsertText(Pos, "x");
    // Remove the "x".  This allows the style to refresh correctly.
    SetTargetStart(Pos);
    SetTargetEnd(Pos + 1);
    ReplaceTarget("");*/
}

void DiffTextEdit::BlankLineStyleBugHack()
{
    int OldZoom = GetZoom();
    if(OldZoom < 20)
        SetZoom(OldZoom + 1);
    else
        SetZoom(OldZoom - 1);

    SetZoom(OldZoom);
    /*// Add an "x" character to the end
    InsertText(0, "x\n");
    // Get length of end line
    int LineLength = this->LineLength(0);
    // Remove the line we just added.  This allows the styles to be refreshed correctly
    SetTargetStart(0);
    SetTargetEnd(LineLength);
    ReplaceTarget("");*/
}

void DiffTextEdit::DisplayText(const wxString& text)
{
    ClearAll();
    ClearDocumentStyle();
    SetText(text);
    BlankLineStyleBugHack();
}

void DiffTextEdit::DeleteLines(int Start, int Length)
{
    // Get start and end positions for line deletion
    int StartPos = PositionFromLine(Start);
    int EndPos = PositionFromLine(Start + Length - 1) + 
        LineLength(Start + Length - 1);
    // Set the target from start to end
    SetTargetStart(StartPos);
    SetTargetEnd(EndPos);
    // Delete the target
    ReplaceTarget("");
}

void DiffTextEdit::ConfigSetFont(wxFont &NewFont)
{
}

void DiffTextEdit::ConfigSetShowEOL(bool ShowEOL)
{
}

void DiffTextEdit::ConfigSetChangeColor(wxColour &Foreground, wxColour &Background)
{
}