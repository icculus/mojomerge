/*  CompareFilesUI.cpp - Implementation of CompareFilesUI class
 *
 */

#include "CompareFilesUI.h"
#include "Application.h"
#include "Identifiers.h"
#include "wx/stc/stc.h"

using namespace MojoMerge;

#define SEPARATOR_WIDTH     50
#define LOST_IN_SPACE       3

CompareFilesUI::CompareFilesUI(bool ThreeWayNotTwoWay)
{
    // Set our member defaults
    MyMerge = NULL;
    FirstHunk = NULL;
    TextCtrlOffset = 0;
    LastScrolledWindow = DiffFile_One;
    memset(LastScrollPosition, 0, sizeof(int) * MAX_DIFF_FILES);
    memset(IgnoreScrollChange, 0, sizeof(bool) * MAX_DIFF_FILES);

    // TODO - Get paths from Config object instead of hard coding them
    MyDiff = new GNUDiff("C:\\cygwin\\bin\\diff.exe", 
        "C:\\cygwin\\bin\\diff3.exe", Application::GetTempFolder());

    // Object is also a CompareFilesUI object
    ID |= TabWindow_CompareFilesUI;

    // For now, just set our Name to some test name
    strcpy(Name, "CompareFilesUI test");

    // Retain variable for future use
    this->ThreeWayNotTwoWay = ThreeWayNotTwoWay;
    // Figure out the last diff file based on flag
    LastDiffFile = ThreeWayNotTwoWay?DiffFile_Three:DiffFile_Two;
}

CompareFilesUI::~CompareFilesUI()
{
    // Deallocate objects we created
    delete MyDiff;
    // TODO - For now we'll remove the hunks.  Eventually the Merge object
    //  will remove this when we pass the hunks to that object
    if(FirstHunk)
        FirstHunk->DeleteList();
    // TODO - Deallocate Merge object when we use it

    // All other members are wxWindow based objects and are therefore cleaned
    //  up automatically upon deletion of their container.
}

void CompareFilesUI::Initialize(wxWindow *Parent)
{
    int i;

    // Create our panel window
    Create(Parent, -1);

    // Create horizontal sizer
    HorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create two or three panes based on how many files compared
    for(i = DiffFile_One; i <= LastDiffFile; i++)
    {
        // Create first file panel object
        FilePanels[i] = new CompareFilePanel(this);
        // Add file panel to the sizer
        HorizontalSizer->Add(FilePanels[i], 1, wxGROW, 5);
        if(i < LastDiffFile)
        {
            // Create panel that goes between each compare window
            SeparatorPanels[i] = new wxPanel(this, ID_SEPARATOR_PANEL,
                wxDefaultPosition, wxSize(SEPARATOR_WIDTH, SEPARATOR_WIDTH), 0);
            HorizontalSizer->Add(SeparatorPanels[i], 0, wxGROW, 5);
        }
    }

    // Set sizers to that they automagically resize
    this->SetAutoLayout(true);
    this->SetSizer(HorizontalSizer);
    HorizontalSizer->Fit(this);
    HorizontalSizer->SetSizeHints(this);
}

void CompareFilesUI::SetFile(DiffFileNumber File)
{
}

void CompareFilesUI::SaveFile(DiffFileNumber File)
{
}

void CompareFilesUI::SaveFileAs(DiffFileNumber File)
{
}

void CompareFilesUI::Print()
{
}

void CompareFilesUI::PrintPreview()
{
}

void CompareFilesUI::Undo()
{
}

void CompareFilesUI::Redo()
{
}

void CompareFilesUI::Cut()
{
}

void CompareFilesUI::Copy()
{
}

void CompareFilesUI::Paste()
{
}

void CompareFilesUI::SelectAll()
{
}

void CompareFilesUI::TwoWayComparison()
{
}

void CompareFilesUI::ThreeWayComparison()
{
}

