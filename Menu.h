/*  Menu.h - Object that represents the menu displayed in the main window
 *
 */

#ifndef _MENU_H_
#define _MENU_H_

#include "Datatypes.h"
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