/*  CompareFoldersUI.cpp - Implementation of CompareFoldersUI class
 *
 */

#include "CompareFoldersUI.h"
#include "FolderMerge.h"
#include "Application.h"
#include "Identifiers.h"

using namespace MojoMerge;

class TreeData : public wxTreeItemData
{
public:
    wxTreeItemId ID[MAX_DIFF_FILES];
    FolderHunk *Hunk;   // Hunk that is associated with item
};

CompareFoldersUI::CompareFoldersUI(bool ThreeWayNotTwoWay)
{
    //TODO - Need to replace with paths from config object
    MyDiff = new GNUDiff("C:\\cygwin\\bin\\diff.exe", 
        "C:\\cygwin\\bin\\diff3.exe", Application::GetTempFolder());

    // Save member variables
    this->ThreeWayNotTwoWay = ThreeWayNotTwoWay;
    MyMerge = NULL;
    // Set what the last diff file is based on the bool argument
    if(ThreeWayNotTwoWay)
        LastDiffFile = DiffFile_Three;
    else
        LastDiffFile = DiffFile_Two;

    // For now, just set our Name to some test name
    strcpy(Name, "CompareFoldersUI");

    // Object is also a CompareFilesUI object
    ID |= TabWindow_CompareFoldersUI;

    // Start timer for once a second
    Start(1000);
}

CompareFoldersUI::~CompareFoldersUI()
{
    // Remove the objects that we created
    if(MyDiff)
        delete MyDiff;
    if(MyMerge)
        delete MyMerge;
}

void CompareFoldersUI::Notify()
{
    wxTreeItemId FirstId, LastId, TempId;
    TreeData *Data;
    wxTreeCtrl *Tree = FolderPanels[DiffFile_One]->GetTreeWindow();
    wxTreeCtrl *Tree2 = FolderPanels[DiffFile_Two]->GetTreeWindow();

    // Search until we find the last visible item in the tree
    FirstId = Tree->GetFirstVisibleItem();
    if(FirstId.IsOk())
    {
        TempId = FirstId;
        while(TempId.IsOk())
        {
            LastId = TempId;
            TempId = Tree->GetNextVisible(TempId);
        }

        Data = (TreeData *)Tree->GetItemData(FirstId);
        // Ensure that item is visible
        //if(!Tree2->IsVisible(Data->ID[DiffFile_Two]))
            Tree2->EnsureVisible(Data->ID[DiffFile_Two]);

        Data = (TreeData *)Tree->GetItemData(LastId);
        // Ensure that item is visible
        //if(!Tree2->IsVisible(Data->ID[DiffFile_Two]))
            Tree2->EnsureVisible(Data->ID[DiffFile_Two]);
    }
}

void CompareFoldersUI::Initialize(wxWindow *Parent)
{
    int i;

    // Create our panel window
    Create(Parent, -1);

    // Create horizontal sizer
    HorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create two or three panes based on how many files compared
    for(i = DiffFile_One; i <= LastDiffFile; i++)
    {
        // Create first file panel object
        FolderPanels[i] = new CompareFolderPanel(this);
        // Add file panel to the sizer
        HorizontalSizer->Add(FolderPanels[i], 1, wxGROW, 5);

        if(i == DiffFile_One)
        {
            // Create the first separator panel
            SeparatorPanels[i] = new SeparatorPanel(this);
            HorizontalSizer->Add(SeparatorPanels[i], 0, wxGROW, 5);
        }
        else if(i == DiffFile_Two && ThreeWayNotTwoWay)
        {
            // Create the first separator panel
            SeparatorPanels[i] = new SeparatorPanel(this);
            HorizontalSizer->Add(SeparatorPanels[i], 0, wxGROW, 5);
        }

    }

    // Set sizers to that they automagically resize
    this->SetAutoLayout(true);
    this->SetSizer(HorizontalSizer);
    HorizontalSizer->Fit(this);
    HorizontalSizer->SetSizeHints(this);
}

void CompareFoldersUI::SetFolder(DiffFileNumber File, wxString &Filename)
{
}

void CompareFoldersUI::Print()
{
}

void CompareFoldersUI::PrintPreview()
{
}

void CompareFoldersUI::Undo()
{
}

void CompareFoldersUI::Redo()
{
}

void CompareFoldersUI::Cut()
{
}

void CompareFoldersUI::Copy()
{
}

void CompareFoldersUI::Paste()
{
}

void CompareFoldersUI::SelectAll()
{
}

void CompareFoldersUI::TwoWayComparison()
{
}

void CompareFoldersUI::ThreeWayComparison()
{
}

void CompareFoldersUI::Recompare()
{
    FolderHunk *CurHunk;
    char *Folder3Name = NULL;
    
    CompareFolders MyCompare(MyDiff);

    // Perform diff and merge of all three files
    if(ThreeWayNotTwoWay)
    {
        CurHunk = MyCompare.Execute(CompareFolderOption_None, DiffOption_None,
            FolderPanels[DiffFile_One]->GetFolder(),
            FolderPanels[DiffFile_Two]->GetFolder(),
            FolderPanels[DiffFile_Three]->GetFolder());
        MyMerge = new FolderMerge(CurHunk, FolderPanels[DiffFile_One]->GetFolder(),
            FolderPanels[DiffFile_Two]->GetFolder(),
            FolderPanels[DiffFile_Three]->GetFolder());
    }
    // Just perform diff and merge of two files
    else
    {
        CurHunk = MyCompare.Execute(CompareFolderOption_None, DiffOption_None,
            FolderPanels[DiffFile_One]->GetFolder(),
            FolderPanels[DiffFile_Two]->GetFolder());
        MyMerge = new FolderMerge(CurHunk, FolderPanels[DiffFile_One]->GetFolder(),
            FolderPanels[DiffFile_Two]->GetFolder());
    }


    // Add the hunks to each of the directory trees
    AddHunks(CurHunk, NULL, NULL, NULL);
    FolderPanels[DiffFile_One]->GetTreeWindow()->ScrollWindow(50, 50);
}

