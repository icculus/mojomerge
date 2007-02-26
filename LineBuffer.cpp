/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  LineBuffer.cpp - Implementation of the LineBuffer class
 *
 */

#include "LineBuffer.h"

using namespace MojoMerge;

LineBuffer::LineBuffer(const char *Buffer)
{
    const char *p;          // Pointer to current char in buffer
    const char *StartP;     // Pointer for start of line
    char c;                 // Current character
    uint32 CharCount;       // Number of characters so far for new line
    bool NewLine;           // Should we create a new line?

    // Set member default values
    LineCount = 0;
    Lines = NULL;
    LinesAllocated = 0;

    // Only do processing if they passed a buffer
    if(Buffer != NULL)
    {
        // Set our pointers to the beginning of the buffer;
        StartP = p = Buffer;
        // Reset our CharCount value
        CharCount = 0;
        // No new line encountered yet
        NewLine = false;

        // Loop through the buffer until we hit a null terminator
        while((c = *p) != 0x00)
        {
            // Increment our number of characters
            CharCount++;

            // If we detect a CR character
            if(c == CR)
            {
                // Get the next character to see if it's a CR/LF combination
                c = *(++p);
                if(c == LF)
                {
                    // We got a LF character, add to the character count
                    CharCount++;
                    // New line encountered (DOS style)
                    NewLine = true;
                }
                // Another character or null terminator encountered
                else
                {
                    // We got another character, that character is a part of
                    // next line, so we need to set our pointer back one.
                    p--;
                    // New line encountered (Mac style)
                    NewLine = true;
                }
            }
            // If we detect a LF character
            else if(c == LF)
            {
                // New line encountered (UNIX style)
                NewLine = true;
            }

            // Go to next character
            p++;    

            // If the NewLine flag is set
            if(NewLine)
            {
                // Add a new line to our array
                AddToLinesArray(StartP, CharCount);
                // Set the start of the next line to our current position
                StartP = p;
                // Reset newline flag
                NewLine = false;
                // Reset our character count
                CharCount = 0;
            }
        }

        // If the last line did not have a newline character, we need to
        //  add that as the last line in the array
        if(CharCount > 0)
            AddToLinesArray(StartP, CharCount);
    }
}

LineBuffer::~LineBuffer()
{
    uint32 i;

    // Deallocate each line in the lines array
    for(i = 0; i < LineCount; i++)
        delete Lines[i];

    // Now delete the array
    delete Lines;
}

void LineBuffer::MoveLinesTo(LineBuffer *DestBuffer, uint32 Start, uint32 End,
    uint32 Dest)
{
    uint32 LineCountToMove;         // Number of lines being moved
    char **NewLines;                // New array for the destination
    uint32 DestOriginalLineCount;   // Line count of original dest buffer
    uint32 SrcOriginalLineCount;    // Line count of original source buffer

    // Start has to be greater than 0
    assert(Start > 0);
    // End has to be less than or equal to the number of lines in the source
    assert(End <= LineCount);
    // Start has to be less than or equal to End
    assert(Start <= End);

    // Figure out how many lines we're moving
    LineCountToMove = End - Start + 1;
    // Retain original line count of src and dest buffers
    SrcOriginalLineCount = LineCount;
    DestOriginalLineCount = DestBuffer->LineCount;
    // Increment the total line count for the destination
    DestBuffer->LineCount += LineCountToMove;
    // Decrement the total line count from the source
    LineCount -= LineCountToMove;
    // Allocate a new array to accomodate the lines being added plus allocate
    //  a little extra for later transactions.
    DestBuffer->LinesAllocated = DestBuffer->LineCount + LINES_ARRAY_BLOCK;
    // Perform memory allocation
    NewLines = new char*[DestBuffer->LinesAllocated];
    // Make sure allocation was successful
    assert(NewLines != NULL);

    // Copy the items from our original dest buffer before the index we're
    //  inserting to.
    memcpy(NewLines, DestBuffer->Lines, Dest * sizeof(char *));
    // Copy the items from our source (from the Start to End index) to our
    //  destination.
    memcpy(&NewLines[Dest], &Lines[Start - 1], LineCountToMove * sizeof(char *));
    // Copy the items from our original Dest buffer after the Dest index to our
    //  new buffer
    memcpy(&NewLines[Dest + LineCountToMove], &DestBuffer->Lines[Dest],
        (DestOriginalLineCount - Dest) * sizeof(char *));

    // In source buffer, copy items from after the End index to the Start index
    //  and therefore overwritting the items that were moved.
    memcpy(&Lines[Start - 1], &Lines[End], (SrcOriginalLineCount - End) * sizeof(char *));

    // Delete original buffer
    delete DestBuffer->Lines;
    // Set it to our new buffer
    DestBuffer->Lines = NewLines;
}

