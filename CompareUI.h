/*  CompareUI.h - Base class for Compare window objects.  This provides an
 *  interface for functions that are used on all types of comparison windows.
 */

#ifndef _COMPAREUI_H_
#define _COMPAREUI_H_

#include "TabWindow.h"
#include "Datatypes.h"

namespace MojoMerge
{
    class CompareUI : public TabWindow
    {
    public:
        CompareUI()
        {
            // Type is a CompareUI object as well
            ID |= TabWindow_CompareUI;
        }
        /*  Edit Commands
         *      These are all commands that are applicable to any CompareUI
         *      window.  They must be overridden by any derived class
         */
        virtual void Print() = 0;
        virtual void PrintPreview() = 0;
        virtual void Undo() = 0;
        virtual void Redo() = 0;
        virtual void Cut() = 0;
        virtual void Copy() = 0;
        virtual void Paste() = 0;
        virtual void SelectAll() = 0;
        virtual void TwoWayComparison() = 0;
        virtual void ThreeWayComparison() = 0;
        virtual void Recompare() = 0;
        
        /*  Overrides from TabWindow
         */
        virtual void Initialize(wxWindow *Parent) = 0;
        virtual uint32 RequestCommandStatus() = 0;

        // Event handler that gets called by the SeparatorPanel class
        virtual void OnSeparatorPainted(wxPaintEvent &event, wxDC *dc) = 0;
    };
}

#endif