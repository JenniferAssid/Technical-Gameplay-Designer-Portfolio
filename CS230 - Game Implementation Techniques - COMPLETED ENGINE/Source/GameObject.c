//------------------------------------------------------------------------------
//
// File Name:	GameObject.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "Stream.h"
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "Animation.h"
#include "Behavior.h"
#include "BehaviorSpaceship.h"
#include "BehaviorBullet.h"
#include "BehaviorAsteroid.h"
#include "BehaviorHudText.h"
#include "Collider.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"

#include "../AE/include/AEEngine.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct GameObject
{
  // The name of the game object.
  // A buffer is used to allow each game object to have a unique name.
  // The buffer is hardcoded to an arbitrary length that will be sufficient
  //	 for all CS230 assignments.  You may, instead, use dynamically-allocated
  //	 arrays for this purpose but the additional complexity is unnecessary
  //	 and it is your responsibility to ensure that the memory is successfully
  //	 allocated and deallocated in all possible situations.
  // [NOTE: When setting the name, use strncpy_s() to reduce the risk of
  //	 buffer overruns. Additionally, do NOT hardcode the number "32" when
  //	 calling this function!  Instead, use the _countof() macro to get the
  //	 size of the "name" array.]
  char name[32];

  // Flag to indicate that the game object is dead and should be destroyed.
  bool isDestroyed;

  // Pointer to an attached animation component.
  AnimationPtr animation;

  // Pointer to an attached behavior component.
  BehaviorPtr behavior;

  // Pointer to an attached physics component.
  PhysicsPtr physics;

  // Pointer to an attached sprite component.
  SpritePtr sprite;

  // Pointer to an attached transform component.
  TransformPtr transform;

  // Pointer to an attacked collider component.
  ColliderPtr collider;

} GameObject;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new game object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
GameObjectPtr GameObjectCreate(void)
{
  GameObjectPtr result = (GameObjectPtr)calloc(1, sizeof(GameObject));

  if (result) return result;
  
  return NULL;
}

// Dynamically allocate a clone of an existing game object.
// (Hint: Make sure to perform a shallow copy or deep copy, as appropriate.)
// (WARNING: When you clone the behavior component you need to change the 'parent' variable.)
// Params:
//	 other = Pointer to the game object to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned object,
//	   else return NULL.
GameObjectPtr GameObjectClone(const GameObjectPtr other)
{
  GameObjectPtr clone = (GameObjectPtr)calloc(1, sizeof(GameObject));
  if (clone == NULL || other == NULL) return NULL;

  GameObjectSetName(clone, other->name);
  clone->isDestroyed = other->isDestroyed;
  
  if (other->animation)
  {
    AnimationPtr animationClone = AnimationClone(other->animation);
    GameObjectAddAnimation(clone, animationClone);
  }

  if (other->behavior)
  {
    BehaviorPtr behaviorClone = BehaviorClone(other->behavior);
    GameObjectAddBehavior(clone, behaviorClone);
  }
  
  if (other->physics)
  {
    PhysicsPtr physicsClone = PhysicsClone(other->physics);
    GameObjectAddPhysics(clone, physicsClone);
  }
  
  if (other->sprite)
  {
    SpritePtr spriteClone = SpriteClone(other->sprite);
    GameObjectAddSprite(clone, spriteClone);
  }
  
  if (other->transform)
  {
    TransformPtr transformClone = TransformClone(other->transform);
    GameObjectAddTransform(clone, transformClone);
  }

  if (other->collider)
  {
    ColliderPtr colliderClone = ColliderClone(other->collider);
    GameObjectAddCollider(clone, colliderClone);
  }

  return clone;
}

// Free the memory associated with a game object, including all components.
// (Also, set the game object pointer to NULL.)
// Params:
//	 gameObject = Pointer to the game object pointer.
void GameObjectFree(GameObjectPtr* gameObject)
{
  if (*gameObject == NULL)
    return;
  
  if(&(*gameObject)->transform)
    TransformFree(&(*gameObject)->transform);

  if(&(*gameObject)->physics)
    PhysicsFree(&(*gameObject)->physics);

  if(&(*gameObject)->sprite)
    SpriteFree(&(*gameObject)->sprite);

  if (&(*gameObject)->animation)
    AnimationFree(&(*gameObject)->animation);

  if (&(*gameObject)->collider)
    ColliderFree(&(*gameObject)->collider);

  if (&(*gameObject)->behavior)
    BehaviorFree(&(*gameObject)->behavior);

  free(*gameObject);
  *gameObject = NULL;
}

