/*  FolderHunk.cpp - Implementation of FolderHunk class
 *
 */

#include "FolderHunk.h"
#include <assert.h>

using namespace MojoMerge;

FolderHunk::FolderHunk(FolderHunk *Prev, const char *Path, bool IsFolder,
    Hunk *FileHunk, FolderHunk *FirstChild, bool ExistsFolder1,
    bool ExistsFolder2, bool ExistsFolder3)
{
    // Set our previous hunk
    this->Prev = Prev;
    // If previous is a valid object
    if(Prev)
        // Set the previous object's next item to this item
        Prev->Next = this;
    // Initialize the next object to null for now
    Next = NULL;
    // Path can't be NULL
    assert(Path);
    // Save the passed arguments to our member variables
    this->Path = Path;
    this->IsFolder = IsFolder;
    this->FileHunk = FileHunk;
    this->FirstChild = NULL;
    Exists[DiffFile_One] = ExistsFolder1;
    Exists[DiffFile_Two] = ExistsFolder2;
    Exists[DiffFile_Three] = ExistsFolder3;    
}

bool FolderHunk::GetExists(DiffFileNumber FolderNumber)
{
    return Exists[FolderNumber];
}

FolderHunk *FolderHunk::GetFirstChild()
{
    return FirstChild;
}

FolderHunk *FolderHunk::GetPrevious()
{
    return Prev;
}

FolderHunk *FolderHunk::GetNext()
{
    return Next;
}

const char *FolderHunk::GetPath()
{
    return Path;
}

bool FolderHunk::GetIsFolder()
{
    return IsFolder;
}