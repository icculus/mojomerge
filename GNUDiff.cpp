/*  GNUDiff.cpp - Implementation for the GNUDiff class
 *
 */

#include <stdlib.h>
#include <time.h>

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "GNUDiff.h"

#ifdef WIN32
    #define mktemp _mktemp
    #define unlink _unlink
    #define popen _popen
#endif

using namespace MojoMerge;

GNUDiff::GNUDiff(const char *DiffPath, const char *Diff3Path,
    const char *TempFolder)
{
    // Set defaults for member variables 
    DiffResult = NULL;

	// Paths must not be NULL
	assert(DiffPath);
	assert(Diff3Path);
    assert(TempFolder);
	// Length of paths must not exceed MOJO_MAX_PATH
	assert(strlen(DiffPath) < MOJO_MAX_PATH - 1);
	assert(strlen(Diff3Path) < MOJO_MAX_PATH - 1);
    assert(strlen(TempFolder) < MOJO_MAX_PATH - 1);

	// Save the paths in our member variables
	strcpy(this->DiffPath, DiffPath);
	strcpy(this->Diff3Path, Diff3Path);
    strcpy(this->TempFolder, TempFolder);
}

GNUDiff::~GNUDiff()
{
}

Hunk *GNUDiff::CompareFiles(DiffOptions Options, const char *Buffer1,
	const char *Buffer2, const char *Buffer3)
{
	// Names of temp files we write
	char File1[MOJO_MAX_PATH];
	char File2[MOJO_MAX_PATH];
	char File3[MOJO_MAX_PATH];
	// Hunk returned from exec'ing diff
	Hunk *DiffHunk = NULL;			

	// First two buffers must not be NULL
	assert(Buffer1);
	assert(Buffer2);

	// Write buffers to temporary files so that the diff program
	//  can access them
	WriteTempFile(Buffer1, File1);
	WriteTempFile(Buffer2, File2);

	// Only process Buffer3 if it's specified
	if(Buffer3)
	{
		// Write third buffer to temp file
		WriteTempFile(Buffer3, File3);
		// Run the diff3 application
		DiffHunk = RunDiff3(Options, File1, File2, File3);
		// Remove the third diff file
		RemoveTempFile(File3);
	}
	// Else, it's only a two-way diff
	else
	{
		// Run the diff application
		DiffHunk = RunDiff(Options, File1, File2);
	}

	// Remove temp diff files
	RemoveTempFile(File1);
	RemoveTempFile(File2);
    // Deallocate the DiffResult buffer if it was allocated
    if(DiffResult)
    {
        delete DiffResult;
        DiffResult = NULL;
    }

	return DiffHunk;
}

void GNUDiff::WriteTempFile(const char *Buffer, char *File)
{
	char Template[] = "tmp_mojoXXXXXX";
	char *TmpFile = NULL;
	FILE *FileStream = NULL;

	// Create the full filename/path combination
	strcpy(File, TempFolder);
	strcat(File, Template);
	// TODO - Make sure _mktemp or equivelant is available on all platforms.
	// Create a unique filename
	TmpFile = mktemp(File);
	// If TmpFile is NULL, we've got a problem
	assert(TmpFile);

	// Open the file
	FileStream = fopen(File, "wb");
	// File stream can't be NULL
	assert(FileStream);
	// Write buffer to the file
	fwrite(Buffer, sizeof(char), strlen(Buffer), FileStream);
	// Close the file
	assert(fclose(FileStream) == 0);
}

void GNUDiff::RemoveTempFile(const char *File)
{
	// Delete the file
	assert(unlink(File) == 0);
}

