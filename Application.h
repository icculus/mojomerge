/*  Application.h - Main application class (program starts here)
 *
 */

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "wx/wx.h"
#include "TabBrowser.h"
#include "CompareFilesUI.h"
#include "MainWindow.h"

namespace MojoMerge
{
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
        static void CmdNewComparison();
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
        static void CmdCompareFiles();
        static void CmdCompareFolders();
        static void CmdTwoWayComparison();
        static void CmdThreeWayComparison();
        static void CmdRecompare();
        static void CmdPreferences();
        static void CmdToolbar();
        static void CmdStatusbar();
        static void CmdExit();

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

        /*  CreateDefaultComparisonWindow
         *      Creates a comparison window based on configured default settings
         *  Params
         *      none
         *  Returns
         *      A default comparison window object.
         */
        static CompareUI *CreateDefaultComparisonWindow();

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