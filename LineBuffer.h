/*  LineBuffer.h - Class to maintain a buffer of lines merged in and out
 *  during merge transactions.
 *
 *  IMPORTANT: Start and End values are always "1-based"
 *
 */

#ifndef _LINEBUFFER_H_
#define _LINEBUFFER_H_

#include <stdlib.h>

namespace MojoMerge
{
    class LineBuffer
    {
    public:
        /*  LineBuffer Constructor
         *  Params
         *      FileBuffer
         *          Buffer of characters that represent lines of text.  This 
         *          paramater may be NULL if not initial buffer is desired.
         *  Returns
         *      none
         */
        LineBuffer(const char *Buffer = NULL);

        /*  LineBuffer Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        ~LineBuffer();

        /*  MoveLinesTo
         *      Move lines from this LineBuffer to Buffer
         *  Params
         *      Buffer
         *          Reference to a LineBuffer to move lines to
         *      Start
         *          Starting line to move lines from
         *      End
         *          Ending line to move lines from
         *      Dest
         *          Position in Buffer to move lines to insert after.
         *  Returns
         *      none
         */
        void MoveLinesTo(LineBuffer *Buffer, uint32 Start, uint32 End,
            uint32 Dest);

        /*  CopyLinesTo
         *      Copy lines from this LineBuffer to Buffer
         *  Params
         *      Buffer
         *          Reference to a LineBuffer to copy lines to
         *      Start
         *          Starting line to copy lines from
         *      End
         *          Ending line to copy lines from
         *      Dest
         *          Position in Buffer to copy lines to insert after.
         *  Returns
         *      none
         */
        void CopyLinesTo(LineBuffer *Buffer, uint32 Start, uint32 End,
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
        //void DeleteLines(uint32 Start, uint32 End);

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
    };
}

#endif