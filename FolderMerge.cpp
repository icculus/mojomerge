/*  FolderMerge.cpp - Implementation of FolderMerge class
 *
 */

#include "FolderMerge.h"
#include "Config.h"

using namespace MojoMerge;

FolderMerge::FolderMerge(FolderHunk *FirstHunk, const char *Folder1,
    const char *Folder2, const char *Folder3)
{
    // First two folders can't be NULL
    assert(Folder1);
    assert(Folder2);
    // Save folders
    strcpy(this->Folder[DiffFile_One], Folder1);
    strcpy(this->Folder[DiffFile_Two], Folder2);
    if(Folder3)
        strcpy(this->Folder[DiffFile_Three], Folder3);
    // Save hunk
    this->FirstHunk = new FolderHunk(FirstHunk);
    // Create the undo buffer (get size from config settings)
    UndoBuffer = new TransactionBuffer(Config::GetUndoSize());
    // UndoBuffer can't be NULL
    assert(UndoBuffer);
}

FolderMergeTransaction *FolderMerge::ResolveDiff(FolderHunk *MergeHunk,
    DiffFileNumber SourceFolder, DiffFileNumber DestFolder)
{
    // Create new merge transaction that will perform the resolve.  This also
    //  executes the transaction
    /*FolderMergeTransaction *NewTransaction = new 
        FolderMergeTransaction(MergeHunk, Folder[SourceFolder],
        Folder[DestFolder], SourceFolder, DestFolder);
    // Add the transaction to the undo buffer
    UndoBuffer->PushTransaction(NewTransaction);
    // Return the new transaction object
    return NewTransaction;*/
    return NULL;
}

FolderMergeTransaction *FolderMerge::Undo()
{
    return NULL;
}

FolderMergeTransaction *FolderMerge::Redo()
{
    return NULL;
}

FolderHunk *FolderMerge::GetFirstHunk()
{
    // First hunk is always a dummy "list head"
    return FirstHunk->GetNext();
}

FolderMerge::~FolderMerge()
{
    FirstHunk->DeleteList();
}
