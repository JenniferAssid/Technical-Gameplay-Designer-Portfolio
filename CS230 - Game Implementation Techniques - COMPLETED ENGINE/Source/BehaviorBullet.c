//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 4
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorBullet.h"
#include "Behavior.h"
#include "GameObject.h"

#include "Teleporter.h"
#include "Collider.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
enum BulletBehaviors { cBulletInvalid = -1, cBulletIdle };

static const float bulletSpeedMax = 500.0f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorBulletUpdateLifeTimer(BehaviorPtr behavior, float dt);
static void BehaviorBulletCollisionHandler(GameObjectPtr collider, GameObjectPtr other);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorBulletCreate(void)
{
  BehaviorPtr bullet = (BehaviorPtr)calloc(1, sizeof(Behavior));
  if (bullet == NULL) return NULL;

  bullet->memorySize = sizeof(Behavior);

  bullet->stateCurr = cBulletInvalid;
  bullet->stateNext = cBulletInvalid;
  bullet->onInit = &BehaviorBulletInit;
  bullet->onUpdate = &BehaviorBulletUpdate;
  bullet->onExit = &BehaviorBulletExit;

  return bullet;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorBulletInit(BehaviorPtr behavior)
{
  if (behavior->stateCurr == cBulletIdle)
  {
    ColliderPtr collider = GameObjectGetCollider(behavior->parent);
    if (collider) ColliderSetCollisionHandler(collider, BehaviorBulletCollisionHandler);
  }
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletUpdate(BehaviorPtr behavior, float dt)
{
  switch (behavior->stateCurr)
  {
  case cBulletIdle:
    BehaviorBulletUpdateLifeTimer(behavior, dt);
    break;
  }

  TeleporterUpdateObject(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletExit(BehaviorPtr behavior)
{
  UNREFERENCED_PARAMETER(behavior);
  return;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void BehaviorBulletUpdateLifeTimer(BehaviorPtr behavior, float dt)
{
  if (behavior->timer > 0)
  {
    behavior->timer -= dt;
    if (behavior->timer <= 0)
      GameObjectDestroy(behavior->parent);
  }
}

static void BehaviorBulletCollisionHandler(GameObjectPtr collider, GameObjectPtr other)
{
  if (collider == NULL || other == NULL) return;

  if (GameObjectIsNamed(other, "Asteroid")) GameObjectDestroy(collider);
}
