/*  Test.cpp - Implementation of the Test class
 *
 */

#include "Test.h"
#include "GNUDiff.h"
#include "Application.h"
#include <stdio.h>

using namespace MojoMerge;

#define TEST_DIFFPATH       "C:\\cygwin\\bin\\diff.exe"
#define TEST_DIFF3PATH      "C:\\cygwin\\bin\\diff3.exe"
// Maximum file size that the test files can be
#define TEST_MAXFILESIZE    10240
// Names of each test file used for diff testing
#define TEST_FILE1          "testfile1.txt"
#define TEST_FILE2          "testfile2.txt"
#define TEST_FILE3          "testfile3.txt"

Hunk *Test::TestGNUDiff_TwoWay()
{
    Application::Debug("Performing GNUDiff two-way test...");

    Hunk *MyHunk;
    Hunk *p;

    // Object used to perform diffs
    GNUDiff MyDiff(TEST_DIFFPATH, TEST_DIFF3PATH, Application::GetTempFolder());
    // Buffers that contain data from each file being compared
    char Buffer1[TEST_MAXFILESIZE];
    char Buffer2[TEST_MAXFILESIZE];
    
    ReadTestFile(TEST_FILE1, Buffer1);
    ReadTestFile(TEST_FILE2, Buffer2);

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
    char Buffer1[TEST_MAXFILESIZE];
    char Buffer2[TEST_MAXFILESIZE];
    char Buffer3[TEST_MAXFILESIZE];
    
    ReadTestFile(TEST_FILE1, Buffer1);
    ReadTestFile(TEST_FILE2, Buffer2);
    ReadTestFile(TEST_FILE3, Buffer3);

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

    Application::Debug("GNUDiff three-way test complete!\n");
    return MyHunk;
}

void Test::ReadTestFile(const char *Filename, char *Buffer)
{
    // Number of characters read from file
    size_t CharsRead;

    // Filename or Buffer can't be NULL
    assert(Filename);
    assert(Buffer);
    // Open file for reading
    FILE *Stream = fopen(Filename, "rb");
    // Stream can't be NULL
    assert(Stream);
    // TODO - Read the file regardless of file size
    // Read contents of file into buffer (up to TEST_MAXFILESIZE bytes)
    CharsRead = fread(Buffer, sizeof(char), TEST_MAXFILESIZE - 1, Stream);
    // Add the terminating null character
    Buffer[CharsRead] = 0x00;
    // Close the file stream
    int i = fclose(Stream);
    // Make sure file closed successfully
    assert(i == 0);
}