void CompareFilesUI::Recompare()
{
    int i;

    // Remove the hunks from the previous list if allocated
    if(FirstHunk)
        FirstHunk->DeleteList();

    // If three-way comparison
    if(ThreeWayNotTwoWay)
    {
        // Perform three-way comparison
        FirstHunk = MyDiff->CompareFiles(DiffOption_None, 
            FilePanels[DiffFile_One]->GetBuffer(),
            FilePanels[DiffFile_Two]->GetBuffer(),
            FilePanels[DiffFile_Three]->GetBuffer());
    }
    // If two-way comparison
    else
    {
        FirstHunk = MyDiff->CompareFiles(DiffOption_None, 
            FilePanels[DiffFile_One]->GetBuffer(),
            FilePanels[DiffFile_Two]->GetBuffer());
    }

    // Initiate a compare on each pane (mark changes based on hunks)
    for(i = DiffFile_One; i <= LastDiffFile; i++)
        FilePanels[i]->Recompare(FirstHunk, (DiffFileNumber)i);
}

void CompareFilesUI::OnPanelResize(wxSizeEvent& event)
{
    Application::Debug("Panel Resize event");
}

void CompareFilesUI::OnFileTextPosChanged(wxStyledTextEvent& event)
{
    Application::Debug("rock on");
}

void CompareFilesUI::OnFileTextPainted(wxStyledTextEvent& event)
{
    int CurVisibleLine;

    // Save DiffTextEdit control that raised the event
    DiffTextEdit *TextEdit = (DiffTextEdit *)event.m_eventObject;
    // File number associated with TextEdit control
    DiffFileNumber FileNumber;

    // Find out which file is represented by the passed control
    if(TextEdit == FilePanels[DiffFile_One]->GetDiffTextEdit())
        FileNumber = DiffFile_One;
    else if(TextEdit == FilePanels[DiffFile_Two]->GetDiffTextEdit())
        FileNumber = DiffFile_Two;
    else
        FileNumber = DiffFile_Three;

    // After the first paint event, this offset does not change.  Only get the
    //  value of it the first time.
    if(!TextCtrlOffset)
        TextCtrlOffset = FilePanels[FileNumber]->GetTextCtrlOffset() - LOST_IN_SPACE;

    // Get the top line that is visible in the control
    CurVisibleLine = TextEdit->GetFirstVisibleLine();
    // If scroll position has moved
    if(CurVisibleLine != LastScrollPosition[FileNumber])
    {
        // Save this new position as our last line
        LastScrollPosition[FileNumber] = CurVisibleLine;
        // If we have to ignore the scroll change this time, set
        //  the flag back to false so that it resets for the next
        //  paint event.  Usually this flag is set true if it was scrolled
        //  programmatically instead of manually.  This prevents us from
        //  getting into an endless loop of updating
        if(IgnoreScrollChange[FileNumber])
            IgnoreScrollChange[FileNumber] = false;
        // Else, 
        else
        {
            // Adjust other window scroll positions based on this one
            //  that was manually moved.
            AutoAdjustScrolling(FileNumber);
            // Retain a reference to the window that scrolled last
            LastScrolledWindow = FileNumber;
        }
    }

    // TODO - It'd be nice to reduce the drawing of these lines somehow
    // Draw the diff connection lines between the window(s)
    DrawDiffConnectLines();
    // Draw the "insert after" lines for the edit control that repainted
    DrawInsertAfterLines(FileNumber);
}

void CompareFilesUI::AutoAdjustScrolling(DiffFileNumber FileNumber)
{
    int CenterLine;     // Line located at "centerline"
    Hunk *MyHunk;       // Hunk located at center or before center
    bool IsHunk;        // True if CenterLine is not a hunk
    int i;              // Generic counter

    // Get the DiffTextEdit control associated with file number
    DiffTextEdit *TextEdit = FilePanels[FileNumber]->GetDiffTextEdit();
    // Get position at center
    CenterLine = GetCenterLine(TextEdit);
    if(CenterLine >= 0)
    {
        // Gets the hunk located at the centerline.
        MyHunk = GetHunkAtLine(FileNumber, CenterLine, &IsHunk);
        // Loop through all of the diff files
        for(i = DiffFile_One; i <= LastDiffFile; i++)
        {
            // Only scroll the file if it's not the one that was manually
            //  scrolled.
            if(i != FileNumber)
            {
                ScrollOtherTextControl((DiffFileNumber)i, FileNumber, MyHunk,
                CenterLine, IsHunk);
            }
        }
    }
}

