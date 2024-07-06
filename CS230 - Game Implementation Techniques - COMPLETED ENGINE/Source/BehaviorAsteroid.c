//------------------------------------------------------------------------------
//
// File Name:	BehaviorAsteroid.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorAsteroid.h"
#include "Behavior.h"

#include "Random.h"
#include "Teleporter.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "Transform.h"
#include "GameObject.h"
#include "Physics.h"
#include "Collider.h"
#include "GameStateAsteroids.h"
#include "ScoreSystem.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
typedef enum
{
	cAsteroidLarge,
	cAsteroidMedium,
	cAsteroidSmall,

} AsteroidSize;

typedef enum
{
	cAsteroidOriginTlc,
	cAsteroidOriginTrc,
	cAsteroidOriginBlc,
	cAsteroidOriginBrc,
	cAsteroidOriginCount,

} AsteroidOrigin;

typedef enum AsteroidStates
{
	cAsteroidInvalid = -1,	// HUD Text has not yet been initialized.
	cAsteroidIdle,			// HUD Text will normally remain static.

} HudTextStates;

static const float asteroidSpeedMin = 50.0f;
static const float asteroidSpeedMax = 100.0f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct BehaviorAsteroid
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add asteroid-specific behavior variables.
	AsteroidSize	size;
	AsteroidOrigin	origin;

} BehaviorAsteroid, * BehaviorAsteroidPtr;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorAsteroidSetPosition(BehaviorAsteroidPtr);
static void BehaviorAsteroidSetVelocity(BehaviorAsteroidPtr);
static void BehaviorAsteroidCollisionHandler(GameObjectPtr, GameObjectPtr);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Asteroid) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorAsteroidCreate(void)
{
	BehaviorAsteroidPtr asteroid = (BehaviorAsteroidPtr)calloc(1, sizeof(BehaviorAsteroid));
	if (asteroid == NULL) return NULL;

	asteroid->base.memorySize = sizeof(BehaviorAsteroid);
	asteroid->base.stateCurr = cAsteroidInvalid;
	asteroid->base.stateNext = cAsteroidInvalid;
	asteroid->base.onInit = &BehaviorAsteroidInit;
	asteroid->base.onUpdate = &BehaviorAsteroidUpdate;
	asteroid->base.onExit = &BehaviorAsteroidExit;
	
	asteroid->size = cAsteroidLarge;

	return (BehaviorPtr)asteroid;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorAsteroidInit(BehaviorPtr behavior)
{
	if (behavior == NULL) return;

	BehaviorAsteroidPtr asteroid = (BehaviorAsteroidPtr)behavior;

	if (asteroid->base.stateCurr == cAsteroidIdle)
	{
		asteroid->origin = RandomRange(0, 3);
		BehaviorAsteroidSetPosition(asteroid);
		BehaviorAsteroidSetVelocity(asteroid);
		ColliderPtr collider = GameObjectGetCollider(asteroid->base.parent);
		if (collider) ColliderSetCollisionHandler(collider, BehaviorAsteroidCollisionHandler);
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorAsteroidUpdate(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	
	TeleporterUpdateObject(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorAsteroidExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void BehaviorAsteroidSetPosition(BehaviorAsteroidPtr asteroid)
{
	if (asteroid == NULL) return;

	Vector2D position = { 0 };

	if (asteroid->size == cAsteroidLarge)
	{
		switch (asteroid->origin)
		{
		case cAsteroidOriginTlc:
			Vector2DSet(&position, AEGfxGetWinMinX(), AEGfxGetWinMaxY());
			break;
		case cAsteroidOriginTrc:
			Vector2DSet(&position, AEGfxGetWinMaxX(), AEGfxGetWinMaxY());
			break;
		case cAsteroidOriginBlc:
			Vector2DSet(&position, AEGfxGetWinMinX(), AEGfxGetWinMinY());
			break;
		case cAsteroidOriginBrc:
			Vector2DSet(&position, AEGfxGetWinMaxX(), AEGfxGetWinMinY());
			break;
		}

		TransformSetTranslation(GameObjectGetTransform(asteroid->base.parent), &position);
	}
}

static void BehaviorAsteroidSetVelocity(BehaviorAsteroidPtr asteroid)
{
	if (asteroid == NULL) return;

	float rotation = 0.0f;

	if (asteroid->size == cAsteroidLarge)
	{
		switch (asteroid->origin)
		{
		case cAsteroidOriginTlc:
			rotation = RandomRangeFloat(-10.0f, -80.0f);
			break;
		case cAsteroidOriginTrc:
			rotation = RandomRangeFloat(-100.0f, -170.0f);
			break;
		case cAsteroidOriginBlc:
			rotation = RandomRangeFloat(10.0f, 80.0f);
			break;
		case cAsteroidOriginBrc:
			rotation = RandomRangeFloat(100.0f, 170.0f);
			break;
		}
	}
	else if (asteroid->size == cAsteroidMedium || asteroid->size == cAsteroidSmall)
		rotation = RandomRangeFloat(0.0f, 359.0f);

	Vector2D direction = { 0 };
	Vector2DFromAngleDeg(&direction, rotation);
	Vector2DScale(&direction, &direction, RandomRangeFloat(asteroidSpeedMin, asteroidSpeedMax));
	PhysicsSetVelocity(GameObjectGetPhysics(asteroid->base.parent), &direction);
}

static void BehaviorAsteroidCollisionHandler(GameObjectPtr collider, GameObjectPtr other)
{
	if (collider == NULL || other == NULL) return;

	if (GameObjectIsNamed(other, "Bullet") || GameObjectIsNamed(other, "Spaceship"))
	{
		ScoreSystemIncreaseScore(20);
		GameObjectDestroy(collider);
	}
}
