/*  LineBuffer.h - Class to maintain a buffer of lines merged in and out
 *  during merge transactions.
 *
 *  IMPORTANT: Start and End values are always "1-based"
 *
 */
namespace MojoMerge
{
    class LineBuffer
    {
    public:
        /*  LineBuffer Constructor
         *  Params
         *      FileBuffer
         *          Buffer of characters that represent lines of text.
         *  Returns
         *      none
         */
        LineBuffer(const char *Buffer);

        /*  LineBuffer Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~LineBuffer();

        /*  MoveLinesFrom
         *      Move lines from Buffer to this LineBuffer
         *  Params
         *      Buffer
         *          Reference to a LineBuffer to move lines from
         *      Start
         *          Starting line in Buffer to move lines from
         *      End
         *          Ending line in Buffer to move lines from
         *      Dest
         *          Position in this LineBuffer to move lines to
         *  Returns
         *      none
         */
        void MoveLinesFrom(LineBuffer *Buffer, uint32 Start, uint32 End,
            uint32 Dest);

        /*  CopyLinesFrom
         *      Copy lines from Buffer to this LineBuffer
         *  Params
         *      Buffer
         *          Reference to a LineBuffer to copy lines from
         *      Start
         *          Starting line in Buffer to copy lines from
         *      End
         *          Ending line in Buffer to copy lines from
         *      Dest
         *          Position in this LineBuffer to copy lines to
         *  Returns
         *      none
         */
        void CopyLinesFrom(LineBuffer *Buffer, uint32 Start, uint32 End,
            uint32 Dest);

        /*  DeleteLines
         *      Delete lines from this LineBuffer
         *  Params
         *      Start
         *          Starting line to remove lines from
         *      End
         *          Ending line to remove lines from
         *  Returns
         *      none
         */
        void DeleteLines(uint32 Start, uint32 End);

        /*  ReplaceLines
         *      Replace lines in the buffer with the Lines LineBuffer
         *  Params
         *      Start
         *          Starting line to begin replacement
         *      End
         *          Ending line to end replacement
         *      Lines
         *          LineBuffer to insert
         *      Count
         *          Number of lines in the Lines array
         */
        void ReplaceLines(uint32 Start, uint32 End, LineBuffer *Lines,
            uint32 Count);

        /*  GetLine
        *       Returns a pointer to the line specified by LineNumber
        *   Params
        *       LineNumber
        *           Line to return
        *   Returns
        *       Pointer to the line, or NULL if invalid line number
        */
        const char *GetLine(uint32 LineNumber);
    private:
        char **Lines;
    }
}