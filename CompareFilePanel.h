/*  CompareFilePanel.h - UI class for a single file.  This class is typically
 *  used in conjunction with the CompareFilesUI class.  CompareFilePanel class
 *  contains a textbox, combobox, and directory selection button.  It binds the
 *  components together to provide an abstract component for displaying diff
 *  files.
 */

#ifndef _COMPAREFILEPANEL_H_
#define _COMPAREFILEPANEL_H_

#include "wx/wx.h"
#include "DiffTextEdit.h"
#include "Hunk.h"

namespace MojoMerge
{
    class CompareFilePanel : public wxPanel
    {
        DECLARE_EVENT_TABLE()
    public:
        /*  CompareFilePanel
         *      Constructs a CompareFilePanel object
         *  Params
         *      Parent
         *          Parent window, usually a CompareFilesUI window
         *  Returns
         *      none
         */
        CompareFilePanel(wxWindow *Parent);

        /*  ~CompareFilePanel
         *      Destructs a CompareFilePanel object
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~CompareFilePanel();

        /*  SetFile
         *      Displays the "file open" standard dialog box and sets the 
         *      displayed file to the selected file.
         *  Params
         *      none
         *  Returns
         *      none
         */
        void SetFile();

        /*  GetFile
         *      Returns the name of the file
         *  Params
         *      none
         *  Returns
         *      The name of the file associated with the panel, or empty if no
         *      file is associated with this panel
         */
        wxString GetFile();

        /*  Recompare
         *      Loads the file, displays it in the text window and shows the
         *      file differences based on the passed Hunk list.
         *  Params
         *      FirstHunk
         *          First hunk in a list of diff hunks
         *      FileNumber
         *          We should take the differences from the Hunk list for the
         *          specified file number.
         *  Returns
         *      none
         */
        void Recompare(Hunk *FirstHunk, DiffFileNumber FileNumber);

        /*  GetBuffer
         *      Returns the original buffer read from the file
         *  Params
         *      none
         *  Returns
         *      See description
         */
        const char *GetBuffer();

        /*  GetTextCtrlOffset
         *      Returns the height difference between the DiffTextEdit control
         *      and the rest of the file panel control.  This value is only
         *      valid after the first EVT_STC_PAINTED has been raised.  Also
         *      this value should not change once a paint even has occured.
         *  Params
         *      none
         *  Returns
         *      none
         */
        int GetTextCtrlOffset();

        /*  GetDiffTextEdit
         *      Returns the DiffTextEdit control associated with this panel
         *  Params
         *      none
         *  Returns
         *      See description
         */
        DiffTextEdit *GetDiffTextEdit();

        // Event handlers
        void OnPickFileButton(wxCommandEvent& event);
    private:
        wxBoxSizer *VerticalSizer;
        wxBoxSizer *TopHorizontalSizer;
        wxComboBox *FileComboBox;
        wxButton *PickFileButton;
        DiffTextEdit *FileTextEdit;
        // Name of file displayed in the panel (empty string if none)
        wxString Filename;
        char *Buffer;
    };
}

#endif