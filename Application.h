/*  Application.h - Main application class (program starts here)
 *
 */

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "wx/wx.h"
#include "TabBrowser.h"
#include "CompareFilesUI.h"
#include "CompareFoldersUI.h"
#include "MainWindow.h"

namespace MojoMerge
{
    enum AppCommand
    {
        Cmd_SaveFirstFile       =   0x00000001,
        Cmd_SaveSecondFile      =   0x00000002,
        Cmd_SaveThirdFile       =   0x00000004,
        Cmd_SaveFirstFileAs     =   0x00000008,
        Cmd_SaveSecondFileAs    =   0x00000010,
        Cmd_SaveThirdFileAs     =   0x00000020,
        Cmd_Print               =   0x00000040,
        Cmd_PrintPreview        =   0x00000080,
        Cmd_Undo                =   0x00000100,
        Cmd_Redo                =   0x00000200,
        Cmd_Cut                 =   0x00000400,
        Cmd_Copy                =   0x00000800,
        Cmd_Paste               =   0x00001000,
        Cmd_SelectAll           =   0x00002000,
        Cmd_Recompare           =   0x00004000,
        Cmd_SetFile             =   0x00006000
    };

    class Application : public wxApp
    {
    public:
        /*  Commands
         *      Each of these static functions represnt specific high-level
         *      commands that can be executed in MojoMerge.  They can be
         *      called, for example, from a menu event or toolbar event.
         *
         *      An exception will occur if the methods are executed during
         *      an invalid state.  For example, 'CmdSaveThirdFile' will
         *      fail if it is called and there is no third file in the
         *      current comparison window.
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void CmdNewTwoWayFileComparison();
        static void CmdNewThreeWayFileComparison();
        static void CmdNewTwoWayFolderComparison();
        static void CmdNewThreeWayFolderComparison();
        static void CmdSaveFirstFile();
        static void CmdSaveSecondFile();
        static void CmdSaveThirdFile();
        static void CmdSaveFirstFileAs();
        static void CmdSaveSecondFileAs();
        static void CmdSaveThirdFileAs();
        static void CmdPrint();
        static void CmdPrintPreview();
        static void CmdUndo();
        static void CmdRedo();
        static void CmdCut();
        static void CmdCopy();
        static void CmdPaste();
        static void CmdSelectAll();
        static void CmdRecompare();
        //static void CmdPreferences();
        static void CmdIgnoreWhitespace(bool Value);
        static void CmdIgnoreLineEndings(bool Value);
        static void CmdIgnoreCase(bool Value);
        static void CmdToolbar();
        static void CmdStatusbar();
        static void CmdExit();
        static void CmdCloseWindow();
        static void CmdSetFile(DiffFileNumber FileNumber);
        static void CmdSetStatusBarMsg(wxString Message);

        /*  UpdateAvailableCommandsForActiveWindow
         *      Requests an update from the active window for a list of
         *      commands to enable and disable.  This is commonly called from
         *      the active window itself to let the application know that
         *      something has changed.
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void UpdateAvailableCommandsForActiveWindow();

        /*  UpdateNameForActiveWindow
         *      Requests an update from the active window for the name.  This
         *      is commonly called by the active window itself to let the
         *      application know that something has changed.
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void UpdateNameForActiveWindow();

        /*  ReadFile
         *      Standard function that reads a file and stores the bytes in a
         *      chararacter buffer
         *  Params
         *      Filename
         *          Full filename and path to read
         *      NullTerminator
         *          Set to true if ReadFile should put a null terminator at the
         *          end of the buffer.
         *      BufferLength
         *          This function writes to this variable if it is not NULL.
         *          The value represents how many bytes are in the buffer.
         *          This does NOT include the terminating null.
         *  Returns
         *      Returns the buffer contained in the file, or returns NULL if
         *      the file could not be read.
         *
         *      The caller is responsible for deallocating the returned char *
         *      when they are finished with it.
         */
        static char *ReadFile(const char *Filename, 
            bool NullTerminator = true, uint32 *BufferLength = NULL);

        /*  GetTempFolder
         *      Returns the folder that should be used for putting temporary
         *      files into.
         *  Params
         *      none
         *  Returns
         *      Location of temp folder including ending platform specific
         *      folder separator
         */
        static const char *GetTempFolder();

        /*  GetMainWindow
         *      Returns a pointer to the main wiondow of the application.
         *  Params
         *      none
         *  Returns
         *      See description
         */
        static MainWindow *GetMainWindow();

        /*  Debug
         *      Prints out the specified debug message if built in debug mode.
         *      Also, appends a newline to the end of the specified message.
         *  Params
         *      See 'printf'
         *  Returns
         *      none
         */
        // TODO - Make this a global function so we can #define it as nothing
        // when not in debug mode.
#ifdef _DEBUG
        static void Debug(char *format, ...);
        static void DebugNoCR(char *format, ...);
#else
        static void Debug(char *format, ...) {};
        static void DebugNoCR(char *format, ...) {};
#endif

        /*  AddTestTab
         *      Adds the specified window to the TabBrowser.  This is purely
         *      used for testing/debugging and is not available in release
         *      builds
         *  Params
         *      NewWindow
         *          Window to add to the browser
         *  Returns
         *      none
         */
#ifdef _DEBUG
        static void AddTestTab(TabWindow *NewWindow);
#endif
    private:
        // Location of temp folder
        static char *TempFolder;
        // Main application window
        static MainWindow *Window;
        // Tab browser object contained in Window
        static TabBrowser *Browser;

        virtual bool OnInit();
        virtual int OnExit();

        /*  GetWindowAsCompareFilesUI
         *      Coerces the specified TabWindow into a CompareFilesUI object
         *      with basic error checking.
         *  Params
         *      Window
         *          Any valid tab window object
         *  Returns
         *      A pointer to a CompareFilesUI object, or NULL if the object
         *      could not be converted.
         */
        static CompareFilesUI *GetWindowAsCompareFilesUI(TabWindow *Window);

        /*  GetWindowAsCompareUI
         *      Coerces the specified TabWindow into a CompareUI object
         *      with basic error checking.
         *  Params
         *      Window
         *          Any valid tab window object
         *  Returns
         *      A pointer to a CompareUI object, or NULL if the object
         *      could not be converted.
         */
        static CompareUI *GetWindowAsCompareUI(TabWindow *Window);
    };

    DECLARE_APP(Application)
}
#endif
