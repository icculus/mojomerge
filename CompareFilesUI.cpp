/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  CompareFilesUI.cpp - Implementation of CompareFilesUI class
 *
 */

#include "CompareFilesUI.h"
#include "Application.h"
#include "Identifiers.h"
#include "wx/stc/stc.h"
#include "wx/file.h"
#include "MergeHunkRef.h"
#include "Config.h"

using namespace MojoMerge;

#define MERGE_BUTTON_WIDTH      20
#define MERGE_BUTTON_HEIGHT     14
#define MERGE_BUTTON_CENTER     7
#define LOST_IN_SPACE           3

CompareFilesUI::CompareFilesUI(bool ThreeWayNotTwoWay)
{
    // Set our member defaults
    ReadyToCompare = false;
    MyMerge = NULL;
    TextCtrlOffset = 0;
    LastScrolledWindow = DiffFile_One;
    memset(LastScrollPosition, 0, sizeof(int) * MAX_DIFF_FILES);
    memset(IgnoreScrollChange, 0, sizeof(bool) * MAX_DIFF_FILES);
    memset(MergeButtons, 0, sizeof(wxButton*) * (MAX_DIFF_FILES - 1) *
        MAX_MERGE_BUTTONS);
    memset(NextFreeMergeButton, 0, sizeof(uint32) * (MAX_DIFF_FILES - 1));
    memset(ReadyToSave, 0, sizeof(bool) * MAX_DIFF_FILES);
    memset(ReadyToSaveAs, 0, sizeof(bool) * MAX_DIFF_FILES);

    // TODO - Get paths from Config object instead of hard coding them
    MyDiff = new GNUDiff(Config::GetDiffPath(), Config::GetDiff3Path(),
        Application::GetTempFolder());

    // Object is also a CompareFilesUI object
    ID |= TabWindow_CompareFilesUI;

    // For now, just set our Name to some test name
    strcpy(Name, "New Comparison");

    // Retain variable for future use
    this->ThreeWayNotTwoWay = ThreeWayNotTwoWay;
    // Figure out the last diff file based on flag
    LastDiffFile = ThreeWayNotTwoWay?DiffFile_Three:DiffFile_Two;
}

CompareFilesUI::~CompareFilesUI()
{
    // Deallocate objects we created
    delete MyDiff;
    // Delete merge object if we used it
    if(MyMerge)
        delete MyMerge;
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
        FilePanels[i] = new CompareFilePanel(this, (DiffFileNumber)i);
        // Make editing control readonly at first
        FilePanels[i]->GetDiffTextEdit()->SetReadOnly(true);
        // Add file panel to the sizer
        HorizontalSizer->Add(FilePanels[i], 1, wxGROW, 5);

        if(i == DiffFile_One)
        {
            // Create the first separator panel
            SeparatorPanels[i] = new SeparatorPanel(this);
            HorizontalSizer->Add(SeparatorPanels[i], 0, wxGROW, 5);
        }
        else if(i == DiffFile_Two && ThreeWayNotTwoWay)
        {
            // Create the first separator panel
            SeparatorPanels[i] = new SeparatorPanel(this);
            HorizontalSizer->Add(SeparatorPanels[i], 0, wxGROW, 5);
        }
    }

    // Set sizers to that they automagically resize
    this->SetAutoLayout(true);
    this->SetSizer(HorizontalSizer);
    HorizontalSizer->Fit(this);
    HorizontalSizer->SetSizeHints(this);
}

