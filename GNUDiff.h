/*  GNUDiff.h - Implementation of Diff using the GNU Diff command-line tools
 *
 */

#ifndef _GNUDIFF_H_
#define _GNUDIFF_H_

#include "Diff.h"

namespace MojoMerge
{
    // Size in bytes that will reserve for the 'diff' output result
    #define DIFF_RESULT_BUFFER_SIZE     1048576     

    class GNUDiff : public Diff
    {
    public:
        /*  Diff Constructor
         *  Params
         *      DiffPath
		 *			Full file and folder pathname of the GNU 'diff' program
		 *		Diff3Path
		 *			Full file and folder pathname of the GNU 'diff3' program
		 *		TempFolder
		 *			Location where temp files can go.  This path must have
		 *			a trailing '/' or '\' depending on platform.
         *  Returns
         *      none
         */
        GNUDiff(const char *DiffPath, const char *Diff3Path, const char *TempFolder);

        /*  Diff Destructor
         *  Params
         *      none
         *  Returns
         *      none
         */
        virtual ~GNUDiff();

		/*** Overrides ***/
        /*  CompareFiles
         *  See Diff::CompareFiles for more information
         */
        virtual Hunk *CompareFiles(DiffOptions Options, const char *Buffer1,
			const char *Buffer2, const char *Buffer3 = NULL);

    private:
		// Path to GNU 'diff' program
		char DiffPath[MOJO_MAX_PATH];
		// Path to GNU 'diff3' program
		char Diff3Path[MOJO_MAX_PATH];
		// Temp folder
		char TempFolder[MOJO_MAX_PATH];
        // Result of 'diff' or 'diff3' output.
        char *DiffResult;

		/*	WriteTempFile
		 *		Writes contents of Buffer to a new temporary file.
		 *	Params
		 *		Buffer
		 *			Bytes to write to the temporary file
		 *		File
		 *			Full file name and path of the temp file are written to
		 *			this variable.
		 *	Returns
		 *		none
		 */
		void WriteTempFile(const char *Buffer, char *File);
		/*	RemoveTempFile
		 *		Removes the specified temp file
		 *	Params
		 *		File
		 *			Full file name and path of file to remove
		 *	Returns
		 *		none
		 */
		void RemoveTempFile(const char *File);

        /*  GetDiffOutput
         *      Runs the program specified in 'Path' and writes the output to
         *      the DiffResult member
         *  Params
         *      Path
         *          Full file and path representing program to execute
         *      CommandLine
         *          Optional commandline parameters (can't be NULL).  Specify
         *          an empty string ("") instead.
         *  Returns
         *      none
         */
        void GetDiffOutput(const char *Path, const char *CommandLine);

		/*	RunDiff
		 *		Runs the 'diff' application and calls ParseDiff on the result.
		 *	Params
		 *		Options
		 *			Any combination of one or more DiffOption values
		 *		File1
		 *			Name of first file to diff
		 *		File2
		 *			Name of second file to diff
		 *	Returns
		 *		Pointer to the first diff hunk.  Returns NULL if no differences
		 *		were encountered, or an error occured.  The caller is
		 *		responsible for deallocating this object.
		 */
		Hunk *RunDiff(DiffOptions Options, const char *File1, const char *File2);

		/*	RunDiff3
		 *		Runs the 'diff3' application and calls ParseDiff3 on the result.
		 *	Params
		 *		Options
		 *			Any combination of one or more DiffOption values
		 *		File1
		 *			Name of first file to diff
		 *		File2
		 *			Name of second file to diff
		 *		File3
		 *			Name of third file to diff
		 *	Returns
		 *		Pointer to the first diff hunk.  Returns NULL if no differences
		 *		were encountered, or an error occured.  The caller is
		 *		responsible for deallocating this object.
		 */
		Hunk *RunDiff3(DiffOptions Options, const char *File1, const char *File2,
			const char *File3);

		/*  ParseDiff3
		 *      Parses "normal" output of the 'diff3' GNU tool.  Reads data from
         *      NULL terminated DiffResult member.
		 *  Params
		 *      none
		 *          
		 *  Returns
		 *      NULL if no differences encountered, otherwise returns the first
		 *      hunk in the list of differences.
		 *  Assumptions:
		 *     1. All Hunks start with "====n" at the beginning of the line. Where:
		 *        "n" = 1,2,3 or blank.
		 *     2. File lines start with "f:Rc" or "f:La". Where:
		 *        "f" = 1,2,3
		 *        "R" = a range value in the form "#,#" or "#"
		 *        "L" = a single "#"
		 *
		 */
		Hunk *ParseDiff3();

		/*	ParseDiff
		 *		Parses a "normal" output of the 'diff' GNU tool.  Reads data from
         *      NULL terminated DiffResult member.
         *  Params
         *      none
		 *	Returns
		 *      NULL if no differences encountered, otherwise returns the first
		 *      hunk in the list of differences.
		 *	Assumptions:
		 *     1. Hunks are definded as "RdL", "RcR", or "LaR" at the beginning of a line. Where:
		 *        "R" = a range value in the form "#,#" or "#"
		 *        "L" = a single "#"
		 *     Ref:
		 *        "d" indicates "delete"
		 *        "c" indicates "change"
		 *        "a" indicates "append"
		 */
		Hunk *ParseDiff();

		/*  ParseDiff2Hunk
		 *  Parses a single line of "Hunk" code from "diff" output.
		 *  Params
		 *      Line
		 *          The NULL terminated "Hunk" line to parse
		 *      Startx
		 *          Returned value, specifies the beginning of the hunk in file x
		 *      Endx
		 *          Returned value, specifies the end of the hunk in file x.
		 *          If Endx is -1, then Startx is treated as an "insert after" line number.
		 *  Returns
		 *      -1 if any error, else 0
		 *
		 * Assumptions:
		 *     1. Hunks are definded as "RdL", "RcR", or "LaR" at the beginning of the line. Where:
		 *        "R" = a range value in the form "#,#" or "#"
	 	 *        "L" = a single "#"
		 *     Ref:
		 *        "d" indicates "delete"
		 *        "c" indicates "change"
		 *        "a" indicates "append"
		 */
        int ParseDiff2Hunk(const char *Line, uint32 *Start1, uint32 *End1,
            uint32 *Start2, uint32 *End2);
	};
}

#endif