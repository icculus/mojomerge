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
         *      FileThatIsDifferent
         *          Indicates which file in a three-way diff is different
         *          for the specified hunk.  If UNSPECIFIED, that indicates
         *          that all files are different for that particular hunk.
         *  Returns
         *      none
         */
        Hunk(Hunk *Prev, uint32 Start1, uint32 End1, uint32 Start2, uint32 End2,
            uint32 Start3 = UNSPECIFIED, uint32 End3 = UNSPECIFIED, 
            DiffFileNumber FileThatIsDifferent = DiffFile_Unspecified);

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

        /*  GetDiffFile
         *      Gets the file that is different in a three-way diff
         *  Params
         *      none
         *  Returns
         *      File number that is different from the other two.  If all
         *      three files are different, or it's a two-way diff, then
         *      the return value is UNSPECIFIED
         */
        DiffFileNumber GetDiffFile();

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

        /*  Replace
         *      Removes the hunk from the list and replaces it with the
         *      specified hunk.  If we are not part of a list, this function
         *      has no affect.  Note that this does not delete the original
         *      hunk from memory.
         *  Params
         *      NewHunk
         *          Hunk that will replace this hunk.  It is assumed that this
         *          hunk is not part of another list.  This value can be NULL
         *          which indicates that the hunk should not be replaced with
         *          a new one.
         *      ChangedFile
         *          File that got changed in the old hunk that created the new hunk
         *      Offset
         *          Number to offset all hunks that occur after the one being
         *          replaced.  This is useful if the new hunk is a different
         *          size than the original one, therefore inadvertantly
         *          shifting all hunks after it.  0 is also valid to indicate
         *          no offset.
         *  Returns
         *      none
         */
        void Replace(Hunk *NewHunk, DiffFileNumber ChangedFile, sint32 Offset);
    private:
        // Pointers to previous and next hunk in the list
        Hunk *Prev;
        Hunk *Next;
        // Line numbers associated with hunk
        uint32 Start[MAX_DIFF_FILES];
        uint32 End[MAX_DIFF_FILES];
        // File that is different in a three-way diff
        DiffFileNumber FileThatIsDifferent;
    };
}

#endif