// Read (and construct) the components associated with a game object.
// [NOTE: See project instructions for implementation instructions.]
// Params:
//	 gameObject = Pointer to the game object.
//	 stream = Pointer to the data stream used for reading.
void GameObjectRead(GameObjectPtr gameObject, Stream stream)
{
  if (gameObject == NULL || stream == NULL)
    return;

  const char* token = StreamReadToken(stream);
  strncpy_s(gameObject->name, _countof(gameObject->name), token, strlen(token));

  int i = 1;
  while (i)
  {
    token = StreamReadToken(stream);

    if (strcmp(token, "Transform") == 0)
    {
      TransformPtr transform = TransformCreate();
      TransformRead(transform, stream);
      GameObjectAddTransform(gameObject, transform);
    } else if (strcmp(token, "Physics") == 0)
    {
      PhysicsPtr physics = PhysicsCreate();
      PhysicsRead(physics, stream);
      GameObjectAddPhysics(gameObject, physics);
    } else if (strcmp(token, "Sprite") == 0)
    {
      SpritePtr sprite = SpriteCreate();
      SpriteRead(sprite, stream);
      GameObjectAddSprite(gameObject, sprite);
    } else if (strcmp(token, "Animation") == 0)
    {
      AnimationPtr animation = AnimationCreate();
      AnimationRead(animation, stream);
      GameObjectAddAnimation(gameObject, animation);
    } else if (strcmp(token, "BehaviorSpaceship") == 0)
    {
      BehaviorPtr behavior = BehaviorSpaceshipCreate();
      BehaviorRead(behavior, stream);
      GameObjectAddBehavior(gameObject, behavior);
    } else if (strcmp(token, "BehaviorBullet") == 0)
    {
      BehaviorPtr behavior = BehaviorBulletCreate();
      BehaviorRead(behavior, stream);
      GameObjectAddBehavior(gameObject, behavior);
    } else if (strcmp(token, "BehaviorAsteroid") == 0)
    {
      BehaviorPtr behavior = BehaviorAsteroidCreate();
      BehaviorRead(behavior, stream);
      GameObjectAddBehavior(gameObject, behavior);
    } else if (strcmp(token, "BehaviorHudText") == 0)
    {
      BehaviorPtr behavior = BehaviorHudTextCreate();
      BehaviorHudTextRead(behavior, stream);
      GameObjectAddBehavior(gameObject, behavior);
    } else if (strcmp(token, "ColliderCircle") == 0)
    {
      ColliderPtr collider = ColliderCircleCreate();
      ColliderCircleRead(collider, stream);
      GameObjectAddCollider(gameObject, collider);
    }
    else if (strcmp(token, "ColliderLine") == 0)
    {
      ColliderPtr collider = ColliderLineCreate();
      ColliderLineRead(collider, stream);
      GameObjectAddCollider(gameObject, collider);
    } else if (!*token)
      i = 0;
  }
}

// Flag a game object for destruction.
// (Note: This is to avoid game objects being destroyed while they are being processed.)
// Params:
//	 gameObject = Pointer to the game object to be destroyed.
// Returns:
//	 If 'gameObject' is valid,
//	   then set the 'isDestroyed' boolean variable,
//	   else do nothing.
void GameObjectDestroy(GameObjectPtr gameObject)
{
  if (gameObject)
    gameObject->isDestroyed = true;
}

// Check whether a game object has been flagged for destruction.
// Params:
//	 gameObject = Pointer to the game object to be checked.
// Returns:
//	 If 'gameObject' is valid,
//	   then return the 'isDestroyed' boolean variable,
//	   else return false.
bool GameObjectIsDestroyed(GameObjectPtr gameObject)
{
  if (gameObject)
    return gameObject->isDestroyed;

  return false;
}

// Attach an animation component to a game object.
// (NOTE: This function must set the animation component's parent pointer by
//	  calling the AnimationSetParent() function.)
// Params:
//	 gameObject = Pointer to the game object.
//   animation = Pointer to the animation component to be attached.
void GameObjectAddAnimation(GameObjectPtr gameObject, AnimationPtr animation)
{
  gameObject->animation = animation;
  AnimationSetParent(animation, gameObject);
}

// Attach a behavior component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   behavior = Pointer to the behavior component to be attached.
void GameObjectAddBehavior(GameObjectPtr gameObject, BehaviorPtr behavior)
{
  gameObject->behavior = behavior;
  BehaviorSetParent(behavior, gameObject);
}

// Attach a collider component to a game object.
// (NOTE: This function must set the collider component's parent pointer by
//	  calling the ColliderSetParent() function.)
// Params:
//	 gameObject = Pointer to the game object.
//   collider = Pointer to the collider component to be attached.
void GameObjectAddCollider(GameObjectPtr gameObject, ColliderPtr collider)
{
  if (gameObject == NULL) return;

  gameObject->collider = collider;
  ColliderSetParent(collider, gameObject);
}

