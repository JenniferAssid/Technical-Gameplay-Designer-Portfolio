//---------------------------------------------------------
// file:	collision.c
// author:	Jennifer Assid
// email:	jennifer.assid@digipen.edu
//
// brief:	template file for holding gamestate functions
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "collisions.h"
#include "gamestate_gameover.h"
#include "math.h"
#include <stdio.h>

CP_Image goal_Flame;      //goal sprite
CP_Image spriteCharacter; //character sprite
float playerDirection;    //indication of which way the sprite should be 'facing'
float spriteRow;          //variables that implement changing the sprite
float spriteColumn;

int instructionBool;

CP_Color worldPrimary, worldSecondary, worldTritary; //used to easily switch colors to represent world state changes

#define num_obstacles 3 // max number of obstacles in a world

typedef struct
{
	float x;
	float y;
} Vector2;             // Vector2D struct to store position values

typedef struct
{
	Vector2 min;
	Vector2 max;
} AABB;               // Axis-Aligned Bounding Box struct to store values

typedef struct
{
	Vector2	position;
	float width;
	float height;
} Box2D;				// Box Shaped stuct to store values

// Initializing / Defining Global Variables
float playerPosition_x = 0.0f;
float playerPosition_y = 0.0f;
int collisionTester;
Vector2 playerVelocity = { 0,0 };

int worldChange, worldState; //used to determine current world state

// Collision Bools
float penetrationUP;
float penetrationDOWN;
float penetrationRIGHT;
float penetrationLEFT;

int rightCollision;
int topCollision;
int leftCollision;
int bottomCollision;

// Fragments of each movement to check for collisions
int stepAmount = 15;

Vector2 camera = { 0,0 }; 

Box2D obstaclesConstant[num_obstacles];     //array with Constant obstacles drawing information

AABB obstaclesBoundConstant[num_obstacles]; //array with Constant obstacle collision information

Box2D obstaclesWorld[num_obstacles];        //array with World obstacles drawing information

AABB obstaclesBoundWorld[num_obstacles];    //array with World obstacle collision information

AABB player;  //bounding box for the player

Box2D goal;   //dimensions for the goal

Box2D screen; //dimensions for the screen

// defines player bounding box
void PlayerAABBAssignment()
{
	AABB* sprite = &player;

	sprite->min.x = playerPosition_x - (CP_System_GetWindowWidth() / 12.0f);  //top left
	sprite->min.y = playerPosition_y - (CP_System_GetWindowWidth() / 12.0f);
	sprite->max.x = playerPosition_x + (CP_System_GetWindowWidth() / 12.0f);  //bottom right
	sprite->max.y = playerPosition_y + (CP_System_GetWindowWidth() / 12.0f);

}

// draws all of the constant obstacles
void DrawObstaclesCONSTANT()//Box2D *worldObstacles)
{
	for (int i = 0; i < num_obstacles; ++i)   // iterates through all Obstacles
	{
		Box2D* box = &obstaclesConstant[i];
		CP_Graphics_DrawRect(box->position.x - camera.x, // draws the obstacles
			box->position.y - camera.y,
			box->width,
			box->height);
	}
}

// draws all of the world obstacles
void DrawObstaclesWORLD()//Box2D *worldObstacles)
{
	for (int i = 0; i < num_obstacles; ++i)   // iterates through all Obstacles
	{
		Box2D* worldObstacles = &obstaclesWorld[i];
		CP_Graphics_DrawRect(worldObstacles->position.x - camera.x, // draws the obstacles
			worldObstacles->position.y - camera.y,
			worldObstacles->width,
			worldObstacles->height);
	}
}

// initializes the collision information for constant obstacles
void InitializeObstaclesAndBoundingCONSTANT()
{
	Box2D* floor = &obstaclesConstant[0];
	floor->position.x = 0.0f - (CP_System_GetWindowWidth() / 9.0f);
	floor->position.y = 0.0f;
	floor->width = 8.0f * CP_System_GetWindowWidth() / 9.0f;
	floor->height = CP_System_GetWindowHeight() / 12.0f;

	AABB* floorBound = &obstaclesBoundConstant[0];

	floorBound->min.x = floor->position.x;
	floorBound->min.y = floor->position.y;
	floorBound->max.x = floorBound->min.x + floor->width;
	floorBound->max.y = floorBound->min.y + floor->height;

}

