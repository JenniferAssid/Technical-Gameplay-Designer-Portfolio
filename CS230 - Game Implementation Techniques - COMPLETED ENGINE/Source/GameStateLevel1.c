//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
//				Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateLevel1.h"
#include "Trace.h"
#include "Vector2D.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Transform.h"
#include "Vector2D.h"
#include "SpriteSource.h"
#include "Physics.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Matrix2D.h"
#include "Animation.h"



//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static int numLives1 = 0;
static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

static const float WallDistance = 350.0f;
static const float CheckSquareDistance = (75.0f * 75.0f);

const enum MonkeyStates{ MonkeyInvalid = -1, MonkeyIdle, MonkeyWalk, MonkeyJump };

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------


static AEGfxVertexList* mesh1;
static AEGfxTexture * tex1;
static SpriteSourcePtr spriteSource;
static GameObjectPtr gameObjectp;

static AEGfxVertexList* mesh3;
static AEGfxVertexList* mesh16;

static enum MonkeyStates monkeyState = MonkeyInvalid;
static char livesBuffer[16] = "";

// Monkey

static AEGfxTexture* monkeyIdle;
static AEGfxTexture* monkeyWalk;
static AEGfxTexture* monkeyJump;
static AEGfxTexture* fontSheet;

static SpriteSourcePtr monkeyIdleSource;
static SpriteSourcePtr monkeyWalkSource;
static SpriteSourcePtr monkeyJumpSource;
static SpriteSourcePtr fontSheetSource;


static GameObjectPtr monkeyGameObjectp;

// LivesText
static AEGfxVertexList* livesMesh;
static AEGfxTexture* livesTexture;
static GameObjectPtr livesGameObjectp;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
void GameStateLevel1MovementController(GameObjectPtr gameObject);
void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, enum MonkeyStates newState);
void GameStateLevel1BounceController(GameObjectPtr gameObject);
bool GameStateLevel1IsColliding(GameObjectPtr objectA, GameObjectPtr objectB);

