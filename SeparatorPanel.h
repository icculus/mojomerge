/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  SeparatorPanel.h - Panel that separates comparison windows.  This is kind
 *  of a workaround to not being able to trap paint events that are generated
 *  by a wxPanel object with the CompareFilesUI object.  This class basically
 *  raises "paint" events to the specified CompareFilesUI object
 *
 */

#ifndef _SEPARATORPANEL_H_
#define _SEPARATORPANEL_H_

#include "wx/wx.h"
#include "CompareUI.h"

namespace MojoMerge
{
    #define SEPARATOR_WIDTH         90

    class SeparatorPanel : public wxPanel
    {
        DECLARE_EVENT_TABLE()
    public:
        /*  SeparatorPanel
         *      Create a new CompareFilesUI window
         *  Params
         *      CompareWindow
         *          Window to pass events to
         *  Returns
         *      none
         */
        SeparatorPanel(CompareUI *CompareWindow);

        // Event handlers
        void OnPaint(wxPaintEvent &event);
    private:
        // Window associated with panel that will receive events
        CompareUI *CompareWindow;
    };
}

#endif
