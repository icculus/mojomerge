/*  FileMergeTransaction - Implementation of the FileMergeTransaction class.
 *
 */

#include "FileMergeTransaction.h"
#include <assert.h>

using namespace MojoMerge;

FileMergeTransaction::FileMergeTransaction(Hunk *OriginalHunk,
    LineBuffer *SourceBuffer, LineBuffer *DestBuffer,
    DiffFileNumber SourceFileNumber, DiffFileNumber DestFileNumber)
{
    // Initialize members
    this->OriginalHunk = NULL;
    NewHunk = NULL;
    OriginalDestHunkBuffer = NULL;
    this->SourceFileNumber = DiffFile_Unspecified;
    this->DestFileNumber = DiffFile_Unspecified;
    memset(&LastChange, 0, sizeof(MergeChange));

    // These values can't be NULL
    assert(OriginalHunk);
    assert(SourceBuffer);
    assert(DestBuffer);
    // Check ranges of source and dest file numbers
    assert(SourceFileNumber >= DiffFile_One &&
        SourceFileNumber <= DiffFile_Three);
    assert(DestFileNumber >= DiffFile_One &&
        DestFileNumber <= DiffFile_Three);
    // Source and Destination can't be equal
    assert(SourceFileNumber != DestFileNumber);

    // Save the passed values for use later
    this->OriginalHunk = OriginalHunk;
    this->SourceBuffer = SourceBuffer;
    this->DestBuffer = DestBuffer;
    this->SourceFileNumber = SourceFileNumber;
    this->DestFileNumber = DestFileNumber;
    LastChange.FileNumber = DestFileNumber;

    // Do the initial transaction
    Do();
}

FileMergeTransaction::~FileMergeTransaction()
{
    // TODO - Delete OriginalHunk or NewHunk based on transaction status upon deletion
    // For now, just delete the original hunk since we haven't implemented
    //  undo support
    delete OriginalHunk;
}

void FileMergeTransaction::Undo()
{
    // Not implemented yet
    assert(0);
}

void FileMergeTransaction::Redo()
{
    // Not implemented yet
    assert(0);
}