int CompareFilesUI::GetCenterLine(DiffTextEdit *TextEdit)
{
    int RetVal;

    // Text edit control can't be NULL
    assert(TextEdit);
    // Gives us an approximate center line
    RetVal = TextEdit->LinesOnScreen() / 2 + TextEdit->GetFirstVisibleLine();
    // If the number of lines is less than the center line, we have no center
    //  line, so set our value to -1
    if(RetVal >= TextEdit->GetLineCount())
        RetVal = -1;

    return RetVal;
}

Hunk *CompareFilesUI::GetHunkAtLine(DiffFileNumber FileNumber, int Line,
bool *IsHunk)
{
    Hunk *CurHunk = NULL;       // Current hunk we're looking at
    Hunk *PrevHunk = NULL;      // Previous hunk we looked at
    uint32 Start, End;          // Start and end of current hunk

    // IsHunk can't be NULL
    assert(IsHunk);
    // By default, line is not on a hunk
    *IsHunk = false;
    // Start looping at the first hunk
    CurHunk = FirstHunk;
    // Loop through all the hunks
    while(CurHunk)
    {
        // Get start and end of the hunk for given file (convert to 0-based)
        Start = Convert1To0(CurHunk->GetStart(FileNumber));
        End = Convert1To0(CurHunk->GetEnd(FileNumber));
        // We only can be centered at change hunks.  "Insert into" hunks do not
        //  really represent a set of one or more lines, so they are treated as
        //  non-hunk area on the side that is manually scrolled
        if(End != UNSPECIFIED)
        {
            // If line equals, we found a matching hunk, so we can break
            if(Line >= (int)Start && Line <= (int)End)
            {
                *IsHunk = true;
                break;
            }
            // Else if hunk is located after the line
            else if((int)Start > Line)
            {
                // Set our hunk to the previous hunk if it exists
                CurHunk = PrevHunk;
                break;
            }
        }
        // Else, hunk is an "insert after".  This is only relevant if the hunk
        //  exists after our current line.  This means that we are on non-hunk
        //  space.
        else if((int)Start > Line)
        {
            // Set our hunk to the previous hunk if it exists
            CurHunk = PrevHunk;
            break;
        }

        // Retain current hunk as our previuos
        PrevHunk = CurHunk;
        // Get the next hunk
        CurHunk = PrevHunk->GetNextHunk();
        // If the next hunk doesn't exist, we got to the end of the list
        //  without finding a hunk that exists on or after the line, so we
        //  will return the previous which will either be the hunk above
        //  the line, or NULL if there are no hunks existing
        if(CurHunk == NULL)
        {
            CurHunk = PrevHunk;
            break;
        }
    }

    // Return the hunk that we found
    return CurHunk;
}

uint32 CompareFilesUI::Convert1To0(uint32 Line)
{
    // Only convert the line if it's a valid line number
    if(Line != UNSPECIFIED)
        Line--;

    return Line;
}