uint32 CompareFilesUI::RequestCommandStatus()
{
    uint32 Status = 0;

    // Set the command status mask based on what we can and can't do
    Status |= ReadyToCompare?Cmd_Recompare:0;
    Status |= ReadyToSave[DiffFile_One]?Cmd_SaveFirstFile:0;
    Status |= ReadyToSave[DiffFile_Two]?Cmd_SaveSecondFile:0;
    Status |= ReadyToSave[DiffFile_Three]?Cmd_SaveThirdFile:0;
    Status |= ReadyToSaveAs[DiffFile_One]?Cmd_SaveFirstFileAs:0;
    Status |= ReadyToSaveAs[DiffFile_Two]?Cmd_SaveSecondFileAs:0;
    Status |= ReadyToSaveAs[DiffFile_Three]?Cmd_SaveThirdFileAs:0;

    // If merge object is created, get undo/redo status
    if(MyMerge)
    {
        Status |= MyMerge->GetTransactionBuffer()->CanUndo()?Cmd_Undo:0;
        Status |= MyMerge->GetTransactionBuffer()->CanRedo()?Cmd_Redo:0;
    }

    return Status;
}

void CompareFilesUI::SetFile(DiffFileNumber File)
{
    // Turn off readonly so we can make changes to the text control
    FilePanels[File]->GetDiffTextEdit()->SetReadOnly(false);

    if(FilePanels[File]->SetFile())
    {
        // Command to Save As for specified file is now valid
        ReadyToSaveAs[File] = true;
        // Command to Save is not valid since it was just loaded
        ReadyToSave[File] = false;

        // If new file has been set, the MyMerge object is no longer relavent
        //  to what's currently there.  Remove it.
        if(MyMerge)
        {
            delete MyMerge;
            MyMerge = NULL;
        }

        // Redraw the separator panels since we update the files.  In this case
        //  we're just clearing the old lines since the Merge object is NULL.
        wxClientDC PanelDC1(SeparatorPanels[0]);
        DrawDiffConnectLines(&PanelDC1, 0);
        if(ThreeWayNotTwoWay)
        {
            wxClientDC PanelDC2(SeparatorPanels[1]);
            DrawDiffConnectLines(&PanelDC2, 1);
        }

        // Loop through all of the file panels
        int i;
        // Reset name of our CompareFilesUI window
        strcpy(Name, "");
        for(i = DiffFile_One; i <= LastDiffFile; i++)
        {
            // Clear all of the styling markings for each panel
            FilePanels[i]->GetDiffTextEdit()->MarkClearAll();
            // Append the first filename
            strcat(Name, TrimFilename(FilePanels[i]->GetFile()).GetData());
            // Append separator if it's not the last file
            if(i < LastDiffFile)
                strcat(Name, ":");
        }
    }

    CheckReadyToRecompare();
    Application::UpdateAvailableCommandsForActiveWindow();
    Application::UpdateNameForActiveWindow();

    // Turn on readonly since we're done making changes
    FilePanels[File]->GetDiffTextEdit()->SetReadOnly(true);
}

void CompareFilesUI::CheckReadyToRecompare()
{
    int i;
    bool Flag = false;

    for(i = DiffFile_One; i <= LastDiffFile; i++)
    {
        // If one of the files is not set
        if(FilePanels[i]->GetFile().IsEmpty())
        {
            Application::CmdSetStatusBarMsg(wxT("Please select all files to be compared."));
            ReadyToCompare = false;
            Flag = true;
            break;
        }
    }

    // If flag not set, all of the files are valid
    if(!Flag)
    {
        ReadyToCompare = true;
        Application::CmdSetStatusBarMsg(wxT("To perform a recompare press F5"));
    }
}

void CompareFilesUI::SaveFile(DiffFileNumber File)
{
    uint32 i;
    // Get buffer associated with specified file
    LineBuffer *Buffer = MyMerge->GetBuffer(File);
    // Get linecount for buffer
    uint32 LineCount = Buffer->GetLineCount();

    // Create file object
    wxFile MyFile(FilePanels[File]->GetFile(), wxFile::write);

    // Make sure file was opened successfully
    if(MyFile.IsOpened())
    {
        // Loop through each line in the file
        for(i = 1; i <= LineCount; i++)
            MyFile.Write(Buffer->GetLine(i));

        // Close the file since we're done writing it
        MyFile.Close();

        // File has just been saved, disable save command
        ReadyToSave[File] = false;
        FilePanels[File]->UpdateSavedIndicatorStatus(ReadyToSave[File]);
        Application::UpdateAvailableCommandsForActiveWindow();
    }
    else
    {
        // File could not be saved, so tell the user
        wxMessageBox(wxT("File could not be saved.  File may be readonly or folder permissions may be preventing the file from being saved.  Please select an alternative location in the following dialog."),
            wxT("Error"), wxOK | wxICON_ERROR);
        // Bring up the SaveAs dialog in case the error is because of a file
        //  or folder being readonly, or permissions prevent writing.
        SaveFileAs(File);
    }
}

