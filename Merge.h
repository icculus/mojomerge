/*  Merge.h - Class for making merge transactions from hunks taken
 *  from a Diff class.  This class is responsible for doing any and all
 *  merges and manual edit changes to the buffer data for each file.
 *
 */
namespace MojoMerge
{
    class Merge
    {
    public:
        /*  Merge Constructor
         *  Params
         *      FirstHunk
         *          Reference to the first Hunk of a Diff comparison
         *      Buffer1
         *          Contents of file 1
         *      Buffer2
         *          Contents of file 2
         *      Buffer3
         *          Contents of file 3.  This parameter can be NULL if
         *          performing a two-way merge.
         *  Returns
         *      none
         */
        Merge(Hunk *FirstHunk, const char *Buffer1, const char *Buffer2,
            const char *Buffer3 = NULL);

        /*  ResolveDiff
         *      Resolves a difference contained in the specified hunk.  This
         *      function can be read as "make DestFileNumber look like
         *      SourceFileNumber".
         *      
         *      The passed hunk object is deleted.  If the hunk was only
         *      partially resolved (as would be the case in a three-way
         *      hunk), then a new Hunk object is created and placed in
         *      the linked list in the same position as the passed hunk.
         *
         *      IMPORTANT: Since the passed hunk object is deleted, you should
         *      no longer reference that object after a call to ResolveDiff.
         *
         *      Also, this function assumes that the hunk is part of the
         *      list associated with this Merge class.  Results are undefined
         *      if it is not.
         *  Params
         *      MergeHunk
         *          Diff hunk that is used to reference the differences that
         *          will be resolved.
         *      SourceFileNumber
         *          File number that the change will come from
         *      DestFileNumber
         *          File number that the change will be written to.  This file
         *          number must be different than SourceFileNumber.
         *      HunkResolved
         *          ResolveDiff writes a TRUE to this variable if the action
         *          resulting in the hunk being completely resolved.  Otherwise
         *          FALSE is written to this variable.
         *  Returns
         *      Returns a pointer to new hunk, or NULL if the hunk was
         *      completely resolved.
         */
        Hunk *ResolveDiff(Hunk *MergeHunk, DiffFileNumber SourceFileNumber,
            DiffFileNumber DestFileNumber);

        /*  Merge Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~Merge();
    private:
        // First hunk in linked list of changes.  This hunk is actually a bogus
        // hunk that never directly gets referenced.  It is used to establish
        // a head of the linked list that will never be removed.  To get to the
        // first hunk, use HunkHead->Next;
        Hunk *HunkHead;
        // Buffers for keeping track of merge changes for each file
        LineBuffer *Buffer1;
        LineBuffer *Buffer2;
        LineBuffer *Buffer3;
        // Log for undoing merges
        UndoBuffer *Undo;
    }
}