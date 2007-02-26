/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

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
    //OriginalDestHunkBuffer = NULL;
    this->SourceFileNumber = DiffFile_Unspecified;
    this->DestFileNumber = DiffFile_Unspecified;
    memset(&RedoChange, 0, sizeof(MergeChange));
    memset(&UndoChange, 0, sizeof(MergeChange));
    Offset = 0;

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
    RedoChange.FileNumber = DestFileNumber;
    UndoChange.FileNumber = DestFileNumber;

    // Do the initial transaction
    Do();
}

FileMergeTransaction::~FileMergeTransaction()
{
    // Delete the hunk that is not currently in the hunk list for the DestFile
    if(HunkToDelete)
        delete HunkToDelete;
}

void FileMergeTransaction::Undo()
{
    LineBuffer TempBuffer;

    // When an Undo is performed, we replace the new hunk with the original
    //  hunk that was there before we performed the transaction.  Also, we
    //  apply the OPPOSITE offset so to remove the affect of the offset
    //  that was originally during the transaction.
    if(NewHunk)
        NewHunk->Replace(OriginalHunk, DestFileNumber, -Offset);
    // Else, there is no new hunk so just put the OriginalHunk back in
    //  its original spot in the list
    else
        OriginalHunk->Revert(DestFileNumber, -Offset);

    // Set our hunk to delete to the one that isn't in the Dest buffer
    HunkToDelete = NewHunk;

    // If Undo action is an "insert after"
    if(UndoChange.Type == MergeChangeType_Insert)
    {
        // Move lines back into the destination buffer at the original location
        ChangeBuffer.MoveLinesTo(DestBuffer, 1, ChangeBuffer.GetLineCount(),
            UndoChange.Start - 1);
    }
    // Else, if Undo action is deleting lines from the buffer
    else if(UndoChange.Type == MergeChangeType_Delete)
    {
        // Move lines into our ChangeBuffer that were inserted during Do/Redo
        DestBuffer->MoveLinesTo(&ChangeBuffer, UndoChange.Start,
            UndoChange.Start + UndoChange.Length - 1, 0);
    }
    // Else, if the Undo action is replacing lines with other lines
    else if(UndoChange.Type == MergeChangeType_Replace)
    {
        // Exchange lines from Do/Redo transaction with original lines
        DestBuffer->MoveLinesTo(&TempBuffer, UndoChange.Start,
            UndoChange.Start + UndoChange.Length - 1, 0);
        ChangeBuffer.MoveLinesTo(DestBuffer, 1, ChangeBuffer.GetLineCount(),
            UndoChange.Start - 1);
        TempBuffer.MoveLinesTo(&ChangeBuffer, 1, TempBuffer.GetLineCount(),
            0);
    }
    // Shouldn't get here
    else
        assert(0);
}

void FileMergeTransaction::Redo()
{
    LineBuffer TempBuffer;

    // When a redo is performed, we replace the original hunk with the new
    //  hunk that was created as a result of the transaction.  Also, we need
    //  to apply the offset that was done with the original transaction.
    OriginalHunk->Replace(NewHunk, DestFileNumber, Offset);

    // Set our hunk to delete to the one that isn't in the Dest buffer
    HunkToDelete = OriginalHunk;

    // If Undo action is an "insert after"
    if(RedoChange.Type == MergeChangeType_Insert)
    {
        // Move lines back into the destination buffer at the original location
        ChangeBuffer.MoveLinesTo(DestBuffer, 1, ChangeBuffer.GetLineCount(),
            RedoChange.Start - 1);
    }
    // Else, if Undo action is deleting lines from the buffer
    else if(RedoChange.Type == MergeChangeType_Delete)
    {
        // Move lines into our ChangeBuffer that were inserted during Do/Redo
        DestBuffer->MoveLinesTo(&ChangeBuffer, RedoChange.Start,
            RedoChange.Start + RedoChange.Length - 1, 0);
    }
    // Else, if the Undo action is replacing lines with other lines
    else if(RedoChange.Type == MergeChangeType_Replace)
    {
        // Exchange lines from Do/Redo transaction with original lines
        DestBuffer->MoveLinesTo(&TempBuffer, RedoChange.Start,
            RedoChange.Start + RedoChange.Length - 1, 0);
        ChangeBuffer.MoveLinesTo(DestBuffer, 1, ChangeBuffer.GetLineCount(),
            RedoChange.Start - 1);
        TempBuffer.MoveLinesTo(&ChangeBuffer, 1, TempBuffer.GetLineCount(),
            0);
    }
    // Shouldn't get here
    else
        assert(0);
}