void CompareFilesUI::SaveFileAs(DiffFileNumber File)
{
    // Prompt user for save path
    wxString Filename = wxFileSelector(wxT("Choose a file to save"), "", "", "", "*", wxSAVE | wxOVERWRITE_PROMPT, Application::GetMainWindow());

    // If user did not cancel the operation
    if(!Filename.IsEmpty())
    {
        FilePanels[File]->ChangeFilename(Filename);
        // Since we renamed the file, set our save flag to true.  This is
        //  particularly important if the SaveFile call fails.  This way
        //  we'll maintain the "ready to save" status with the new name.
        ReadyToSave[File] = true;
        FilePanels[File]->UpdateSavedIndicatorStatus(ReadyToSave[File]);
        // Save the file
        SaveFile(File);
    }
}

void CompareFilesUI::Print()
{
}

void CompareFilesUI::PrintPreview()
{
}

void CompareFilesUI::Undo()
{
    FileMergeTransaction *MyTransaction;
    DiffFileNumber Dest;

    // Undo the last transaction in the buffer
    MyTransaction = (FileMergeTransaction *)MyMerge->GetTransactionBuffer()->
        UndoTransaction();

    // Show the transaction as it currently should be represented
    ShowTransaction(MyTransaction);

    // Changes were made to the dest file, we're ready to save it
    Dest = MyTransaction->GetLastChange()->FileNumber;
    ReadyToSave[Dest] = true;
    FilePanels[Dest]->UpdateSavedIndicatorStatus(ReadyToSave[Dest]);
    Application::UpdateAvailableCommandsForActiveWindow();
}

