/*  TabWindow - Base class in which all windows displayed in the TabBrowser
 *  object will be derived from.  It is a way to provide very basic RTTI
 *  for these types of objects without the overhead of full RTTI support.
 *
 *  Typically, this class is used to decide what object to coerce a generic
 *  TabWindow object to.  If the value of ID is TabWindow_CompareFilesUI for
 *  example, then it can safely be cast to the CompareFilesUI type.
 */

#ifndef _TABWINDOW_H
#define _TABWINDOW_H

#include "wx/wx.h"

namespace MojoMerge
{
    // List of possible TabWindow classes
    enum TabWindowID
    {
        TabWindow_TabWindow = 0x01,
        TabWindow_CompareFilesUI = 0x02,
        TabWindow_CompareFoldersUI = 0x04,
        TabWindow_CompareUI = 0x08
    };

    class TabWindow : public wxPanel
    {
    public:
        /*  TabWindow
         *      Constructs a TabWindow object.  All it really does is that it
         *      sets the default value of ID to TABWINDOW_TABWINDOW
         *  Params
         *      none
         *  Returns
         *      none
         */
        TabWindow()
        {
            ID = TabWindow_TabWindow;
        }

        /*  GetID
         *      Returns the ID of the derived class.  This ID should be set in
         *      the constructor of the derived object by setting the private
         *      ID member to the appropriate TabWindowID value.
         *
         *      IMPORTANT: This value should be set using the '|' operator.
         *      This is so that objects can be recognized as all of their
         *      valid types.  For example, a CompareFilesUI object is a
         *      TabWindow, a CompareUI, and a CompareFilesUI object.
         *  Parameters
         *      none
         *  Returns
         *      ID of the derived class.  This ID should be tested for equality
         *      by using the '&&' operator.
         */
        TabWindowID GetID()
        {
            return ID;
        }
    protected:
        // ID of TabWindow (see TabWindowID definition for more info)
        TabWindowID ID;
    };
}

#endif