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

int Application::OnExit()
{
    // Cleanup the config object
    Config::Deinit();

    // Delete the temporary folder string if it has been set
    if(TempFolder)
        delete TempFolder;

    return 0;
}

void Application::CmdNewTwoWayFileComparison()
{
    // Create a new comparison window
    Browser->AddWindow(new CompareFilesUI());
}

void Application::CmdNewThreeWayFileComparison()
{
    // Create a new comparison window
    Browser->AddWindow(new CompareFilesUI(true));
}

void Application::CmdNewTwoWayFolderComparison()
{
    // Create a new comparison window
    Browser->AddWindow(new CompareFoldersUI());
}

void Application::CmdNewThreeWayFolderComparison()
{
    // Create a new comparison window
    Browser->AddWindow(new CompareFoldersUI(true));
}

void Application::CmdCloseWindow()
{
    // Create a new comparison window
    Browser->RemoveActiveWindow();
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

void Application::CmdRecompare()
{
    // Coerce the current window and print
    GetWindowAsCompareUI(Browser->GetActiveWindow())->Recompare();
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

void Application::UpdateAvailableCommandsForActiveWindow()
{
    Debug("UpdateAvailableCommandForActiveWindow() called");
    uint32 Status;
    TabWindow *MyWindow = Browser->GetActiveWindow();
        
    if(MyWindow)
        Status = MyWindow->RequestCommandStatus();

    // Notify main window of new command status
    Window->SetCommandStatus(Status);
}

void Application::CmdSetFile(DiffFileNumber FileNumber)
{
    GetWindowAsCompareFilesUI(Browser->GetActiveWindow())->SetFile(FileNumber);
}

void Application::CmdSetStatusBarMsg(wxString Message)
{
    // Set the text on the status bar appropriately
    Window->GetStatusBar()->SetStatusText(Message);
}

char *Application::ReadFile(const char *Filename,
    bool NullTerminator, uint32 *BufferLength)
{
    char *Buffer = NULL;    // Buffer to read characters in
    uint32 FileLength;      // Length of input file
    int i;                  // Temporary variable

    // Number of characters read from file
    size_t CharsRead;

    // Filename or Buffer can't be NULL
    assert(Filename);
    // Open file for reading
    FILE *Stream = fopen(Filename, "rb");
    // Stream can't be NULL
    if(Stream)
    {
        // Go to the end of the file
        fseek(Stream, 0, SEEK_END);
        // Get the current position (the length of the file)
        FileLength = ftell(Stream);
        // Allocate buffer to hold the entire file (plus optional null term.)
        Buffer = new char[FileLength + 1];
        // Buffer can't be NULL
        assert(Buffer);
        // Go back to the beginning of the file
        fseek(Stream, 0, SEEK_SET);
        // Read contents of file into buffer
        CharsRead = fread(Buffer, sizeof(char), FileLength, Stream);
        // If we couldn't read the entire file
        if(CharsRead != FileLength)
        {
            // Remove the buffer
            delete Buffer;
            // Return NULL to indicate an error
            Buffer = NULL;
        }
        else
        {
            // Add the terminating null character if applicable
            if(NullTerminator)
                Buffer[CharsRead] = 0x00;
            // Close the file stream
            i = fclose(Stream);
            // Make sure file closed successfully
            assert(i == 0);
        }
    }

    return Buffer;
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

MainWindow *Application::GetMainWindow()
{
    return Window;
}

#ifdef _DEBUG
void Application::Debug(char *format, ...)
{
    va_list args;

    va_start(args, format);
    wxVLogTrace(format, args);
}

void Application::DebugNoCR(char *format, ...)
{
    va_list args;

    va_start(args, format);
    wxVLogTrace(format, args);
}

void Application::AddTestTab(TabWindow *NewWindow)
{
    // Add specified window to our tab browser object
    Browser->AddWindow(NewWindow);
}
#endif
