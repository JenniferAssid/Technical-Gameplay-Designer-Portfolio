//------------------------------------------------------------------------------
//
// File Name:	GameStateSandbox.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateSandbox.h"
#include "Trace.h"
#include "Transform.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const float enemySpeed = 250.0f;
static const float agroRadi = 100.0f;
Vector2D patrolPoints[3] = { 0 };
Vector2D currentPatrolPoint;
int currentPatrolPointIndex;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList* pMeshEnemy;
static GameObjectPtr enemy;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
void GameStateSandboxMovementController(GameObjectPtr gameObject);
static void GameStateSandboxTraceFloat(const char* text, float f);
static void GameStateSandboxTraceVector(const char* text, const Vector2D* v);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateSandboxLoad()
{
	// Creates a "unit" sized triangular mesh.
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMeshEnemy = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshEnemy, "Failed to create enemy mesh!");
}

// Initialize the memory associated with the Stub game state.
void GameStateSandboxInit()
{
	enemy = GameObjectCreate();

	if (enemy)
	{
		SpritePtr enemySprite = SpriteCreate();
		GameObjectAddSprite(enemy, enemySprite);
		SpriteSetMesh(GameObjectGetSprite(enemy), pMeshEnemy);

		PhysicsPtr enemyPhysics = PhysicsCreate();
		GameObjectAddPhysics(enemy, enemyPhysics);

		patrolPoints[0].x = -300.0f;
		patrolPoints[0].y = 150.0f;

		patrolPoints[1].x = 300.0f;
		patrolPoints[1].y = 150.0f;
		
		patrolPoints[2].x = 0.0f;
		patrolPoints[2].y = -150.0f;

		TransformPtr enemyTransform = TransformCreate();
		GameObjectAddTransform(enemy, enemyTransform);

		Vector2D scale = { 100, 80 };
		TransformSetScale(GameObjectGetTransform(enemy), &scale);
		TransformSetTranslation(GameObjectGetTransform(enemy), &patrolPoints[0]);

		currentPatrolPointIndex = 0;
		currentPatrolPoint = patrolPoints[0];
	}

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateSandboxUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	//UNREFERENCED_PARAMETER(dt);

	GameStateSandboxMovementController(enemy);
	GameObjectUpdate(enemy, dt);
	GameObjectDraw(enemy);

	// Gamestate Changing
	if (AEInputCheckTriggered('1'))
		GameStateManagerSetNextState(GsLevel1);

	if (AEInputCheckTriggered('2'))
		GameStateManagerSetNextState(GsLevel2);

	if (AEInputCheckTriggered('3'))
		GameStateManagerSetNextState(GsAsteroids);

	if (AEInputCheckTriggered('4'))
		GameStateManagerSetNextState(GsOmega);

	if (AEInputCheckTriggered('9'))
		GameStateManagerSetNextState(GsRestart);

	if (AEInputCheckTriggered('0'))
		GameStateManagerSetNextState(GsDemo);

	/*Stream vectorTest = StreamOpen("Data/VectorTests.txt");

	if (vectorTest)
	{
		Vector2D result;
		float scale;
		float value;

		Vector2DZero(&result);
		GameStateSandboxTraceVector("Vector2DZero", &result);

		Vector2DSet(&result, 1.0f, 1.5f);
		GameStateSandboxTraceVector("Vector2DSet", &result);

		Vector2DNeg(&result, &result);
		GameStateSandboxTraceVector("Vector2DNeg", &result);

		Vector2DAdd(&result, &result, &result);
		GameStateSandboxTraceVector("Vector2DAdd", &result);

		Vector2DSub(&result, &result, &result);
		GameStateSandboxTraceVector("Vector2DSub", &result);

		StreamReadVector2D(vectorTest, &result);
		GameStateSandboxTraceVector("v", &result);

		Vector2DNormalize(&result, &result);
		GameStateSandboxTraceVector("Vector2DNormalize", &result);

		scale = StreamReadFloat(vectorTest);
		GameStateSandboxTraceFloat("scale", scale);

		Vector2DScale(&result, &result, scale);
		GameStateSandboxTraceVector("Vector2DScale", &result);

		Vector2DScaleAdd(&result, &result, &result, scale);
		GameStateSandboxTraceVector("Vector2DScaleAdd", &result);

		Vector2DScaleSub(&result, &result, &result, scale);
		GameStateSandboxTraceVector("Vector2DScaleSub", &result);

		value = Vector2DLength(&result);
		GameStateSandboxTraceFloat("Vector2DLength", value);

		value = Vector2DSquareLength(&result);
		GameStateSandboxTraceFloat("Vector2DSquareLength", value);

		Vector2D v1, v2;

		StreamReadVector2D(vectorTest, &v1);
		GameStateSandboxTraceVector("p1", &v1);

		StreamReadVector2D(vectorTest, &v2);
		GameStateSandboxTraceVector("p2", &v2);

		value = Vector2DDistance(&v1, &v2);
		GameStateSandboxTraceFloat("Vector2DDistance", value);

		value = Vector2DSquareDistance(&v1, &v2);
		GameStateSandboxTraceFloat("Vector2DSquareDistance", value);

		value = Vector2DDotProduct(&v1, &v2);
		GameStateSandboxTraceFloat("Vector2DDotProduct", value);

		float angle;

		angle = StreamReadFloat(vectorTest);
		GameStateSandboxTraceFloat("deg", angle);

		Vector2DFromAngleDeg(&result, angle);
		GameStateSandboxTraceVector("Vector2DFromAngleDeg", &result);

		angle = StreamReadFloat(vectorTest);
		GameStateSandboxTraceFloat("rad", angle);

		Vector2DFromAngleRad(&result, angle);
		GameStateSandboxTraceVector("Vector2DFromAngleRad", &result);

		value = Vector2DToAngleRad(&result);
		GameStateSandboxTraceFloat("Vector2DToAngleRad", value);

		StreamClose(&vectorTest);

		//GameStateManagerSetNextState(GsQuit);
	}

	//GameStateManagerSetNextState(GsQuit);*/
}

