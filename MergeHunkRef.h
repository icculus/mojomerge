/*  MergeHunkRef.h - Class derived from wxObjectRefData that is used to provide
 *  a relationship between wxWindow objects and Hunks.
 *
 *  Since this is derived from the wxObjectRefData, it will automatically be
 *  destructed when all of the wxObjects referencing no longer reference it
 */

#ifndef _MOJOREF_H_
#define _MOJOREF_H_

#include "wx/wx.h"

namespace MojoMerge
{
    class MergeHunkRef : public wxObjectRefData
    {
    public:
        /*  MergeHunkRef
         *      Constructs a MergeHunkRef object
         *  Params
         *      Data
         *          Pointer to create reference for
         *  Returns
         *      none
         */
        MergeHunkRef(Hunk *TheHunk, DiffFileNumber Source, DiffFileNumber Dest)
        {
            // TheHunk can't be NULL
            assert(TheHunk);
            // Save member variables
            this->TheHunk = TheHunk;
            this->Source = Source;
            this->Dest = Dest;
        }
        /*  GetXXX
         *      Returns the data associated with "xxx"
         *  Params
         *      none
         *  Returns
         *      none
         */
        Hunk *GetHunk()
        {
            return TheHunk;
        }
        DiffFileNumber GetSource()
        {
            return Source;
        }
        DiffFileNumber GetDest()
        {
            return Dest;
        }
    private:
        Hunk *TheHunk;
        DiffFileNumber Source;
        DiffFileNumber Dest;
    };
}
#endif
