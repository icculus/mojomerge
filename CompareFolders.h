/*  CompareFolders.h - This class examines two or three folders (and optionally
 *  subfolders) and determines how the directory structures and the content
 *  differ.
 */

#ifndef _COMPAREFOLDERS_H
#define _COMPAREFOLDERS_H

#include "Diff.h"
#include "FolderHunk.h"

namespace MojoMerge
{
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
        Diff *DiffObject;
    };
}

#endif