/*  Print.h - Provides an interface to print information contained in a Merge
 *      object.
 */

#ifndef _PRINT_H_
#define _PRINT_H_

namespace MojoMerge
{
    class Print
    {
    public:
        /*  Print Constructor
         *  Params
         *      MyMerge
         *          Reference to a merge object which has already performed
         *          a comparison
         *  Returns
         *      none
         */
        Print(Merge *MyMerge);

        /*  Execute
         *      Starts printing
         *  Params
         *      none
         *  Returns
         *      none
         */
        void Execute();

        /*  Preview
         *      Previews the printing without actually doing the print
         *  Params
         *      none
         *  Returns
         *      none
         */
        void Preview();

        /*  Print Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~Print();
    private:
    }
}

#endif