/*  DiffTextEdit.h - wxStyledTextCtrl with special "helper" functions used
 *  specifically for marking diffs, merging, etc...
 */

#ifndef _DIFFTEXTEDIT_H_
#define _DIFFTEXTEDIT_H_

#include "wx/stc/stc.h"

namespace MojoMerge
{
    class DiffTextEdit : public wxStyledTextCtrl
    {
        //DECLARE_EVENT_TABLE()
    public:
        /*  DiffTextEdit
         *      Constructs a new DiffTextEdit object
         *  Params
         *      Parent
         *          Window that his control is added to
         *      id
         *          ID associated with control
         *  Returns
         *      none
         */
        DiffTextEdit(wxWindow *Parent, wxWindowID id);

        /*  MarkChangedLine
         *      Highlights the specified line range with the color used for
         *      marking chnages.
         *  Params
         *      Start
         *          Starting line of range (starting at line 1)
         *      End
         *          Endling line of range
         *  Returns
         *      none
         */
        void MarkChangedLine(int Start, int End);

        /*  MarkInsertAfterLine
         *      Marks the specified line as an "insert after" line
         *  Params
         *      Line
         *          Line to change (starting at line 1)
         *  Returns
         *      none
         */
        void MarkInsertAfterLine(int Line);

        /*  MarkClearAll
         *      Clear all marks made in text
         *  Params
         *      none
         *  Returns
         *      none
         */
        void MarkClearAll();

        /*  DisplayText
         *      Just like the wxStyledTextCtrl::SetText except this one also
         *      does some resetting of styles
         *  Params
         *      See wxStyledTextCtrl::SetText control for more information
         *  Returns
         *      none
         */
        void DisplayText(const wxString& text);

        /*  EOFStyleBugHack
         *      Work around for a styling bug in the wxStyledTextCtrl.  See the
         *      function implementation for specifics.
         *  Params
         *      none
         *  Returns
         *      none
         */
        void EOFStyleBugHack();

        /*  BlankLineStyleBugHack
         *      Work around for a styling bug in the wxStyledTextCtrl.  See the
         *      function implementation for specifics.
         *  Params
         *      none
         *  Returns
         *      none
         */
        void BlankLineStyleBugHack();

        /*  DeleteLines
         *      Removes the specified lines from the control
         *  Params
         *      Start
         *          0-based starting index
         *      Length
         *          Number of lines to remove
         *  Returns
         *      none
         */
        void DeleteLines(int Start, int Length);

        void ConfigSetFont(wxFont &NewFont);
        void ConfigSetShowEOL(bool ShowEOL);
        void ConfigSetChangeColor(wxColour &Foreground, wxColour &Background);

        // Event handlers
        //void OnPainted(wxStyledTextEvent& event);
    private:
	};
}

#endif