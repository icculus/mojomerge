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
    enum MergeChangeType
    {
        MergeChangeType_None = 0,
        MergeChangeType_Insert,
        MergeChangeType_Delete,
        MergeChangeType_Replace
    };

    struct MergeChange
    {
        DiffFileNumber FileNumber;  // File that changed
        MergeChangeType Type;       // Type of change
        uint32 Start;               // Starting line of change
        uint32 Length;              // Length of change
        uint32 NewLength;           // New length of change if "Replace" type
        //LineBuffer Buffer;          // Buffer that represents the change
    };

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
        FileMergeTransaction(Hunk *OriginalHunk, LineBuffer *SourceBuffer, 
            LineBuffer *DestBuffer, DiffFileNumber SourceFileNumber,
            DiffFileNumber DestFileNumber);
        
        /*  ~FileMergeTransaction
         *      Destructs the FileMergeTransaction object
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~FileMergeTransaction();

        /*  GetLastChange
         *      Returns information on what file changed based on the
         *      transaction.  This value may change when executing an
         *      "undo" and "redo".  Typically this value is used for
         *      displaying the results of a transaction visually.
         *  Params
         *      none
         *  Returns
         *      Change that happened as a result of the transaction
         */
        MergeChange *GetLastChange();

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
        /*  Do
         *      Performs the transaction for the first time.  This is called
         *      on construction of the FileMergeTransaction object.
         *  Parameters
         *      none
         *  Returns
         *      none
         */
        void Do();

        // Original hunk that existed before resolving it
        Hunk *OriginalHunk;
        // Hunk that represents a partially resolved hunk (or NULL)
        Hunk *NewHunk;
        // This points to either OriginalHunk or NewHunk.  It is based on what
        //  hunk actually exists in the hunk list (whether we've done an Undo
        //  or a Redo).  This object gets deleted when the transaction is
        //  deleted.
        Hunk *HunkToDelete;
        // Buffer where change will come from
        LineBuffer *SourceBuffer;
        // Buffer where change will be written to
        LineBuffer *DestBuffer;
        // Source and destination file numbers
        DiffFileNumber SourceFileNumber;
        DiffFileNumber DestFileNumber;
        // Offset to adjust hunks located after NewHunk.  This offset
        //  represents how hunk lines are affected that are positioned after
        //  the new hunk
        sint32 Offset;
        // What lines and buffers are affected if calling Do or Redo
        MergeChange RedoChange;
        // What lines and buffers are affected if calling Undo
        MergeChange UndoChange;
        // Buffer that is retained that represents the Undo/Redo transaction
        LineBuffer ChangeBuffer;
    };
}

#endif