void LineBuffer::CopyLinesTo(LineBuffer *DestBuffer, uint32 Start, uint32 End,
    uint32 Dest)
{
    uint32 LineCountToCopy;         // Number of lines being copied
    char **NewLines;                // New array for the destination
    uint32 DestOriginalLineCount;   // Line count of original dest buffer
    char **pDest, **pSrc;           // Temporary pointers for copying data
    uint32 i;                       // Generic counter

    // Start has to be greater than 0
    assert(Start > 0);
    // End has to be less than or equal to the number of lines in the source
    assert(End <= LineCount);
    // Start has to be less than or equal to End
    assert(Start <= End);
    // Dest has to be less than or equal to number of lines in dest buffer
    assert(Dest <= DestBuffer->LineCount);

    // Figure out how many lines we're copying
    LineCountToCopy = End - Start + 1;
    // Retain original line count of dest buffer
    DestOriginalLineCount = DestBuffer->LineCount;
    // Increment the total line count for the destination
    DestBuffer->LineCount += LineCountToCopy;
    // Allocate a new array to accomodate the lines being added plus allocate
    //  a little extra for later transactions.
    DestBuffer->LinesAllocated = DestBuffer->LineCount + LINES_ARRAY_BLOCK;
    // Perform memory allocation
    NewLines = new char*[DestBuffer->LinesAllocated];
    //NewLines = (char **)malloc(DestBuffer->LinesAllocated * sizeof(char *));
    // Make sure allocation was successful
    assert(NewLines != NULL);

    // Copy the items from our original dest buffer before the index we're
    //  inserting to.
    memcpy(NewLines, DestBuffer->Lines, Dest * sizeof(char *));
    // Copy the items from our source (from the Start to End index) to our
    //  destination.
    pDest = &NewLines[Dest];
    pSrc = &Lines[Start - 1];
    for(i = 0; i < LineCountToCopy; i++)
    {
        // Allocate for new string (size of source string + null terminator)
        *pDest = new char[strlen(*pSrc) + 1];
        // Make sure allocation was successful
        assert(*pDest != NULL);
        // Copy source string to destination string
        strcpy(*pDest, *pSrc);
        // Increment to next index in each array
        pDest++;
        pSrc++;
    }
    // Copy the items from our original Dest buffer after the Dest index to our
    //  new buffer
    memcpy(&NewLines[Dest + LineCountToCopy], &DestBuffer->Lines[Dest],
        (DestOriginalLineCount - Dest) * sizeof(char *));

    // Delete original buffer
    delete DestBuffer->Lines;
    // Set it to our new buffer
    DestBuffer->Lines = NewLines;
}

void LineBuffer::AddToLinesArray(const char *StartP, uint32 Length)
{
    char *NewString;    // New string added to lines array

    // Increment the total line count
    LineCount++;
    // If we have to allocate more memory for the new line
    if(LineCount > LinesAllocated)
    {
        // Add another "block" of lines to our allocated lines
        LinesAllocated = LinesAllocated + LINES_ARRAY_BLOCK;
        // Perform memory allocation while retaining data
        Lines = (char **)realloc(Lines, LinesAllocated * sizeof(char *));
        // Make sure allocation was successful
        assert(Lines != NULL);
    }
    // Allocate new string based on specified length plus null terminator
    NewString = new char[Length + 1];
    // Make sure the allocate was successful
    assert(NewString != NULL);
    // Copy characters to new string
    strncpy(NewString, StartP, Length);
    // Add null terminator
    NewString[Length] = 0x00;
    // Add the string to the array (array is 0-based)
    Lines[LineCount-1] = NewString;
}

const char *LineBuffer::GetLine(uint32 LineNumber)
{
    // Make sure the LineNumber is within the array range
    assert(LineNumber > 0 && LineNumber <= LineCount);
    // Return requested line.  Array is 0-based, but LineNumber is 1-based
    return Lines[LineNumber - 1];
}

uint32 LineBuffer::GetLineCount()
{
    return LineCount;
}