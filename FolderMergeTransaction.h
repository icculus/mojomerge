/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  FileMergeTransaction.h - A transaction specific to file merging
 *
 */

#ifndef _FOLDERMERGETRANSACTION_H_
#define _FOLDERMERGETRANSACTION_H_

#include "Transaction.h"
#include "DataTypes.h"
#include "FolderHunk.h"

namespace MojoMerge
{
    class FolderMergeTransaction : public Transaction
    {
    public:
        /*  FolderMergeTransaction
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
        FolderMergeTransaction(FolderHunk *OriginalHunk, const char *SourceFolder, 
            const char *DestFolder, DiffFileNumber SourceFolderNumber,
            DiffFileNumber DestFolderNumber);
        
        /*  ~FileMergeTransaction
         *      Destructs the FileMergeTransaction object
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~FolderMergeTransaction();

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
    };
}

#endif
