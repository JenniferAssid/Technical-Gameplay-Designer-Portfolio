//------------------------------------------------------------------------------
//
// File Name:	ScoreSystem.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ScoreSystem.h"

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
static int asteroidScore = 0;
static int asteroidHighScore = 0;
static int asteroidWaveCount;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Score system.
// (NOTE: All private variables must be initialized to 0.)
// (NOTE: There should be separate values for Score, High Score, and Wave.)
void ScoreSystemInit()
{
  
}

// Restart the Score system after updating the high score.
// (HINT: The high score value should be updated before zeroing out the score and wave variables.
void ScoreSystemRestart()
{
  asteroidHighScore = asteroidScore;
  asteroidScore = asteroidWaveCount = 0;
}

// Get the specified score system value.
// (Hint: Check to make sure that ssid is between SsiInvalid and SsiMax (non-inclusive).)
// Params:
//	 ssid = The score system value to be returned.
// Returns:
//	 If "ssid" is a valid index,
//	   then return the value associated with that index,
//	   else return 0.
int ScoreSystemGetValue(ScoreSystemId ssid)
{
  if (SsiInvalid < ssid && ssid < SsiMax)
  {
    switch (ssid)
    {
    case SsiScore:
      return asteroidScore;
      break;
    case SsiHighScore:
      return asteroidHighScore;
      break;
    case SsiWaveCount:
      return asteroidWaveCount;
      break;
    }
  }

  return 0;
}

// Increase the game score by score value.
// Params:
//	 scoreValue = The amount to be added to the game score.
void ScoreSystemIncreaseScore(unsigned int scoreValue)
{
  asteroidScore += scoreValue;
}

// Increase the wave count by 1.
void ScoreSystemIncreaseWaveCount()
{
  asteroidWaveCount++;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