void TesterFunction(void);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Level 1 game state.
void GameStateLevel1Load()
{
	// Tries to open file for lives in level 1
	Stream lives = StreamOpen("Data/Level1_Lives.txt");
	
	// If the stream has been opened successfully...
	if (lives)
	{
		// Assign numLives1 as the first integer in file
		numLives1 = StreamReadInt(lives);

		// Close the stream
		StreamClose(&lives);
	}

	mesh1 = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");
	tex1 = AEGfxTextureLoad("Assets/PlanetTexture.png");
	spriteSource = SpriteSourceCreate(1, 1, tex1);
	
	mesh3 = MeshCreateQuad(0.5f, 0.5f, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	mesh16 = MeshCreateQuad(0.5f, 0.5f, 1.0f / 16, 1.0f / 6, "Mesh16x16");
	
	monkeyIdle = AEGfxTextureLoad("Assets/MonkeyIdle.png");
	monkeyWalk = AEGfxTextureLoad("Assets/MonkeyWalk.png");
	monkeyJump = AEGfxTextureLoad("Assets/MonkeyJump.png");
	fontSheet = AEGfxTextureLoad("Assets/fontSheet.png");

	monkeyIdleSource = SpriteSourceCreate(1, 1, monkeyIdle);
	monkeyWalkSource = SpriteSourceCreate(3, 3, monkeyWalk);
	monkeyJumpSource = SpriteSourceCreate(1, 1, monkeyJump);
	fontSheetSource = SpriteSourceCreate(16, 6, fontSheet);

	TesterFunction();
}

// Initialize the memory associated with the Level 1 game state.
void GameStateLevel1Init()
{
	gameObjectp = GameObjectFactoryBuild("PlanetBounce");

	if (gameObjectp)
	{
		SpritePtr planetSprite = GameObjectGetSprite(gameObjectp);
		SpriteSetMesh(planetSprite, mesh1);
		SpriteSetSpriteSource(planetSprite, spriteSource);
		SpriteSetFrame(planetSprite, 0);
	}

	monkeyGameObjectp = GameObjectFactoryBuild("Monkey");

	if (monkeyGameObjectp)
	{
		SpritePtr monkeySprite = GameObjectGetSprite(monkeyGameObjectp);
		SpriteSetMesh(monkeySprite, mesh1);
		SpriteSetSpriteSource(monkeySprite, monkeyIdleSource);
		SpriteSetFrame(monkeySprite, 0);
		monkeyState = MonkeyInvalid;
		GameStateLevel1SetMonkeyState(monkeyGameObjectp, MonkeyIdle);
	}

	livesGameObjectp = GameObjectFactoryBuild("MonkeyLivesText");

	if (livesGameObjectp)
	{
		SpritePtr gameObjectSprite = GameObjectGetSprite(livesGameObjectp);
		SpriteSetMesh(gameObjectSprite, mesh16);
		SpriteSetSpriteSource(gameObjectSprite, fontSheetSource);
		sprintf_s(livesBuffer, sizeof(livesBuffer), "Lives: %d", numLives1);
		SpriteSetText(gameObjectSprite, livesBuffer);
	}

	AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Level 1 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	//UNREFERENCED_PARAMETER(dt);
	//AEFrameRateControllerGetFrameTime();

	GameStateLevel1MovementController(monkeyGameObjectp);
	GameStateLevel1BounceController(gameObjectp);
	GameObjectUpdate(monkeyGameObjectp, dt);
	GameObjectUpdate(gameObjectp, dt);
	GameObjectUpdate(livesGameObjectp, dt);
	GameObjectDraw(monkeyGameObjectp);
	GameObjectDraw(gameObjectp);
	GameObjectDraw(livesGameObjectp);

	if (GameStateLevel1IsColliding(gameObjectp, monkeyGameObjectp))
	{
		numLives1--;
		
		if (numLives1 <= 0)
			GameStateManagerSetNextState(GsLevel2);
		else
			GameStateManagerSetNextState(GsRestart);
	}

	// Gamestate Changing
	if (AEInputCheckTriggered('1'))
		GameStateManagerSetNextState(GsRestart);

	if (AEInputCheckTriggered('2'))
		GameStateManagerSetNextState(GsLevel2);

	if (AEInputCheckTriggered('3'))
		GameStateManagerSetNextState(GsAsteroids);

	if (AEInputCheckTriggered('4'))
		GameStateManagerSetNextState(GsOmega);

	if (AEInputCheckTriggered('9'))
		GameStateManagerSetNextState(GsSandbox);

	if (AEInputCheckTriggered('0'))
		GameStateManagerSetNextState(GsDemo);
	
}

// Free any memory associated with the Level 1 game state.
void GameStateLevel1Shutdown()
{
	GameObjectFree(&gameObjectp);
	GameObjectFree(&monkeyGameObjectp);
	GameObjectFree(&livesGameObjectp);
}

// Unload the resources associated with the Level 1 game state.
void GameStateLevel1Unload()
{
	// Free all sprite sources.
	SpriteSourceFree(&spriteSource);
	SpriteSourceFree(&monkeyIdleSource);
	SpriteSourceFree(&monkeyWalkSource);
	SpriteSourceFree(&monkeyJumpSource);
	SpriteSourceFree(&fontSheetSource);

	// Unload all textures.
	/*AEGfxTextureUnload(tex1);
	AEGfxTextureUnload(monkeyIdle);
	AEGfxTextureUnload(monkeyWalk);
	AEGfxTextureUnload(monkeyJump);
	AEGfxTextureUnload(fontSheet);
	AEGfxTextureUnload(livesTexture);*/


	// Free all meshes.
	AEGfxMeshFree(mesh1);
	AEGfxMeshFree(mesh3);
	AEGfxMeshFree(mesh16);
	AEGfxMeshFree(livesMesh);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void GameStateLevel1MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr physics = GameObjectGetPhysics(gameObject);
	TransformPtr transform = GameObjectGetTransform(gameObject);

	if (!physics || !transform)
		return;

	Vector2D velocity = *(PhysicsGetVelocity(physics));
	
	// Right - Left movement
	if (AEInputCheckCurr(VK_LEFT))
	{
		velocity.x = -1.0f * moveVelocity;
		if (monkeyState != MonkeyJump)
			GameStateLevel1SetMonkeyState(gameObject, MonkeyWalk);
	}
		
	if (AEInputCheckCurr(VK_RIGHT))
	{
		velocity.x = moveVelocity;
		if (monkeyState != MonkeyJump)
			GameStateLevel1SetMonkeyState(gameObject, MonkeyWalk);
	}
		
	if (!AEInputCheckCurr(VK_LEFT) && !AEInputCheckCurr(VK_RIGHT))
	{
		velocity.x = 0.0f;
		if (monkeyState != MonkeyJump)
			GameStateLevel1SetMonkeyState(gameObject, MonkeyIdle);
	}

	// Jump
	if (AEInputCheckTriggered(VK_UP))
	{
		velocity.y = jumpVelocity;
		PhysicsSetAcceleration(physics, &gravityNormal);
		GameStateLevel1SetMonkeyState(gameObject, MonkeyJump);
	}

	Vector2D translation = *(TransformGetTranslation(transform));

	if (translation.y < groundHeight)
	{
		translation.y = groundHeight;
		TransformSetTranslation(transform, &translation);
		velocity.y = 0.0f;
		PhysicsSetAcceleration(physics, &gravityNone);
		GameStateLevel1SetMonkeyState(gameObject, MonkeyIdle);
	}
	
	PhysicsSetVelocity(physics, &velocity);
}

void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, enum MonkeyStates newState)
{
	if (monkeyState != newState)
	{
		monkeyState = newState;
		SpritePtr gameObjectSprite = GameObjectGetSprite(gameObject);
		AnimationPtr gameObjectAnimation = GameObjectGetAnimation(gameObject);
		switch (newState)
		{
		case MonkeyIdle:
				SpriteSetMesh(gameObjectSprite, mesh1);
				SpriteSetSpriteSource(gameObjectSprite, monkeyIdleSource);
				AnimationPlay(gameObjectAnimation, 1, 0.0f, false);
				break;
		case MonkeyWalk:
			SpriteSetMesh(gameObjectSprite, mesh3);
			SpriteSetSpriteSource(gameObjectSprite, monkeyWalkSource);
			AnimationPlay(gameObjectAnimation, 8, 0.05f, true);
			break;
		case MonkeyJump:
			SpriteSetMesh(gameObjectSprite, mesh1);
			SpriteSetSpriteSource(gameObjectSprite, monkeyJumpSource);
			AnimationPlay(gameObjectAnimation, 1, 0.0f, false);
		}
	}
}

