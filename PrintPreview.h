/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  PrintPreview.h - Provides an interface to preview information contained in a
 *      Merge object.
 */
namespace MojoMerge
{
    class PrintPreview
    {
    public:
        /*  PrintPreview Constructor
         *  Params
         *      MyMerge
         *          Reference to a merge object which has already performed
         *          a comparison
         *  Returns
         *      none
         */
        PrintPreview(Merge *MyMerge);

        /*  Print Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~PrintPreview();
    private:
    }
}