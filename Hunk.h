/*  Hunk.h - This class represents a change in a diff comparison
 *
 */

#ifndef _HUNK_H_
#define _HUNK_H_

#include "DataTypes.h"

namespace MojoMerge
{
    class Hunk
    {
    public:
        /*  Hunk Constructor
         *      Sets the range of lines for each of the three files associated
         *      with the hunk.
         *  Params
         *      Prev
         *          Reference to a previous Hunk in a linked list.  If the
         *          hunk is at the head of a linked list, this parameter
         *          should be set to NULL or not specified.
         *      Startx
         *          Specifies the beginning of the hunk in file x
         *      Endx
         *          Specifies the end of the hunk in file x.  If Endx is
         *          set to UNSPECIFIED then Startx is treated as an "insert
         *          after" line number.
         *      Start3
         *          Specifies the beginning of the hunk in file 3.  This
         *          parameter is optional when dealing with a two-way
         *          file comparison.
         *      End3
         *          Specifies the end of the hunk in file 3.  If Endx is
         *          set to UNSPECIFIED then Start3 is treated as an "insert
         *          after" line number.  This parameter is optional when
         *          dealing with a two-way file comparison.
         *  Returns
         *      none
         */
        Hunk(Hunk *Prev, uint32 Start1, uint32 End1, uint32 Start2, uint32 End2,
            uint32 Start3 = UNSPECIFIED, uint32 End3 = UNSPECIFIED);

        /*  Hunk Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~Hunk();

        /*  GetStart()
         *      Get start line number of specified file.
         *  Parameters
         *      FileNumber
         *          Number of file to get the start line from
         *  Returns
         *      Start position of hunk, or UNSPECIFIED.
         */
        uint32 GetStart(DiffFileNumber FileNumber);

        /*  GetEndx()
         *      Get end line number of hunk for file x.
         *  Parameters
         *      FileNumber
         *          Number of file to get the start line from
         *  Returns
         *      End position of hunk, or UNSPECIFIED.
         */
        uint32 GetEnd(DiffFileNumber FileNumber);

        /*  GetPrevHunk and GetNextHunk
         *      Returns the previous or next hunk in the linked list respectively.
         *  Parameters
         *      none
         *  Returns
         *      The previous or next hunk in the list or NULL if beginning or
         *      end of list.
         */
        Hunk *GetPrevHunk();
        Hunk *GetNextHunk();

        /*  CheckValidity
         *      Checks to see if the hunk lines are logically correct
         *  Params
         *      none
         *  Returns
         *      TRUE if valid, otherwise FALSE.
         */
        bool CheckValidity();

        /*  DeleteList
         *      Deallocates this hunk, and all hunks after it
         *  Params
         *      none
         *  Returns
         *      none
         */
        void DeleteList();
    private:
        #define MAX_DIFF_FILES  3

        // Pointers to previous and next hunk in the list
        Hunk *Prev;
        Hunk *Next;
        // Line numbers associated with hunk
        uint32 Start[MAX_DIFF_FILES];
        uint32 End[MAX_DIFF_FILES];
    };
}

#endif