/*  CompareFilesUI.h - Window that provides the interface for comparing files
 *
 */

#ifndef _COMPAREFILESUI_H
#define _COMPAREFILESUI_H

#include "wx/wx.h"
#include "DataTypes.h"
#include "CompareUI.h"
#include "GNUDiff.h"
#include "Merge.h"
#include "CompareFilePanel.h"

namespace MojoMerge
{
    class CompareFilesUI : public CompareUI
    {
        DECLARE_EVENT_TABLE()
    public:
        /*  CompareFilesUI
         *      Create a new CompareFilesUI window
         *  Params
         *      ThreeWayNotTwoWay
         *          Set to true if doing a three-way comparison, otherwise do
         *          a two-way comparison.
         *  Returns
         *      none
         */
        CompareFilesUI(bool ThreeWayNotTwoWay = false);

        /*  CompareFilesUI
         *      Destructs a CompareFilesUI window
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~CompareFilesUI();

        /*  SetFile
         *      Sets the file to use as the first, second, or third file.  This
         *      function calls the SetFile method in the CompareFilePanel object
         *      associated with the specific file number
         *  Params
         *      File
         *          File number to assign the filename to
         *  Returns
         *      none
         */
        void SetFile(DiffFileNumber File);

        /*  SaveFile
         *      Saves the specified file's changes
         *  Params
         *      File
         *          File number to save
         *  Returns
         *      none
         */
        void SaveFile(DiffFileNumber File);

        /*  SaveFileAs
         *      Saves the specified file's changes.  Displays a "Save As"
         *      dialog box for the user to enter an alternate name
         *  Params
         *      File
         *          File number to save
         *  Returns
         *      none
         */
        void SaveFileAs(DiffFileNumber File);

        /*  CompareUI Overrides
         *      See description in CompareUI class for more information
         */
        virtual void Print();
        virtual void PrintPreview();
        virtual void Undo();
        virtual void Redo();
        virtual void Cut();
        virtual void Copy();
        virtual void Paste();
        virtual void SelectAll();
        virtual void TwoWayComparison();
        virtual void ThreeWayComparison();
        virtual void Recompare();

        /*  Initialize override
         *      See description in CompareUI class for more information
         */
        virtual void Initialize(wxWindow *Parent);
    private:
        /*  GetCenterLine
         *      Returns the linenumber (0-based) of the closest line to the
         *      center of the panel.
         *  Params
         *      TextEdit
         *          File to get the centerline of
         *  Returns
         *      Returns less than 0 if there is no line at center
         */
        int GetCenterLine(DiffTextEdit *TextEdit);

        /*  DrawInsertAfterLines
         *      Draws the "insert after" lines on the specified text control.
         *      These lines are taken from the FirstHunk linked list
         *  Params
         *      FileNumber
         *          File who's text edit control was painted
         *  Returns
         *      none
         */
        void DrawInsertAfterLines(DiffFileNumber FileNumber);
        
        /*  DrawDiffConnectLines
         *      Draws the lines between the diff windows that connect the hunks
         *      in each window together.  These lines are taken from the
         *      FirstHunk linked list.
         *  Params
         *      none
         *  Returns
         *      none
         */
        void DrawDiffConnectLines();

        /*  DrawSingleDiffLine
         *      Draws line between source and destination file to connect the
         *      specified hunk.
         *  Params
         *      none
         *  Returns
         *      none
         */
        void DrawSingleDiffLine(DiffFileNumber SourceNum,
            DiffFileNumber DestNum, Hunk *HunkToDraw);

        /*  CalculateY
         *      Calculates the Y coordinate for where the diff line draws to
         *      or from (based on the FileNumber).
         *  Params
         *      FileNumber
         *          File to calculate Y for
         *      HunkToDraw
         *          Hunk where diff is located
         */
        int CalculateY(DiffFileNumber FileNumber, Hunk *HunkToDraw);

        /*  AutoAdjustScrolling
         *      Automatically adjusts the position of the text edit windows
         *      that were not manually scrolled
         *  Params
         *      FileNumber
         *          File that was manually scrolled
         *  Returns
         *      none
         */
        void AutoAdjustScrolling(DiffFileNumber FileNumber);

        /*  GetHunkAtLine
         *      Returns the hunk located at the specified line in the specified
         *      file.  If a hunk is not found at the line number, the hunk
         *      located previous is returned.  If neither of these cases are
         *      true, than NULL is returned
         *  Params
         *      FileNumber
         *          File to search for a hunk in
         *      Line
         *          0-based line number to get hunk for
         *      IsHunk
         *          A value of true is written here if the line is found in a 
         *          hunk.  Otherwise, false is written.
         *  Returns
         *      See description
         */
        Hunk *GetHunkAtLine(DiffFileNumber FileNumber, int Line, bool *IsHunk);

        /*  ScrollOtherTextControl
         *      Scrolls the specified text control to the line proportional to
         *      the line in another file relative to the hunk location.
         *  Params
         *      FileToScroll
         *          File that will be scrolled based on arguments
         *      FileScrolled
         *          File that was already scrolled.  This file is what we're
         *          baseing the scrolling on
         *      TheHunk
         *          Hunk that we're scrolling against
         *      IsHunk
         *          True if line is located 
         *  Returns
         *      none
         */
        void ScrollOtherTextControl(DiffFileNumber FileToScroll,
            DiffFileNumber FileScrolled, Hunk *TheHunk, int Line, bool IsHunk);

        /*  Convert1To0
         *      Converts a 1-based line number to 0-based.  In addition,
         *      checking is done to ensure it does not convert line numbers
         *      that are equal to UNSPECIFIED.
         *  Params
         *      Line
         *          Line to convert
         *  Returns
         *      The converted line number, or UNSPECIFIED
         */
        uint32 Convert1To0(uint32 Line);

        // Event handlers
        void OnFileTextPainted(wxStyledTextEvent& event);
        void OnFileTextPosChanged(wxStyledTextEvent& event);
        void OnPanelResize(wxSizeEvent& event);
        //void OnSeparatorPainted(wxPaintEvent& event);
        
        // Diff object to use for file comparisons
        Diff *MyDiff;
        // First hunk returned from a diff
        Hunk *FirstHunk;
        // Merge object to use for merging differences
        Merge *MyMerge;
        // File path for each file in diff
        wxString FilePath[MAX_DIFF_FILES];

        wxBoxSizer *HorizontalSizer;
        wxPanel *SeparatorPanels[MAX_DIFF_FILES - 1];
        CompareFilePanel *FilePanels[MAX_DIFF_FILES];
        int LastDiffFile;
        int TextCtrlOffset;
        bool ThreeWayNotTwoWay;
        // Which file was scrolled last
        DiffFileNumber LastScrolledWindow;
        // Scroll position of each file after the last painted event
        int LastScrollPosition[MAX_DIFF_FILES];
        // Flag to ignore scroll change during paint event
        bool IgnoreScrollChange[MAX_DIFF_FILES];
    };
}

#endif