void CompareFoldersUI::SetTreeItemData(wxTreeItemId Id1, wxTreeItemId Id2,
wxTreeItemId Id3, FolderHunk *Hunk)
{
    int i;                  // Generic counter
    int LastDiffFolder;     // Last folder to process
    // Data objects associated with each tree node
    TreeData *Data[MAX_DIFF_FILES];

    if(Id3)
        LastDiffFolder = DiffFile_Three;
    else
        LastDiffFolder = DiffFile_Two;

    for(i = DiffFile_One; i <= LastDiffFolder; i++)
    {
        Data[i] = new TreeData();
        Data[i]->ID[DiffFile_One] = Id1;
        Data[i]->ID[DiffFile_Two] = Id2;
        Data[i]->ID[DiffFile_Three] = Id3;
        Data[i]->Hunk = Hunk;
        FolderPanels[i]->GetTreeWindow()->SetItemData(Data[i]->ID[i], Data[i]);
    }
}

void CompareFoldersUI::AddHunks(FolderHunk *CurHunk, wxTreeItemId *Parent1,
wxTreeItemId *Parent2, wxTreeItemId *Parent3)
{
    wxTreeCtrl *TreeWindow1 = FolderPanels[DiffFile_One]->GetTreeWindow();
    wxTreeCtrl *TreeWindow2 = FolderPanels[DiffFile_Two]->GetTreeWindow();
    wxTreeCtrl *TreeWindow3 = NULL;

    // IDs of new nodes we add in this function
    wxTreeItemId Node1, Node2, Node3;
    wxTreeItemId Temp1, Temp2, Temp3;

    if(ThreeWayNotTwoWay)
        TreeWindow3 = FolderPanels[DiffFile_Three]->GetTreeWindow();

    // If root node
    if(Parent1 == NULL)
    {
        // Clear all items existing from previous list
        TreeWindow1->DeleteAllItems();
        TreeWindow2->DeleteAllItems();
        if(ThreeWayNotTwoWay)
            TreeWindow3->DeleteAllItems();
        // Add root items
        Temp1 = TreeWindow1->AddRoot(FolderPanels[DiffFile_One]->GetFolder());
        Temp2 = TreeWindow2->AddRoot(FolderPanels[DiffFile_Two]->GetFolder());
        if(ThreeWayNotTwoWay)
            Temp3 = TreeWindow3->AddRoot(FolderPanels[DiffFile_Three]->GetFolder());

        // Set the data associated with the items we added
        SetTreeItemData(Temp1, Temp2, Temp3, CurHunk);

        // Save these root nodes as the new parents so that the top level
        //  items get added to the root parent
        Parent1 = &Temp1;
        Parent2 = &Temp2;
        Parent3 = &Temp3;
    }

    // Loop through all the hunks
    while(CurHunk)
    {
        Node1 = TreeWindow1->AppendItem(*Parent1, CurHunk->GetBarePath());
        Node2 = TreeWindow2->AppendItem(*Parent2, CurHunk->GetBarePath());
        if(ThreeWayNotTwoWay)
        {
            Node3 = TreeWindow3->AppendItem(*Parent3, 
            CurHunk->GetBarePath());
        }

        // Set the data associated with the items we added
        SetTreeItemData(Node1, Node2, Node3, CurHunk);

        // Process the children
        if(CurHunk->GetFirstChild())
            AddHunks(CurHunk->GetFirstChild(), &Node1, &Node2, &Node3);
        CurHunk = CurHunk->GetNext();
    }

    TreeWindow1->Expand(*Parent1);
    TreeWindow2->Expand(*Parent2);
    if(ThreeWayNotTwoWay)
        TreeWindow3->Expand(*Parent3);
}

void CompareFoldersUI::OnSeparatorPainted(wxPaintEvent &event, wxDC *dc)
{
}

void CompareFoldersUI::OnTreeCollapsing(wxTreeEvent &event)
{
    // We don't want the user collapsing any branches
    event.Veto();
}

// Event mappings for the file panel
BEGIN_EVENT_TABLE(CompareFoldersUI, wxPanel)
    EVT_TREE_ITEM_COLLAPSING(ID_FOLDER_TREE_CTRL, CompareFoldersUI::OnTreeCollapsing)
    //EVT_BUTTON(ID_MERGE_BUTTON, CompareFilesUI::OnMergeButtonClick)
    //EVT_STC_POSCHANGED(ID_FILE_TEXT_EDIT, CompareFilesUI::OnFileTextPosChanged)
    //EVT_STC_PAINTED(ID_FILE_TEXT_EDIT, CompareFilesUI::OnFileTextPainted)
    //EVT_PAINT(CompareFilesUI::OnPanelPaint)
    //EVT_CUSTOM(wxEVT_SIZE, ID_SEPARATOR_PANEL, CompareFilesUI::OnPanelPaint)
END_EVENT_TABLE()