void CompareFilesUI::Redo()
{
    FileMergeTransaction *MyTransaction;
    DiffFileNumber Dest;

    // Undo the last transaction in the buffer
    MyTransaction = (FileMergeTransaction *)MyMerge->GetTransactionBuffer()->
        RedoTransaction();

    // Show the transaction as it currently should be represented
    ShowTransaction(MyTransaction);

    // Changes were made to the dest file, we're ready to save it
    Dest = MyTransaction->GetLastChange()->FileNumber;
    ReadyToSave[Dest] = true;
    FilePanels[Dest]->UpdateSavedIndicatorStatus(ReadyToSave[Dest]);
    Application::UpdateAvailableCommandsForActiveWindow();
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
    Hunk *FirstHunk;
    int i;
    int MyDiffOptions = 0;

    // Set our diffing options based on the Config object
    MyDiffOptions |= Config::GetIgnoreCase()?DiffOption_IgnoreCase:0;
    MyDiffOptions |= Config::GetIgnoreWhitespace()?DiffOption_IgnoreWhitespace:0;
    MyDiffOptions |= Config::GetIgnoreLineEndings()?DiffOption_IgnoreLineEnding:0;

    // Get buffers from both file panels
    wxString Buffer1 = FilePanels[DiffFile_One]->GetBuffer();
    wxString Buffer2 = FilePanels[DiffFile_Two]->GetBuffer();

    // Remove the previous merge object if created
    if(MyMerge)
    {
        delete MyMerge;
        MyMerge = NULL;
    }

    // If three-way comparison
    if(ThreeWayNotTwoWay)
    {
        // Get third buffer for three-way diff
        wxString Buffer3 = FilePanels[DiffFile_Three]->GetBuffer();

        // Perform three-way comparison
        FirstHunk = MyDiff->CompareFiles((DiffOptions)MyDiffOptions, 
            Buffer1.GetData(),
            Buffer2.GetData(),
            Buffer3.GetData());
        // Only create Merge object if there are diffs
        //if(FirstHunk)
        //{
            // Create our merge object with the hunks returned from the diff
            MyMerge = new Merge(FirstHunk, Buffer1.GetData(),
                Buffer2.GetData(),
                Buffer3.GetData());
        //}
    }
    // If two-way comparison
    else
    {
        FirstHunk = MyDiff->CompareFiles((DiffOptions)MyDiffOptions, 
            Buffer1.GetData(),
            Buffer2.GetData());
        // Only create Merge object if there are diffs
        //if(FirstHunk)
        //{
            // Create our merge object with the hunks returned from the diff
            MyMerge = new Merge(FirstHunk, Buffer1.GetData(),
                Buffer2.GetData());
        //}
    }

    // Initiate a compare on each pane (mark changes based on hunks)
    for(i = DiffFile_One; i <= LastDiffFile; i++)
    {
        // Turn off readonly so we can make changes to the text control
        FilePanels[i]->GetDiffTextEdit()->SetReadOnly(false);
        FilePanels[i]->Recompare(FirstHunk, (DiffFileNumber)i);
        // Turn on readonly since we're done making changes
        FilePanels[i]->GetDiffTextEdit()->SetReadOnly(true);
    }

    // Redraw the separator panels since we update the files
    wxClientDC PanelDC1(SeparatorPanels[0]);
    DrawDiffConnectLines(&PanelDC1, 0);
    if(ThreeWayNotTwoWay)
    {
        wxClientDC PanelDC2(SeparatorPanels[1]);
        DrawDiffConnectLines(&PanelDC2, 1);
    }
}

void CompareFilesUI::OnSeparatorPainted(wxPaintEvent &event, wxDC *dc)
{
    int PanelIndex;        // Which panel to draw on

    // Get the separator panel that raised the event
    SeparatorPanel *Panel = (SeparatorPanel *)event.m_eventObject;
    if(Panel == SeparatorPanels[0])
        PanelIndex = 0;
    else
        PanelIndex = 1;

    // Draw the diff connection lines between the windows for any hunks that
    //  currently exist in our hunk list.
    DrawDiffConnectLines(dc, PanelIndex);
}

void CompareFilesUI::OnFileTextPosChanged(wxStyledTextEvent& event)
{
    Application::Debug("rock on");
}

void CompareFilesUI::DetermineAutoScrolling(DiffFileNumber FileNumber)
{
    int CurVisibleLine;

    // Return if we haven't created our merge object yet
    if(!MyMerge)
        return;

    // Get text edit control for specified file
    DiffTextEdit *TextEdit = FilePanels[FileNumber]->GetDiffTextEdit();
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
            // Redraw the separator panels since we've scrolled one or more
            //  of the windows
            wxClientDC PanelDC1(SeparatorPanels[0]);
            DrawDiffConnectLines(&PanelDC1, 0);
            if(ThreeWayNotTwoWay)
            {
                wxClientDC PanelDC2(SeparatorPanels[1]);
                DrawDiffConnectLines(&PanelDC2, 1);
            }
        }
    }
}