void GNUDiff::GetDiffOutput(const char *Path, const char *CommandLine)
{
    // Characters to use to separate path and commandline
    #define SEPARATOR   " "
    // Full path and commandline combination
    char FullCommandLine[MOJO_MAX_PATH];
    // Stream used for reading data from diff output
    FILE *Pipe = NULL;
    // Number of bytes read from diff output
    size_t NumBytesRead;

    // Path and CommandLine can't be NULL
    assert(Path);
    assert(CommandLine);
    // Total length of commandline + path has to be less than MOJO_MAX_PATH
    assert(strlen(Path) + strlen(CommandLine) + strlen(SEPARATOR) < 
        MOJO_MAX_PATH);
    // Combine the path and commandline
    strcpy(FullCommandLine, Path);
    strcat(FullCommandLine, SEPARATOR);
    strcat(FullCommandLine, CommandLine);

    // Allocate buffer for diff result
    DiffResult = new char[DIFF_RESULT_BUFFER_SIZE];
    // Memory allocation error
    assert(DiffResult);
    // Run the diff program
    Pipe = popen(FullCommandLine, "rb");
    // Pipe can't be NULL
    assert(Pipe);
    // Read the output of 'diff'
    NumBytesRead = fread(DiffResult, sizeof(char), DIFF_RESULT_BUFFER_SIZE,
        Pipe);
    // If buffer is full, diff outputted too much data
    assert(NumBytesRead < DIFF_RESULT_BUFFER_SIZE);
    // Append terminating null char to end of buffer so it can be processed
    //  by the parser
    DiffResult[NumBytesRead] = 0x00;
}

Hunk *GNUDiff::RunDiff(DiffOptions Options, const char *File1,
    const char *File2)
{
    // Commandline we pass to the 'diff' program
    char CommandLine[MOJO_MAX_PATH];

    // Files can't be NULL
    assert(File1);
    assert(File2);
    // Files can't be empty strings
    assert(strlen(File1) > 0);
    assert(strlen(File2) > 0);
    // Combination of file name length plus space can't be too big
    assert(strlen(File1) + strlen(File2) + 1 < MOJO_MAX_PATH - 1);

    strcpy(CommandLine, File1);
    strcat(CommandLine, " ");
    strcat(CommandLine, File2);

	// TODO - Add Options support for 'diff'
    GetDiffOutput(DiffPath, CommandLine);
    // Parse the output and return any hunks found
    return ParseDiff();
}

Hunk *GNUDiff::RunDiff3(DiffOptions Options, const char *File1,
    const char *File2, const char *File3)
{
    // Commandline we pass to the 'diff3' program
    char CommandLine[MOJO_MAX_PATH];

    // Files can't be NULL
    assert(File1);
    assert(File2);
    assert(File3);
    // Files can't be empty strings
    assert(strlen(File1) > 0);
    assert(strlen(File2) > 0);
    assert(strlen(File3) > 0);
    // Combination of file name length plus two spaces can't be too big
    assert(strlen(File1) + strlen(File2) + strlen(File3) + 2 < 
        MOJO_MAX_PATH - 1);

    strcpy(CommandLine, File1);
    strcat(CommandLine, " ");
    strcat(CommandLine, File2);
    strcat(CommandLine, " ");
    strcat(CommandLine, File3);

	// TODO - Options are not supported by 'diff3'
    GetDiffOutput(Diff3Path, CommandLine);
    // Parse the output and return any hunks found
    return ParseDiff3();
}

Hunk *GNUDiff::ParseDiff()
{
	uint32 i,j;
	uint32 iError, iParseHunkError;
	const char *pBuffer = NULL;
	char cTemp;
	char sLine[256];
	uint32 Start1, End1, Start2, End2;
    Hunk *FirstHunk = NULL;     // First hunk created in list
    Hunk *LastHunk = NULL;      // Last hunk created in list

	// Use local pointer
	pBuffer = DiffResult;

	// Initialize variables, and output buffer
	j = iError = 0;

	// Get first character of the input buffer.
	cTemp = *pBuffer;
	// Loop starting a 2nd character
	i = 0;
	// Loop until end of input buffer
	while(cTemp != NULL)
	{
		// Get first character of this line of the input buffer.
		cTemp = *pBuffer++;
		// Is it an ASCII digit 0-9 or =? All "Hunk" lines begin with 0-9 or =.
		if((cTemp >= '0' && cTemp <= '9') || (cTemp == '='))
		{
			// Yes. Put this line in to output the buffer
			j = 0;
			while(cTemp != '\n' && cTemp != NULL)
			{
				sLine[j++] = cTemp;
				cTemp = *pBuffer++;
			}
			// Add newline character
			sLine[j++] = '\n';
			sLine[j] = NULL;
			// Parse this "Hunk"
			iParseHunkError = ParseDiff2Hunk(sLine, &Start1, &End1, &Start2,
                &End2);
			// Any errors from the parsing function?
			if(iParseHunkError)
				// Set the error flag
				iError = -1;
            // Create new hunk object that represents the diff
            LastHunk = new Hunk(LastHunk, Start1, End1, Start2, End2);
            // If FirstHunk is NULL, this is our first hunk.  Retain it as
            //  the first one in the list.
            if(FirstHunk == NULL)
                FirstHunk = LastHunk;
		}
		else
		{
			// No. Move to next line
			while(cTemp != '\n' && cTemp != NULL)
			{
				cTemp = *pBuffer++;
			}
		}
	}

	// Assertion fails if we get an error parsing diff3 output
	assert(!iError);
	return FirstHunk;
}

