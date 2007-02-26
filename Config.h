/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  Config.h - Class to read/write configuration data for MojoMerge
 *  The Config class will use the wxConfig class to save/restore configuration
 *  data in a portable manor.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "wx/wx.h"
#include "wx/config.h"
#include "DataTypes.h"

namespace MojoMerge
{
    class Config
    {
    public:
        /*  Init
         *      Initializes the config data
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void Init();

        /*  Deinit
         *      Deinitializes the config data
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void Deinit();

        /*  SetShowLineNumbers
         *  Sets option for showing line numbers when displaying file diffs.
         *  Params
         *      Value
         *          TRUE to show line numbers, FALSE to hide them.
         *  Returns
         *      none
         */
        static void SetShowLineNumbers(bool Value);

        /*  GetShowLineNumbers
         *  Gets option for showing line numbers when displaying file diffs.
         *  Params
         *      none
         *  Returns
         *      TRUE if showing line numbers, FALSE if hiding them.
         */
        static bool GetShowLineNumbers();

        /*  SetShowCRLF
         *  Sets option for showing CR/LF as individual characters.
         *  Params
         *      Value
         *          TRUE to show CF/LF, FALSE to hide them.
         *  Returns
         *      none
         */
        static void SetShowCRLF(bool Value);

        /*  GetShowCRLF
         *  Gets option for showing CR/LF as individual characters.
         *  Params
         *      none
         *          
         *  Returns
         *      TRUE to show CF/LF, FALSE to hide them.
         */
        static bool GetShowCRLF();

        /*  SetShowInlineChanges
         *  Sets option for showing inline character changes on each line
         *  Params
         *      Value
         *          TRUE to show inline changes, FALSE to hide them
         *  Returns
         *      none
         */
        static void SetShowInlineChanges(bool Value);

        /*  GetShowInlineChanges
         *  Gets option for showing inline character changes on each line
         *  Params
         *      none
         *          
         *  Returns
         *      TRUE to show inline changes, FALSE to hide them
         */
        static bool GetShowInlineChanges();

        /*  GetUndoSize
         *  Gets option for how large the undo buffer will be
         *  Params
         *      none
         *  Returns
         *      See description
         */
        static uint32 GetUndoSize();

        /*  SetUndoSize
         *  Sets option for how large the undo buffer will be
         *  Params
         *      Size
         *          Size of the undo buffer
         *  Returns
         *      none
         */
        static void SetUndoSize(uint32 Size);

        /*  GetDiffPath
         *      Gets the path to the "diff" program
         *  Params
         *      none
         *  Returns
         *      String representing the path of "diff"
         */
        static wxString GetDiffPath();

        /*  SetDiffPath
         *      Sets the path to the "diff" program
         *  Params
         *      Value
         *          String representing the path of "diff"
         *  Returns
         *      none
         */
        static void SetDiffPath(wxString &Value);

        /*  GetDiff3Path
         *      Gets the path to the "diff3" program
         *  Params
         *      none
         *  Returns
         *      String representing the path of "diff3"
         */
        static wxString GetDiff3Path();

        /*  SetDiff3Path
         *      Sets the path to the "diff3" program
         *  Params
         *      Value
         *          String representing the path of "diff3"
         *  Returns
         *      none
         */
        static void SetDiff3Path(wxString &Value);

        static void SetIgnoreWhitespace(bool Value);
        static void SetIgnoreLineEndings(bool Value);
        static void SetIgnoreCase(bool Value);
        static bool GetIgnoreWhitespace();
        static bool GetIgnoreLineEndings();
        static bool GetIgnoreCase();
    private:
        static wxConfig *MyConfigData;
    };
}
#endif
