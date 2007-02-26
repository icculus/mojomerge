/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  MainWindow.h - Primary application window
 *
 */

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "wx/wx.h"
#include "Menu.h"
#include "Toolbar.h"
#include "Test.h"
#include "TabBrowser.h"

namespace MojoMerge
{
    class MainWindow : public wxFrame
    {
    public:
        MainWindow();

        /*  GetTabBrowser
         *      Returns tab browser associated with this window
         *  Params
         *      none
         *  Returns
         *      A TabBrowser object, or NULL if not created
         */
        TabBrowser *GetTabBrowser();

        /*  GetStatusWindow
         *      Returns status bar associated with this window
         *  Params
         *      none
         *  Returns
         *      A wxStatusBar object, or NULL if not created
         */
        wxStatusBar *GetStatusBar();

        /*  SetCommandStatus
         *      This call just passed the Status mask to the toolbar and the
         *      menu objects so they can process them accordingly.
         *  Params
         *      Status
         *  Returns
         */
        void SetCommandStatus(uint32 Status);
    private:
        /*  Menu Event Handlers
         *      These functions handle the associated menu events.  They will
         *      simply call the Application command functions assoicated with
         *      the event.  These are implicitly inline to avoid the overhead
         *      of calling several functions unnecessarily.
         *  Parameters
         *      event
         *          Event object passed when event is generated
         *  Returns
         *      none
         */
        void OnNewTwoWayFileComparison(wxCommandEvent& event);
        void OnNewThreeWayFileComparison(wxCommandEvent& event);
        void OnNewTwoWayFolderComparison(wxCommandEvent& event);
        void OnNewThreeWayFolderComparison(wxCommandEvent& event);
        void OnCloseWindow(wxCommandEvent& event);
        void OnSaveFirstFile(wxCommandEvent& event);
        void OnSaveSecondFile(wxCommandEvent& event);
        void OnSaveThirdFile(wxCommandEvent& event);
        void OnSaveFirstFileAs(wxCommandEvent& event);
        void OnSaveSecondFileAs(wxCommandEvent& event);
        void OnSaveThirdFileAs(wxCommandEvent& event);
        void OnPrint(wxCommandEvent& event);
        void OnPrintPreview(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnUndo(wxCommandEvent& event);
        void OnRedo(wxCommandEvent& event);
        void OnCut(wxCommandEvent& event); 
        void OnCopy(wxCommandEvent& event); 
        void OnPaste(wxCommandEvent& event); 
        void OnSelectAll(wxCommandEvent& event);
        void OnRecompare(wxCommandEvent& event);
        //void OnPreferences(wxCommandEvent& event);
        void OnIgnoreWhitespace(wxCommandEvent& event);
        void OnIgnoreLineEndings(wxCommandEvent& event);
        void OnIgnoreCase(wxCommandEvent& event);
        void OnToolbar(wxCommandEvent& event);
        void OnStatusbar(wxCommandEvent& event);
        void OnChangeActiveWindow(wxNotebookEvent& event);
#ifdef _DEBUG
        void OnTestTwoWayDiff(wxCommandEvent& event);
        void OnTestThreeWayDiff(wxCommandEvent& event);
        void OnTestLineBuffer(wxCommandEvent& event);
        void OnTestTwoWayMerge(wxCommandEvent& event);
        void OnTestStack(wxCommandEvent& event);
        void OnTestCompareFolders(wxCommandEvent& event);
        void OnTestCompareFilesUI(wxCommandEvent& event);
        void OnTestCompareFilesUI3Way(wxCommandEvent& event);
#endif
        // Objects contained inside the window
        Menu *MyMenu;
        Toolbar *MyToolbar;
        TabBrowser *MyTabBrowser;
        wxStatusBar *MyStatusBar;

        // Use the event table defined in MainWindow.cpp
        DECLARE_EVENT_TABLE()
    };
}

#endif
