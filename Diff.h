/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  Diff.h - Base class for various Diff implementations
 *
 */

#ifndef _DIFF_H_
#define _DIFF_H_

#include "Hunk.h"
#include <stdlib.h>

namespace MojoMerge
{
	enum DiffOptions
	{
		DiffOption_None = 0x0000,
		DiffOption_IgnoreWhitespace = 0x0001,
		DiffOption_IgnoreLineEnding = 0x0002,
		DiffOption_IgnoreCase = 0x0004
	};

    class Diff
    {
    public:
        /*  CompareFiles
         *      Pure virtual function which derived classes must inherit.  This
         *      function shall be called to initiate a comparison given two or
         *      three files.  File1 and File2 must be specified.
         *
         *      Each buffer must be terminated with a null character.
         *  Params
		 *		Options
		 *			Any combination of one or more DiffOptions
         *      File1
         *          Buffer representing first file contents
         *      File2
         *          Buffer representing second file contents
         *      File3
         *          Buffer representing third file contents (NULL if two-way)
         *  Returns
         *      Reference to first hunk in linked list or NULL if files
         *      are identical.  Caller is responsible for deallocating this
         *      Hunk list by calling Hunk::DeleteList.
         */
        virtual Hunk *CompareFiles(DiffOptions Options, const char *Buffer1, const char *Buffer2,
            const char *Buffer3 = NULL) = 0;
    };
}

#endif
