/*  Hunk.cpp - Implementation for the Hunk class
 *
 */

#include "Hunk.h"
#include <assert.h>
#include <stdlib.h>

using namespace MojoMerge;

Hunk::Hunk(Hunk *Prev, uint32 Start1, uint32 End1, uint32 Start2, uint32 End2,
    uint32 Start3, uint32 End3)
{
    // Initialize default values for member variables
    this->Prev = Prev;
    // If previous is a valid object
    if(Prev)
        // Set the previous object's next item to this item
        Prev->Next = this;
    // Initialize the next object to null for now
    Next = NULL;
    // Check the validity of the hunks
    assert(CheckValidity());
    // Set start and end of each hunk
    Start[0] = Start1;
    Start[1] = Start2;
    Start[2] = Start3;
    End[0] = End1;
    End[1] = End2;
    End[2] = End3;
}

Hunk::~Hunk()
{
    // If hunk is deleted, remove it from the linked list while
    //  maintaining proper linkage.
    if(Prev)
        Prev->Next = Next;
    if(Next)
        Next->Prev = Prev;
}

uint32 Hunk::GetStart(DiffFileNumber FileNumber)
{
    // Return the starting line number of specified file
    return Start[FileNumber - 1];
}

uint32 Hunk::GetEnd(DiffFileNumber FileNumber)
{
    // Return the ending line number of specified file
    return End[FileNumber - 1];
}

Hunk *Hunk::GetPrevHunk()
{
    return Prev;
}

Hunk *Hunk::GetNextHunk()
{
    return Next;
}

bool Hunk::CheckValidity()
{
    uint32 i;

    // TODO - Add more checks for Hunk validity
    for(i = 0; i < 3; i++)
    {
        // UNSPECIFIED is not valid for 1st and 2nd files
        if(i != DiffFile_Three && Start[i] == UNSPECIFIED)
            return false;
    }

    return true;
}

void Hunk::DeleteList()
{
    // Delete next hunk if applicable
    if(this->Next)
        this->Next->DeleteList();
    // Delete this hunk...duh.
    delete this;
}