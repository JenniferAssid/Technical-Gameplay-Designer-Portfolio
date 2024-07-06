//------------------------------------------------------------------------------
//
// File Name:	Mesh.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "AEEngine.h"
#include "Trace.h"
#include "SpriteSource.h"
#include "Matrix2D.h"
#include "Stream.h"

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

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a quadrilateral mesh using the Alpha Engine.
// Params:
//	 xHalfSize = The X half-size of the mesh.
//	 yHalfSize = The Y half-size of the mesh.
//   uSize = The U size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//   vSize = The V size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//	 name = A name for the mesh.  Currently used only for error messaging.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
AEGfxVertexList* MeshCreateQuad(float xHalfSize, float yHalfSize, float uSize, float vSize, const char* name)
{  
  AEGfxVertexList* pMesh1;

  AEGfxMeshStart();

  AEGfxTriAdd(
    -xHalfSize, -yHalfSize, 0xFF000000, 0.0f, vSize,
    xHalfSize, -yHalfSize, 0xFF000000, uSize, vSize,
    -xHalfSize, yHalfSize, 0xFF000000, 0.0f, 0.0f
  );
  AEGfxTriAdd(
    xHalfSize, -yHalfSize, 0xFF000000, uSize, vSize,
    xHalfSize, yHalfSize, 0xFF000000, uSize, 0.0f,
    -xHalfSize, yHalfSize, 0xFF000000, 0.0f, 0.0f
  );

  pMesh1 = AEGfxMeshEnd();

  if (pMesh1)
    return pMesh1;
  else
  {
    TraceMessage("Couldn't create %c mesh.", name);
    return NULL;
  }
}

// Create a mesh using data read from a file.
// (NOTE: First read an integer indicating the number of vertices to be read.)
// (NOTE: For each vertex, read two floats (position), a hexidecimal integer (color), and two floats (UV).)
// (HINT: Call AEGfxVertexAdd() to add a single vertex to the mesh.)
// Params:
//	 stream = Pointer to the data stream used for reading.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
AEGfxVertexList* MeshRead(Stream stream)
{
  if (stream == NULL) return NULL;

  AEGfxVertexList* pResult = NULL;
  const char* name = StreamReadToken(stream);

  if (strncmp(name, "Quad", sizeof(*name)) == 0)
  {
    Vector2D halfSizes = { 0 };
    StreamReadVector2D(stream, &halfSizes);
    int numCols = StreamReadInt(stream);
    int numRows = StreamReadInt(stream);
    const char* sourceName = StreamReadToken(stream);
    pResult = MeshCreateQuad(halfSizes.x, halfSizes.y, 1.0f / numCols, 1.0f / numRows, sourceName);
  }else if (strncmp(name, "Mesh", sizeof(*name)) == 0)
  {
    AEGfxMeshStart();

    int vertices = StreamReadInt(stream);

    int i = 0;
    while (i < vertices)
    {
      Vector2D position = { StreamReadFloat(stream), StreamReadFloat(stream) };
      int color = StreamReadHex(stream);
      Vector2D UV = { StreamReadFloat(stream), StreamReadFloat(stream) };

      AEGfxVertexAdd(position.x, position.y, color, UV.x, UV.y);

      i++;
    }

    pResult = AEGfxMeshEnd();
  }
  
  if (pResult)
    return pResult;
  else
  {
    TraceMessage("MeshRead: Failure");
    return NULL;
  }
}

// Render a mesh (textured or untextured) using the Alpha Engine.
// Params:
//	 mesh = Pointer to the mesh to be rendered.
//	 spriteSource = Pointer to sprite source used for rendering (this may be NULL).
//	 transform = The transformation matrix used to scale, rotate, and translate the sprite.
//	 frameIndex = The index into a sprite sheet; used for calculating UV offsets.
void MeshRender(AEGfxVertexList* mesh, SpriteSource* spriteSource, Matrix2D* transform, unsigned frameIndex)
{
  if (spriteSource)
  {
    float u, v;
    SpriteSourceGetUV(spriteSource, frameIndex, &u, &v);

    AEGfxTextureSet(SpriteSourceGetTexture(spriteSource), u, v);
  }

  AEGfxSetTransform(transform->m);
  AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

