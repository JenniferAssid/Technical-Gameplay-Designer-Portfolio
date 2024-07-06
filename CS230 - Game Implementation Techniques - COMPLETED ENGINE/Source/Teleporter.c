//------------------------------------------------------------------------------
//
// File Name:	Teleporter.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Teleporter.h"
#include "AEEngine.h"

#include "Vector2D.h"
#include "Physics.h"
#include "Transform.h"
#include "GameObject.h"

#define TRIM 0

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

// When a game object goes off-screen, teleport it to the other side.
// Params:
//	 gameObject = Pointer to the game object to be checked.
void TeleporterUpdateObject(GameObjectPtr gameObject)
{
  Vector2D velocity = *PhysicsGetVelocity(GameObjectGetPhysics(gameObject));
  Vector2D position = *TransformGetTranslation(GameObjectGetTransform(gameObject));

  if (velocity.x > 0.0f && position.x > AEGfxGetWinMaxX() + TRIM)
    Vector2DSet(&position, AEGfxGetWinMinX(), position.y);

  if (velocity.x < 0.0f && position.x < AEGfxGetWinMinX() - TRIM)
    Vector2DSet(&position, AEGfxGetWinMaxX(), position.y);

  if (velocity.y > 0.0f && position.y > AEGfxGetWinMaxY() + TRIM)
    Vector2DSet(&position, position.x, AEGfxGetWinMinY());

  if (velocity.y < 0.0f && position.y < AEGfxGetWinMinY() - TRIM)
    Vector2DSet(&position, position.x, AEGfxGetWinMaxY());

  TransformSetTranslation(GameObjectGetTransform(gameObject), &position);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

