/*  MainWindow.cpp - Implementation for the MainWindow class
 *
 */

#include "MainWindow.h"
#include "Identifiers.h"
#include "Application.h"

using namespace MojoMerge;

MainWindow::MainWindow()
{
    // Initialize member variables
    MyMenu = NULL;
    MyToolbar = NULL;

    // Create the window
    Create(NULL, ID_MAIN_WINDOW, wxT("MojoMerge ALPHA"));
    // Generate the menubar
    MyMenu = new Menu();
    // Generate the toolbar
    // TODO - Not implemented yet.  We have this commented out since creating
    //  this empty window generates lots of Win32 API messages
    //MyToolbar = new Toolbar();
    // Create the tab browser window
    MyTabBrowser = new TabBrowser(this);
    // Add the menu bar to the frame
    SetMenuBar(MyMenu);
    // Add the toolbar to the frame
    // TODO - Not implemented yet.  We have this commented out since creating
    //  this empty window generates lots of Win32 API messages
    //SetToolBar(MyToolbar);
}

TabBrowser *MainWindow::GetTabBrowser()
{
    return MyTabBrowser;
}

void MainWindow::OnNewTwoWayFileComparison(wxCommandEvent& event)
{
    Application::CmdNewTwoWayFileComparison();
}

void MainWindow::OnNewThreeWayFileComparison(wxCommandEvent& event)
{
    Application::CmdNewThreeWayFileComparison();
}

void MainWindow::OnNewTwoWayFolderComparison(wxCommandEvent& event)
{
    Application::CmdNewTwoWayFolderComparison();
}

void MainWindow::OnNewThreeWayFolderComparison(wxCommandEvent& event)
{
    Application::CmdNewThreeWayFolderComparison();
}

void MainWindow::OnSaveFirstFile(wxCommandEvent& event)
{
    Application::CmdSaveFirstFile();
}

void MainWindow::OnSaveSecondFile(wxCommandEvent& event)
{
    Application::CmdSaveSecondFile();
}

void MainWindow::OnSaveThirdFile(wxCommandEvent& event)
{
    Application::CmdSaveThirdFile();
}

void MainWindow::OnSaveFirstFileAs(wxCommandEvent& event)
{
    Application::CmdSaveFirstFileAs();
}

void MainWindow::OnSaveSecondFileAs(wxCommandEvent& event)
{
    Application::CmdSaveSecondFileAs();
}

void MainWindow::OnSaveThirdFileAs(wxCommandEvent& event)
{
    Application::CmdSaveThirdFileAs();
}

void MainWindow::OnPrint(wxCommandEvent& event)
{
    Application::CmdPrint();
}

void MainWindow::OnPrintPreview(wxCommandEvent& event) 
{
    Application::CmdPrintPreview();
}

void MainWindow::OnExit(wxCommandEvent& event) 
{
    Application::CmdExit();
}

void MainWindow::OnUndo(wxCommandEvent& event)
{
    Application::CmdUndo();
}

void MainWindow::OnRedo(wxCommandEvent& event) 
{
    Application::CmdRedo();
}

void MainWindow::OnCut(wxCommandEvent& event) 
{
    Application::CmdCut();
}

void MainWindow::OnCopy(wxCommandEvent& event) 
{
    Application::CmdCopy();
}

void MainWindow::OnPaste(wxCommandEvent& event) 
{
    Application::CmdPaste();
}

void MainWindow::OnSelectAll(wxCommandEvent& event)
{
    Application::CmdSelectAll();
}

void MainWindow::OnRecompare(wxCommandEvent& event)
{
    Application::CmdRecompare();
}

void MainWindow::OnPreferences(wxCommandEvent& event)
{
    Application::CmdPreferences();
}

void MainWindow::OnToolbar(wxCommandEvent& event)
{
    Application::CmdToolbar();
}

void MainWindow::OnStatusbar(wxCommandEvent& event) 
{
    Application::CmdStatusbar();
}
void MainWindow::OnCloseWindow(wxCommandEvent& event)
{
    Application::CmdCloseWindow();
}

