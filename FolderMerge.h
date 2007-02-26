/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  FolderMerge.h - Class for making merge transactions from FolderHunks
 *
 */
#ifndef _FOLDERMERGE_H_
#define _FOLDERMERGE_H_

#include "TransactionBuffer.h"
#include "FolderHunk.h"
#include "FolderMergeTransaction.h"
#include <stdlib.h>

namespace MojoMerge
{
    class FolderMerge
    {
    public:
        /*  FolderMerge Constructor
         *  Params
         *      FirstHunk
         *          Reference to the first FolderHunk
         *      Folder1
         *          First folder to compare
         *      Folder2
         *          Second folder to compare
         *      Folder3
         *          Third folder to compare.  This parameter can be NULL if
         *          performing a two-way merge.
         *  Returns
         *      none
         */
        FolderMerge(FolderHunk *FirstHunk, const char *Folder1, const char *Folder2,
            const char *Folder3 = NULL);

        /*  ResolveDiff
         *      Resolves a difference contained in the specified hunk.  This
         *      function can be read as "make DestFolder look like
         *      SourceFolder for the given hunk".
         *      
         *      The passed hunk object is deleted.  A new one is created
         *      to represent the new transaction.  If the files in all
         *      three are deleted, the hunk is deleted.
         *      
         *      IMPORTANT: Since the passed hunk object is no longer in the
         *      list (added to the Undo object), it should not be referenced.
         *
         *      Also, this function assumes that the hunk is part of the
         *      list associated with this Merge class.  Results are undefined
         *      if it is not.
         *  Params
         *      MergeHunk
         *          Hunk that is used to reference the differences that
         *          will be resolved.
         *      SourceFolder
         *          Folder number that the change will come from
         *      DestFolder
         *          Folder number that the change will be written to.  This
         *          number must be different than SourceFolder.
         *  Returns
         *      Returns a pointer to the new transaction object that was
         *      created when doing the resolution.
         */
        FolderMergeTransaction *ResolveDiff(FolderHunk *MergeHunk,
            DiffFileNumber SourceFolder, DiffFileNumber DestFolder);

        /*  Undo
         *      Calls the Undo::Undo method, returning the result of the Undo
         *      call as a FileMergeTransaction
         *  Params
         *      none
         *  Returns
         *      Return value of Undo::Undo (casted to a FolderMergeTransaction)
         */
        FolderMergeTransaction *Undo();

        /*  Redo
         *      Calls the Undo::Redo method, returning the result of the Undo
         *      call as a FileMergeTransaction
         *  Params
         *      none
         *  Returns
         *      Return value of Undo::Undo (casted to a FolderMergeTransaction)
         */
        FolderMergeTransaction *Redo();

        /*  GetFirstHunk
         *      Get first hunk in list.  This value may change at any time so
         *      code should not save the result of this value.
         *  Params
         *      none
         *  Returns
         *      First hunk in list.
         */
        FolderHunk *GetFirstHunk();

        /*  FolderMerge Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~FolderMerge();
    private:
        // First hunk in linked list of changes.
        FolderHunk *FirstHunk;
        // Folders involved in the merge
        char Folder[MAX_DIFF_FILES][MOJO_MAX_PATH];
        // Log for undoing merges
        TransactionBuffer *UndoBuffer;
    };
}
#endif
