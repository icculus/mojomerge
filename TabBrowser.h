/*  TabBrowser.h - Provides the "tab browser" interface for viewing several
 *      comparison windows simultaneously.
 */

#ifndef _TABBROWSER_H
#define _TABBROWSER_H

#include "wx/wx.h"
#include "wx/notebook.h"
#include "TabWindow.h"

namespace MojoMerge
{
    class TabBrowser : public wxPanel
    {
    public:
        /*  TabBrowser
         *      Creates the tab browser window where each comparison window
         *      will be contained.
         *  Params
         *      Container
         *          Window to add the tab browser to.  The tab browser will
         *          take up the entire client area of the parent window.
         *  Returns
         *      none
         */
        TabBrowser(wxWindow *Container);
        
        /*  AddWindow
         *      Add window to the tab browser.  This will usually be a
         *      CompareFoldersUI or CompareFilesUI object.
         *
         *      If this is the first window added, the tab browser is
         *      automatically shown in its container window.
         *  Params
         *      Window
         *          Window to add to the tab browser.
         *  Returns
         *      none
         */
        void AddWindow(TabWindow *Window);

        /*  RemoveActiveWindow
         *      Removes the window that is currently active in the tab browser.
         *      Also, the wxWindow object is deallocated.
         *
         *      If there are no more windows left, the tab browser is hidden
         *      until another one is added.
         *  Params
         *      none
         *  Returns
         *      none
         */
        void RemoveActiveWindow();

        /*  GetActiveWindow
         *      Gets the active window
         *  Params
         *      none
         *  Returns
         *      The active TabWindow object
         */
        TabWindow *GetActiveWindow();
    private:
        // Tab control
        wxNotebook *Notebook;
        // Box sizer
        wxBoxSizer *BoxSizer;
        // Notebook sizer
        wxNotebookSizer *NotebookSizer;
    };
}

#endif