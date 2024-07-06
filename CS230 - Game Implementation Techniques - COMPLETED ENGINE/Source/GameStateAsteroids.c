//------------------------------------------------------------------------------
//
// File Name:	GameStateAsteroids.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 4
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateAsteroids.h"

#include "GameStateManager.h"
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
static const int cAsteroidSpawnInitial = 8;
static const int cAsteroidSpawnMaximum = 20;

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
static void GameStateAsteroidsSpawnAsteroidWave(void);
static void GameStateAsteroidsSpawnAsteroid(void);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateAsteroidsLoad()
{
	MeshManagerInit();
	SpriteSourceManagerInit();
	ScoreSystemInit();
}

// Initialize the memory associated with the Stub game state.
void GameStateAsteroidsInit()
{
	GameObjectPtr spaceship = GameObjectFactoryBuild("Spaceship");

	if (spaceship)
		GameObjectManagerAdd(spaceship);

	GameObjectPtr asteroidScore = GameObjectFactoryBuild("AsteroidsScore");
	if (asteroidScore)
		GameObjectManagerAdd(asteroidScore);

	GameObjectPtr asteroidsHighScore = GameObjectFactoryBuild("AsteroidsHighScore");
	if (asteroidsHighScore)
		GameObjectManagerAdd(asteroidsHighScore);

	GameObjectPtr asteroidsWave = GameObjectFactoryBuild("AsteroidsWave");
	if (asteroidsWave)
		GameObjectManagerAdd(asteroidsWave);

	ScoreSystemRestart();
	asteroidSpawnCount = cAsteroidSpawnInitial;
	GameStateAsteroidsSpawnAsteroidWave();

	GameStateAsteroidsSpawnAsteroid();

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateAsteroidsUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	if (GameObjectManagerGetObjectByName("Asteroid") == NULL)
		GameStateAsteroidsSpawnAsteroidWave();

	// Gamestate Changing
	if (AEInputCheckTriggered('1'))
		GameStateManagerSetNextState(GsLevel1);

	if (AEInputCheckTriggered('2'))
		GameStateManagerSetNextState(GsLevel2);

	if (AEInputCheckTriggered('3'))
		GameStateManagerSetNextState(GsRestart);

	if (AEInputCheckTriggered('4'))
		GameStateManagerSetNextState(GsOmega);

	if (AEInputCheckTriggered('9'))
		GameStateManagerSetNextState(GsSandbox);

	if (AEInputCheckTriggered('0'))
		GameStateManagerSetNextState(GsDemo);
}

// Free any memory associated with the Stub game state.
void GameStateAsteroidsShutdown()
{
	// Free all objects.
	GameObjectManagerShutdown();
}

// Unload the resources associated with the Stub game state.
void GameStateAsteroidsUnload()
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
static void GameStateAsteroidsSpawnAsteroidWave(void)
{
	ScoreSystemIncreaseWaveCount();
	int i = 0;
	for (i = 0; i < asteroidSpawnCount; i++)
		GameStateAsteroidsSpawnAsteroid();

	asteroidSpawnCount++;

	if (asteroidSpawnCount > cAsteroidSpawnMaximum)
		asteroidSpawnCount = cAsteroidSpawnMaximum;
}

static void GameStateAsteroidsSpawnAsteroid(void)
{
	GameObjectPtr asteroid = GameObjectFactoryBuild("Asteroid");
	GameObjectManagerAdd(asteroid);
}