// initializes the collision information for default obstacles
void InitializeObstaclesAndBoundingDEFAULT()
{
	
	Box2D* obstacleCage = &obstaclesWorld[0];
	obstacleCage->position.x = 0.0f + (CP_System_GetWindowWidth() / 2.0f);
	obstacleCage->position.y = 0.0f - (CP_System_GetWindowHeight() / 3.0f);
	obstacleCage->width = 1.5f * CP_System_GetWindowWidth() / 18.0f;
	obstacleCage->height = CP_System_GetWindowHeight() / 3.0f;

	AABB* obstacleCageBound = &obstaclesBoundWorld[0];

	obstacleCageBound->min.x = obstacleCage->position.x;
	obstacleCageBound->min.y = obstacleCage->position.y;
	obstacleCageBound->max.x = obstacleCageBound->min.x + obstacleCage->width;
	obstacleCageBound->max.y = obstacleCageBound->min.y + obstacleCage->height;
	
}

// initializes the collision information for change obstacles
void InitializeObstaclesAndBoundingCHANGE()
{
	Box2D* obstacleCage = &obstaclesWorld[0];
	obstacleCage->position.x = 0.0f;
	obstacleCage->position.y = 0.0f;
	obstacleCage->width = 0.0f;
	obstacleCage->height = 0.0f;

	AABB* obstacleCageBound = &obstaclesBoundWorld[0];

	obstacleCageBound->min.x = obstacleCage->position.x;
	obstacleCageBound->min.y = obstacleCage->position.y;
	obstacleCageBound->max.x = obstacleCageBound->min.x + obstacleCage->width;
	obstacleCageBound->max.y = obstacleCageBound->min.y + obstacleCage->height;
}

// stops players from going into objects
void sinkCorrection()
{
	if (leftCollision)
	{
		playerPosition_x += penetrationLEFT * 0.5f;
	}
	if (rightCollision)
	{
		playerPosition_x -= penetrationRIGHT * 0.5f;
	}
	if (topCollision)
	{
		playerPosition_y += penetrationUP * 0.5f;
	}
	if (bottomCollision)
	{
		playerPosition_y -= penetrationDOWN * 0.5f;
	}
}

// tests if the player is colliding with constant and world obstacles
int TestAABBCollison(AABB* a, AABB* b)
{
	//determines instances of collisions
	if (a->min.x > b->max.x || a->max.x < b->min.x)
		return 0;

	if (a->min.y > b->max.y || a->max.y < b->min.y)
		return 0;

	return 1;
}

// when a collision hits, determines which side of the player is being collided with
void testSideCollision(int* top, int* left, int* bottom, int* right, AABB* a, AABB* b)
{
	float halfWidthA = (a->max.x - a->min.x) / 2.0f;
	float halfWidthB = (b->max.x - b->min.x) / 2.0f;

	float halfHeightA = (a->max.y - a->min.y) / 2.0f;
	float halfHeightB = (b->max.y - b->min.y) / 2.0f;

	// Determines midpoint of player
	Vector2 aMidpoint;
	aMidpoint.x = a->min.x + (halfWidthA);
	aMidpoint.y = a->min.y + halfHeightA;

	// Determines midpoint of obstacle
	Vector2 bMidpoint;
	bMidpoint.x = b->min.x + halfWidthB;
	bMidpoint.y = b->min.y + halfHeightB;

	// Determines vector between player and obstacle
	Vector2 collisionBoi;
	collisionBoi.x = bMidpoint.x - aMidpoint.x;
	collisionBoi.y = bMidpoint.y - aMidpoint.y;

	/**************************************************************************
		 ____2____
		|         |
	  3 |  player | 1    Visual of which side the number corresponds to
		|_________|
			 4
	 *************************************************************************/

	float overlapX = halfWidthA + halfWidthB - fabsf(collisionBoi.x);
	if (overlapX > 0.0f)
	{

		float overlapY = halfHeightA + halfHeightB - fabsf(collisionBoi.y);
		if (overlapY > 0.0f)
		{
			if (overlapX < overlapY)
			{
				if (collisionBoi.x < 0.0f)
				{
					*left = 1;
					penetrationLEFT = overlapX;
				}
				else {
					*right = 1;
					penetrationRIGHT = overlapX;
				}
			}
			else {
				if (collisionBoi.y < 0.0f)
				{
					*top = 1;
					penetrationUP = overlapY;
				}
				else {
					*bottom = 1;
					penetrationDOWN = overlapY;
				}
			}
		}
	}
}

