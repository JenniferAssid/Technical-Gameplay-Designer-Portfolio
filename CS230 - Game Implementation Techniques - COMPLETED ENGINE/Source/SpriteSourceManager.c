//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceManager.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSourceManager.h"
#include "SpriteSource.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
int sourceMax = 255;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct SpriteSourceManager
{
	// The number of sprite sources currently in the list.
	unsigned int spriteSourceCount;

	// A list of all currently loaded sprite sources.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	SpriteSourcePtr	spriteSourceList[255];

} SpriteSourceManager;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
static SpriteSourceManager sourceManager;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
SpriteSourcePtr SpriteSourceManagerFind(const char* name);
void SpriteSourceManagerAdd(SpriteSourcePtr spriteSource);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Sprite Source Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void SpriteSourceManagerInit()
{
	memset(&sourceManager, 0, sizeof(sourceManager));
}

// Create a SpriteSource and add it to the SpriteSource manager.
// 1: Call SpriteSourceManagerFind() to determine if the sprite source already exists
// 2: If the named sprite source does not already exist,
//	  a: Use sprintf_s() to construct a path name using SpriteSourceName
//	     (HINT: The correct path name should be constructed using "Data/&s.txt".)
//	  b: Call StreamOpen(), passing the pathname
//    c: If the stream was opened successfully,
//		 1: Call SpriteSourceRead() to construct a SpriteSource using data read from the file
//		 2: Call SpriteSourceManagerAdd(), passing the created SpriteSource
// 3: Return the SpriteSource (already existing or just created)
// Params:
//	 SpriteSourceName = The name of the SpriteSource to be created.
// Returns:
//	 If the SpriteSource already exists or was created successfully,
//	   then return a pointer to the SpriteSource,
//	   else return NULL.
SpriteSourcePtr SpriteSourceManagerBuild(const char* spriteSourceName)
{
	SpriteSourcePtr source = SpriteSourceManagerFind(spriteSourceName);

	if (source == NULL)
	{
		char pathName[FILENAME_MAX] = "";
		sprintf_s(pathName, _countof(pathName), "Data/%s.txt", spriteSourceName);

		Stream file = StreamOpen(pathName);

		if (file == NULL) return NULL;

		source = SpriteSourceRead(file);
		SpriteSourceManagerAdd(source);
		StreamClose(&file);
	}

	return source;
}

// Free all SpriteSourcees in the SpriteSource Manager.
// (NOTE: Make sure to call SpriteSourceFree() to free each SpriteSource.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void SpriteSourceManagerFreeAll()
{
	int i;
	for (i = 0; i < sourceMax; i++)
	{
		if (sourceManager.spriteSourceList[i] != NULL)
		{
			SpriteSourceFree(&sourceManager.spriteSourceList[i]);
			sourceManager.spriteSourceCount--;
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
SpriteSourcePtr SpriteSourceManagerFind(const char* name)
{
	int i;
	for (i = 0; i < sourceMax; i++)
	{
		if (SpriteSourceIsNamed(sourceManager.spriteSourceList[i], name))
			return sourceManager.spriteSourceList[i];
	}

	return NULL;
}

void SpriteSourceManagerAdd(SpriteSourcePtr spriteSource)
{
	int i;
	for (i = 0; i < sourceMax; i++)
	{
		if (sourceManager.spriteSourceList[i] == NULL)
		{
			sourceManager.spriteSourceList[i] = spriteSource;
			sourceManager.spriteSourceCount++;
			return;
		}
	}
}