Hunk *GNUDiff::ParseDiff3()
{
    uint32 j;
    uint32 iError;
    char cValue[8];
    char cTemp;
    const char *pBuffer = NULL;
    bool bHunkStart;
    uint32 *pStart = NULL, *pEnd = NULL;
    uint32 Start1, End1, Start2, End2, Start3, End3;
    DiffFileNumber DiffFile;
    Hunk *FirstHunk = NULL;     // First hunk created in list
    Hunk *LastHunk = NULL;      // Last hunk created in list

    // Use local buffer pointer
    pBuffer = DiffResult;
    // Get first character of the input buffer.
    cTemp = *pBuffer;
    // Preset all values to -1
    Start1 = End1 = Start2 = End2 = Start3 = End3 = UNSPECIFIED;
    DiffFile = DiffFile_Unspecified;
    // Preset for no error
    iError = 0;
    // Preset for Start of Hunk NOT found yet
    bHunkStart = false;
    // Loop until end of input buffer
    while(cTemp != NULL)
    {
        // Find lines starting with "1,2,3,="
        switch((cTemp = *pBuffer++))
        {
        case '1':
        case '2':
        case '3':
            // Did we find the start of this Hunk?
            if(!bHunkStart)
                // No. Set error
                iError = -1;
            if(cTemp == '1')
            {
                pStart = &Start1;
                pEnd = &End1;
            }
            else if(cTemp == '2')
            {
                pStart = &Start2;
                pEnd = &End2;
            }
            else
            {
                pStart = &Start3;
                pEnd = &End3;
            }
            // Now calculate and save Start and End values
            if((cTemp = *pBuffer++) == ':')
            {
                j = 0;
                // Save all digits
                while(((cTemp = *pBuffer++) >= '0') && (cTemp <= '9'))
                    cValue[j++] = cTemp;
                // Null terminate string
                cValue[j] = NULL;
                // Find any digits?
                if(j>0)
                {
                    // Yes. Convert value
                    *pStart = int(strtoul(cValue,NULL,10));
                    // See if End value
                    if(cTemp == ',')
                    {
                        // Should be End value here
                        j = 0;
                        // Save all digits
                        while(((cTemp = *pBuffer++) >= '0') && (cTemp <= '9'))
                            cValue[j++] = cTemp;
                        // Null terminate string
                        cValue[j] = NULL;
                        // If any digits, convert value; Else error
                        if(j>0)
                            *pEnd = int(strtoul(cValue,NULL,10));
                        else
                            iError = -1;
                    }
                    // If single range and 'c' command, make End = Start
                    if((cTemp == 'c') && (*pEnd == UNSPECIFIED))
                        *pEnd = *pStart;
                }
                else
                    iError = -1;
            }
            else
                iError = -1;
            break;
        case '=':
            // Verify "====" and get file number at end (if any)
            if((cTemp = *pBuffer++) == '=')
            {
                if((cTemp = *pBuffer++) == '=')
                {
                    if((cTemp = *pBuffer++) == '=')
                    {
                        // If get here, found start of Hunk
                        // Clear error
                        bHunkStart = true;
                        // The file number would be here if there is one.
                        if(((cTemp = *pBuffer++) == '1') || (cTemp == '2') ||
                        (cTemp == '3'))
                            // DiffFile is 0 based
                            DiffFile = (DiffFileNumber)(int(cTemp & 0x0f) - 1);
                    }
                    else
                        iError = -1;
                }
                else
                    iError = -1;
            }
            else
                iError = -1;
            break;
        }

        // Have we found all the parameters for this Hunk?
        if((bHunkStart) && (Start1 != UNSPECIFIED) && (Start2 != UNSPECIFIED)
        && (Start3 != UNSPECIFIED))
        {
            // Create new hunk object that represents the diff
            LastHunk = new Hunk(LastHunk, Start1, End1, Start2, End2, Start3,
                End3, DiffFile);
            // If FirstHunk is NULL, this is our first hunk.  Retain it as
            //  the first one in the list.
            if(FirstHunk == NULL)
                FirstHunk = LastHunk;
            // Preset all values to UNSPECIFIED
            Start1 = End1 = Start2 = End2 = Start3 = End3 =UNSPECIFIED;
            DiffFile = DiffFile_Unspecified;
            // Preset for no error
            iError = 0;
            // Preset for Start of Hunk NOT found yet
            bHunkStart = false;
        }

        // Find end of line or end of buffer
        while((cTemp != '\n') && (cTemp != NULL))
            cTemp = *pBuffer++;
    }

	// Assertion fails if we get an error parsing diff3 output
	assert(!iError);
    return FirstHunk;
}

