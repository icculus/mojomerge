/*  UndoBuffer.h - Keeps a log of MergeBuffer transactions for later "playback"
 *
 */
namespace MojoMerge
{
    class UndoBuffer
    {
    public:
        /*  UndoBuffer Constructor
         *  Params
         *      Number of transactions to maintain in undo log
         *  Returns
         *      none
         */
        UndoBuffer(uint32 Size);

        /*  UndoBuffer Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~UndoBuffer();

        //!!!TODO
    private:
    }
}