/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  Transaction.h - Abstract object tha represents a user action.  This class
 *      must be overridden in order to provide a specific transaction
 *      implementation for specific types of actions.
 */

#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

namespace MojoMerge
{
    class Transaction
    {
    public:
        /*  Undo
         *      Reverts a transaction as though it never took place.  The
         *      derived class must override this function.
         *  Params
         *      none
         *  Returns
         *      none
         */
        virtual void Undo() = 0;

        /*  Redo
         *      Replays a transaction that has been previous undone.  The
         *      derived class must override this function.  Typically this
         *      would be called by the Undo class only.
         *  Params
         *      none
         *  Returns
         *      none
         */
        virtual void Redo() = 0;
    };
}

#endif
