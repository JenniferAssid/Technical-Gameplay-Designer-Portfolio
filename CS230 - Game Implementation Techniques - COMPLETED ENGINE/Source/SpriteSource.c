//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"

#include "AEEngine.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct SpriteSource
{
	char name[32];

	// The dimensions of the sprite sheet.
	int	numRows;
	int	numCols;

	// Pointer to a texture created using the Alpha Engine.
	AEGfxTexturePtr	pTexture;

} SpriteSource;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new sprite source object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 pTexture = A pointer to a texture that has been loaded by the Alpha Engine.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
SpriteSourcePtr SpriteSourceCreate(int numCols, int numRows, AEGfxTexturePtr pTexture)
{
	SpriteSourcePtr result = (SpriteSourcePtr)calloc(1, sizeof(SpriteSource));
	
	if (result)
	{
		result->numCols = numCols;
		result->numRows = numRows;
		result->pTexture = pTexture;

		return result;
	}

	return NULL;
}

// Free the memory associated with a sprite source object.
// (Also, set the sprite source pointer to NULL.)
// Params:
//	 spriteSource = Pointer to the sprite source pointer.
void SpriteSourceFree(SpriteSourcePtr* spriteSource)
{
	if ((*spriteSource)->pTexture)
		AEGfxTextureUnload((*spriteSource)->pTexture);

	free(*spriteSource);
	*spriteSource = NULL;
}

// Create a SpriteSource using data read from a file.
// Params:
//	 stream = Pointer to the data stream used for reading.
// Returns:
//	 If the sprite source was created successfully,
//	   then return a pointer to the created sprite source,
//	   else return NULL.
SpriteSourcePtr SpriteSourceRead(Stream stream)
{
	if (stream == NULL) return NULL;

	if (strcmp(StreamReadToken(stream), "SpriteSource") != 0) return NULL;

	SpriteSourcePtr source = SpriteSourceCreate(0, 0, NULL);
	if (source == NULL) return NULL;
	sprintf_s(source->name, sizeof(source->name), StreamReadToken(stream));

	source->numCols = StreamReadInt(stream);
	source->numRows = StreamReadInt(stream);
	source->pTexture = AEGfxTextureLoad(StreamReadToken(stream));

	return source;
}

// Determines if a sprite source has the specified name.
// Params:
//	 spriteSource = Pointer to the sprite source object.
//	 name = Pointer to the name to be compared.
// Returns:
//	 If the sprite source and name pointers are valid,
//		then perform a string comparison and return the result (match = true),
//		else return false.
bool SpriteSourceIsNamed(SpriteSourcePtr spriteSource, const char* name)
{
	if (spriteSource == NULL) return false;

	if (strncmp(spriteSource->name, name, _countof(spriteSource->name)) == 0)
		return true;

	return false;
}

// Returns a pointer to the texture, for the purposes of rendering a sprite.
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return a pointer to its texture,
//		else return NULL.
AEGfxTexturePtr SpriteSourceGetTexture(SpriteSourcePtr spriteSource)
{
	if (spriteSource)
		return spriteSource->pTexture;

	return NULL;
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// (Hint: Frame count = numCols * numRows.)
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return the calculated frame count,
//		else return 0.
unsigned int SpriteSourceGetFrameCount(SpriteSourcePtr spriteSource)
{
	if (spriteSource)
	{
		unsigned int maxFrameCount = (unsigned int)(spriteSource->numCols * spriteSource->numRows);
		return maxFrameCount;
	}
	return 0;
}

// Returns the UV coordinates of the specified frame in a sprite sheet.
// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
// Params:
//	 spriteSource = Pointer to the sprite source object.
//	 frameIndex = A frame index within a sprite sheet.
//   u = Pointer to a f32 to contain the U coordinate. 
//   v = Pointer to a f32 to contain the V coordinate. 
void SpriteSourceGetUV(SpriteSourcePtr spriteSource, unsigned int frameIndex, float* u, float* v)
{
	float uSize = 1.0f / spriteSource->numCols;
	float vSize = 1.0f / spriteSource->numRows;

	*u = uSize * (frameIndex % spriteSource->numCols);
	*v = vSize * (frameIndex / spriteSource->numCols);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

