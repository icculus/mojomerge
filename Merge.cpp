/*  Merge.cpp - Implementation of the Merge class
 *
 */

#include "Merge.h"
#include "FileMergeTransaction.h"
#include "Config.h"
#include <assert.h>

using namespace MojoMerge;

Merge::Merge(Hunk *FirstHunk, const char *Buffer1, const char *Buffer2,
    const char *Buffer3)
{
    // Initialize members
    // First hunk is a dummy hunk list head
    this->FirstHunk = NULL;
    this->Buffer[DiffFile_One] = NULL;
    this->Buffer[DiffFile_Two] = NULL;
    this->Buffer[DiffFile_Three]= NULL;
    UndoBuffer = NULL;

    // The FirstHunk can't be NULL
    assert(FirstHunk);
    // The first two buffers can't be NULL either
    assert(Buffer1);
    assert(Buffer2);
    // Create linked list head
    this->FirstHunk = new Hunk(FirstHunk);
    // Save the buffer references
    this->Buffer[DiffFile_One] = new LineBuffer(Buffer1);
    this->Buffer[DiffFile_Two] = new LineBuffer(Buffer2);
    if(Buffer3)
        this->Buffer[DiffFile_Three] = new LineBuffer(Buffer3);
    // Create the undo buffer (get size from config settings)
    UndoBuffer = new TransactionBuffer(Config::GetUndoSize());
    // UndoBuffer can't be NULL
    assert(UndoBuffer);
}

FileMergeTransaction *Merge::ResolveDiff(Hunk *MergeHunk,
    DiffFileNumber SourceFileNumber, DiffFileNumber DestFileNumber)
{
    // Create new merge transaction that will perform the resolve.  This also
    //  executes the transaction
    FileMergeTransaction *NewTransaction = new FileMergeTransaction(MergeHunk,
        Buffer[SourceFileNumber], Buffer[DestFileNumber], SourceFileNumber,
        DestFileNumber);
    // Add the transaction to the undo buffer
    UndoBuffer->PushTransaction(NewTransaction);
    // Return the new transaction object
    return NewTransaction;
}

LineBuffer *Merge::GetBuffer(DiffFileNumber FileNumber)
{
    return Buffer[FileNumber];
}

Hunk *Merge::GetFirstHunk()
{
    // First hunk is always a dummy "list head"
    return FirstHunk->GetNextHunk();
}

Merge::~Merge()
{
    // Deallocate the objects we created
    delete UndoBuffer;
    delete Buffer[DiffFile_One];
    delete Buffer[DiffFile_Two];
    delete Buffer[DiffFile_Three];
    FirstHunk->DeleteList();
}