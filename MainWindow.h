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
    private:
        DECLARE_EVENT_TABLE()

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
        void OnNewComparison(wxCommandEvent& event);
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
        void OnCompareFiles(wxCommandEvent& event);
        void OnCompareFolders(wxCommandEvent& event);
        void OnTwoWayComparison(wxCommandEvent& event);
        void OnThreeWayComparison(wxCommandEvent& event);
        void OnRecompare(wxCommandEvent& event);
        void OnPreferences(wxCommandEvent& event);
        void OnToolbar(wxCommandEvent& event);
        void OnStatusbar(wxCommandEvent& event);
#ifdef _DEBUG
        void OnTestTwoWayDiff(wxCommandEvent& event);
        void OnTestThreeWayDiff(wxCommandEvent& event);
        void OnTestLineBuffer(wxCommandEvent& event);
        void OnTestTwoWayMerge(wxCommandEvent& event);
        void OnTestStack(wxCommandEvent& event);
        void OnTestCompareFolders(wxCommandEvent& event);
#endif
        // Objects contained inside the window
        Menu *MyMenu;
        Toolbar *MyToolbar;
        TabBrowser *MyTabBrowser;
    };
}

#endif