void GameStateLevel1BounceController(GameObjectPtr gameObject)
{
	PhysicsPtr gameObjectPhysics = GameObjectGetPhysics(gameObject);
	TransformPtr gameObjectTransform = GameObjectGetTransform(gameObject);

	if (gameObjectPhysics == NULL || gameObjectTransform == NULL)
		return;

	Vector2D currentPosition = *(TransformGetTranslation(gameObjectTransform));
	Vector2D currentVelocity = *(PhysicsGetVelocity(gameObjectPhysics));

	if (currentPosition.x <= -WallDistance)
	{
		currentPosition.x = -WallDistance;
		currentVelocity.x = -currentVelocity.x;
	}
	if (currentPosition.x >= WallDistance)
	{
		currentPosition.x = WallDistance;
		currentVelocity.x = -currentVelocity.x;
	}
	if (currentPosition.y <= groundHeight)
	{
		currentPosition.y = groundHeight + (groundHeight - currentPosition.y);
		currentVelocity.y = -currentVelocity.y;
	}

	PhysicsSetVelocity(gameObjectPhysics, &currentVelocity);
	TransformSetTranslation(gameObjectTransform, &currentPosition);
}

bool GameStateLevel1IsColliding(GameObjectPtr objectA, GameObjectPtr objectB)
{
	Vector2D objectAPosition = *(TransformGetTranslation(GameObjectGetTransform(objectA)));
	Vector2D objectBPosition = *(TransformGetTranslation(GameObjectGetTransform(objectB)));

	if (Vector2DSquareDistance(&objectAPosition, &objectBPosition) < CheckSquareDistance)
		return true;

	return false;
}