void CompareFilesUI::ScrollOtherTextControl(DiffFileNumber FileToScroll,
DiffFileNumber FileScrolled, Hunk *TheHunk, int Line, bool IsHunk)
{
    uint32 Start, End;          // Start and end of current hunk
    uint32 FileScrolledStart;   // Start of hunk for file scrolled
    uint32 FileScrolledEnd;     // End of hunk for file scrolled
    int LineToScrollTo;         // Line to scroll to in file to scroll
    int Bottom;                 // Bottom line of hunk
    int FileScrolledBottom;     // Bottom line of hunk for file scrolled
    int CurCenterLine;          // Current centerline of file to scroll

    // Get the DiffTextEdit control associated with file number
    DiffTextEdit *TextEdit = FilePanels[FileToScroll]->GetDiffTextEdit();
    // Get line currently at the center
    CurCenterLine = GetCenterLine(TextEdit);

    // If there was a hunk on or above the line in the scrolled file
    if(TheHunk)
    {
        // Get start and end of the hunk for given file
        Start = Convert1To0(TheHunk->GetStart(FileToScroll));
        End = Convert1To0(TheHunk->GetEnd(FileToScroll));
        FileScrolledStart = Convert1To0(TheHunk->GetStart(FileScrolled));
        FileScrolledEnd = Convert1To0(TheHunk->GetEnd(FileScrolled));

        // If line is located on a hunk in scrolled file (also a changed hunk)
        if(IsHunk)
        {
            // If hunk in file that will be scrolled is an "insert into" hunk
            if(End == UNSPECIFIED)
                LineToScrollTo = Start;
            // Else, hunk is a changed hunk
            else
            {
                // Figure out the fraction that the line position represents
                float Fraction = (float)(Line - FileScrolledStart + 1) /
                    (float)(FileScrolledEnd - FileScrolledStart + 1);
                // Duplicate that fraction on the other file so it is in the
                //  same position proportionally to the scrolled file
                LineToScrollTo = ((float)Start + (float)(End - Start) * Fraction);
            }
        }
        // Else, line is not on a hunk
        else
        {
            // If a hunk exists above our line in the scrolled file
            if(TheHunk)
            {
                // If previous hunk on the file scrolled is an "insert after" hunk
                if(FileScrolledEnd == UNSPECIFIED)
                    FileScrolledBottom = FileScrolledStart;
                // Else, previous hunk is a change hunk
                else
                    FileScrolledBottom = FileScrolledEnd;
                // If previous hunk in file we're scrolling is an "insert after" hunk
                if(End == UNSPECIFIED)
                    Bottom = Start;
                // Else, previous hunk is a change hunk
                else
                    Bottom = End;
                // Scroll to the non-hunk line that matches to relative position of
                //  the line in the scrolled file
                LineToScrollTo = (Line - FileScrolledBottom) + Bottom;
            }
        }
    }
    // No more hunks on or above the line in the scrolled file.  Current line
    //  position in both files will therefore be identical.
    else
        LineToScrollTo = Line;

    // If we have to autoscroll the window and if there are enough lines that
    //  we can actually do a scroll with
    if(LineToScrollTo != CurCenterLine && CurCenterLine >= 0)
    {
        // Set ignore flag when auto scrolling the control so we don't think it
        //  was scrolled manually when we get the painted event
        IgnoreScrollChange[FileToScroll] = true;
        // Scroll to the new line relative to the current center
        TextEdit->LineScroll(0, LineToScrollTo - CurCenterLine);
    }
}

void CompareFilesUI::DrawInsertAfterLines(DiffFileNumber FileNumber)
{
    // Get the DiffTextEdit control associated with file number
    DiffTextEdit *TextEdit = FilePanels[FileNumber]->GetDiffTextEdit();

    Hunk *p;                        // Temporary hunk pointer
    uint32 LineNumber;              // Line number to insert after
    wxClientDC EditDC(TextEdit);    // Device context of text control
    int dummy;                      // Dummy value not used
    int X1, Y1, X2, Y2;             // Coordinates for drawing line

    p = FirstHunk;
    while(p)
    {
        // Get start line of hunk
        LineNumber = p->GetStart(FileNumber);
        // If end line is unspecified, it's an "insert after" hunk
        if(p->GetEnd(FileNumber) == UNSPECIFIED)
        {
            EditDC.SetBrush(*wxRED_BRUSH);

            // Always draw starting from left of text control
            X1 = 0;
            // Y coordinates should be right below the line we're inserting
            //  after.  Hunk lines are 1-based...the text edit control is
            //  0-based.
            Y1 = Y2 = TextEdit->PointFromPosition
                (TextEdit->PositionFromLine(LineNumber - 1)).y +
                TextEdit->TextHeight(LineNumber - 1);
            // Find out how long we have to make the line
            TextEdit->GetClientSize(&X2, &dummy);
            EditDC.DrawLine(X1, Y2, X2, Y2);
        }

        // Go to the next hunk
        p = p->GetNextHunk();
    }
}

