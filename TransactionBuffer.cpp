/*  TransactionBuffer.cpp - Implementation of TransactionBuffer class
 *
 */

#include "TransactionBuffer.h"
#include <stdlib.h>
#include <assert.h>

using namespace MojoMerge;

TransactionBuffer::TransactionBuffer(uint32 Size)
{
    UndoBuffer = new Stack<Transaction>(Size);
    RedoBuffer = new Stack<Transaction>(Size);
}

void TransactionBuffer::PushTransaction(Transaction *NewTransaction)
{
    // Add the transaction to our undo stack
    UndoBuffer->Push(NewTransaction);
    // Clear the redo stack since those transactions can no longer be redone
    //  since we've added a brand new transaction.  This also deallocates all
    //  of the transactions.
    RedoBuffer->Clear();
}

Transaction *TransactionBuffer::UndoTransaction()
{
    Transaction *TempTransaction = NULL;

    // Take the transaction off the Undo buffer.
    TempTransaction = UndoBuffer->Pop();
    // Can't undo if nothing is in the buffer
    assert(TempTransaction);
    // Undo the transaction
    TempTransaction->Undo();
    // Add the transaction to the Redo buffer
    RedoBuffer->Push(TempTransaction);

    return TempTransaction;
}

Transaction *TransactionBuffer::RedoTransaction()
{
    Transaction *TempTransaction = NULL;

    // Take the transaction off the Redo buffer.
    TempTransaction = RedoBuffer->Pop();
    // Can't undo if nothing is in the buffer
    assert(TempTransaction);
    // Redo the transaction
    TempTransaction->Redo();
    // Add the transaction to the Undo buffer
    UndoBuffer->Push(TempTransaction);

    return TempTransaction;
}

bool TransactionBuffer::CanUndo()
{
    return !UndoBuffer->IsEmpty();
}

bool TransactionBuffer::CanRedo()
{
    return !RedoBuffer->IsEmpty();
}

TransactionBuffer::~TransactionBuffer()
{
    delete UndoBuffer;
    delete RedoBuffer;
}