#ifdef _DEBUG
void MainWindow::OnTestTwoWayDiff(wxCommandEvent& event)
{
    Hunk *MyHunk;
    // Test two-way diff and just remove the hunk list for now
    MyHunk = Test::TestGNUDiff_TwoWay();
    // Just remove the returned hunks for now
    if(MyHunk)
        MyHunk->DeleteList();
}
void MainWindow::OnTestThreeWayDiff(wxCommandEvent& event) 
{
    Hunk *MyHunk;
    // Test two-way diff and just remove the hunk list for now
    MyHunk = Test::TestGNUDiff_ThreeWay();
    // Just remove the returned hunks for now
    if(MyHunk)
        MyHunk->DeleteList();
}
void MainWindow::OnTestLineBuffer(wxCommandEvent& event) 
{
    // Test the LineBuffer object
    Test::TestLineBuffer();
}
void MainWindow::OnTestTwoWayMerge(wxCommandEvent& event)
{
    // Test the two-way merge functionality
    Test::TestMerge_TwoWay();
}
void MainWindow::OnTestStack(wxCommandEvent& event)
{
    // Test the Stack object
    Test::TestStack();
}
void MainWindow::OnTestCompareFolders(wxCommandEvent& event)
{
    // Test the CompareFolders object
    Test::TestCompareFolders();
}
void MainWindow::OnTestCompareFilesUI(wxCommandEvent& event)
{
    // Test the CompareFolders object
    Test::TestCompareFilesUI();
}
void MainWindow::OnTestCompareFilesUI3Way(wxCommandEvent& event)
{
    // Test the CompareFolders object
    Test::TestCompareFilesUI3Way();
}

#endif

// Event mappings for the menu
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_TWOWAYFILECOMPARE_MENU, MainWindow::OnNewTwoWayFileComparison)
    EVT_MENU(ID_THREEWAYFILECOMPARE_MENU, MainWindow::OnNewThreeWayFileComparison)
    EVT_MENU(ID_TWOWAYFOLDERCOMPARE_MENU, MainWindow::OnNewTwoWayFolderComparison)
    EVT_MENU(ID_THREEWAYFOLDERCOMPARE_MENU, MainWindow::OnNewThreeWayFolderComparison)
    EVT_MENU(ID_CLOSEWINDOW_MENU, MainWindow::OnCloseWindow)
    EVT_MENU(ID_SAVE_FILE1_MENU, MainWindow::OnSaveFirstFile)
    EVT_MENU(ID_SAVE_FILE2_MENU, MainWindow::OnSaveSecondFile)
    EVT_MENU(ID_SAVE_FILE3_MENU, MainWindow::OnSaveThirdFile)
    EVT_MENU(ID_SAVE_FILE1_AS_MENU, MainWindow::OnSaveFirstFileAs)
    EVT_MENU(ID_SAVE_FILE2_AS_MENU, MainWindow::OnSaveSecondFileAs)
    EVT_MENU(ID_SAVE_FILE3_AS_MENU, MainWindow::OnSaveThirdFileAs)
    EVT_MENU(ID_PRINTPREVIEW_MENU, MainWindow::OnPrintPreview)
    EVT_MENU(ID_PRINT_MENU, MainWindow::OnPrint)
    EVT_MENU(ID_EXIT_MENU, MainWindow::OnExit)
    EVT_MENU(ID_UNDO_MENU, MainWindow::OnUndo)
    EVT_MENU(ID_REDO_MENU, MainWindow::OnRedo)
    EVT_MENU(ID_CUT_MENU, MainWindow::OnCut)
    EVT_MENU(ID_COPY_MENU, MainWindow::OnCopy)
    EVT_MENU(ID_PASTE_MENU, MainWindow::OnPaste)
    EVT_MENU(ID_SELECTALL_MENU, MainWindow::OnSelectAll)
    EVT_MENU(ID_RECOMPARE_MENU, MainWindow::OnRecompare)
    EVT_MENU(ID_PREFERENCES_MENU, MainWindow::OnPreferences)
    EVT_MENU(ID_TOOLBAR_MENU, MainWindow::OnToolbar)
    EVT_MENU(ID_STATUSBAR_MENU, MainWindow::OnStatusbar)
#ifdef _DEBUG
    EVT_MENU(ID_TEST_TWOWAY_DIFF_MENU, MainWindow::OnTestTwoWayDiff)
    EVT_MENU(ID_TEST_THREEWAY_DIFF_MENU, MainWindow::OnTestThreeWayDiff)
    EVT_MENU(ID_TEST_LINEBUFFER_MENU, MainWindow::OnTestLineBuffer)
    EVT_MENU(ID_TEST_TWOWAY_MERGE_MENU, MainWindow::OnTestTwoWayMerge)
    EVT_MENU(ID_TEST_STACK_MENU, MainWindow::OnTestStack)
    EVT_MENU(ID_TEST_COMPAREFOLDERS_MENU, MainWindow::OnTestCompareFolders)
    EVT_MENU(ID_TEST_COMPAREFILESUI_MENU, MainWindow::OnTestCompareFilesUI)
    EVT_MENU(ID_TEST_COMPAREFILESUI3WAY_MENU, MainWindow::OnTestCompareFilesUI3Way)
#endif
END_EVENT_TABLE()