// draws the default sprite
void drawCharacter_WorldStateDefault()
{
	CP_Image_DrawSubImage(spriteCharacter,
		playerPosition_x - camera.x,
		playerPosition_y - camera.y,
		playerDirection,
		(CP_System_GetWindowWidth() / 6.0f),
		0.0f,
		spriteRow,
		240.0f,
		spriteColumn,
		255);
}

// draws the change sprite
void drawCharacter_WorldStateChange()
{
	CP_Image_DrawSubImage(spriteCharacter,
		playerPosition_x - camera.x,
		playerPosition_y - camera.y,
		playerDirection,
		(CP_System_GetWindowWidth() / 6.0f),
		0.0f,
		spriteRow,
		240.0f,
		spriteColumn,
		255);
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_game_init(void)
{
	// sets screen to default settings
	CP_Settings_Background(CP_Color_Create(0, 0, 0, 255));

	// loads in sprite / goal images
	spriteCharacter = CP_Image_Load("./Assets/worldstateChibi-SpriteSheet.png");
	goal_Flame = CP_Image_Load("./Assets/Flame.png");

	//restarts camera vector
	camera.x = 0.0f;
	camera.y = 0.0f;

	//Sets initial player position
	playerPosition_x = 0.0f + camera.x;//CP_System_GetWindowWidth() / 2.0f + camera.x;
	playerPosition_y = 0.0f + camera.x;//CP_System_GetWindowHeight() / 2.0f + camera.y;

	// defines the starting values for all variables
	collisionTester = 0;

	playerVelocity.x = 0.0f;
	playerVelocity.y = 0.0f;

	worldState, worldChange = 0;

	worldPrimary = CP_Color_Create(0, 0, 0, 255);
	worldSecondary = CP_Color_Create(255, 255, 255, 255);

	playerDirection = (CP_System_GetWindowWidth() / 6.0f);

	spriteRow = 480.0f;
	spriteColumn = 720.0f;

	instructionBool = 0;
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_game_update(void)
{
	// sets world to default state
	CP_Settings_Background(worldPrimary);

	// Sets camera x and y coordinates
	camera.x += (playerPosition_x - (CP_System_GetWindowWidth() / 2.0f) - camera.x) * 0.50f;
	camera.y += (playerPosition_y - (CP_System_GetWindowHeight() / 2.0f) - camera.y) * 0.50f;

	InitializeObstaclesAndBoundingCONSTANT();
	
	CP_Settings_Fill(CP_Color_Create(127, 127, 127, 255));
	DrawObstaclesCONSTANT();
	
	// sets the boundaries for the screen
	Box2D* boundaries = &screen;

	boundaries->position.x = 0.0f - (CP_System_GetWindowWidth()/8.0f);
	boundaries->position.y = 0.0f - (3.0f  *CP_System_GetWindowHeight() / 4.0f);
	boundaries->width = CP_System_GetWindowWidth() * 1.0f;
	boundaries->height = CP_System_GetWindowHeight() * 1.0f;
	
	// sets the boundaries for the goal
	Box2D *flame = &goal;

	flame->position.x = 0.0f + (13.0f * CP_System_GetWindowWidth() / 18.0f) - camera.x;
	flame->position.y = 0.0f - (CP_System_GetWindowHeight() / 12.0f) - camera.y;
	flame->width = CP_System_GetWindowWidth() / 12.0f;
	flame->height = CP_System_GetWindowWidth() / 12.0f;
	
	// draws the goal
	CP_Image_Draw(goal_Flame,
		flame->position.x, flame->position.y, flame->width, flame->height,
		255);	
	
	AABB* sprite = &player;

	// determines if player is outside of playable screen
	if (sprite->max.x < boundaries->position.x ||
		sprite->max.y < boundaries->position.y ||
		sprite->min.y >(boundaries->position.x + boundaries->width) ||
		sprite->min.y >(boundaries->position.y + boundaries->height))
	{
		CP_Engine_SetNextGameStateForced(gamestate_game_init, gamestate_game_update, gamestate_game_exit);
	}


	//determines if player is touching / in the goal
	if (sprite->min.x >= flame->position.x &&
		sprite->min.y <= flame->position.y &&
		sprite->max.x >= flame->position.x - flame->width &&
		sprite->max.y <= flame->position.y - flame->height)
	{
		CP_Engine_SetNextGameStateForced(gamestate_gameover_init, gamestate_gameover_update, gamestate_gameover_exit);
	}
		
	// determines if world state needs to be changed and which one needs to be shown
	if (CP_Input_KeyTriggered(KEY_C))
	{
		worldChange += 1;
	}

	if (worldChange % 2 == 0)
	{
		worldState = 0;
		worldChange = 0;
	}
	else {
		worldState = 1;
	}

	if (worldState == 0) // default world state
	{

		InitializeObstaclesAndBoundingDEFAULT();
		CP_Settings_Fill(worldSecondary);

		DrawObstaclesWORLD();// defaultObstacles);

		drawCharacter_WorldStateDefault();

		worldPrimary = CP_Color_Create(0, 0, 0, 255);
		worldSecondary =  CP_Color_Create(255, 255, 255, 255);

		// determines with subimage needs to be drawn
		if (playerVelocity.x == 0.0f)
		{
			spriteRow = 480.0f;
			spriteColumn = 720.0f;
		}
		else
		{
			spriteRow = 720.0f;
			spriteColumn = 960.0f;
		}
		
		
	}

	if (worldState == 1) // world change
	{
		InitializeObstaclesAndBoundingCHANGE();
		
		CP_Settings_Fill(worldPrimary);
		DrawObstaclesWORLD();

		drawCharacter_WorldStateChange();

		worldPrimary = CP_Color_Create(255, 255, 255, 255);
		worldSecondary = CP_Color_Create(0, 0, 0, 255);

		// determines with subimage needs to be drawn
		if (playerVelocity.x == 0.0f)
		{
			spriteRow = 0.0f;
			spriteColumn = 240.0f;
		}
		else
		{
			spriteRow = 240.0f;
			spriteColumn = 480.0f;
		}

	}

	//iterating through obstacles and drawing min / max values
	/*for (int i = 0; i < num_obstacles; i++)
	{
		AABB* obstacle = &obstaclesBoundConstant[i];

		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
		CP_Graphics_DrawCircle(obstacle->min.x - camera.x,
			obstacle->min.y - camera.y,
			50.0);
		CP_Graphics_DrawCircle(obstacle->max.x - camera.x,
			obstacle->max.y - camera.y,
			50.0);

		AABB* obstacleDefault = &obstaclesBoundWorld[i];

		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
		CP_Graphics_DrawCircle(obstacleDefault->min.x - camera.x,
			obstacleDefault->min.y - camera.y,
			50.0);
		CP_Graphics_DrawCircle(obstacleDefault->max.x - camera.x,
			obstacleDefault->max.y - camera.y,
			50.0);
	}*/

	//drawing min / max for player
	/*CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Graphics_DrawCircle(sprite->min.x - camera.x,
		sprite->min.y - camera.y,
		50.0);
	CP_Graphics_DrawCircle(sprite->max.x - camera.x,
		sprite->max.y - camera.y,
		50.0);*/

	

	//Assigning the AABB for the player before movement
	PlayerAABBAssignment();
	float playerSpeed = 2.0f;

	playerVelocity.y += 0.5f; //gravity

	if (CP_Input_KeyTriggered(KEY_SPACE) && bottomCollision) //has the player jump when they are on the ground
	{
		playerVelocity.y -= 10.0f;
	}

	if (CP_Input_KeyDown(KEY_D)) //moves player right
	{
		playerVelocity.x += playerSpeed;
		playerDirection = (CP_System_GetWindowWidth() / 6.0f);
	}

	if (CP_Input_KeyDown(KEY_A)) //moves player left
	{
		playerVelocity.x -= playerSpeed;
		playerDirection = -(CP_System_GetWindowWidth() / 6.0f);
	}

	//fragments player's movement to check if there are "sub" collisions between the tick movements
	float currentVelocityMAIN, currentVelocitySUB;

	float ORIGINvelocityMAIN = fabsf(playerVelocity.x) >= fabsf(playerVelocity.y) ? playerVelocity.x : playerVelocity.y; //if statement checking if x > y
	float ORIGINvelocitySUB = fabsf(playerVelocity.x) < fabsf(playerVelocity.y) ? playerVelocity.x : playerVelocity.y; //if statement checking if x < y

	float remainingVelocity = fabsf(ORIGINvelocityMAIN);

	//calculates the MAIN / SUB velocities of the player when in collision
	while (remainingVelocity > 0.0f)
	{
		remainingVelocity -= (float)stepAmount;
		if (remainingVelocity < 0.0f)
		{
			currentVelocityMAIN = (float)(stepAmount + remainingVelocity) * ((ORIGINvelocityMAIN >= 0.0f) ? 1.0f : -1.0f);
		}
		else {
			currentVelocityMAIN = (float)stepAmount * ((ORIGINvelocityMAIN >= 0.0f) ? 1.0f : -1.0f);
		}

		currentVelocitySUB = (currentVelocityMAIN / ORIGINvelocityMAIN) * ORIGINvelocitySUB;

		//adds velocities to the correct positional placements
		if (fabsf(playerVelocity.x) > fabsf(playerVelocity.y))
		{
			playerPosition_x += currentVelocityMAIN;
			playerPosition_y += currentVelocitySUB;
		}
		else
		{
			playerPosition_x += currentVelocitySUB;
			playerPosition_y += currentVelocityMAIN;
		}

		// defining collision bools
		rightCollision = 0;
		topCollision = 0;
		leftCollision = 0;
		bottomCollision = 0;

		penetrationRIGHT = 0;
		penetrationUP = 0;
		penetrationLEFT = 0;
		penetrationRIGHT = 0;

		//iterating through all obstacles to see if player is colliding
		for (int i = 0; i < num_obstacles; ++i)
		{
			AABB* obstacleCounterConstant = &obstaclesBoundConstant[i];

			collisionTester = TestAABBCollison(sprite, obstacleCounterConstant);

			if (collisionTester)
			{
				testSideCollision(&topCollision, &leftCollision, &bottomCollision, &rightCollision, sprite, obstacleCounterConstant);
			}

			AABB* obstacleCounterDefault = &obstaclesBoundWorld[i];

			collisionTester = TestAABBCollison(sprite, obstacleCounterDefault);

			if (collisionTester)
			{
				testSideCollision(&topCollision, &leftCollision, &bottomCollision, &rightCollision, sprite, obstacleCounterDefault);
			}

		}

		sinkCorrection();

		//collision resolution based off of side determination

		 // stops movement to right
		if (rightCollision && playerVelocity.x > 0.0f)
		{
			playerVelocity.x = 0.0f;
			instructionBool = 1;
			break;
		}

		//stops movement upward
		if (topCollision && playerVelocity.y < 0.0f)
		{
			playerVelocity.y = 0.0f;
			break;
		}

		//stops movement to left
		if (leftCollision && playerVelocity.x < 0.0f)
		{
			playerVelocity.x = 0.0f;
			break;
		}

		//stops movement down
		if (bottomCollision && playerVelocity.y > 0.0f)
		{
			playerVelocity.y = 0.0f;
			break;
		}
	}
	
	//resets player to initial position
	if (CP_Input_KeyTriggered(KEY_R))
	{
		CP_Engine_SetNextGameStateForced(gamestate_game_init, gamestate_game_update, gamestate_game_exit);
	}

	// draws instructions once flag is initiated
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_Fill(worldSecondary);
	CP_Settings_TextSize(CP_System_GetWindowHeight() / 12.0f);

	if (instructionBool == 1)
	{
		CP_Font_DrawText("Press 'C'",
			(CP_System_GetWindowWidth() / 2.0f),
			CP_System_GetWindowHeight() / 6.0f);
	}
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
