/*  FileMergeTransaction.h - A transaction specific to file merging
 *
 */

#ifndef _FILEMERGETRANSACTION_H_
#define _FILEMERGETRANSACTION_H_

#include "Transaction.h"
#include "DataTypes.h"
#include "LineBuffer.h"
#include "Hunk.h"

namespace MojoMerge
{
    class FileMergeTransaction : public Transaction
    {
    public:
        /*  FileMergeTransaction
         *      Constructs a new FileMergeTransaction object
         *  Params
         *      OriginalHunk
         *          This is the hunk that existed before it was resolved or
         *          partially resolved.
         *      SourceBuffer
         *          Source file buffer
         *      DestBuffer
         *          Destination file buffer
         *      SourceFileNumber
         *          File number that the change will come from
         *      DestFileNumber
         *          File number that the change will be written to.  This file
         *          number must be different than SourceFileNumber.
         *  Returns
         *      none
         */
        // TODO - Source and dest buffer?
        FileMergeTransaction(Hunk *OriginalHunk, LineBuffer *SourceBuffer, 
            LineBuffer *DestBuffer, DiffFileNumber SourceFileNumber,
            DiffFileNumber DestFileNumber);

        /***Overridden Methods***/
        /*  Undo
         *      See Transaction.h for more information.
         */
        virtual void Undo();

        /*  Redo
         *      See Transaction.h for more information.
         */
        virtual void Redo();

        
    private:
        void Do();

        // Original hunk that existed before resolving it
        Hunk *OriginalHunk;
        // Hunk that represents a partially resolved hunk (or NULL)
        Hunk *NewHunk;
        // Buffer where change will come from
        LineBuffer *SourceBuffer;
        // Buffer where change will be written to
        LineBuffer *DestBuffer;
        // Original buffer that was in the Dest file's hunk
        LineBuffer OriginalDestHunkBuffer;
        // Source and destination file numbers
        DiffFileNumber SourceFileNumber;
        DiffFileNumber DestFileNumber;
    };
}

#endif