void FileMergeTransaction::Do()
{
    // Generic counter
    int i;
    // Data taken from Hunk object
    uint32 Start[MAX_DIFF_FILES], End[MAX_DIFF_FILES];
    // Offset to adjust hunks located after this one
    sint32 Offset = 0;
    // Buffers representing just the hunks from the source
    LineBuffer SrcHunkBuffer;
    // File that is different in original hunk
    DiffFileNumber FileThatIsDifferent = OriginalHunk->GetDiffFile();

    // Get data from Hunk object
    Start[DiffFile_One] = OriginalHunk->GetStart(DiffFile_One);
    End[DiffFile_One] = OriginalHunk->GetEnd(DiffFile_One);
    Start[DiffFile_Two] = OriginalHunk->GetStart(DiffFile_Two);
    End[DiffFile_Two] = OriginalHunk->GetEnd(DiffFile_Two);
    Start[DiffFile_Three] = OriginalHunk->GetStart(DiffFile_Three);
    End[DiffFile_Three] = OriginalHunk->GetEnd(DiffFile_Three);

    // Atleast one of the files in the hunk have to be different.  For
    //  example, it makes no sense to merge file 1 and 2 in a three-way
    //  hunk if file 3 is the only one that is different.
    assert(FileThatIsDifferent == DiffFile_Unspecified ||
        FileThatIsDifferent == SourceFileNumber ||
        FileThatIsDifferent == DestFileNumber);
    // Both files can't be "insert after" hunks
    assert(!(End[DestFileNumber] == UNSPECIFIED && 
        End[SourceFileNumber] == UNSPECIFIED));

    // Remove the original destination hunk buffer if it exists and retain
    //  it in case we call Undo later.
    if(End[DestFileNumber] != UNSPECIFIED)
    {
        DestBuffer->MoveLinesTo(&OriginalDestHunkBuffer, Start[DestFileNumber], 
            End[DestFileNumber], 0);
    }

    // If source is an "insert after" hunk
    if(End[SourceFileNumber] == UNSPECIFIED)
    {
        // We can assume that the destination is a change hunk since
        //  the source is an "insert after" hunk.

        // Retain info as to what we're changing
        LastChange.Type = MergeChangeType_Delete;
        LastChange.Start = Start[DestFileNumber];
        LastChange.Length = End[DestFileNumber] - Start[DestFileNumber] + 1;
        // Set the offset based on how many lines are removed from 
        //  the original destination buffer.  We're removing lines
        //  so offset needs to be a negative value.
        Offset = -(sint32)LastChange.Length;
    }
    // Source hunk is a change hunk
    else
    {
        // If destination was originally an "insert after" hunk
        if(End[DestFileNumber] == UNSPECIFIED)
        {
            // Retain info as to what we're changing
            LastChange.Type = MergeChangeType_Insert;
            // Start is always the first line of the range (not "insert after")
            LastChange.Start = Start[DestFileNumber] + 1;
            LastChange.Length = End[SourceFileNumber] - Start[SourceFileNumber] + 1;
            // Offset is the total number of lines added from the
            //  source buffer
            Offset = (sint32)LastChange.Length;
            // Insert lines from source to the "insert after" line number of
            //  the destination
            SourceBuffer->CopyLinesTo(DestBuffer, Start[SourceFileNumber], 
                End[SourceFileNumber], Start[DestFileNumber]);
        }
        // Else, destination is a change hunk
        else
        {
            // Retain info as to what we're changing
            LastChange.Type = MergeChangeType_Replace;
            LastChange.Start = Start[DestFileNumber];
            LastChange.Length = End[DestFileNumber] - Start[DestFileNumber]
                + 1;
            LastChange.NewLength = End[SourceFileNumber] -
                Start[SourceFileNumber] + 1;
            // Set the offset based on the difference between how many
            //  lines were in the original dest, and how many lines
            //  replaced it
            Offset = (sint32)((End[SourceFileNumber] -
                Start[SourceFileNumber]) - (End[DestFileNumber] -
                Start[DestFileNumber]));
            // Insert lines from source to the same line as the lines we took
            //  out of the destination.  We have to specify "start - 1" because
            //  the Start line represent a start of a range, but CopyLinesTo
            //  method expects an "insert after" line.
            SourceBuffer->CopyLinesTo(DestBuffer, Start[SourceFileNumber], 
                End[SourceFileNumber], Start[DestFileNumber] - 1);
        }
    }

    // If hunk represents a three-way diff
    if(OriginalHunk->GetStart(DiffFile_Three) != UNSPECIFIED)
    {
        // If all three files are different, or the file that is different
        //  is not the destination file number, then that means that the hunk
        //  hasn't been fully resolved.
        if(FileThatIsDifferent == DiffFile_Unspecified ||
        FileThatIsDifferent != DestFileNumber)
        {
            // If source is an "insert after" hunk
            if(End[SourceFileNumber] == UNSPECIFIED)
            {
                // Make the destination an "insert after" hunk
                End[DestFileNumber] = UNSPECIFIED;
                // Set the "insert after" line to the line number just before
                //  the original hunk.
                Start[DestFileNumber]--;
            }
            // Source hunk is a change hunk
            else
            {
                // If original destination hunk is an "insert after", then
                //  change the start line number to the first line of the
                //  hunk (since it's now a change hunk).
                if(End[DestFileNumber] == UNSPECIFIED)
                    Start[DestFileNumber]++;
                // Set the end of the hunk relative to the number of lines
                //  from the source's change hunk.
                End[DestFileNumber] = Start[DestFileNumber] + 
                    (End[SourceFileNumber] - Start[SourceFileNumber]);
            }

            // Figure out which file was not part of the merge transaction.
            //  This file is the one that is now different
            for(i = DiffFile_One; i <= DiffFile_Three; i++)
            {
                // If the file is neither the source or the destination then
                //  this is the file that is different in the hunk.
                if(((DiffFileNumber)i != SourceFileNumber) && 
                ((DiffFileNumber)i != DestFileNumber))
                {
                    FileThatIsDifferent = (DiffFileNumber)i;
                }
            }
            // Create updated hunk.  Since it is not a three-way diff anymore,
            //  the file that is different is always UNSPECIFIED.
            NewHunk = new Hunk(NULL, Start[DiffFile_One], End[DiffFile_One],
                Start[DiffFile_Two], End[DiffFile_Two], Start[DiffFile_Three],
                End[DiffFile_Three], FileThatIsDifferent);
        }
    }

    // Replace the new hunk with the original.  Also, we provide the
    //  offset information for the dest file if the new hunk affects
    //  other hunks after it (i.e. added or removed lines).  NewHunk is
    //  NULL if the hunk was fully resolved, in which case the hunk
    //  is simply removed from the list.
    OriginalHunk->Replace(NewHunk, DestFileNumber, Offset);
}

const MergeChange *FileMergeTransaction::GetLastChange()
{
    return &LastChange;
}