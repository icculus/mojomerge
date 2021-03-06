/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

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

#include "DataTypes.h"
#include "wx/wx.h"

namespace MojoMerge
{
    // Max length of the tab name
    #define MAX_TAB_NAME    128

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
            // Set Name default to an empty string
            strcpy(Name, "");
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
        int GetID()
        {
            return ID;
        }

        const char *GetName()
        {
            return Name;
        }

        /*  Initialize
         *      This method initializes the window with the Parent window.
         *      The method is typically called by the TabBrowser object.
         *  Params
         *      none
         *  Returns
         *      none
         */
        virtual void Initialize(wxWindow *Parent) = 0;

        /*  RequestCommandStatus
         *      Called for requesting what commands are applicable for the
         *      CompareUI window at that given point in time.
         *  Params
         *      none
         *  Returns
         *      Mask of command status flags that are valid
         */
        virtual uint32 RequestCommandStatus() = 0;

    protected:
        // ID of TabWindow (see TabWindowID definition for more info)
        int ID;
        // Name associated with window (this is what gets displayed in the
        //  tab name
        char Name[MAX_TAB_NAME];
    };
}

#endif
