//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel2.h
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Stream.h"

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

// Load the resources associated with the Level 2 game state.
void GameStateLevel2Load();

// Initialize the memory associated with the Level 2 game state.
void GameStateLevel2Init();

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the Level 2 game loop.
void GameStateLevel2Update(float dt);

// Shutdown any memory associated with the Level 2 game state.
void GameStateLevel2Shutdown();

// Unload the resources associated with the Level 2 game state.
void GameStateLevel2Unload();

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

