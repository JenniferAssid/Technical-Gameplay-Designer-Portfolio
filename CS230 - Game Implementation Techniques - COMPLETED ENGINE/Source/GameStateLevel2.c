//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel2.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLevel2.h"
#include "Trace.h"
#include "../AE/include/AEEngine.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "Physics.h"
#include "Transform.h"
#include "Sprite.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const float spaceshipSpeed = 500.0f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList* pMeshSpaceship;
static GameObjectPtr spaceship;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
void GameStateLevel2MovementController(GameObjectPtr gameObject);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Level 2 game state.
void GameStateLevel2Load()
{
	// Creates a "unit" sized triangular mesh.
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMeshSpaceship = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshSpaceship, "Failed to create spaceship mesh!");

}

// Initialize the memory associated with the Level 2 game state.
void GameStateLevel2Init()
{
	spaceship = GameObjectFactoryBuild("SpaceshipHoming");
	if (spaceship)
	{
		SpriteSetMesh(GameObjectGetSprite(spaceship), pMeshSpaceship);
	}

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Level 2 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel2Update(float dt)
{
	if (AEInputCheckCurr('Z'))
		SpriteSetAlpha(GameObjectGetSprite(spaceship), 0.5f);

	if (AEInputCheckCurr('X'))
		SpriteSetAlpha(GameObjectGetSprite(spaceship), 1.0f);

	GameStateLevel2MovementController(spaceship);
	GameObjectUpdate(spaceship, dt);
	GameObjectDraw(spaceship);

	// Gamestate Changing
	if (AEInputCheckTriggered('1'))
		GameStateManagerSetNextState(GsLevel1);

	if (AEInputCheckTriggered('2'))
		GameStateManagerSetNextState(GsRestart);

	if (AEInputCheckTriggered('3'))
		GameStateManagerSetNextState(GsAsteroids);

	if (AEInputCheckTriggered('4'))
		GameStateManagerSetNextState(GsOmega);

	if (AEInputCheckTriggered('9'))
		GameStateManagerSetNextState(GsSandbox);

	if (AEInputCheckTriggered('0'))
		GameStateManagerSetNextState(GsDemo);
}

// Free any memory associated with the Level 2 game state.
void GameStateLevel2Shutdown()
{
	// Free all objects.
	GameObjectFree(&spaceship);
}

// Unload the resources associated with the Level 2 game state.
void GameStateLevel2Unload()
{
	// Free all sprite sources.

	// Unload all textures.

	// Free all meshes.
	AEGfxMeshFree(pMeshSpaceship);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void GameStateLevel2MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr objectPhysics = GameObjectGetPhysics(gameObject);
	TransformPtr objectTransform = GameObjectGetTransform(gameObject);

	if (objectPhysics && objectTransform)
	{
		/*signed long mouseX, mouseY;
		Vector2D mouse, mouseInWorld;
		
		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY,
																				 &mouseInWorld.x, &mouseInWorld.y);
		
		Vector2DSet(&mouse, (float)mouseX, (float)mouseY);
		Vector2D translation = *(TransformGetTranslation(spaceshipTransform));
		Vector2DSub(&translation, &mouse, &translation);
		Vector2DNormalize(&translation, &translation);*/

		signed long mouseX, mouseY;				// Will store the mouse position (In the SCREEN coordinates system)
		float mouseInWorldX, mouseInWorldY;		// Will store the mouse position (In the WORLD coordinates system

		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &mouseInWorldX, &mouseInWorldY);

		Vector2D translation = *(TransformGetTranslation(objectTransform));

		Vector2D direction;
		direction.x = mouseInWorldX - translation.x;
		direction.y = mouseInWorldY - translation.y;

		Vector2DNormalize(&direction, &direction);
		TransformSetRotation(objectTransform, Vector2DToAngleRad(&direction));

		Vector2DScale(&direction, &direction, spaceshipSpeed);
		PhysicsSetVelocity(objectPhysics, &direction);
	}
}
