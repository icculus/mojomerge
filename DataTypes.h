/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/* DataTypes.h - Generic data types used by objects in the MojoMerge namespace
 *
 */
#ifndef _DATATYPE_H_
#define _DATATYPE_H_

namespace MojoMerge
{
#ifdef WIN32
    #define DIR_SEPARATOR '\\'
#else
    #define DIR_SEPARATOR '/'
#endif

    typedef unsigned char       uint8;
    typedef signed char         sint8;
    typedef unsigned short      uint16;
    typedef signed short        sint16;
    typedef unsigned int        uint32;
    typedef signed int          sint32;

#if (defined _MSC_VER)
    typedef signed __int64      sint64;
    typedef unsigned __int64    uint64;
#else
    typedef unsigned long long  uint64;
    typedef signed long long    sint64;
#endif
    enum DiffFileNumber
    {
        DiffFile_Unspecified = -1,
        DiffFile_One = 0,
        DiffFile_Two = 1,
        DiffFile_Three = 2
    };
    #define MAX_DIFF_FILES  3

    // Constant used for setting line number arguments to "not specified"
    #define UNSPECIFIED     0xFFFFFFFF  

	// Maximum length of a file path
	#define MOJO_MAX_PATH	    1024
}

#endif
