/*  Menu.cpp - Implementation of Menu object
 *
 */

#include "Menu.h"
#include "Identifiers.h"
#include "Application.h"

using namespace MojoMerge;

Menu::Menu()
{
    // Create "File" menu
    wxMenu *FileMenu = new wxMenu();
    FileMenu->Append(ID_TWOWAYFILECOMPARE_MENU, wxT("New two-way File Comparison\tCtrl-n"), wxT(""));
    FileMenu->Append(ID_THREEWAYFILECOMPARE_MENU, wxT("New three-way File Comparison\tCtrl-Shift-n"), wxT(""));
    FileMenu->Append(ID_TWOWAYFOLDERCOMPARE_MENU, wxT("New two-way Folder Comparison\tCtrl-d"), wxT(""));
    FileMenu->Append(ID_THREEWAYFOLDERCOMPARE_MENU, wxT("New three-way Folder Comparison\tCtrl-Shift-d"), wxT(""));
    FileMenu->AppendSeparator();
    FileMenu->Append(ID_CLOSEWINDOW_MENU, wxT("Close Window\tCtrl-w"), wxT(""));
    FileMenu->AppendSeparator();
    wxMenu *SaveSubMenu = new wxMenu();
    SaveSubMenu->Append(ID_SAVE_FILE1_MENU, wxT("Save First File\tCtrl-1"), wxT(""));
    SaveSubMenu->Append(ID_SAVE_FILE2_MENU, wxT("Save Second File\tCtrl-2"), wxT(""));
    SaveSubMenu->Append(ID_SAVE_FILE3_MENU, wxT("Save Third File\tCtrl-3"), wxT(""));
    SaveSubMenu->AppendSeparator();
    SaveSubMenu->Append(ID_SAVE_FILE1_AS_MENU, wxT("Save First File As..."), wxT(""));
    SaveSubMenu->Append(ID_SAVE_FILE2_AS_MENU, wxT("Save Second File As..."), wxT(""));
    SaveSubMenu->Append(ID_SAVE_FILE3_AS_MENU, wxT("Save Third File As..."), wxT(""));
    FileMenu->Append(ID_SAVE_MENU, wxT("Save"), SaveSubMenu);
    FileMenu->AppendSeparator();
    FileMenu->Append(ID_PRINTPREVIEW_MENU, wxT("Print Preview"), wxT(""));
    FileMenu->Append(ID_PRINT_MENU, wxT("Print\tCtrl-P"), wxT(""));
    FileMenu->AppendSeparator();
    FileMenu->Append(ID_EXIT_MENU, wxT("Exit"), wxT(""));
    Append(FileMenu, wxT("File") );
    
    // Create "Edit" menu
    wxMenu *EditMenu = new wxMenu();
    EditMenu->Append(ID_UNDO_MENU, wxT("Undo\tCtrl-Z"), wxT(""));
    EditMenu->Append(ID_REDO_MENU, wxT("Redo\tCtrl-Y"), wxT(""));
    EditMenu->AppendSeparator();
    EditMenu->Append(ID_CUT_MENU, wxT("Cut\tCtrl-X"), wxT(""));
    EditMenu->Append(ID_COPY_MENU, wxT("Copy\tCtrl-C"), wxT(""));
    EditMenu->Append(ID_PASTE_MENU, wxT("Paste\tCtrl-V"), wxT(""));
    EditMenu->Append(ID_SELECTALL_MENU, wxT("Select All\tCtrl-A"), wxT(""));
    Append(EditMenu, wxT("Edit"));
    
    // Create "Compare" menu
    wxMenu *CompareMenu = new wxMenu();
    CompareMenu->Append(ID_RECOMPARE_MENU, wxT("Recompare\tF5"), wxT(""));
    CompareMenu->AppendSeparator();
    //CompareMenu->Append(ID_PREFERENCES_MENU, wxT("Preferences"), wxT(""));
    CompareMenu->Append(ID_PREFERENCES_MENU, wxT("Ignore white-space"), wxT(""));
    CompareMenu->Append(ID_PREFERENCES_MENU, wxT("Ignore line ending diffs"), wxT(""));
    Append(CompareMenu, wxT("Compare"));
    
    // Create "View" menu
    //wxMenu *ViewMenu = new wxMenu();
    //ViewMenu->Append(ID_TOOLBAR_MENU, wxT("Toolbar"), wxT(""), TRUE);
    //ViewMenu->Append(ID_STATUSBAR_MENU, wxT("Status Bar"), wxT(""), TRUE);
    //Append(ViewMenu, wxT("View"));
    
    // Create "Help" menu
    wxMenu *HelpMenu = new wxMenu();
    HelpMenu->Append(ID_TODO_MENU, wxT("TODO"), wxT(""));
    Append(HelpMenu, wxT("Help"));

    // Create "Debug" menu if we're a debug build
#ifdef _DEBUG
    wxMenu *DebugMenu = new wxMenu();
    DebugMenu->Append(ID_TEST_TWOWAY_DIFF_MENU, wxT("Test two-way diff"), wxT(""));
    DebugMenu->Append(ID_TEST_THREEWAY_DIFF_MENU, wxT("Test three-way diff"), wxT(""));
    DebugMenu->Append(ID_TEST_LINEBUFFER_MENU, wxT("Test LineBuffer"), wxT(""));
    DebugMenu->Append(ID_TEST_TWOWAY_MERGE_MENU, wxT("Test two-way merge"), wxT(""));
    DebugMenu->Append(ID_TEST_STACK_MENU, wxT("Test Stack"), wxT(""));
    DebugMenu->Append(ID_TEST_COMPAREFOLDERS_MENU, wxT("Test CompareFolders"), wxT(""));
    Append(DebugMenu, wxT("Debug"));
#endif
}

void Menu::SetCommandStatus(uint32 Status)
{
    Enable(ID_SAVE_FILE1_MENU, Cmd_SaveFirstFile & Status);
    Enable(ID_SAVE_FILE2_MENU, Cmd_SaveSecondFile & Status);
    Enable(ID_SAVE_FILE3_MENU, Cmd_SaveThirdFile & Status);
    Enable(ID_SAVE_FILE1_AS_MENU, Cmd_SaveFirstFileAs & Status);
    Enable(ID_SAVE_FILE2_AS_MENU, Cmd_SaveSecondFileAs & Status);
    Enable(ID_SAVE_FILE3_AS_MENU, Cmd_SaveThirdFileAs & Status);
    Enable(ID_PRINT_MENU, Cmd_Print & Status);
    Enable(ID_PRINTPREVIEW_MENU, Cmd_PrintPreview & Status);
    Enable(ID_UNDO_MENU, Cmd_Undo & Status);
    Enable(ID_REDO_MENU, Cmd_Redo & Status);
    Enable(ID_CUT_MENU, Cmd_Cut & Status);
    Enable(ID_COPY_MENU, Cmd_Copy & Status);
    Enable(ID_PASTE_MENU, Cmd_Paste & Status);
    Enable(ID_SELECTALL_MENU, Cmd_SelectAll & Status);
    Enable(ID_RECOMPARE_MENU, Cmd_Recompare & Status);
    //Enable(, Cmd_SetFile & Status);
}