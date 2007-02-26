/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  Test.cpp - Implementation of the Test class
 *
 */

#include "Test.h"
#include "GNUDiff.h"
#include "Application.h"
#include "LineBuffer.h"
#include "Merge.h"
#include "Stack.h"
#include <stdio.h>
#include "wx/dir.h"
#include "CompareFolders.h"

using namespace MojoMerge;

#define TEST_DIFFPATH       "C:\\cygwin\\bin\\diff.exe"
#define TEST_DIFF3PATH      "C:\\cygwin\\bin\\diff3.exe"
// Names of each test file used for diff testing
#define TEST_FILE1          "testfile1.txt"
#define TEST_FILE2          "testfile2.txt"
#define TEST_FILE3          "testfile3.txt"
// Names of the test files to use for LineBuffer test
#define TESTFILE_LINEBUFFER1    "linebuffertext1.txt"
#define TESTFILE_LINEBUFFER2    "linebuffertext2.txt"

Hunk *Test::TestGNUDiff_TwoWay()
{
    Application::Debug("Performing GNUDiff two-way test...");

    Hunk *MyHunk;
    Hunk *p;

    // Object used to perform diffs
    GNUDiff MyDiff(TEST_DIFFPATH, TEST_DIFF3PATH, Application::GetTempFolder());
    // Buffers that contain data from each file being compared
    char *Buffer1;
    char *Buffer2;
    
    Buffer1 = Application::ReadFile(TEST_FILE1);
    Buffer2 = Application::ReadFile(TEST_FILE2);

    // Test two-way comparison
    MyHunk = MyDiff.CompareFiles(DiffOption_None, Buffer1, Buffer2);
    // If MyHunk is a valid object, there are differences
    if(MyHunk)
    {
        Application::Debug("Following differences encountered...");
        p = MyHunk;
        // Loop through all the hunks
        while(p)
        {
            // Print out the hunk information
            Application::Debug("%s: %d,%d\t%s: %d,%d", TEST_FILE1,
                p->GetStart(DiffFile_One), p->GetEnd(DiffFile_One),
                TEST_FILE2, p->GetStart(DiffFile_Two),
                p->GetEnd(DiffFile_Two));
            // Move to the next hunk
            p = p->GetNextHunk();
        }
    }
    else
        Application::Debug("No differences encountered");

    Application::Debug("GNUDiff two-way test complete!\n");

    // Delete the file buffers we read
    delete Buffer1;
    delete Buffer2;

    return MyHunk;
}

Hunk *Test::TestGNUDiff_ThreeWay()
{
    Application::Debug("Performing GNUDiff two-way test...");

    Hunk *MyHunk;
    Hunk *p;

    // Object used to perform diffs
    GNUDiff MyDiff(TEST_DIFFPATH, TEST_DIFF3PATH, Application::GetTempFolder());
    // Buffers that contain data from each file being compared
    char *Buffer1;
    char *Buffer2;
    char *Buffer3;
    
    Buffer1 = Application::ReadFile(TEST_FILE1);
    Buffer2 = Application::ReadFile(TEST_FILE2);
    Buffer3 = Application::ReadFile(TEST_FILE3);

    // Test three-way comparison
    MyHunk = MyDiff.CompareFiles(DiffOption_None, Buffer1, Buffer2, Buffer3);
    // If MyHunk is a valid object, there are differences
    if(MyHunk)
    {
        Application::Debug("Following differences encountered...");
        p = MyHunk;
        // Loop through all the hunks
        while(p)
        {
            // Print out the hunk information
            Application::Debug("%s: %d,%d\t%s: %d,%d\t%s: %d,%d", TEST_FILE1,
                p->GetStart(DiffFile_One), p->GetEnd(DiffFile_One),
                TEST_FILE2, p->GetStart(DiffFile_Two),
                p->GetEnd(DiffFile_Two), TEST_FILE3,
                p->GetStart(DiffFile_Three), p->GetEnd(DiffFile_Three));
            // Move to the next hunk
            p = p->GetNextHunk();
        }
    }
    else
        Application::Debug("No differences encountered");

    // Delete file buffers
    delete Buffer1;
    delete Buffer2;
    delete Buffer3;

    Application::Debug("GNUDiff three-way test complete!\n");
    return MyHunk;
}

