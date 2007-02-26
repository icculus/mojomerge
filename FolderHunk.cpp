/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  FolderHunk.cpp - Implementation of FolderHunk class
 *
 */

#include "FolderHunk.h"
#include <assert.h>
#include <string.h>

using namespace MojoMerge;

FolderHunk::FolderHunk(FolderHunk *Prev, const char *Path,
    const char *BarePath, bool IsFolder,
    Hunk *FileHunk, FolderHunk *FirstChild, bool ExistsFolder1,
    bool ExistsFolder2, bool ExistsFolder3)
{
    LinkHead = false;
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
    strcpy(this->Path, Path);
    strcpy(this->BarePath, BarePath);
    this->IsFolder = IsFolder;
    //this->FileHunk = FileHunk;
    this->FirstChild = FirstChild;
    Exists[DiffFile_One] = ExistsFolder1;
    Exists[DiffFile_Two] = ExistsFolder2;
    Exists[DiffFile_Three] = ExistsFolder3;    
}

FolderHunk::FolderHunk(FolderHunk *FirstHunk)
{
    LinkHead = true;
    Next = FirstHunk;
    Prev = NULL;
    FirstChild = NULL;
}

bool FolderHunk::GetExists(DiffFileNumber FolderNumber)
{
    assert(!LinkHead);
    return Exists[FolderNumber];
}

FolderHunk *FolderHunk::GetFirstChild()
{
    return FirstChild;
}

FolderHunk *FolderHunk::GetPrevious()
{
    assert(!LinkHead);
    return Prev;
}

FolderHunk *FolderHunk::GetNext()
{
    return Next;
}

const char *FolderHunk::GetPath()
{
    assert(!LinkHead);
    return Path;
}

const char *FolderHunk::GetBarePath()
{
    assert(!LinkHead);
    return BarePath;
}

bool FolderHunk::GetIsFolder()
{
    assert(!LinkHead);
    return IsFolder;
}

void FolderHunk::DeleteList()
{
    // Delete next hunk if applicable
    if(this->FirstChild)
        this->FirstChild->DeleteList();
    if(this->Next)
        this->Next->DeleteList();
    // Delete this hunk...duh.
    delete this;
}
