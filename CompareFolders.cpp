#include "CompareFolders.h"
#include <assert.h>

using namespace MojoMerge;

CompareFolders::CompareFolders(Diff *DiffObject)
{
    // Diff object can't be NULL
    assert(DiffObject);
    this->DiffObject = DiffObject;
}

FolderHunk *CompareFolders::Execute(CompareFolderOptions Options,
    DiffOptions FileOptions, const char *Folder1, const char *Folder2,
    const char *Folder3)
{
    //FolderHunk *LastHunk = NULL;
    //FolderHunk *CurrentHunk = NULL;

    //wxArrayString MyFiles1;
    //wxArrayString MyFiles2;
    //wxArrayString MyFiles3;

    /*
    Each line represents a hunk
    a    a    
    b    b
    c          c
    d    d
         e
    */
    // This is what you would do for line 1
    //Hunk *FileDiffs = DiffObject->CompareFiles(FileOptions, buffer1, buffer2, buffer3);
    //CurrentHunk = new FolderHunk(LastHunk, "a", false, FileDiffs, NULL, true, true, false);

    //wxDir::GetAllFiles(Folder1, &MyFiles, const wxString& filespec = wxEmptyString, int flags = wxDIR_DEFAULT)
    // Help for wxWindows classes - http://www.wxwindows.org/manuals/2.4.0/wx.htm
    // TODO - GCR needs to fill this in
    assert(0);
    return NULL;
}