void FileMergeTransaction::Do()
{
    // Generic counter
    int i;
    // Data taken from Hunk object
    uint32 Start[MAX_DIFF_FILES], End[MAX_DIFF_FILES];
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
        DestBuffer->MoveLinesTo(&ChangeBuffer, Start[DestFileNumber], 
            End[DestFileNumber], 0);
    }

    // If source is an "insert after" hunk
    if(End[SourceFileNumber] == UNSPECIFIED)
    {
        // We can assume that the destination is a change hunk since
        //  the source is an "insert after" hunk.

        // Retain info as to what we're changing
        RedoChange.Type = MergeChangeType_Delete;
        RedoChange.Start = Start[DestFileNumber];
        RedoChange.Length = End[DestFileNumber] - Start[DestFileNumber] + 1;
        // Set the offset based on how many lines are removed from 
        //  the original destination buffer.  We're removing lines
        //  so offset needs to be a negative value.
        Offset = -(sint32)RedoChange.Length;

        // Retain info on the change if we perform an undo
        UndoChange.Type = MergeChangeType_Insert;
        UndoChange.Start = Start[DestFileNumber];
        UndoChange.Length = RedoChange.Length;
    }
    // Source hunk is a change hunk
    else
    {
        // If destination was originally an "insert after" hunk
        if(End[DestFileNumber] == UNSPECIFIED)
        {
            // Retain info as to what we're changing
            RedoChange.Type = MergeChangeType_Insert;
            // Start is always the first line of the range (not "insert after")
            RedoChange.Start = Start[DestFileNumber] + 1;
            RedoChange.Length = End[SourceFileNumber] - Start[SourceFileNumber] + 1;
            // Offset is the total number of lines added from the
            //  source buffer
            Offset = (sint32)RedoChange.Length;

            // Retain info on the change if we perform an undo
            UndoChange.Type = MergeChangeType_Delete;
            UndoChange.Start = Start[DestFileNumber] + 1;
            UndoChange.Length = RedoChange.Length;

            // Insert lines from source to the "insert after" line number of
            //  the destination
            SourceBuffer->CopyLinesTo(DestBuffer, Start[SourceFileNumber], 
                End[SourceFileNumber], Start[DestFileNumber]);
            // Retain new buffer in case they undo/redo later
            //SourceBuffer->CopyLinesTo(&RedoChange.Buffer, Start[SourceFileNumber], 
                //End[SourceFileNumber], 0);
        }
        // Else, destination is a change hunk
        else
        {
            // Retain info as to what we're changing
            RedoChange.Type = MergeChangeType_Replace;
            RedoChange.Start = Start[DestFileNumber];
            RedoChange.Length = End[DestFileNumber] - Start[DestFileNumber]
                + 1;
            RedoChange.NewLength = End[SourceFileNumber] -
                Start[SourceFileNumber] + 1;
            // Set the offset based on the difference between how many
            //  lines were in the original dest, and how many lines
            //  replaced it
            Offset = (sint32)((End[SourceFileNumber] -
                Start[SourceFileNumber]) - (End[DestFileNumber] -
                Start[DestFileNumber]));

            // Retain info on the change if we perform an undo
            UndoChange.Type = MergeChangeType_Replace;
            UndoChange.Start = Start[DestFileNumber];
            UndoChange.Length = RedoChange.NewLength;
            UndoChange.NewLength = RedoChange.Length;

            // Insert lines from source to the same line as the lines we took
            //  out of the destination.  We have to specify "start - 1" because
            //  the Start line represent a start of a range, but CopyLinesTo
            //  method expects an "insert after" line.
            SourceBuffer->CopyLinesTo(DestBuffer, Start[SourceFileNumber], 
                End[SourceFileNumber], Start[DestFileNumber] - 1);
            // Retain new buffer in case they undo/redo later
            //SourceBuffer->CopyLinesTo(&RedoChange.Buffer, Start[SourceFileNumber], 
                //End[SourceFileNumber], 0);
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

    // OriginalHunk will no longer be in the list, mark that for deletion
    HunkToDelete = OriginalHunk;
    // Replace the new hunk with the original.  Also, we provide the
    //  offset information for the dest file if the new hunk affects
    //  other hunks after it (i.e. added or removed lines).  NewHunk is
    //  NULL if the hunk was fully resolved, in which case the hunk
    //  is simply removed from the list.
    OriginalHunk->Replace(NewHunk, DestFileNumber, Offset);
}

MergeChange *FileMergeTransaction::GetLastChange()
{
    MergeChange *ReturnValue;

    // If hunk marked for deletion is the original hunk, then our last action
    //  performed on this transaction object was a Do() or Redo().  So, we
    //  should return the RedoChange
    if(HunkToDelete == OriginalHunk)
        ReturnValue = &RedoChange;
    // Else, last action was Undo...so return that change info
    else
        ReturnValue = &UndoChange;

    return ReturnValue;
}