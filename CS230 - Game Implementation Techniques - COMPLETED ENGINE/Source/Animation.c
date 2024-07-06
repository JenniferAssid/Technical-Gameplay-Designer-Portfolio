//------------------------------------------------------------------------------
//
// File Name:	Animation.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
#include "Stream.h"
#include "Sprite.h"
#include "GameObject.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Animation
{
	// Pointer to the parent game object associated with the animation.
	GameObjectPtr parent;

	// The current frame being displayed.
	unsigned int frameIndex;

	// The maximum number of frames in the sequence.
	unsigned int frameCount;

	// The time remaining for the current frame.
	float frameDelay;

	// The amount of time to display each successive frame.
	float frameDuration;

	// True if the animation is running; false if the animation has stopped.
	bool isRunning;

	// True if the animation loops back to the beginning.
	bool isLooping;

	// True if the end of the animation has been reached, false otherwise.
	// (Hint: This should be true for only one game loop.)
	bool isDone;

	// The animation sequence currently being played, if any.
	//AnimationSequencePtr	sequence;

} Animation;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void AnimationAdvanceFrame(AnimationPtr animation);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
// Dynamically allocate a new animation object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
AnimationPtr AnimationCreate(void)
{
	AnimationPtr animation = (AnimationPtr)calloc(1, sizeof(Animation));

	if (animation)
		return animation;

	return NULL;
}

// Dynamically allocate a clone of an existing animation.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
AnimationPtr AnimationClone(const AnimationPtr other)
{
	if (other == NULL) return NULL;

	AnimationPtr clone = (AnimationPtr)calloc(1, sizeof(Animation));
	if (clone == NULL) return NULL;

	*clone = *other;
	return clone;
}

// Free the memory associated with an animation.
// (Also, set the animation pointer to NULL.)
// Params:
//	 animation = Pointer to the animation pointer.
void AnimationFree(AnimationPtr* animation)
{
	if (!*animation)
		return;

	free(*animation);
	*animation = NULL;
}

// Read the properties of an Animation component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameDelay and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 animation = Pointer to the Animation component.
//	 stream = Pointer to the data stream used for reading.
void AnimationRead(AnimationPtr animation, Stream stream)
{
	if (!animation || !stream)
		return;

	animation->frameIndex = StreamReadInt(stream);
	animation->frameCount = StreamReadInt(stream);
	animation->frameDelay = StreamReadFloat(stream);
	animation->frameDuration = StreamReadFloat(stream);
	animation->isRunning = StreamReadBoolean(stream);
	animation->isLooping = StreamReadBoolean(stream);
}

// Set the parent game object for an animation component.
// Params:
//	 animation = Pointer to the animation pointer.
//	 parent = Pointer to the parent game object.
void AnimationSetParent(AnimationPtr animation, GameObjectPtr parent)
{
	if (!animation || !parent)
		return;

	animation->parent = parent;
}

// Play a simple animation sequence (0 .. frameCount).
// (Hint: Make sure to initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the animation object.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void AnimationPlay(AnimationPtr animation, int frameCount, float frameDuration, bool isLooping)
{
	if (animation == NULL)
		return;
	
	// Initialize all variables
	animation->isRunning = true, animation->isDone = false, animation->isLooping = isLooping;
	animation->frameCount = frameCount, animation->frameDelay = frameDuration;
	animation->frameDuration = frameDuration, animation->frameIndex = 0;

	SpriteSetFrame(GameObjectGetSprite(animation->parent), animation->frameIndex);
}

// Play a complex animation sequence (any frame/any delay).
// (Hint: Make sure to initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the animation object.
//	 sequence = Pointer to the animation sequence to be played.
//void AnimationPlaySequence(AnimationPtr animation, AnimationSequencePtr sequence);

// Update the animation.
// Params:
//	 animation = Pointer to the animation object.
//	 dt = Change in time (in seconds) since the last game loop.
void AnimationUpdate(AnimationPtr animation, float dt)
{
	if (animation == NULL)
		return;

	animation->isDone = false;
	
	if (animation->isRunning == true)
	{
		animation->frameDelay -= dt;
		if (animation->frameDelay <= 0)
			AnimationAdvanceFrame(animation);
		else
			return;
	}

	return;
	
}

// Determine if the animation has reached the end of its sequence.
// Params:
//	 animation = Pointer to the animation object.
// Returns:
//	 If the animation pointer is valid,
//		then return the value in isDone,
//		else return false.
bool AnimationIsDone(AnimationPtr animation)
{
	if (animation)
		return animation->isDone;

	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void AnimationAdvanceFrame(AnimationPtr animation)
{
	if (animation == NULL)
		return;

	animation->frameIndex++;

	if (animation->frameIndex >= animation->frameCount)
	{
		if (animation->isLooping)
		{
			animation->frameIndex = 0;
		}
		else
		{
			animation->frameIndex = animation->frameCount - 1;
			animation->isRunning = false;
		}

		animation->isDone = true;
	}

	if (animation->isRunning)
	{
		SpriteSetFrame(GameObjectGetSprite(animation->parent), animation->frameIndex);
		animation->frameDelay += animation->frameDuration;
	}
	else
		animation->frameDelay = 0;

	return;
}
