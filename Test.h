/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  Test.h - This class creates test cases for various MojoMerge objects.
 *
 */

#ifndef _TEST_H_
#define _TEST_H_

#include "Hunk.h"
#include "FolderHunk.h"

namespace MojoMerge
{
    class Test
    {
    public:
        /*  TestGNUDiff_TwoWay
         *      Tests the two-way diff functionality in GNUDiff and sends the
         *      output results to the debug window
         *  Params
         *      none
         *  Returns
         *      First hunk of a hunk list, or NULL if no differences found.
         */
        static Hunk *TestGNUDiff_TwoWay();

        /*  TestGNUDiff_ThreeWay
         *      Tests the three-way diff functionality in GNUDiff and sends the
         *      output results to the debug window
         *  Params
         *      none
         *  Returns
         *      First hunk of a hunk list, or NULL if no differences found.
         */
        static Hunk *TestGNUDiff_ThreeWay();

        /*  TestLineBuffer
         *      Tests the functionality of the LineBuffer object
         *  Params
         *      none
         *  Returns
         */
        static void TestLineBuffer();

        /*  TestMerge_TwoWay
         *      Tests the two-way merge functionality by merging the result of
         *      the TestGNUDiff_TwoWay
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void TestMerge_TwoWay();

        /*  TestStack
         *      Tests the Stack object
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void TestStack();

        /*  TestCompareFolders
         *      Tests the CompareFolders object
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void TestCompareFolders();

        /*  TestCompareFilesUI
         *      Tests the CompareFilesUI object
         *
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void TestCompareFilesUI();
        /*  TestCompareFilesUI3Way
         *      Tests the CompareFilesUI object with 3-way diff
         *  Params
         *      none
         *  Returns
         *      none
         */
        static void TestCompareFilesUI3Way();
    private:
        static void PrintHunks(FolderHunk *FirstHunk, int Indent);
    };
}

#endif
