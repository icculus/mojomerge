/*  Hunk.cpp - Implementation for the Hunk class
 *
 */

#include "Hunk.h"
#include <assert.h>
#include <stdlib.h>

using namespace MojoMerge;

Hunk::Hunk(Hunk *Prev, uint32 Start1, uint32 End1, uint32 Start2, uint32 End2,
    uint32 Start3, uint32 End3, DiffFileNumber FileThatIsDifferent)
{
    // Initialize default values for member variables
    this->Prev = Prev;
    LinkHead = false;
    // If previous is a valid object
    if(Prev)
        // Set the previous object's next item to this item
        Prev->Next = this;
    // Initialize the next object to null for now
    Next = NULL;
    // Check the validity of the hunks
    assert(CheckValidity());
    // Set start and end of each hunk
    Start[DiffFile_One] = Start1;
    Start[DiffFile_Two] = Start2;
    Start[DiffFile_Three] = Start3;
    End[DiffFile_One] = End1;
    End[DiffFile_Two] = End2;
    End[DiffFile_Three] = End3;
    // Save the file that is different
    this->FileThatIsDifferent = FileThatIsDifferent;
}

Hunk::Hunk(Hunk *FirstHunk)
{
    // First hunk can't be NULL
    //assert(FirstHunk);
    // Set flag that designates it as a link head
    LinkHead = true;
    Prev = NULL;
    Next = FirstHunk;
    if(FirstHunk)
        FirstHunk->Prev = this;
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
    // This function fails if item is a linked list head
    assert(!LinkHead);
    // Return the starting line number of specified file
    return Start[FileNumber];
}

uint32 Hunk::GetEnd(DiffFileNumber FileNumber)
{
    // This function fails if item is a linked list head
    assert(!LinkHead);
    // Return the ending line number of specified file
    return End[FileNumber];
}

DiffFileNumber Hunk::GetDiffFile()
{
    // This function fails if item is a linked list head
    assert(!LinkHead);
    // Return the file that is different, if applicable
    return FileThatIsDifferent;
}

Hunk *Hunk::GetPrevHunk()
{
    // This function fails if item is a linked list head
    assert(!LinkHead);
    return Prev;
}

Hunk *Hunk::GetNextHunk()
{
    return Next;
}

bool Hunk::CheckValidity()
{
    // This function fails if item is a linked list head
    assert(!LinkHead);
    uint32 i;

    // TODO - Add more checks for Hunk validity
    for(i = DiffFile_One; i <= DiffFile_Three; i++)
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

void Hunk::Replace(Hunk *NewHunk, DiffFileNumber ChangedFile, sint32 Offset)
{
    // This function fails if item is a linked list head
    assert(!LinkHead);
    // Temporary hunk pointer
    Hunk *Temp = Next;

    // If NewHunk isn't NULL
    if(NewHunk)
    {
        // Do actual replacement in the linked list
        NewHunk->Prev = Prev;
        NewHunk->Next = Next;
        if(Prev)
            Prev->Next = NewHunk;
        if(Next)
            Next->Prev = NewHunk;
    }
    // Else, hunk is NULL (just remove it)
    else
    {
        if(Prev)
            Prev->Next = Next;
        if(Next)
            Next->Prev = Prev;
    }

    // Reset values on this hunk
    //Prev = NULL;
    //Next = NULL;

    // Apply offset to all hunks after the current one
    if(Offset != 0)
    {
        // File must be specified for offset
        assert(ChangedFile != DiffFile_Unspecified);
        // Loop through all the hunks after the new one we inserted,
        while(Temp)
        {
            // Apply offset to changed file part of hunk
            Temp->Start[ChangedFile] += Offset;
            // If End of the hunk is not unspecified, then apply the offset
            //  to the end as well.
            if(Temp->End[ChangedFile] != UNSPECIFIED)
                Temp->End[ChangedFile] += Offset;

            // Get next hunk
            Temp = Temp->Next;
        }
    }
}

void Hunk::Revert(DiffFileNumber ChangedFile, sint32 Offset)
{
    // This function fails if item is a linked list head
    assert(!LinkHead);
    // Temporary hunk pointer
    Hunk *Temp = Next;

    // Put back the original hunk in the list
    if(Prev)
        Prev->Next = this;
    if(Next)
        Next->Prev = this;

    // Apply offset to all hunks after the current one
    if(Offset != 0)
    {
        // File must be specified for offset
        assert(ChangedFile != DiffFile_Unspecified);
        // Loop through all the hunks after the new one we inserted,
        while(Temp)
        {
            // Apply offset to changed file part of hunk
            Temp->Start[ChangedFile] += Offset;
            // If End of the hunk is not unspecified, then apply the offset
            //  to the end as well.
            if(Temp->End[ChangedFile] != UNSPECIFIED)
                Temp->End[ChangedFile] += Offset;

            // Get next hunk
            Temp = Temp->Next;
        }
    }
}