void TesterFunction(void)
{
	//////////////////////////// MESH.C TESTING /////////////////////////////////

	/*listOne = MeshCreateQuad(10.0f, 10.0f, 1.0f, 1.0f, "Mesh1x1");
	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);			// This step must be done first.
	// Set position for object 1
	AEGfxSetPosition(0.0f, 0.0f);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the mesh (list of triangles)
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(listOne, AE_GFX_MDM_TRIANGLES);*/
	
	////////////////////////// TRANSFORM.C TESTING //////////////////////////////

	/*TransformPtr test = TransformCreate(); //SUCCESS
	const Vector2D* v1 = (Vector2D*)calloc(1, sizeof(Vector2D));
	const Vector2D* v2 = (Vector2D*)calloc(1, sizeof(Vector2D));
	float temp;

	if (test)
		TraceMessage("TransformCreate:Success");
	else
		TraceMessage("TransformCreate:Failure");

	Stream transform = StreamOpen("Data/VectorTests.txt");

	if (!transform)
	{
		TraceMessage("StreamOpen: Failed");
		return;
	}

	//TraceMessage("StreamOpen:Success");

	TransformRead(test, transform); //Success

	
	Matrix2D* testMatrix = TransformGetMatrix(test);
	UNREFERENCED_PARAMETER(testMatrix);

	v1 = TransformGetTranslation(test); // Success
	//TraceMessage("TransformGetTranslation: Pointer Returned: %p", v1);

	temp = TransformGetRotation(test);
	//TraceMessage("TransformGetRotation: Rotation Returned: %f", temp);

	v2 = TransformGetScale(test);
	//TraceMessage("TransformGetScale: Pointer Returned: %p", v2);

	TransformSetTranslation(test, v2);

	TransformSetRotation(test, 7.0f);

	TransformSetScale(test, v1);

	StreamClose(&transform);

	if (!transform)
		TraceMessage("StreamClose: Success");
	else
		TraceMessage("StreamClose: Failure");

	TransformFree(&test);

	if (test) // SUCCESS
		TraceMessage("TransformFree: Failed to free pointer.");*/

	/////////////////////////// SPRITE.C TESTING ////////////////////////////////

	/*SpritePtr spriteTester = SpriteCreate();
	AEGfxVertexList* spriteMesh = MeshCreateQuad(10.0f, 10.0f, 1.0f, 1.0f, "SpriteTester");
	
	AEGfxTexture* texture = AEGfxTextureLoad("Assets/PlanetTexture.png");

	TransformPtr test = TransformCreate(); //SUCCESS
	SpriteSourcePtr spriteSourceTest = SpriteSourceCreate(1, 1, texture);


	if (spriteSourceTest)
		TraceMessage("SpriteSourceCreate: Success");
	else
		TraceMessage("SpriteSourceCreate: Failure");

	SpriteSetMesh(spriteTester, spriteMesh);
	SpriteSetSpriteSource(spriteTester, spriteSourceTest);

	SpriteSetFrame(spriteTester, 0);
	SpriteSetFrame(spriteTester, 1);

	SpriteDraw(spriteTester, test); // SUCCESS
	
	TransformFree(&test);

	SpriteFree(&spriteTester);

	if (spriteTester)
		TraceMessage("SpriteFree: Failure");
	else
		TraceMessage("SpriteFree: Success");

	SpriteSourceFree(&spriteSourceTest);

	if (spriteSourceTest)
		TraceMessage("SpriteSourceFree: Failure");
	else
		TraceMessage("SpriteSourceFree: Success");

	free(spriteMesh);*/
	
  /////////////////////////// PHYSICS.C TESTING ///////////////////////////////
	
	/*PhysicsPtr physicsObject = PhysicsCreate();
	TransformPtr transformObject = TransformCreate();

	if (physicsObject)
		TraceMessage("PhysicsCreate: Success");
	else
		TraceMessage("PhysicsCreate: Failure");

	Stream file = StreamOpen("Data/TransformTester.txt");
	PhysicsRead(physicsObject, file);
	TransformRead(transformObject, file);

	const Vector2D* v1 = PhysicsGetAcceleration(physicsObject); // Success
	TraceMessage("PhysicsGetAcceleration: x = %f, y = %f", v1->x, v1->y);
	const Vector2D* v2 = PhysicsGetVelocity(physicsObject); // Success
	TraceMessage("PhysicsGetVelocity: x = %f, y = %f", v2->x, v2->y);
	const Vector2D* v3 = PhysicsGetOldTranslation(physicsObject); // Success
	TraceMessage("PhysicsGetOldTranslation: x = %f, y = %f", v3->x, v3->y);

	PhysicsUpdate(physicsObject, transformObject, 0.01f); // Success
	v1 = PhysicsGetAcceleration(physicsObject);
	TraceMessage("PhysicsGetAcceleration: x = %f, y = %f", v1->x, v1->y);
	v2 = PhysicsGetVelocity(physicsObject);
	TraceMessage("PhysicsGetVelocity: x = %f, y = %f", v2->x, v2->y);
	v3 = PhysicsGetOldTranslation(physicsObject);
	TraceMessage("PhysicsGetOldTranslation: x = %f, y = %f", v3->x, v3->y);


	StreamClose(&file);
	TransformFree(&transformObject);
	PhysicsFree(&physicsObject);

	if (physicsObject)
		TraceMessage("PhysicsFree: Failure");
	else
		TraceMessage("PhysicsFree: Success");*/

  ////////////////////////// GAMEOBJECT.C TESTING /////////////////////////////

	/*GameObjectPtr gameObjectTest = GameObjectCreate(); // Success

	if (gameObjectTest)
		TraceMessage("GameObjectCreate: Success");
	else
		TraceMessage("GameObjectCreate: Failure");

	Stream file = StreamOpen("Data/SpaceshipHoming.txt");

	GameObjectRead(gameObjectTest, file);

	// Need to check Set functions
	TraceMessage("GameObjectGetName: %s", GameObjectGetName(gameObjectTest));

	Vector2D gameObjectV1 = { 0 };
	const Vector2D* gameObjectPhysicsV1 = &gameObjectV1;

	PhysicsPtr gameObjectPhysics = GameObjectGetPhysics(gameObjectTest);
	gameObjectPhysicsV1 = PhysicsGetAcceleration(gameObjectPhysics);
	TraceMessage("GameObjectGetPhysics: Acceleration: x = %f, y = %f",
		gameObjectPhysicsV1->x, gameObjectPhysicsV1->y);
	gameObjectPhysicsV1 = PhysicsGetVelocity(gameObjectPhysics);
	TraceMessage("GameObjectGetPhysics: Velocity: x = %f, y = %f",
		gameObjectPhysicsV1->x, gameObjectPhysicsV1->y);
	gameObjectPhysicsV1 = PhysicsGetOldTranslation(gameObjectPhysics);
	TraceMessage("GameObjectGetPhysics: OldTranslation: x = %f, y = %f",
		gameObjectPhysicsV1->x, gameObjectPhysicsV1->y);

	SpritePtr gameObjectSprite = GameObjectGetSprite(gameObjectTest);
	TraceMessage("GameObjectGetSprite: Alpha = %f", SpriteGetAlpha(gameObjectSprite));

	TransformPtr gameObjectTransform = GameObjectGetTransform(gameObjectTest);
	const Vector2D* gameObjectTransformV1 = TransformGetTranslation(gameObjectTransform);
	TraceMessage("GameObjectGetTransform: Translation: x = %f, y = %f",
		gameObjectTransformV1->x, gameObjectTransformV1->y);
	gameObjectTransformV1 = TransformGetScale(gameObjectTransform);
	TraceMessage("GameObjectGetTransform: Scale: x = %f, y = %f",
		gameObjectTransformV1->x, gameObjectTransformV1->y);
	TraceMessage("GetObjectGetTranslation: Rotation = %f", TransformGetRotation(gameObjectTransform));

	StreamClose(&file);
		
	GameObjectFree(&gameObjectTest); 

	if (gameObjectTest)
		TraceMessage("GameObjectFree: Failure");
	else
		TraceMessage("GameObjectFree: Success");*/

	////////////////////// GAMEOBJECTFACTORY.C TESTING //////////////////////////

  /*GameObjectPtr gameObjectTest = GameObjectFactoryBuild("Data/SpaceshipHoming.txt");

	TraceMessage("GameObjectGetName: %s", GameObjectGetName(gameObjectTest));

	Vector2D gameObjectV1 = { 0 };
	const Vector2D* gameObjectPhysicsV1 = &gameObjectV1;

	PhysicsPtr gameObjectPhysics = GameObjectGetPhysics(gameObjectTest);
	gameObjectPhysicsV1 = PhysicsGetAcceleration(gameObjectPhysics);
	TraceMessage("GameObjectGetPhysics: Acceleration: x = %f, y = %f",
		gameObjectPhysicsV1->x, gameObjectPhysicsV1->y);
	gameObjectPhysicsV1 = PhysicsGetVelocity(gameObjectPhysics);
	TraceMessage("GameObjectGetPhysics: Velocity: x = %f, y = %f",
		gameObjectPhysicsV1->x, gameObjectPhysicsV1->y);
	gameObjectPhysicsV1 = PhysicsGetOldTranslation(gameObjectPhysics);
	TraceMessage("GameObjectGetPhysics: OldTranslation: x = %f, y = %f",
		gameObjectPhysicsV1->x, gameObjectPhysicsV1->y);

	SpritePtr gameObjectSprite = GameObjectGetSprite(gameObjectTest);
	TraceMessage("GameObjectGetSprite: Alpha = %f", SpriteGetAlpha(gameObjectSprite));

	TransformPtr gameObjectTransform = GameObjectGetTransform(gameObjectTest);
	const Vector2D* gameObjectTransformV1 = TransformGetTranslation(gameObjectTransform);
	TraceMessage("GameObjectGetTransform: Translation: x = %f, y = %f",
		gameObjectTransformV1->x, gameObjectTransformV1->y);
	gameObjectTransformV1 = TransformGetScale(gameObjectTransform);
	TraceMessage("GameObjectGetTransform: Scale: x = %f, y = %f",
		gameObjectTransformV1->x, gameObjectTransformV1->y);
	TraceMessage("GetObjectGetTranslation: Rotation = %f", TransformGetRotation(gameObjectTransform));

	GameObjectFree(&gameObjectTest);*/

	////////////////////////// MATRIX2D.C TESTING ///////////////////////////////
	Matrix2D matrix;

	Matrix2DIdentity(&matrix);

	// Fills matrix with arbitrary
	int row, col, count = 0;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			matrix.m[row][col] = (float)count;
			count++;
		}
	}

	Matrix2DTranspose(&matrix, &matrix);
	/*Matrix2DConcat(&matrix, &matrix, &matrix);
	Matrix2DTranslate(&matrix, 2.0f, 3.0f);
	Matrix2DScale(&matrix, 2.0f, 3.0f);
	Matrix2DRotDeg(&matrix, 90.0f);
	Matrix2DRotRad(&matrix, 1.5708f);

	// Fills matrix with arbitrary
	row, col, count = 0;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			matrix.m[row][col] = (float)count;
			count++;
		}
	}

	Vector2D vector = { 3, 4 };
	Matrix2DMultVec(&vector, &matrix, &vector);*/


	AEMtx33 AEMatrix;
	row, col, count = 0;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			AEMatrix.m[row][col] = (float)count;
			count++;
		}
	}

	

	AEMtx33Transpose(&AEMatrix, &AEMatrix);
	AEMtx33Concat(&AEMatrix, &AEMatrix, &AEMatrix);
	AEMtx33Trans(&AEMatrix, 2.0f, 3.0f);
	AEMtx33Scale(&AEMatrix, 2.0f, 3.0f);
	AEMtx33RotDeg(&AEMatrix, 90.0f);
	AEMtx33Rot(&AEMatrix, 1.5708f);

	// Fills matrix with arbitrary
	row, col, count = 0;
	for (row = 0; row < 3; row++)
	{
		for (col = 0; col < 3; col++)
		{
			AEMatrix.m[row][col] = (float)count;
			count++;
		}
	}

	AEVec2 AEvector = { 3, 4 };
	AEMtx33MultVec(&AEvector, &AEMatrix, &AEvector);

	////////////////////////// ANIMATION.C TESTING //////////////////////////////
  /*AnimationPtr animation = AnimationCreate();

	Stream file = StreamOpen("Data/FunctionTester.txt");
	if (!file)
		return;

	AnimationRead(animation, file);
	
	AnimationFree(&animation);*/
}
