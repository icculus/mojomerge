/*  Application.cpp - Implementation for the Application class
 *
 */

#include "Application.h"
#include "ConfigUI.h"
#include "Config.h"
#include <stdio.h>
#include <windows.h>
#include <stdarg.h>

using namespace MojoMerge;

// Initialize static member variables
char *Application::TempFolder = NULL;
MainWindow *Application::Window = NULL;
TabBrowser *Application::Browser = NULL;

// Creates a standard wxWindows main starting point
IMPLEMENT_APP(Application)

bool Application::OnInit()
{
#ifdef _WINDOWS
#ifdef _DEBUG
    // Allocate a console for outputting debug info if we don't
    // have a console already (Windows only)
    //AllocConsole();
#endif // _DEBUG
#endif // _WINDOWS
    // Load up config data
    Config::Init();
    // Create the main application window
    Window = new MainWindow();
    // Window can't be NULL
    assert(Window);
    // Save reference to tab browser object
    Browser = Window->GetTabBrowser();
    // Browser object can't be NULL
    assert(Browser);
    // Show the window
    Window->Show(true);
    SetTopWindow(Window);
    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return true;
}

void Application::CmdNewComparison()
{
    // Create a new comparison window
    Browser->AddWindow(CreateDefaultComparisonWindow());
}

void Application::CmdSaveFirstFile()
{
    // Coerce the current window and save the file
    GetWindowAsCompareFilesUI(Browser->GetActiveWindow())->SaveFile(DiffFile_One);
}

void Application::CmdSaveSecondFile()
{
    // Coerce the current window and save the file
    GetWindowAsCompareFilesUI(Browser->GetActiveWindow())->SaveFile(DiffFile_Two);
}

void Application::CmdSaveThirdFile()
{
    // Coerce the current window and save the file
    GetWindowAsCompareFilesUI(Browser->GetActiveWindow())->SaveFile(DiffFile_Three);
}

void Application::CmdSaveFirstFileAs()
{
    // Coerce the current window and save the file
    GetWindowAsCompareFilesUI(Browser->GetActiveWindow())->SaveFileAs(DiffFile_One);
}

void Application::CmdSaveSecondFileAs()
{
    // Coerce the current window and save the file
    GetWindowAsCompareFilesUI(Browser->GetActiveWindow())->SaveFileAs(DiffFile_Two);
}

void Application::CmdSaveThirdFileAs()
{
    // Coerce the current window and save the file
    GetWindowAsCompareFilesUI(Browser->GetActiveWindow())->SaveFileAs(DiffFile_Three);
}

void Application::CmdPrint()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Print();
}

void Application::CmdPrintPreview()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->PrintPreview();
}

void Application::CmdUndo()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Undo();
}

void Application::CmdRedo()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Redo();
}

void Application::CmdCut()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Cut();
}

void Application::CmdCopy()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Copy();
}

void Application::CmdPaste()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Paste();
}

void Application::CmdSelectAll()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->SelectAll();
}

void Application::CmdCompareFiles()
{
}

void Application::CmdCompareFolders()
{
}

void Application::CmdTwoWayComparison()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Print();
}

void Application::CmdThreeWayComparison()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Print();
}

void Application::CmdRecompare()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Print();
}

void Application::CmdPreferences()
{
    // TODO - Eventually we'll pass a real config object here
    ConfigUI MyConfigUI(NULL);
    MyConfigUI.ShowModal();
}

void Application::CmdToolbar()
{
}

void Application::CmdStatusbar()
{
}

void Application::CmdExit()
{
    // Dispose of our main window
    delete Window;
}

const char *Application::GetTempFolder()
{
    #define UNIX_TEMPFOLDER     "/tmp/"

    if(TempFolder == NULL)
    {
        TempFolder = new char[MOJO_MAX_PATH];
// If running on Windows, use WIN32 API call
#ifdef _WINDOWS
        int i = GetTempPath(MOJO_MAX_PATH, TempFolder);
        // GetTempPath return 0 on failure
        assert(i != 0);
// Else, assume we're running on a UNIX based system
#else
        strcpy(TempFolder, UNIX_TEMPFOLDER);
#endif
    }
    
    return TempFolder;
}

CompareUI *Application::CreateDefaultComparisonWindow()
{
    // TODO - Add code to look at Config object for actual default
    // For now, just return a two-way diff object
    CompareUI *NewWindow = new CompareFilesUI();
    // Window can't be NULL
    assert(NewWindow);

    return NewWindow;
}

CompareFilesUI *Application::GetWindowAsCompareFilesUI(TabWindow *Window)
{
    CompareFilesUI *NewWindow = NULL;

    // Specified window can't be NULL
    assert(Window);
    // If TabWindow is also a CompareFilesUI object
    if(Window->GetID() && TabWindow_CompareFilesUI)
        NewWindow = (CompareFilesUI *)Window;
    // Make sure it was converted correctly
    assert(NewWindow);

    return NewWindow;
}

CompareUI *Application::GetWindowAsCompareUI(TabWindow *Window)
{
    CompareUI *NewWindow = NULL;

    // Specified window can't be NULL
    assert(Window);
    // If TabWindow is also a CompareUI object
    if(Window->GetID() && TabWindow_CompareUI)
        NewWindow = (CompareUI *)Window;
    // Make sure it was converted correctly
    assert(NewWindow);
    
    return NewWindow;
}

#ifdef _DEBUG
void Application::Debug(char *format, ...)
{
    va_list args;
    int len;
    char *buffer;

    va_start(args, format);
    // Get total number of characters plus null terminator and newline
    len = _vscprintf(format, args) + 2;
    buffer = new char[len];
    vsprintf(buffer, format, args);
    // Append newline to end of buffer
    strcat(buffer, "\n");
    // Output the debug message
    printf(buffer);
#ifdef _WINDOWS
    // Under windows, we have to flush stdout so that messages will display
    //  as they are written.
    fflush(stdout);
#endif
    delete buffer;
}
void Application::DebugNoCR(char *format, ...)
{
    va_list args;
    int len;
    char *buffer;

    va_start(args, format);
    // Get total number of characters plus null terminator and newline
    len = _vscprintf(format, args) + 2;
    buffer = new char[len];
    vsprintf(buffer, format, args);
    // Output the debug message
    printf(buffer);
#ifdef _WINDOWS
    // Under windows, we have to flush stdout so that messages will display
    //  as they are written.
    fflush(stdout);
#endif
    delete buffer;
}
#endif
