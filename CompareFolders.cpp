#include "CompareFolders.h"
#include <assert.h>
#include "wx/dir.h"
#include "Application.h"

using namespace MojoMerge;

// This small class provides our own object to traverse directories
class DirTraverser : public wxDirTraverser
{
public:
    DirTraverser(wxArrayString *Array, ExistsFolderHash *Hash, wxString Folder,
    int FileExistConstant)
    {
        // Add "\" to end of folder if not present.  This is so we know to cut
        //  it out of the path when we add the entries
        // TODO - Add platform specific separator
        if(Folder[Folder.Length() - 1] != '\\')
            Folder.Append('\\');
        this->Array = Array;
        this->Folder = Folder;
        this->PathStart = Folder.Length();
        this->Hash = Hash;
        this->FileExistConstant = FileExistConstant;
    }

    void AddItem(const wxString& filename, bool IsDir = false)
    {
        // Extract just the unique part of the path
        wxString Path = filename.Mid(PathStart);
        // Add item to our hash table for keeping track of which folders this
        //  path occurs in
        // TODO - Add case insensitivity for Win32
        Application::Debug("%s: Hash entry = %d", Path, (*Hash)[Path]);
        // If it's a directoy, retain that info in our item data
        if(IsDir)
            (*Hash)[Path] = (*Hash)[Path] | FileExistConstant | ISFOLDER;
        else
            (*Hash)[Path] = (*Hash)[Path] | FileExistConstant;
        Application::Debug("Hash entry now = %d", (*Hash)[Path]);
        // Add the path to our array of paths if it doesn't already exist
        if(Array->Index(Path) == wxNOT_FOUND)
            Array->Add(Path);
    }

    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        AddItem(filename);
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        AddItem(dirname, true);
        return wxDIR_CONTINUE;
    }
private:
    wxArrayString *Array;
    wxString Folder;
    DiffFileNumber FolderNumber;
    ExistsFolderHash *Hash;
    int FileExistConstant;
    // Start character of files and directories added.  This is the first
    //  character position after the root path.
    size_t PathStart;
};

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
    // First two folders can't be NULL
    assert(Folder1);
    assert(Folder2);

    // Clear items from the previous execute if any
    MyFiles.Clear();
    MyHash.clear();

    // Get all files for folder1
    DirTraverser MyTraverser1(&MyFiles, &MyHash, Folder1, FILEEXIST_ONE);
    wxDir Dir1(Folder1);
    Dir1.Traverse(MyTraverser1);
    // Get all files for folder2
    DirTraverser MyTraverser2(&MyFiles, &MyHash, Folder2, FILEEXIST_TWO);
    wxDir Dir2(Folder2);
    Dir2.Traverse(MyTraverser2);
    // Get all files for folder3
    if(Folder3)
    {
        DirTraverser MyTraverser3(&MyFiles, &MyHash, Folder3, FILEEXIST_THREE);
        wxDir Dir3(Folder3);
        Dir3.Traverse(MyTraverser3);
    }

    // Sort our file list array
    MyFiles.Sort();

    size_t i;
    Application::Debug("%s", Folder1);
    for(i = 0; i < MyFiles.Count(); i++)
        Application::Debug("%d:   %s", MyHash[MyFiles.Item(i)], MyFiles.Item(i));

    // Begin creation of FolderHunk children
    int CurIndex = 0;
    return CreateChildren(NULL, CurIndex, "");
}

FolderHunk *CompareFolders::CreateChildren(FolderHunk *Prev, int &CurIndex, wxString CurPath)
{
    int ArrayLength = (int)MyFiles.GetCount();
    wxString CurItem;
    int ItemInfo;
    bool ExitTime = false;
    FolderHunk *CurChild = NULL;
    FolderHunk *FirstChild = NULL;
    FolderHunk *ReturnValue = NULL;
    wxString BarePath;

    // If we're at a root node
    if(CurPath == "")
    {
        // Loop through all child nodes until the end of the array
        while(CurIndex < ArrayLength)
        {
            // Get current item in the array
            CurItem = MyFiles[CurIndex];
            // Get extended item information
            ItemInfo = MyHash[CurItem];
            // If item is a folder
            if(ItemInfo & ISFOLDER)
            {
                // Create child folder and append it to the previous child
                CurChild = CreateChildren(CurChild, CurIndex, CurItem);
            }
            else
            {
                // Just extract the last part of the path
                BarePath = CurItem.Mid(CurPath.Length());
                // TODO - Perform file diff if requested
                // Create child file and append it to the previous child
                CurChild = new FolderHunk(CurChild, CurItem, BarePath, false,
                    NULL, NULL, ItemInfo & FILEEXIST_ONE,
                    ItemInfo & FILEEXIST_TWO, ItemInfo & FILEEXIST_THREE);
                // Increment to the next child
                CurIndex++;
            }
            // If the first child hasn't been set, set it now to the first
            //  child in the list
            if(!FirstChild)
                FirstChild = CurChild;
        }
        // If we're at the root, just return the first item in the list
        ReturnValue = FirstChild;
    }
    else
    {
        // Go to the next item after the new folder
        CurIndex++;
        // Loop through items in the array
        while(!ExitTime)
        {
            if(CurIndex < MyFiles.Count())
            {
                // Get current item in the array
                CurItem = MyFiles[CurIndex];
                // Get extended item information
                ItemInfo = MyHash[CurItem];
                // Determine if item is a child of us by comparing the current
                //  path with the current item to see if it's part of it.
                if(CurItem.Mid(0, CurPath.Length()) == CurPath)
                {
                    // If item is a folder
                    if(ItemInfo & ISFOLDER)
                    {
                        // Create child folder and append it to the previous
                        //  child.
                        CurChild = CreateChildren(CurChild, CurIndex, CurItem);
                    }
                    else
                    {
                        // Just extract the last part of the path
                        BarePath = CurItem.Mid(CurPath.Length() + 1);
                        // TODO - Perform file diff if requested
                        // Create child file and append it to the previous
                        //  child.
                        CurChild = new FolderHunk(CurChild, CurItem, BarePath,
                            false, NULL, NULL, ItemInfo & FILEEXIST_ONE,
                            ItemInfo & FILEEXIST_TWO,
                            ItemInfo & FILEEXIST_THREE);
                        // Increment to the next child
                        CurIndex++;
                    }
                    // If the first child hasn't been set, set it now to the first
                    //  child in the list
                    if(!FirstChild)
                        FirstChild = CurChild;
                }
                else
                    ExitTime = true;
            }
            else
                ExitTime = true;
        }
        // Just extract the last part of the path
        //TODO - Make this platform independant
        BarePath = CurPath.Mid(CurPath.Find('\\', true) + 1);
        // Get extended item information
        ItemInfo = MyHash[CurPath];
        // Create folder hunk with all of it's children
        ReturnValue = new FolderHunk(Prev, CurPath, BarePath,
            true, NULL, FirstChild, ItemInfo & FILEEXIST_ONE,
            ItemInfo & FILEEXIST_TWO,
            ItemInfo & FILEEXIST_THREE);
    }

    return ReturnValue;
}