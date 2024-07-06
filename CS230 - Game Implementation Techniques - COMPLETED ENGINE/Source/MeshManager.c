//------------------------------------------------------------------------------
//
// File Name:	MeshManager.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 4
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MeshManager.h"
#include "Stream.h"
#include "Mesh.h"
#include "AEEngine.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static int meshMax = 255;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct MeshManager
{
	// The number of game objects currently in the list.
	unsigned int meshCount;

	// The maximum number of game objects ever in the list, at any given moment.
	//unsigned int meshMax;

	// A list of all currently loaded meshes.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	AEGfxVertexList* meshList[255]; //meshListSize

} MeshManager;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static MeshManager meshManager;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
void MeshManagerAdd(AEGfxVertexList* mesh);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Mesh Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void MeshManagerInit()
{	
	memset(&meshManager, 0, sizeof(meshManager));
}

// Create a mesh and add it to the mesh manager.
// 1: Use sprintf_s() to construct a path name using meshName
//	   (HINT: The correct path name should be constructed using "Data/&s.txt".)
// 2: Call StreamOpen(), passing the pathname
// 3: If the stream was opened successfully,
//	  a: Call MeshRead() to construct a mesh using data read from the file
//	  b: Call MeshManagerAdd(), passing the created mesh
//	  c: Close the stream
//	  d: Return the created mesh
// Params:
//	 meshName = The name of the mesh to be created.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
AEGfxVertexList* MeshManagerBuild(const char* meshName)
{
	char pathName[FILENAME_MAX] = "";
	sprintf_s(pathName, _countof(pathName), "Data/%s.txt", meshName);

	Stream file = StreamOpen(pathName);
	if (file == NULL) return NULL;

	AEGfxVertexList* mesh = MeshRead(file);
	if (mesh == NULL) return NULL;
	MeshManagerAdd(mesh);
	StreamClose(&file);

	return mesh;
}

// Free all meshes in the Mesh Manager.
// (NOTE: Make sure to call AEGfxMeshFree() to free each mesh.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void MeshManagerFreeAll()
{
	int i = 0;
	while (i < meshMax)
	{
		if (meshManager.meshList[i] != NULL)
		{
			AEGfxMeshFree(meshManager.meshList[i]);
			meshManager.meshCount--;
		}

		i++;
	}
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void MeshManagerAdd(AEGfxVertexList* mesh)
{
	if (mesh == NULL) return;

	int i = 0;
	while (i < meshMax)
	{
		if (meshManager.meshList[i] == NULL)
		{
			meshManager.meshList[i] = mesh;
			meshManager.meshCount++;
			return;
		}

		i++;
	}
}
