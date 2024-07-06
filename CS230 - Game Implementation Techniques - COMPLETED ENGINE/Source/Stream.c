//------------------------------------------------------------------------------
//
// File Name:	Stream.c
//				Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stream.h"
#include "Vector2D.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static char tokenBuffer[1024];
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Open a file for reading.
// (NOTE: Use fopen_s() with the "rt" option.)
// (NOTE: You must generate the following trace message if the file was not opened successfully:
//	  TraceMessage("Error: StreamOpen could not open file %s; %s", fileName, errorMsg);
//	  Where errorMsg is a char array containing the error message obtained using strerror_s().)
// Params:
//	 filename = Name of the file to be opened for reading.
// Returns:
//	 NULL if the file name is null or the file could not be opened.
//	 Pointer to a FILE object if the file was opened successfully.
Stream StreamOpen(const char* filename)
{
	errno_t err;
	Stream fp;

	// Determine if file can be opened
	err = fopen_s(&fp, filename, "rt");

	// If file failed to open...
	if (err)
	{
		// Create char buffer to hold generated error message
		char errorMsg[256];

		// Place the error message in the errorMsg buffer
		strerror_s(errorMsg, 256, errno);

		// Print out the specified and generated error messages to console
		TraceMessage("Error:  StreamOpen could not open file %s: %s", filename, errorMsg);

		return NULL;
	}
	else
	{
		// Return pointer to successfully opened file
		return fp;
	}

}

// Read a single boolean value from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return a boolean value read from the file as an integer,
//	   else return 0.
bool StreamReadBoolean(Stream stream)
{
	if (!stream)
		return 0;

	int value;

	// Scans given stream for an integer and places found integer in value
	fscanf_s(stream, "%d", &value);

	return value;
}

// Read a single hexadecimal integer from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an hexadecimal integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return an unsigned integer value read from the file,
//	   else return 0.
unsigned StreamReadHex(Stream stream)
{
	// Checks if given stream is invalid
	if (stream == NULL)
		return 0;

	unsigned int value;

	// Scans given stream for an integer and places found integer in value
	fscanf_s(stream, "%x", &value);

	// Returns found integer
	return value;
}

// Read a single integer from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 0 if the stream was not opened succesfully;
//	 otherwise, an integer value read from the file.
int StreamReadInt(Stream stream)
{
	// Checks if given stream is invalid
	if (stream == NULL)
		return 0;

	int value;

	// Scans given stream for an integer and places found integer in value
	fscanf_s(stream, "%d", &value);

	// Returns found integer
	return value;
}

// Read a single float from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for a float.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return a float value read from the file,
//	   else return 0.
float StreamReadFloat(Stream stream)
{
	if (stream == NULL)
		return 0;

	float value;

	fscanf_s(stream, "%f", &value);

	if (value)
		return value;
	else
		return 0;
}

// Read the data for a Vector2D from a stream.
// (NOTE: Verify that the stream and vector pointer are valid first.)
// (HINT: Use StreamReadFloat() to read the x and y values, in sequence.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream and vector pointer are both valid,
//	   then fill the vector with two float values (x & y),
//	   else do nothing (optionally, write an error message to the trace log).
void StreamReadVector2D(Stream stream, Vector2D* vector)
{
	if (stream == NULL || vector == NULL)
		return;

	fscanf_s(stream, "%f", &vector->x);
	fscanf_s(stream, "%f", &vector->y);
}

// Read a token (a single word) from a file.
// Suggested steps:
//	 - Set the first value in tokenBuffer to 0
//	 - If the stream was opened successfully,
//	   - Read a string ("%s") into tokenBuffer using fscanf_s()
//	 - Return tokenBuffer
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 Pointer to tokenBuffer
const char* StreamReadToken(Stream stream)
{
	tokenBuffer[0] = 0;
	const char* tokenBufferp = &tokenBuffer[0];

	if (stream)
	{
		fscanf_s(stream, "%s", tokenBuffer, _countof(tokenBuffer));
		return tokenBufferp;
	}
	else
		return NULL;
}

// Close an opened stream.
// (NOTE: Do not attempt to close the stream if the pointer is null.)
// Params:
//	 stream = The file stream to be closed.
void StreamClose(Stream *stream)
{
	if (*stream == NULL)
		return;

	fclose(*stream);
	*stream = NULL;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

