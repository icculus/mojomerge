/*  Test.h - This class creates test cases for various MojoMerge objects.
 *
 */

#ifndef _TEST_H_
#define _TEST_H_

#include "Hunk.h"

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
    private:
        static void ReadTestFile(const char *Filename, char *Buffer);
    };
}

#endif