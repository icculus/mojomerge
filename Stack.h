/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  Stack.h - Template for creating a Stack object with any type
 */

#ifndef _STACK_H_
#define _STACK_H_

#include "DataTypes.h"
#include <stdlib.h>
#include <assert.h>

namespace MojoMerge
{
    template <class T>
    class Stack
    {
    public:
        /*  Stack
         *      Constructs a new Stack object
         *  Params
         *      Size
         *          Number of items allowed in stack.  If item count exceeds
         *          this value, items will be taken off the bottom of the stack
         *          to maintain appropriate size.
         *  Returns
         *      none
         */
        Stack(uint32 Size);

        /*  Stack destructor
         *      Clears the stack before deleting the Stack object
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~Stack() { Clear(); }

        /*  Push
         *      Adds an item to the top of the stack.  If stack is full, the
         *      bottom item is removed and also deleted from memory
         *  Params
         *      Item
         *          Item to be pushed on the stack
         *  Returns
         *      none
         */
        void Push(T *Item);

        /*  Pop
         *      Removes an item from the top of the stack
         *  Params
         *      none
         *  Returns
         *      Item that was removed
         */
        T *Pop();

        /*  Clear
         *      Clears the entire stack and removes all objects contained in
         *      the stack from memory.
         *  Params
         *      none
         *  Returns
         *      none
         */
        void Clear();

        /*  IsEmpty
         *      Returns true if stack is empty
         *  Params
         *      none
         *  Returns
         *      See description
         */
        bool IsEmpty();

    private:
        // This is the data type used with the Stack object to maintain the linked
        //  list between each stack item.
        struct StackItem
        {
            T *Item;
            StackItem *Prev;
            StackItem *Next;
        };
        // Item at top of stack
        StackItem *Top;
        // Item at bottom of stack
        StackItem *Bottom;
        // Number of items in the linked list
        uint32 Count;
        // Size of stack
        uint32 Size;
    };

    template <class T>
    Stack<T>::Stack(uint32 Size)
    {
        // Size has to be greater than 0 or there's no point of having a stack
        assert(Size > 0);
        // Initialize member variables
        Top = NULL;
        Bottom = NULL;
        Count = 0;
        // Set our maximum size based on the passed argument
        this->Size = Size;
    }

    template <class T>
    void Stack<T>::Push(T *Item)
    {
        StackItem *TempItem;        // Temp item reference

        // Item can't be NULL
        assert(Item);
        // Create new item (setting default values for the item)
        TempItem = new StackItem;
        TempItem->Item = NULL;
        TempItem->Prev = NULL;
        TempItem->Next = NULL;
        // Set new items "item" :-)
        TempItem->Item = Item;
        // If no items have been added yet, set the new item to the bottom
        if(Count == 0)
            Bottom = TempItem;
        // Else, atleast one item already exists
        else
        {
            // Add new item to the stack
            Top->Next = TempItem;
            TempItem->Prev = Top;
        }
        // Set the new top to be the new item
        Top = TempItem;
        // We added an item, increment the count
        Count++;
        // If we've exceeded the stack size
        if(Count > Size)
        {
            // Retain our original bottom
            TempItem = Bottom;
            // Set our new bottom to be the next one up
            Bottom = TempItem->Next;
            Bottom->Prev = NULL;
            // Decrement one from the count since we removed an item
            Count--;
            // Remove the bottom object from memory
            delete TempItem->Item;
            delete TempItem;
        }
    }

    template <class T>
    T *Stack<T>::Pop()
    {
        T *ReturnValue;         // Item to return
        StackItem *TempItem;    // Temp StackItem object

        // Save the current top object
        TempItem = Top;
        // If top is a valid object, set the previous object as our new top
        if(Top)
        {
            // Set our top the the one before the current top
            Top = TempItem->Prev;
            // Take the value from the top
            ReturnValue = TempItem->Item;
            // Delete the old top StackItem object
            delete TempItem;
            // Decrement our counter
            Count--;
        }
        else
            ReturnValue = NULL;

        // Return the object contained in the stack item
        return ReturnValue;
    }

    template <class T>
    void Stack<T>::Clear()
    {
        T *Item;        // Current item popped off stack

        // While there's stuff in the stack, pop items and delete them
        while((Item = Pop()))
            delete Item;
    }

    template <class T>
    bool Stack<T>::IsEmpty()
    {
        return(Count == 0);
    }
}

#endif
