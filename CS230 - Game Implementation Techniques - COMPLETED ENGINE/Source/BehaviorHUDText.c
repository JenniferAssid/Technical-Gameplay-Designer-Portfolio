//------------------------------------------------------------------------------
//
// File Name:	BehaviorHudText.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright � 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorHudText.h"
#include "Behavior.h"
#include "Sprite.h"
#include "GameObject.h"
#include "ScoreSystem.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
typedef enum HudTextStates
{
	cHudTextInvalid = -1,	// HUD Text has not yet been initialized.
	cHudTextIdle,			// HUD Text will normally remain static.

} HudTextStates;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct BehaviorHudText
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add HUD Text-specific behavior variables.

	// The index used to access values from the Score System.
	ScoreSystemId scoreSystemId;

	// The format string to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: %d")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char formatString[32];

	// The buffer to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: 9001")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char displayString[32];

	// The value currently being displayed by the HUD Text object.
	// (NOTE: This value can be compared with *watchValue to determine when the text must be updated.)
	// (NOTE: Make sure to update this value each time the text is updated.)
	int displayValue;

} BehaviorHudText, * BehaviorHudTextPtr;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorHudTextUpdateText(BehaviorHudTextPtr);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorHudTextCreate(void)
{
	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)calloc(1, sizeof(BehaviorHudText));
	if (hud == NULL) return NULL;

	hud->base.memorySize = sizeof(BehaviorHudText);

	hud->base.stateCurr = cHudTextInvalid;
	hud->base.stateNext = cHudTextInvalid;
	hud->base.onInit = &BehaviorHudTextInit;
	hud->base.onUpdate = &BehaviorHudTextUpdate;
	hud->base.onExit = &BehaviorHudTextExit;

	hud->scoreSystemId = SsiInvalid;

	return (BehaviorPtr)hud;
}

// Set the display characteristics of the HUD Text object.
// (NOTE: This function must be called after cloning the HUD Text archetype object.)
// Params:
//	 behavior = Pointer to the behavior component.
//	 formatString = Pointer for the string used to format the display text.
//	 watchValue = Pointer to the float value to be displayed.
//void BehaviorHudTextSetDisplay(BehaviorPtr behavior, const char* formatString, const int* watchValue)
//{
//	if (behavior == NULL) return;
//
//	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
//	
//	hud->formatString = formatString;
//	hud->watchValue = watchValue;
//}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorHudTextInit(BehaviorPtr behavior)
{
	if (behavior == NULL) return;
	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
	BehaviorHudTextUpdateText(hud);

	SpriteSetText(GameObjectGetSprite(hud->base.parent), hud->displayString);
}

// Read the properties of a Behavior component from a file.
// [NOTE: Read the base Behavior values using BehaviorRead.]
// [NOTE: Read the format string using StreamReadString.]
// Params:
//	 behavior = Pointer to the Behavior component.
//	 stream = Pointer to the data stream used for reading.
void BehaviorHudTextRead(BehaviorPtr behavior, Stream stream)
{
	BehaviorRead(behavior, stream);
	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
	strcpy_s(hud->formatString, sizeof(hud->formatString), StreamReadToken(stream));
	hud->scoreSystemId = StreamReadInt(stream);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorHudTextUpdate(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (behavior == NULL) return;

	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
	if (hud->displayValue != ScoreSystemGetValue(hud->scoreSystemId))
		BehaviorHudTextUpdateText(hud);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorHudTextExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void BehaviorHudTextUpdateText(BehaviorHudTextPtr hud)
{
	if (hud == NULL) return;

	hud->displayValue = ScoreSystemGetValue(hud->scoreSystemId);
	sprintf_s(hud->displayString, sizeof(hud->displayString), hud->formatString, hud->displayValue);
}
