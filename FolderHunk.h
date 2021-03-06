/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  FolderHunk.h - Class that represents a block of files and folders
 *  resulting from a folder comparison.  Unlike the "Hunk" object, this object
 *  contains everything in the comparison, not just differences.
 */

#ifndef _FOLDERHUNK_H_
#define _FOLDERHUNK_H_

#include "Hunk.h"
#include "Diff.h"

namespace MojoMerge
{
    class FolderHunk
    {
    public:
        /*  FolderHunk
         *      Constructs a FolderHunk object.
         *  Params
         *      Prev
         *          Pointer to the previous hunk in the list.  This value may
         *          be NULL if the new FolderHunk is the first in the list.
         *      Path
         *          Path of file or folder being represented.  It must not 
         *          an ending path separator.
         *      BarePath
         *          This is just the name of the node (not including the rest
         *          of the path).
         *      IsFolder
         *          This value should be true if the specified path represents
         *          a folder.
         *      FileHunk
         *          This represents a file hunk of differences encountered in
         *          the file between each folder being compared.  This value
         *          may be NULL if no file diffs were encountered, or if a
         *          file diff was not performed.
         *      FirstChild
         *          If IsFolder is true, this value can be a pointer to the
         *          first file or folder contained inside the current folder.
         *      ExistsFolderX
         *          Set to true if the Path exists in the respective folder
         *          represented by X.  ExistsFolder3 is optional if performing
         *          a two-way folder comparison.
         *  Returns
         *      none
         */
        FolderHunk(FolderHunk *Prev, const char *Path, const char *BarePath,
            bool IsFolder, Hunk *FileHunk, FolderHunk *FirstChild,
            bool ExistsFolder1, bool ExistsFolder2,
            bool ExistsFolder3 = false);

        FolderHunk(FolderHunk *FirstHunk);

        /*  GetExists
         *      Returns true if the path specified in the constructor exists
         *      in the specified folder
         *  Params
         *      FolderNumber
         *          Folder to determine if the path exists in
         *  Returns
         *      True if path exists, false if it does not.
         */
        bool GetExists(DiffFileNumber FolderNumber);

        /*  GetFirstChild
         *      Returns the first hunk contained inside this folder.
         *  Params
         *      none
         *  Returns
         *      See description.  May return NULL if folder is empty, or if
         *      the GetIsFolder method returns false.
         */
        FolderHunk *GetFirstChild();

        /*  GetPrevious
         *      Returns the hunk located just before this one.
         *  Params
         *      none
         *  Returns
         *      See description.  May return NULL if this is the first item.
         */
        FolderHunk *GetPrevious();
        
        /*  GetNext
         *      Returns the hunk located just after this one.
         *  Params
         *     none
         *  Returns
         *      See description.  May return NULL if this is the last item.
         */
        FolderHunk *GetNext();

        /*  GetPath
         *      Returns the value of path
         *  Params
         *      none
         *  Returns
         *      See description
         */
        const char *GetPath();

        /*  GetBarePath
         *      Returns the value of bare path
         *  Params
         *      none
         *  Returns
         *      See description
         */
        const char *GetBarePath();

        /*  GetIsFolder
         *      Returns true if hunk represents a folder
         *  Params
         *      none
         *  Returns
         *      See description
         */
        bool GetIsFolder();

        /*  DeleteList
         *      Deallocates this hunk, and all hunks after it
         *  Params
         *      none
         *  Returns
         *      none
         */
        void DeleteList();
    private:
        // True if linkhead (FolderHunk that is not used as an actual hunk)
        bool LinkHead;
        // Path represented by this hunk
        char Path[MOJO_MAX_PATH];
        // Just the name of the item (no path included)
        char BarePath[MOJO_MAX_PATH];
        // True if this hunk represents a folder
        bool IsFolder;
        // Value that indicates if the path exists in the specified folder
        bool Exists[MAX_DIFF_FILES];
        // Hunks that represent internal file differences.  Can be NULL if no
        //  diffs were encountered, or file comparison was not done.
        //Hunk *FileHunk;
        // Hunk that is contained inside this hunk.  This is only valid if
        //  the IsFolder value is true.
        FolderHunk *FirstChild;
        // Previous folder or file in the list
        FolderHunk *Prev;
        // Next folder or file in the list
        FolderHunk *Next;
    };
}

#endif
