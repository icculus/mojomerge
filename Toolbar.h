/*  Toolbar.h - Object that represents the toolbar displayed in the main window
 *
 */

#ifndef _TOOLBAR_H_
#define _TOOLBAR_H_

#include "DataTypes.h"
#include "wx/wx.h"

namespace MojoMerge
{
    class Toolbar : public wxToolBar
    {
    public:
        /*  Toolbar
         *      Creates a Toolbar object
         *  Params
         *      Parent
         *          Window that toolbar will be added to
         *  Returns
         *      none
         */
        Toolbar(wxWindow *Parent);

        void SetCommandStatus(uint32 Status);
    private:
        /*  GetBitmap
         *      Returns a wxBitmap object for the specified index.  This method
         *      is used to populate the toolbar buttons with the appopriate
         *      icon for the action.
         *  Params
         *      index
         *          Index of bitmap
         *  Returns
         *      none
         */
        wxBitmap GetBitmap(size_t index);
    };
}

#endif