int GNUDiff::ParseDiff2Hunk(const char *Line,
    uint32 *Start1, uint32 *End1,
    uint32 *Start2, uint32 *End2)
{
    char cTemp;
    char cCommand;
    char cValue[8];
    int j;

    // Preset for no error
    int iError = 0;
    // Preset for no range values
    *Start1 = *Start2 = *End1 = *End2 = UNSPECIFIED;

    // Hunk must start with a digit; Else error
    j = 0;
    // Save all digits
    while(((cTemp = *Line++) >= '0') && (cTemp <= '9'))
        cValue[j++] = cTemp;
    // Null terminate string
    cValue[j] = NULL;
    // Find any digits?
    if(j>0)
    {
        // Yes. Convert value
        *Start1 = int(strtoul(cValue,NULL,10));
        // See if End1 value
        if(cTemp == ',')
        {
            // Should be End1 value here
            j = 0;
            // Save all digits
            while(((cTemp = *Line++) >= '0') && (cTemp <= '9'))
                cValue[j++] = cTemp;
            // Null terminate string
            cValue[j] = NULL;
            // If any digits, convert value; Else error
            if(j>0)
                *End1 = int(strtoul(cValue,NULL,10));
            else
                iError = -1;
        }
    }
    else
        iError = -1;

    // Save command character for later use
    cCommand = cTemp;
    // Should now be at the command character "c,a,d"; Else error
    if(cTemp == 'c' || cTemp == 'a' || cTemp == 'd')
    {
        // Must be a digit here for Start2; Else error
        j = 0;
        // Save all digits
        while(((cTemp = *Line++) >= '0') && (cTemp <= '9'))
            cValue[j++] = cTemp;
        // Null terminate string
        cValue[j] = NULL;
        // Find any digits?
        if(j>0)
        {
            // Yes. Convert value
            *Start2 = int(strtoul(cValue,NULL,10));
            // See if End1 value
            if(cTemp == ',')
            {
                // Should be End2 value here
                j = 0;
                // Save all digits
                while(((cTemp = *Line++) >= '0') && (cTemp <= '9'))
                    cValue[j++] = cTemp;
                // Null terminate string
                cValue[j] = NULL;
                // If any digits, convert value; Else error
                if(j>0)
                    *End2 = int(strtoul(cValue,NULL,10));
                else
                    iError = -1;
            }
        }
        else
            iError = -1;


        // Check End1 value being UNSPECIFIED for "d" or "c" command (not "a").
        // If it is, make End1 the same as Start1.
        if((*End1 == UNSPECIFIED) && (cCommand != 'a'))
            *End1 = *Start1;
        // End1 must be UNSPECIFIED for the "a" commnand.
        else if((cCommand == 'a') && (*End1 != UNSPECIFIED))
            iError = -1;

        // Check End2 value being UNSPECIFIED for "a" or "c" command (not "d").
        // If it is, make End2 the same as Start2.
        if((*End2 == UNSPECIFIED) && (cCommand != 'd'))
            *End2 = *Start2;
        // End2 must be UNSPECIFIED for the "d" commnand.
        else if((cCommand == 'd') && (*End2 != UNSPECIFIED))
            iError = -1;
    }
    else
        iError = -1;

    return iError;
}
