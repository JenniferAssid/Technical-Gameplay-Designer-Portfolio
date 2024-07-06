//------------------------------------------------------------------------------
//
// File Name:	GameStateAsteroids.h
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 4
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateAsteroidsLoad();

// Initialize the memory associated with the Stub game state.
void GameStateAsteroidsInit();

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateAsteroidsUpdate(float dt);

// Shutdown any memory associated with the Stub game state.
void GameStateAsteroidsShutdown();

// Unload the resources associated with the Stub game state.
void GameStateAsteroidsUnload();

// Increase the asteroids score by score value.
// Params:
//     scoreValue = The amount to be added to the game score.
//void GameStateAsteroidsIncreaseScore(unsigned int scoreValue);


/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

