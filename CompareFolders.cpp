#include "CompareFolders.h"
#include <assert.h>

using namespace MojoMerge;

CompareFolders::CompareFolders(Diff *DiffObject)
{
    // Diff object can't be NULL
    assert(DiffObject);
    this->DiffObject = DiffObject;
}

FolderHunk *CompareFolders::Execute(CompareFolderOptions Options,
    DiffOptions FileOptions, const char *Folder1, const char *Folder2,
    const char *Folder3)
{
    // TODO - GCR needs to fill this in
    assert(0);
    return NULL;
}
