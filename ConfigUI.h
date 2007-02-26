/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  ConfigUI.h - User interface to the Config object
 *
 */

#ifndef _CONFIGUI_H_
#define _CONFIGUI_H_
#include "Config.h"
#include "wx/wx.h"

namespace MojoMerge
{
    class ConfigUI : public wxDialog
    {
    public:
        /*  ConfigUI
         *      Creates a ConfigUI object
         *  Params
         *      MyConfig
         *          Reference to a Config object that will be represented
         *  Returns
         *      none
         */
        ConfigUI(Config *MyConfig);
        //~ConfigUI();
    private:
    };
}

#endif
