/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  Menu.h - Object that represents the menu displayed in the main window
 *
 */

#ifndef _MENU_H_
#define _MENU_H_

#include "DataTypes.h"
#include "wx/wx.h"

namespace MojoMerge
{
    class Menu : public wxMenuBar
    {
    public:
        Menu();
        void SetCommandStatus(uint32 Status);
    private:
    };
}

#endif
