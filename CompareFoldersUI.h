/*  CompareFoldersUI.h - Window that provides the interface for comparing folders
 *
 */

#ifndef _COMPAREFOLDERSUI_H
#define _COMPAREFOLDERSUI_H

#include "wx/wx.h"
#include "DataTypes.h"
#include "CompareUI.h"
#include "SeparatorPanel.h"
#include "CompareFolderPanel.h"
#include "CompareFolders.h"
#include "FolderMerge.h"
#include "GNUDiff.h"

namespace MojoMerge
{
    class CompareFoldersUI : public CompareUI, public wxTimer
    {
        DECLARE_EVENT_TABLE()
    public:
        /*  CompareFoldersUI
         *      Create a new CompareFilesUI window
         *  Params
         *      ThreeWayNotTwoWay
         *          Set to true if doing a three-way comparison, otherwise do
         *          a two-way comparison.
         *  Returns
         *      none
         */
        CompareFoldersUI(bool ThreeWayNotTwoWay = false);

        /*  CompareFoldersUI destructor
         *      Deletes the CompareFoldersUI object
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~CompareFoldersUI();

        /*  SetFile
         *      Sets the file to use as the first, second, or third file.
         *  Params
         *      File
         *          File number to assign the filename to
         *      Foldername
         *          Name of folder to set for the specified file number
         *  Returns
         *      none
         */
        void SetFolder(DiffFileNumber File, wxString &Foldername);

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
        virtual void Initialize(wxWindow *Parent);
        virtual void OnSeparatorPainted(wxPaintEvent &event, wxDC *dc);
        virtual uint32 RequestCommandStatus();

        // Event Handlers
        void OnTreeCollapsing(wxTreeEvent &event);

        // wxTimer overrides
        void Notify();
    private:
        /*  AddHunks
         *      Recursively calls itself to create the tree of folder hunks
         *      for each item in FolderPanels array.
         *  Params
         *      CurHunk
         *          First item in the hunk list
         *      ParentX
         *          Parent tree node of the current hunk being added
         *  Returns
         *      none
         */
        void AddHunks(FolderHunk *CurHunk, wxTreeItemId *Parent1,
            wxTreeItemId *Parent2, wxTreeItemId *Parent3);

        void SetTreeItemData(wxTreeItemId Id1, wxTreeItemId Id2,
            wxTreeItemId Id3, FolderHunk *Hunk);

        FolderMerge *MyMerge;
        GNUDiff *MyDiff;
        wxBoxSizer *HorizontalSizer;
        SeparatorPanel *SeparatorPanels[MAX_DIFF_FILES - 1];
        CompareFolderPanel *FolderPanels[MAX_DIFF_FILES];
        DiffFileNumber LastDiffFile;
        bool ThreeWayNotTwoWay;
    };
}

#endif