void CompareFilesUI::DrawDiffConnectLines()
{
    Hunk *p;                        // Temporary hunk pointer

    wxClientDC Panel1DC(SeparatorPanels[DiffFile_One]);
    if(ThreeWayNotTwoWay)
    {
        wxClientDC Panel2DC(SeparatorPanels[DiffFile_Two]);
        // Clear the panel since we're drawing new lines on them.
        Panel2DC.Clear();
    }

    // Clear the panel since we're drawing new lines on them.
    Panel1DC.Clear();

    p = FirstHunk;
    while(p)
    {
        // Draw connecting line for hunk between file one and two
        DrawSingleDiffLine(DiffFile_One, DiffFile_Two, p);
        // If three way diff, draw line between file two and three
        if(ThreeWayNotTwoWay)
            DrawSingleDiffLine(DiffFile_Two, DiffFile_Three, p);
        // Go to the next hunk
        p = p->GetNextHunk();
    }
}

void CompareFilesUI::DrawSingleDiffLine(DiffFileNumber SourceNum,
    DiffFileNumber DestNum, Hunk *HunkToDraw)
{
    int X1, X2, Y1, Y2;          // Coordinates for line to draw

    // Allocate device context on the stack (required by wxWindows) for the
    //  separator between the source and dest file
    wxClientDC MyDC(SeparatorPanels[SourceNum]);

    // Calculate coordinates for line
    X1 = 0;
    X2 = SEPARATOR_WIDTH;
    Y1 = CalculateY(SourceNum, HunkToDraw);
    Y2 = CalculateY(DestNum, HunkToDraw);

    // Draw line between diffs
    MyDC.SetBrush(*wxRED_BRUSH);
    MyDC.DrawLine(X1, Y1, X2, Y2);
}

int CompareFilesUI::CalculateY(DiffFileNumber FileNumber, Hunk *HunkToDraw)
{
    // Get control associate with FileNumber
    DiffTextEdit *TextEdit = FilePanels[FileNumber]->GetDiffTextEdit();
    // Start and End of hunk for current FileNumber
    uint32 Start, End;
    int Y, Y2;

    // Get start and end line of hunk
    Start = HunkToDraw->GetStart(FileNumber);
    End = HunkToDraw->GetEnd(FileNumber);

    // ***Calculate Y***
    // If end line is unspecified, it's an "insert after" hunk
    if(End == UNSPECIFIED)
    {
        Y = TextEdit->PointFromPosition(
            TextEdit->PositionFromLine(Start - 1)).y +
            TextEdit->TextHeight(Start - 1) + TextCtrlOffset; 
    }
    // Else, it's a "change" hunk
    else
    {
        // Get top of start line
        Y = TextEdit->PointFromPosition(
            TextEdit->PositionFromLine(Start - 1)).y + TextCtrlOffset;
        // Get bottom of end line
        Y2 = TextEdit->PointFromPosition(
            TextEdit->PositionFromLine(End - 1)).y +
            TextEdit->TextHeight(End - 1) + TextCtrlOffset;

        // Find the pixel between Y and Y2
        Y = Y + ((Y2 - Y) / 2);
    }

    return Y;
}

// Event mappings for the file panel
BEGIN_EVENT_TABLE(CompareFilesUI, wxPanel)
    //EVT_STC_POSCHANGED(ID_FILE_TEXT_EDIT, CompareFilesUI::OnFileTextPosChanged)
    EVT_STC_PAINTED(ID_FILE_TEXT_EDIT, CompareFilesUI::OnFileTextPainted)
    //EVT_CUSTOM(wxEVT_SIZE, ID_SEPARATOR_PANEL, CompareFilesUI::OnPanelResize)
END_EVENT_TABLE()