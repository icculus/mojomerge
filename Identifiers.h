/*  Identifiers.h - Contains a list of unique identifiers associated with
 *  various GUI items and events
 */

#ifndef _IDENTIFIERS_H_
#define _IDENTIFIERS_H_

#include "wx/wx.h"

enum
{
    ID_DO_NOT_REMOVE = wxID_HIGHEST,
    // MainWindow IDs
    ID_MAIN_WINDOW,
    ID_DEBUG_WINDOW,
    // TabBrowser IDs
    ID_TAB_BROWSER_NOTEBOOK,
    ID_TAB_BROWSER_PANEL,
    // CompareFilePanel IDs
    ID_COMPARE_FILE_PANEL,
    ID_FILE_TEXT_BOX,
    ID_PICK_FILE_BUTTON,
    ID_FILE_TEXT_EDIT,
    // CompareFilesUI IDs
    ID_SEPARATOR_PANEL,
    ID_MERGE_BUTTON,
    // Menu bar IDs
    ID_FILE_MENU,
    ID_FILECOMPARE_MENU,
    ID_MENU,
    ID_SAVE_MENU,
    ID_SAVE_FILE1_MENU,
    ID_SAVE_FILE2_MENU,
    ID_SAVE_FILE3_MENU,
    ID_SAVE_FILE1_AS_MENU,
    ID_SAVE_FILE2_AS_MENU,
    ID_SAVE_FILE3_AS_MENU,
    ID_PRINTPREVIEW_MENU,
    ID_PRINT_MENU,
    ID_EXIT_MENU,
    ID_EDIT_MENU,
    ID_UNDO_MENU,
    ID_REDO_MENU,
    ID_CUT_MENU,
    ID_COPY_MENU,
    ID_PASTE_MENU,
    ID_SELECTALL_MENU,
    ID_COMPARE_MENU,
    ID_CMPFILES_OPT_MENU,
    ID_CMPFOLDERS_MENU,
    ID_TWOWAY_MENU,
    ID_THREEWAY_MENU,
    ID_RECOMPARE_MENU,
    ID_PREFERENCES_MENU,
    ID_VIEW_MENU,
    ID_TOOLBAR_MENU,
    ID_STATUSBAR_MENU,
    ID_HELP_MENU,
    ID_TODO_MENU,
    ID_TEST_TWOWAY_DIFF_MENU,
    ID_TEST_THREEWAY_DIFF_MENU,
    ID_TEST_LINEBUFFER_MENU,
    ID_TEST_TWOWAY_MERGE_MENU,
    ID_TEST_STACK_MENU,
    ID_TEST_COMPAREFOLDERS_MENU,
    ID_TEST_COMPAREFILESUI_MENU,
    ID_TEST_COMPAREFILESUI3WAY_MENU
};

#endif