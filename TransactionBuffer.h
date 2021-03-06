/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  TransactionBuffer.h - Class for support Undo/Redo functionality for
 *      Transaction objects.
 */

#ifndef _TRANSACTIONBUFFER_H_
#define _TRANSACTIONBUFFER_H_

#include "DataTypes.h"
#include "Transaction.h"
#include "Stack.h"

namespace MojoMerge
{
    class TransactionBuffer
    {
    public:
        /*  TransactionBuffer Constructor
         *  Params
         *      Number of transactions to maintain in undo log
         *  Returns
         *      none
         */
        TransactionBuffer(uint32 Size);

        /*  PushTransaction
         *      Adds a Transaction object to the end of the undo stack.
         *      This will inadvertently clear the redo stack since this is
         *      a new transaction.  Items that are removed from the redo
         *      stack are deallocated as well.
         *  Params
         *      Transaction
         *          UndoTransaction object to add.
         *  Returns
         *      none
         */
        void PushTransaction(Transaction *NewTransaction);

        /*  UndoTransaction
         *      Removes the transaction from the top of the stack.  When
         *      removing the transaction, the transaction's "Undo" method
         *      is called.  Also, the transaction object is added to the
         *      redo stack.
         *  Params
         *      none
         *  Returns
         *      Transaction that was undone
         */
        Transaction *UndoTransaction();

        /*  RedoTransaction
         *      Calls the "Redo" method on the transaction from the top of the
         *      redo stack.  The transaction is added to the undo stack.
         *  Params
         *      none
         *  Returns
         *      Transaction that was redone
         */
        Transaction *RedoTransaction();

        /*  CanUndo
         *      Returns true if the undo stack is not empty
         *  Params
         *      none
         *  Returns
         *      none
         */
        bool CanUndo();

        /*  CanRedo
         *      Returns true if the redo stack is not empty
         *  Params
         *      none
         *  Returns
         *      none
         */
        bool CanRedo();

        /*  Undo Destructor
         *      Removes all of the transactions from the undo and redo stacks,
         *      and deallocates any resources allocates by the Undo object.
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~TransactionBuffer();
    private:
        // Our stacks that we use for undo and redo stuff
        Stack<Transaction> *UndoBuffer;
        Stack<Transaction> *RedoBuffer;
    };
}
#endif
