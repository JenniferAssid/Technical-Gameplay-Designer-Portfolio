//------------------------------------------------------------------------------
//
// File Name:	GameStateOmega.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateOmega.h"
#include "Trace.h"



#include "MeshManager.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "AEEngine.h"
#include "SpriteSourceManager.h"
#include "BehaviorHudText.h"
#include "BehaviorAsteroid.h"
#include "Random.h"
#include "ScoreSystem.h"
//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const int cAsteroidSpawnInitial = 8; //8
static const int cAsteroidSpawnMaximum = 20; //20

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static int asteroidSpawnCount;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
//static void GameStateAsteroidsCreateHudElement(const char* objectName, const char* formatString, const int* watchValue);
static void GameStateOmegaSpawnAsteroidWave(void);
static void GameStateOmegaSpawnAsteroid(void);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateOmegaLoad()
{
	MeshManagerInit();
	SpriteSourceManagerInit();
	ScoreSystemInit();
}

// Initialize the memory associated with the Stub game state.
void GameStateOmegaInit()
{
	GameObjectPtr spaceship = GameObjectFactoryBuild("SpaceshipOmega");

	if (spaceship)
		GameObjectManagerAdd(spaceship);

	GameObjectPtr arena = GameObjectFactoryBuild("Arena");

	if (arena)
		GameObjectManagerAdd(arena);

	GameObjectPtr asteroidScore = GameObjectFactoryBuild("OmegaScore");
	if (asteroidScore)
		GameObjectManagerAdd(asteroidScore);

	GameObjectPtr asteroidsHighScore = GameObjectFactoryBuild("OmegaHighScore");
	if (asteroidsHighScore)
		GameObjectManagerAdd(asteroidsHighScore);

	GameObjectPtr asteroidsWave = GameObjectFactoryBuild("OmegaWave");
	if (asteroidsWave)
		GameObjectManagerAdd(asteroidsWave);

	ScoreSystemRestart();
	asteroidSpawnCount = cAsteroidSpawnInitial;
	GameStateOmegaSpawnAsteroidWave();

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateOmegaUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	if (GameObjectManagerGetObjectByName("Asteroid") == NULL)
		GameStateOmegaSpawnAsteroidWave();

	// Gamestate Changing
	if (AEInputCheckTriggered('1'))
		GameStateManagerSetNextState(GsLevel1);

	if (AEInputCheckTriggered('2'))
		GameStateManagerSetNextState(GsLevel2);

	if (AEInputCheckTriggered('3'))
		GameStateManagerSetNextState(GsAsteroids);

	if (AEInputCheckTriggered('4'))
		GameStateManagerSetNextState(GsRestart);

	if (AEInputCheckTriggered('9'))
		GameStateManagerSetNextState(GsSandbox);

	if (AEInputCheckTriggered('0'))
		GameStateManagerSetNextState(GsDemo);
}

// Free any memory associated with the Stub game state.
void GameStateOmegaShutdown()
{
	// Free all objects.
	GameObjectManagerShutdown();
}

// Unload the resources associated with the Stub game state.
void GameStateOmegaUnload()
{
	// Free all sprite sources.
	SpriteSourceManagerFreeAll();
	// Unload all textures.

	// Free all meshes.
	MeshManagerFreeAll();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void GameStateOmegaSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWaveCount();
	int i = 0;
	for (i = 0; i < asteroidSpawnCount; i++)
		GameStateOmegaSpawnAsteroid();

	asteroidSpawnCount++;

	if (asteroidSpawnCount > cAsteroidSpawnMaximum)
		asteroidSpawnCount = cAsteroidSpawnMaximum;
}

static void GameStateOmegaSpawnAsteroid(void)
{
	GameObjectPtr asteroid = GameObjectFactoryBuild("Asteroid");
	GameObjectManagerAdd(asteroid);
}