// Free any memory associated with the Stub game state.
void GameStateSandboxShutdown()
{
	// Free all objects.
	GameObjectFree(&enemy);
}

// Unload the resources associated with the Stub game state.
void GameStateSandboxUnload()
{
	// Free all sprite sources.

	// Unload all textures.

	// Free all meshes.
	AEGfxMeshFree(pMeshEnemy);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void GameStateSandboxMovementController(GameObjectPtr enemyObject)
{
	Vector2D directionalVector;
	float margin = 1.0f;

	Vector2D enemyTranslation = *(TransformGetTranslation(GameObjectGetTransform(enemyObject)));

	if (enemyTranslation.x >= (currentPatrolPoint.x - margin) &&
		enemyTranslation.x <= (currentPatrolPoint.x + margin) &&
		enemyTranslation.y >= (currentPatrolPoint.y - margin) &&
		enemyTranslation.y <= (currentPatrolPoint.y + margin))
	{
		if (currentPatrolPointIndex == 2)
			currentPatrolPointIndex = 0;
		else
			currentPatrolPointIndex++;

		currentPatrolPoint = patrolPoints[currentPatrolPointIndex];
	}

	Vector2DSub(&directionalVector, &currentPatrolPoint, &enemyTranslation);
	Vector2DNormalize(&directionalVector, &directionalVector);

	//float rotation = Vector2DToAngleRad(&directionalVector);
	
	//TransformSetRotation(GameObjectGetTransform(enemyObject), rotation);

	Vector2DScale(&directionalVector, &directionalVector, enemySpeed);
	PhysicsSetVelocity(GameObjectGetPhysics(enemyObject), &directionalVector);
}

/*static void GameStateSandboxTraceFloat(const char* text, float f)
{
	TraceMessage("Vector Test: %s = %f", text, f);
}

static void GameStateSandboxTraceVector(const char* text, const Vector2D* v)
{
	TraceMessage("Vector Test: %s = [%f, %f]", text, v->x, v->y);
}*/