// TODO - We're getting painted events all the time for some reason?
void CompareFilesUI::OnFileTextPainted(wxStyledTextEvent& event)
{

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

    // Determine if something needs to be auto scrolled
    DetermineAutoScrolling(FileNumber);

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
    CurHunk = MyMerge->GetFirstHunk();
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

    // Return if we haven't created our merge object yet
    if(!MyMerge)
        return;

    p = MyMerge->GetFirstHunk();
    while(p)
    {
        // Get start line of hunk
        LineNumber = p->GetStart(FileNumber);
        // If end line is unspecified, it's an "insert after" hunk
        if(p->GetEnd(FileNumber) == UNSPECIFIED)
        {
            //EditDC.SetBrush(*wxRED_BRUSH);

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

void CompareFilesUI::DrawDiffConnectLines(wxDC *DC, int PanelIndex)
{
    Hunk *p;                    // Temporary hunk pointer
    wxButton **CurButton;       // Temp pointer to a button
    DiffFileNumber Src, Dest;   // Source and dest file numbers for drawing
    //wxMemoryDC MemPanel1;
    //wxMemoryDC MemPanel2;
    //wxClientDC *Panel1DC = new wxClientDC(SeparatorPanels[DiffFile_One]);
    //wxClientDC *Panel2DC = NULL;

    DC->Clear();

    // Don't draw any lines or buttons if merge object is NULL
    if(MyMerge)
    {
        // Reset the free merge button value since we're redrawing buttons
        NextFreeMergeButton[PanelIndex] = 0;

        /*// Get size of panel area
        wxSize PanelSize = SeparatorPanels[DiffFile_One]->GetClientSize();
        // Initialize the memory device contexts for both panels
        wxBitmap BitmapPanel1(PanelSize.x, PanelSize.y);
        wxBitmap BitmapPanel2(PanelSize.x, PanelSize.y);
        MemPanel1.SelectObject(BitmapPanel1);
        MemPanel2.SelectObject(BitmapPanel2);*/

        // If we're drawing on the first panel
        if(PanelIndex == 0)
        {
            Src = DiffFile_One;
            Dest = DiffFile_Two;
        }
        // Else, drawing on the second panel
        else
        {
            Src = DiffFile_Two;
            Dest = DiffFile_Three;
        }

        p = MyMerge->GetFirstHunk();
        while(p)
        {
            // Draw connecting line for hunk between file one and two
            DrawSingleDiffLine(Src, Dest, p, DC);
            // Go to the next hunk
            p = p->GetNextHunk();
        }

        /***Paint the lines drawn in memory onto the screen***/
        /*wxClientDC Panel1DC(SeparatorPanels[DiffFile_One]);
        Panel1DC.Blit(0, 0, PanelSize.x, PanelSize.y, &MemPanel1, 0, 0);
        if(ThreeWayNotTwoWay)
        {
            wxClientDC Panel2DC(SeparatorPanels[DiffFile_Two]);
            Panel2DC.Blit(0, 0, PanelSize.x, PanelSize.y, &MemPanel2, 0, 0);
        }*/
    }

    // Get the first button in the list associated with the file number
    CurButton = &MergeButtons[PanelIndex][NextFreeMergeButton[PanelIndex]];
    // Go through all of the buttons for each separator panel
    while(*CurButton)
    {
        // Hide the buttons we're not using
        (*CurButton)->Show(false);
        // Go to next button in list
        CurButton++;
    }
}

void CompareFilesUI::DrawSingleDiffLine(DiffFileNumber SourceNum,
    DiffFileNumber DestNum, Hunk *HunkToDraw, wxDC *Context)
{
    int X1, X2, Y1, Y2;          // Coordinates for line to draw
    // Get the file that is different in the hunk
    DiffFileNumber FileThatIsDifferent = HunkToDraw->GetDiffFile();

    // Calculate Y coordinates for line
    Y1 = CalculateY(SourceNum, HunkToDraw);
    Y2 = CalculateY(DestNum, HunkToDraw);

    // If the file that is different is not the one we're drawing the buttons
    //  for, then we shouldn't draw buttons since the source and destination
    //  files have the same contents for the specified hunk.
    if((FileThatIsDifferent != SourceNum) && (FileThatIsDifferent != DestNum)
    && (FileThatIsDifferent != UNSPECIFIED))
    {
        // Draw lines the entire width of panel
        X1 = 0;
        X2 = SEPARATOR_WIDTH;
    }
    // Else, draw the buttons
    else
    {
        // Set coordinates to compensate for buttons
        X1 = MERGE_BUTTON_WIDTH;
        X2 = SEPARATOR_WIDTH - MERGE_BUTTON_WIDTH;
        // Draw merge buttons for the hunk if it's showing
        DrawMergeButton(Y1, HunkToDraw, SourceNum, DestNum, SourceNum);
        DrawMergeButton(Y2, HunkToDraw, DestNum, SourceNum, SourceNum);
    }

    // Draw line between diffs
    Context->SetPen(wxPen(*wxBLACK, 1, wxDOT));
    Context->DrawLine(X1, Y1, X2, Y2);
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

void CompareFilesUI::DrawMergeButton(int Y, Hunk *TheHunk,
    DiffFileNumber SourceNum, DiffFileNumber DestNum, int SeparatorIndex)
{
    wxButton **CurButton;   // Current button we're working with
    int X;                  // X coordinate of button
    wxString Arrow;         // What symbol to use for button

    // Does the button go on the left or right of the panel
    if(SourceNum < DestNum)
        X = 0;
    else
        X = SEPARATOR_WIDTH - MERGE_BUTTON_WIDTH;

    // Set Y so that button will be centered on that point
    Y = Y - MERGE_BUTTON_CENTER;

    // Figure out which arrow to use
    if(SourceNum == DiffFile_One)
        Arrow = ">>";
    else if(SourceNum == DiffFile_Two)
    {
        if(DestNum == DiffFile_One)
            Arrow = "<<";
        else
            Arrow = ">>";
    }
    else
        Arrow = "<<";

    // Get reference to next available position in array
    CurButton = &MergeButtons[SeparatorIndex][NextFreeMergeButton[SeparatorIndex]];
    // If there's no button allocated at the current position
    if(!(*CurButton))
    {
        // Allocate a new button
        *CurButton = new wxButton(SeparatorPanels[SeparatorIndex],
            ID_MERGE_BUTTON, Arrow, wxPoint(X, Y),
            wxSize(MERGE_BUTTON_WIDTH, MERGE_BUTTON_HEIGHT));
    }
    else
    {
        // Move the existing button to the appropriate position
        (*CurButton)->Move(X, Y);
        (*CurButton)->SetLabel(Arrow);
    }

    // Creates a referentual bind between the button and the hunk
    (*CurButton)->UnRef();
    (*CurButton)->SetRefData(new MergeHunkRef(TheHunk, SourceNum, DestNum));

    // Show the button
    (*CurButton)->Show();
    (*CurButton)->Refresh();

    // Increment to the next index since we added another button
    NextFreeMergeButton[SeparatorIndex]++;
}

void CompareFilesUI::OnMergeButtonClick(wxCommandEvent& event)
{
    // Get HunkRef associated with button that raised the event
    MergeHunkRef *Ref = (MergeHunkRef *)event.m_eventObject->GetRefData();
    // Get the hunk associated with the HunkRef
    Hunk *MyHunk = (Hunk *)Ref->GetHunk();
    // Get the source and destination files for the button
    DiffFileNumber Source = Ref->GetSource();
    DiffFileNumber Dest = Ref->GetDest();

    // Resolve the diff associated with the button and show the results
    ShowTransaction(MyMerge->ResolveDiff(MyHunk, Source, Dest));

    // Changes were made to the dest file, we're ready to save it
    ReadyToSave[Dest] = true;
    FilePanels[Dest]->UpdateSavedIndicatorStatus(ReadyToSave[Dest]);
    Application::UpdateAvailableCommandsForActiveWindow();
}

void CompareFilesUI::ShowTransaction(FileMergeTransaction *NewTransaction)
{
    int Pos;        // Position in text edit control
    int i;          // Generic counter variable
    int EndLine;    // Line to stop processing lines at
    // Get details on what happened in the transaction
    const MergeChange *LastChange = NewTransaction->GetLastChange();
    // Get text edit control associated with file number
    DiffTextEdit *TextEdit = FilePanels[LastChange->FileNumber]->
        GetDiffTextEdit();
    // Get buffer associated with file
    LineBuffer *Buffer = MyMerge->GetBuffer(LastChange->FileNumber);

    // Turn off readonly since we're making changes
    TextEdit->SetReadOnly(false);

    switch(LastChange->Type)
    {
        case MergeChangeType_Delete:
            // Delete the specified lines from the control
            TextEdit->DeleteLines(LastChange->Start - 1, LastChange->Length);
            break;
        case MergeChangeType_Insert:
            // Get starting position for inserting lines (Start is 1-based)
            Pos = TextEdit->PositionFromLine(LastChange->Start - 1);
            EndLine = (int)LastChange->Length + LastChange->Start - 1;
            for(i = LastChange->Start; i <= EndLine; i++)
            {
                // Insert line into text edit control
                TextEdit->InsertText(Pos, Buffer->GetLine(i));
                // Increment the position to the length of the line we just added
                Pos += (int)strlen(Buffer->GetLine(i));
            }
            break;
        case MergeChangeType_Replace:
            TextEdit->DeleteLines(LastChange->Start - 1, LastChange->Length);
            // Get starting position for inserting lines (Start is 1-based)
            Pos = TextEdit->PositionFromLine(LastChange->Start - 1);
            EndLine = (int)LastChange->NewLength + LastChange->Start - 1;
            for(i = LastChange->Start; i <= EndLine; i++)
            {
                // Insert line into text edit control
                TextEdit->InsertText(Pos, Buffer->GetLine(i));
                // Increment the position to the length of the line we just added
                Pos += (int)strlen(Buffer->GetLine(i));
            }
            break;
        default:
            // Shouldn't get here
            assert(0);
    }

    // Refresh the changes marked on each file
    for(i = DiffFile_One; i <= LastDiffFile; i++)
    {
        FilePanels[i]->Recompare(MyMerge->GetFirstHunk(), (DiffFileNumber)i);
        // Refresh the "insert after" lines for each file
        DrawInsertAfterLines((DiffFileNumber)i);
    }

    // Redraw the separator panels since we've changed text
    wxClientDC PanelDC1(SeparatorPanels[0]);
    DrawDiffConnectLines(&PanelDC1, 0);
    if(ThreeWayNotTwoWay)
    {
        wxClientDC PanelDC2(SeparatorPanels[1]);
        DrawDiffConnectLines(&PanelDC2, 1);
    }

    // Force the auto scroll to work even though we haven't scrolled anything
    LastScrollPosition[LastChange->FileNumber] = 0;
    // Auto scroll so that files are now lined up based on the file that
    //  was changed.
    DetermineAutoScrolling(LastChange->FileNumber);

    // Turn on readonly since we're done making changes
    TextEdit->SetReadOnly(true);
}

wxString CompareFilesUI::TrimFilename(wxString Filename)
{
    #define TRIMFILENAME_MAX    15

    // Get length of string
    size_t Length = Filename.Length();
    wxString NewFilename;

    // If length is greater than our max, only take the end of it and prepend
    //  an ellipse indicating there's more of the filename that is unseen.
    if(Length > TRIMFILENAME_MAX)
        Filename = "..." + Filename.Right(TRIMFILENAME_MAX);

    return Filename;
}

// Event mappings for the file panel
BEGIN_EVENT_TABLE(CompareFilesUI, wxPanel)
    EVT_BUTTON(ID_MERGE_BUTTON, CompareFilesUI::OnMergeButtonClick)
    //EVT_STC_POSCHANGED(ID_FILE_TEXT_EDIT, CompareFilesUI::OnFileTextPosChanged)
    EVT_STC_PAINTED(ID_FILE_TEXT_EDIT, CompareFilesUI::OnFileTextPainted)
    //EVT_PAINT(CompareFilesUI::OnPanelPaint)
    //EVT_CUSTOM(wxEVT_SIZE, ID_SEPARATOR_PANEL, CompareFilesUI::OnPanelPaint)
END_EVENT_TABLE()