/*  Config.h - Class to read/write configuration data for MojoMerge
 *  The Config class will use the wxConfig class to save/restore configuration
 *  data in a portable manor.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "wx/config.h"

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
         *  Sets option for showing inline character changes on each line
         *  Params
         *      none
         *          
         *  Returns
         *      TRUE to show inline changes, FALSE to hide them
         */
        static bool GetShowInlineChanges();
    private:
        static wxConfig *MyConfigData;
    };
}
#endif