// Attach a physics component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   physics = Pointer to the physics component to be attached.
void GameObjectAddPhysics(GameObjectPtr gameObject, PhysicsPtr physics)
{
  gameObject->physics = physics;
}

// Attach a sprite component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   sprite = Pointer to the sprite component to be attached.
void GameObjectAddSprite(GameObjectPtr gameObject, SpritePtr sprite)
{
  gameObject->sprite = sprite;
}

// Attach a transform component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   transform = Pointer to the transform component to be attached.
void GameObjectAddTransform(GameObjectPtr gameObject, TransformPtr transform)
{
  gameObject->transform = transform;
}

// Set the game object's name.
// [NOTE: Verify that both pointers are valid before setting the name.]
// [NOTE: When setting the name, use strncpy_s() to reduce the risk of
//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
//	 calling this function!  Instead, use the _countof() macro to get the
//	 size of the "name" array.]
// Params:
//	 gameObject = Pointer to the game object.
//	 name = Pointer to the game object's new name.
void GameObjectSetName(GameObjectPtr gameObject, const char* name)
{
  if (gameObject == NULL || name == NULL)
    return;

  strncpy_s(gameObject->name, _countof(gameObject->name), name, strlen(name));
}

// Get the game object's name.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the game object's name,
//		else return NULL.
const char* GameObjectGetName(const GameObject* gameObject)
{
  if (gameObject)
  {
    const char* result = gameObject->name;
    return result;
  }
  
  return NULL;
}

// Compare the game object's name with the specified name.
// Params:
//	 gameObject = Pointer to the game object.
//   name = Pointer to the name to be checked.
// Returns:
//	 If the game object pointer is valid,
//		then return true if the names match, false otherwise,
//		else return false.
bool GameObjectIsNamed(const GameObject* gameObject, const char* name)
{
  if (gameObject == NULL) return false;

  if (strncmp(gameObject->name, name, _countof(gameObject->name)) == 0)
    return true;

  return false;
}

// Get the animation component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached animation component,
//		else return NULL.
AnimationPtr GameObjectGetAnimation(const GameObjectPtr gameObject)
{
  if (gameObject == NULL)
    return NULL;

  return gameObject->animation;
}

// Get the behavior component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached behavior component,
//		else return NULL.
BehaviorPtr GameObjectGetBehavior(const GameObjectPtr gameObject)
{
  if (gameObject)
    return gameObject->behavior;

  return NULL;
}

// Get the collider component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached collider component,
//		else return NULL.
ColliderPtr GameObjectGetCollider(const GameObjectPtr gameObject)
{
  if (gameObject) return gameObject->collider;

  return NULL;
}

// Get the physics component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached physics component,
//		else return NULL.
PhysicsPtr GameObjectGetPhysics(const GameObjectPtr gameObject)
{
  if (gameObject)
  {
    PhysicsPtr result = gameObject->physics;
    return result;
  }

  return NULL;
}

// Get the sprite component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached sprite component,
//		else return NULL.
SpritePtr GameObjectGetSprite(const GameObjectPtr gameObject)
{
  if (gameObject)
  {
    SpritePtr result = gameObject->sprite;
    return result;
  }

  return NULL;
}

// Get the transform component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached transform component,
//		else return NULL.
TransformPtr GameObjectGetTransform(const GameObjectPtr gameObject)
{
  if (gameObject)
  {
    TransformPtr result = gameObject->transform;
    return result;
  }

  return NULL;
}

// Update any components attached to the game object.
// (Hint: You will need to call PhysicsUpdate().)
// (NOTE: You must first check for a valid pointer before calling this function.)
// Params:
//	 gameObject = Pointer to the game object.
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectUpdate(GameObjectPtr gameObject, float dt)
{
  if (gameObject && gameObject->animation)
    AnimationUpdate(gameObject->animation, dt);

  if (gameObject && gameObject->behavior)
    BehaviorUpdate(gameObject->behavior, dt);

  if (gameObject && gameObject->physics && gameObject->transform)
    PhysicsUpdate(gameObject->physics, gameObject->transform, dt);
}

// Draw any visible components attached to the game object.
// (Hint: You will need to call SpriteDraw(), passing the sprite and transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 gameObject = Pointer to the game object.
void GameObjectDraw(GameObjectPtr gameObject)
{
  if (gameObject && gameObject->sprite && gameObject->transform)
  {
    SpriteDraw(gameObject->sprite, gameObject->transform);
  }
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

