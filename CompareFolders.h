/*  CompareFolders.h - This class examines two or three folders (and optionally
 *  subfolders) and determines how the directory structures and the content
 *  differ.
 */

#ifndef _COMPAREFOLDERS_H
#define _COMPAREFOLDERS_H

#include "Diff.h"
#include "FolderHunk.h"
#include "wx/wx.h"
#include "wx/hashmap.h"

namespace MojoMerge
{
    // Declare a hash datatype of int indexed by string for keeping track of
    //  what folders the files exist in.
    WX_DECLARE_STRING_HASH_MAP(int, ExistsFolderHash);
    #define FILEEXIST_ONE       0x01
    #define FILEEXIST_TWO       0x02
    #define FILEEXIST_THREE     0x04
    #define ISFOLDER            0x08

    enum CompareFolderOptions
    {
        CompareFolderOption_None = 0x0000,
        CompareFolderOption_SubFolders = 0x0001,
        CompareFolderOption_FileDiff = 0x0002
    };

    class CompareFolders
    {
    public:
        /*  CompareFolders
         *      Constructs a CompareFolders object
         *  Params
         *      DiffObject
         *          Pointer to a Diff object to use for file comparisons.
         *  Returns
         *      none
         */
        CompareFolders(Diff *DiffObject);

        /*  Execute
         *      Starts a folder comparison on two or three folders
         *  Params
         *      Options
         *          One or more folder comparison options may be specified
         *      FileOptions
         *          Options to pass to the Diff object
         *      FolderX
         *          Path of folders to compare.  Folder1 and Folder2 are
         *          required parameters.  Folder3 may be NULL.
         *  Returns
         *      Pointer to first FolderHunk in list.  Caller is responsible for
         *      deleting the FolderHunk list.
         */
        FolderHunk *Execute(CompareFolderOptions Options,
            DiffOptions FileOptions, const char *Folder1, const char *Folder2,
            const char *Folder3 = NULL);
    private:
        /*  CreateChildren
         *      Creates the child files/folder of the specified CurPath
         *  Params
         *      Prev
         *          Previous item which is a sibling of this new item
         *      CurIndex
         *          Current index in the array of items
         *      CurPath
         *          Path of current folder
         */
        FolderHunk *CreateChildren(FolderHunk *Prev, int &CurIndex,
            wxString CurPath);

        // Array of files for each directory
        wxArrayString MyFiles;
        ExistsFolderHash MyHash;
        // Diff object to performs diffs with
        Diff *DiffObject;
    };
}

#endif