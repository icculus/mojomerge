/*  CompareFilesUI.h - Window that provides the interface for comparing files
 *
 */

#ifndef _COMPAREFILESUI_H
#define _COMPAREFILESUI_H

#include "wx/wx.h"
#include "DataTypes.h"
#include "CompareUI.h"

namespace MojoMerge
{
    class CompareFilesUI : public CompareUI
    {
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

        /*  SetFile
         *      Sets the file to use as the first, second, or third file.
         *  Params
         *      File
         *          File number to assign the filename to
         *      Filename
         *          Name of file to set for the specified file number
         *  Returns
         *      none
         */
        void SetFile(DiffFileNumber File, wxString &Filename);

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
    private:
    };
}

#endif