void Test::TestLineBuffer()
{
    Application::Debug("Performing LineBuffer test...");

    uint32 i;
    uint32 Count;
    char *Buffer1;
    char *Buffer2;
    LineBuffer *LineBuffer1;
    LineBuffer *LineBuffer2;

    // Read in two test files
    Buffer1 = Application::ReadFile(TESTFILE_LINEBUFFER1);
    Buffer2 = Application::ReadFile(TESTFILE_LINEBUFFER2);
    // Create line buffer objects with the char buffers
    LineBuffer1 = new LineBuffer(Buffer1);
    LineBuffer2 = new LineBuffer(Buffer2);

    // Move lines 3 to 7 from buffer2 to buffer1 after line 5
    LineBuffer2->MoveLinesTo(LineBuffer1, 3, 7, 5);
    Application::Debug("Output of first buffer after move...");
    Count = LineBuffer1->GetLineCount();
    for(i = 1; i <= Count; i++)
        Application::DebugNoCR("%s", LineBuffer1->GetLine(i));
    Application::Debug("\nOutput of second buffer after move...");
    Count = LineBuffer2->GetLineCount();
    for(i = 1; i <= Count; i++)
        Application::DebugNoCR("%s", LineBuffer2->GetLine(i));

    // Copy lines 1 to 10 from buffer1 to buffer2 at the first line
    LineBuffer1->CopyLinesTo(LineBuffer2, 1, 10, 0);
    Application::Debug("\nOutput of first buffer after copy...");
    Count = LineBuffer1->GetLineCount();
    for(i = 1; i <= Count; i++)
        Application::DebugNoCR("%s", LineBuffer1->GetLine(i));
    Application::Debug("\nOutput of second buffer after copy...");
    Count = LineBuffer2->GetLineCount();
    for(i = 1; i <= Count; i++)
        Application::DebugNoCR("%s", LineBuffer2->GetLine(i));

    // Add a new line
    Application::Debug("");

    delete LineBuffer2;
    delete LineBuffer1;
    delete Buffer1;
    delete Buffer2;
}

void Test::TestMerge_TwoWay()
{
    Hunk *MyHunk;
    Hunk *CurHunk, *NextHunk;
    Merge *MyMerge;
    LineBuffer *FileBuffer;
    uint32 i;

    // Get some diff hunks from our diff test
    MyHunk = Test::TestGNUDiff_TwoWay();

    // Buffers that contain data from each file being compared
    char *Buffer1;
    char *Buffer2;
    Buffer1 = Application::ReadFile(TEST_FILE1);
    Buffer2 = Application::ReadFile(TEST_FILE2);

    // Create the merge object
    MyMerge = new Merge(MyHunk, Buffer1, Buffer2);
    // Get the first hunk in the list
    CurHunk = MyHunk;
    while(CurHunk)
    {
        // Get the next hunk before we resolve the current one since it gets
        //  removed after being resolved.
        NextHunk = CurHunk->GetNextHunk();
        // Execute the merge on each hunk to make Buffer2 look like Buffer1
        MyMerge->ResolveDiff(CurHunk, DiffFile_One, DiffFile_Two);
        // Set the current hunk to be the next hunk in the list
        CurHunk = NextHunk;
    }

    FileBuffer = MyMerge->GetBuffer(DiffFile_Two);
    for(i = 0; i < FileBuffer->GetLineCount(); i++)
    {
        Application::DebugNoCR("%s", FileBuffer->GetLine(i+1));
    }

    // Done with the merge object...gone!!
    delete MyMerge;
    // Remove the file buffers
    delete Buffer1;
    delete Buffer2;
}

void Test::TestStack()
{
    Stack <uint32>MyStack(10);
    uint32 i;
    uint32 *val;

    Application::Debug("Performing Stack test (adding 20 elements to a stack size of 10...");
    for(i = 1; i <= 20; i++)
    {
        val = new uint32;
        *val = i;
        MyStack.Push(val);
        Application::Debug("Added %d to the stack.", *val);
    }

    Application::Debug("Stack output...");
    // Loop as long as there's stuff to pop off the stack
    while(val = MyStack.Pop())
    {
        Application::Debug("Popped %d off the stack.", *val);
        delete val;
    }

    // Add a newline at the end of the test
    Application::Debug("");
}

void Test::TestCompareFolders()
{
    FolderHunk *FirstHunk;
    GNUDiff *MyDiff = new GNUDiff("C:\\cygwin\\bin\\diff.exe", 
        "C:\\cygwin\\bin\\diff3.exe", Application::GetTempFolder());

    CompareFolders MyCompare(MyDiff);

    FirstHunk = MyCompare.Execute(CompareFolderOption_None, DiffOption_None,
        "D:\\Documents\\readgs\\develop\\mojomerge\\Dir Diff Test\\NPPCC (ancestor)",
        "D:\\Documents\\readgs\\develop\\mojomerge\\Dir Diff Test\\NPPCC b",
        "D:\\Documents\\readgs\\develop\\mojomerge\\Dir Diff Test\\NPPCC c");

    PrintHunks(FirstHunk, 0);
    // Delete the list now that we're done with it
    FirstHunk->DeleteList();
}

void Test::PrintHunks(FolderHunk *FirstHunk, int Indent)
{
    FolderHunk *CurHunk;
    wxString Spaces;

    // Insert indent to beginning
    Spaces.Pad(Indent, ' ', false);

    // Get first hunk
    CurHunk = FirstHunk;
    while(CurHunk)
    {
        Application::Debug("%d%d%d: %s%s", CurHunk->GetExists(DiffFile_One),
            CurHunk->GetExists(DiffFile_Two),
            CurHunk->GetExists(DiffFile_Three),
            Spaces.c_str(), CurHunk->GetBarePath());
        // If item has children
        if(CurHunk->GetFirstChild())
            PrintHunks(CurHunk->GetFirstChild(), Indent + 4);
        CurHunk = CurHunk->GetNext();
    }
}

void Test::TestCompareFilesUI()
{
    Application::AddTestTab(new CompareFilesUI());
}

void Test::TestCompareFilesUI3Way()
{
    Application::AddTestTab(new CompareFilesUI(true));
}