/*  CompareFolderPanel.h - UI class for a single file.  This class is typically
 *  used in conjunction with the CompareFilesUI class.  CompareFolderPanel class
 *  contains a textbox, combobox, and directory selection button.  It binds the
 *  components together to provide an abstract component for displaying diff
 *  files.
 */

#ifndef _COMPAREFOLDERPANEL_H_
#define _COMPAREFOLDERPANEL_H_

#include "wx/wx.h"
#include "wx/treectrl.h"
#include "FolderHunk.h"

namespace MojoMerge
{
    class CompareFolderPanel : public wxPanel
    {
        DECLARE_EVENT_TABLE()
    public:
        /*  CompareFolderPanel
         *      Constructs a CompareFolderPanel object
         *  Params
         *      Parent
         *          Parent window, usually a CompareFoldersUI window
         *  Returns
         *      none
         */
        CompareFolderPanel(wxWindow *Parent);

        /*  ~CompareFolderPanel
         *      Destructs a CompareFolderPanel object
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~CompareFolderPanel();

        /*  SetFile
         *      Displays the "file open" standard dialog box and sets the 
         *      displayed file to the selected file.
         *  Params
         *      none
         *  Returns
         *      none
         */
        void SetFolder();

        /*  GetFile
         *      Returns the name of the file
         *  Params
         *      none
         *  Returns
         *      The name of the file associated with the panel, or empty if no
         *      file is associated with this panel
         */
        wxString GetFolder();

        /*  Recompare
         *      Loads the file, displays it in the text window and shows the
         *      file differences based on the passed Hunk list.
         *  Params
         *      FirstHunk
         *          First hunk in a list of hunks
         *      FileNumber
         *          We should take the differences from the Hunk list for the
         *          specified folder number.
         *  Returns
         *      none
         */
        void Recompare(FolderHunk *FirstHunk, DiffFileNumber FolderNumber);

        /*  GetTextCtrlOffset
         *      Returns the height difference between the DiffTextEdit control
         *      and the rest of the file panel control.  This value is only
         *      valid after the first EVT_STC_PAINTED has been raised.  Also
         *      this value should not change once a paint even has occured.
         *  Params
         *      none
         *  Returns
         *      See description
         */
        int GetTextCtrlOffset();

        /*  GetTreeWindow
         *      Returns the tree window used in the panel
         *  Params
         *      none
         *  Returns
         *      See description
         */
        wxTreeCtrl *GetTreeWindow();

        // Event handlers
        void OnPickFolderButton(wxCommandEvent& event);
    private:
        wxBoxSizer *VerticalSizer;
        wxBoxSizer *TopHorizontalSizer;  
        wxStaticText *TextBox;
        wxButton *PickFolderButton;
        wxTreeCtrl *TreeWindow;
        // Name of file displayed in the panel (empty string if none)
        wxString Folder;
    };
}

#endif