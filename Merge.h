/*  Merge.h - Class for making merge transactions from hunks taken
 *  from a Diff class.  This class is responsible for doing any and all
 *  merges and manual edit changes to the buffer data for each file.
 *
 */
#ifndef _MERGE_H_
#define _MERGE_H_

#include "Hunk.h"
#include "LineBuffer.h"
#include "FileMergeTransaction.h"
#include "TransactionBuffer.h"
#include <stdlib.h>

namespace MojoMerge
{
    class Merge
    {
    public:
        /*  Merge Constructor
         *  Params
         *      FirstHunk
         *          Reference to the first Hunk of a Diff comparison
         *      Buffer1
         *          Contents of file 1
         *      Buffer2
         *          Contents of file 2
         *      Buffer3
         *          Contents of file 3.  This parameter can be NULL if
         *          performing a two-way merge.
         *  Returns
         *      none
         */
        Merge(Hunk *FirstHunk, const char *Buffer1, const char *Buffer2,
            const char *Buffer3 = NULL);

        /*  ResolveDiff
         *      Resolves a difference contained in the specified hunk.  This
         *      function can be read as "make DestFileNumber look like
         *      SourceFileNumber".
         *      
         *      The passed hunk object is deleted.  If the hunk was only
         *      partially resolved (as may be the case in a three-way
         *      hunk), then a new Hunk object is created and placed in
         *      the linked list in the same position as the passed hunk.
         *
         *      IMPORTANT: Since the passed hunk object is no longer in the
         *      list (added to the Undo object), it should not be referenced.
         *
         *      Also, this function assumes that the hunk is part of the
         *      list associated with this Merge class.  Results are undefined
         *      if it is not.
         *  Params
         *      MergeHunk
         *          Diff hunk that is used to reference the differences that
         *          will be resolved.
         *      SourceFileNumber
         *          File number that the change will come from
         *      DestFileNumber
         *          File number that the change will be written to.  This file
         *          number must be different than SourceFileNumber.
         *  Returns
         *      Returns a pointer to new hunk, or NULL if the hunk was
         *      completely resolved.
         */
        Hunk *ResolveDiff(Hunk *MergeHunk, DiffFileNumber SourceFileNumber,
            DiffFileNumber DestFileNumber);

        /*  Undo
         *      Calls the Undo::Undo method, returning the result of the Undo
         *      call as a FileMergeTransaction
         *  Params
         *      none
         *  Returns
         *      Return value of Undo::Undo (casted to a FileMergeTransaction)
         */
        FileMergeTransaction *Undo();

        /*  Redo
         *      Calls the Undo::Redo method, returning the result of the Undo
         *      call as a FileMergeTransaction
         *  Params
         *      none
         *  Returns
         *      Return value of Undo::Undo (casted to a FileMergeTransaction)
         */
        FileMergeTransaction *Redo();

        /*  Merge Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~Merge();
    private:
        // First hunk in linked list of changes.
        Hunk *FirstHunk;
        // Buffers for keeping track of merge changes for each file
        LineBuffer *Buffer[MAX_DIFF_FILES];
        // Log for undoing merges
        TransactionBuffer *UndoBuffer;
    };
}
#endif