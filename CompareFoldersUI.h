/*  CompareFoldersUI.h - Window that provides the interface for comparing folders
 *
 */

#ifndef _COMPAREFILESUI_H
#define _COMPAREFILESUI_H

#include "wx/wx.h"
#include "DataTypes.h"
#include "CompareUI.h"

namespace MojoMerge
{
    class CompareFoldersUI : public CompareUI
    {
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
    private:
    };
}

#endif