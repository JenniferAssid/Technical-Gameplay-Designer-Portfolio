//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 4
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorSpaceship.h"
#include "Behavior.h"
#include "AEEngine.h"
#include "Physics.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "GameStateManager.h"
#include "Sprite.h"

#include "Teleporter.h"

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
const enum SpaceShipBehaviors { cSpaceshipInvalid = -1, cSpaceshipIdle, cSpaceshipThrust, cSpaceshipDead };

static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = (float)M_PI / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.25f;
static const float spaceshipWeaponBulletSpeed = 750.0f;
static const float spaceshipDeathDuration = 3.0f;

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
static void BehaviorSpaceshipUpdateRotation(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(BehaviorPtr behavior);
static void BehaviorSpaceshipCollisionHandler(GameObjectPtr collider, GameObjectPtr other);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Spaceship) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorSpaceshipCreate(void)
{
  BehaviorPtr spaceShip = (BehaviorPtr)calloc(1, sizeof(Behavior));

  if (spaceShip == NULL) return NULL;

  spaceShip->memorySize = sizeof(Behavior);

  spaceShip->stateCurr = cSpaceshipInvalid;
  spaceShip->stateNext = cSpaceshipInvalid;
  spaceShip->onInit = &BehaviorSpaceshipInit;
  spaceShip->onUpdate = &BehaviorSpaceshipUpdate;
  spaceShip->onExit = &BehaviorSpaceshipExit;

  return spaceShip;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorSpaceshipInit(BehaviorPtr behavior)
{
  if (behavior->stateCurr == cSpaceshipIdle)
  {
    ColliderPtr collider = GameObjectGetCollider(behavior->parent);
    if (collider) ColliderSetCollisionHandler(collider, BehaviorSpaceshipCollisionHandler);
  }

  if (behavior->stateCurr == cSpaceshipDead)
  {
    behavior->timer = spaceshipDeathDuration;
    SpriteSetAlpha(GameObjectGetSprite(behavior->parent), (behavior->timer / spaceshipDeathDuration));
    Vector2D scale = *TransformGetScale(GameObjectGetTransform(behavior->parent));
    Vector2DScale(&scale, &scale, behavior->timer / spaceshipDeathDuration);
    TransformSetScale(GameObjectGetTransform(behavior->parent), &scale);
    TransformSetRotation(GameObjectGetTransform(behavior->parent), TransformGetRotation(GameObjectGetTransform(behavior->parent)) * (behavior->timer / spaceshipDeathDuration));
  }
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipUpdate(BehaviorPtr behavior, float dt)
{
  switch (behavior->stateCurr)
  {
  case cSpaceshipIdle:
    BehaviorSpaceshipUpdateRotation(behavior, dt);
    BehaviorSpaceshipUpdateWeapon(behavior, dt);
    if (AEInputCheckCurr(VK_UP))
      behavior->stateNext = cSpaceshipThrust;
    break;
  case cSpaceshipThrust:
    BehaviorSpaceshipUpdateRotation(behavior, dt);
    BehaviorSpaceshipUpdateWeapon(behavior, dt);
    BehaviorSpaceshipUpdateVelocity(behavior, dt);
    if (!AEInputCheckCurr(VK_UP))
      behavior->stateNext = cSpaceshipIdle;
    break;
  case cSpaceshipDead:
    behavior->timer -= dt;
    if (behavior->timer < 0)
      GameStateManagerSetNextState(GsRestart);
    SpriteSetAlpha(GameObjectGetSprite(behavior->parent), (behavior->timer / spaceshipDeathDuration));
    Vector2D scale = *TransformGetScale(GameObjectGetTransform(behavior->parent));
    Vector2DScale(&scale, &scale, behavior->timer / spaceshipDeathDuration);
    TransformSetScale(GameObjectGetTransform(behavior->parent), &scale);
    TransformSetRotation(GameObjectGetTransform(behavior->parent), TransformGetRotation(GameObjectGetTransform(behavior->parent)) * (behavior->timer / spaceshipDeathDuration));
    break;
  };

  TeleporterUpdateObject(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipExit(BehaviorPtr behavior)
{
  UNREFERENCED_PARAMETER(behavior);
  return;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void BehaviorSpaceshipUpdateRotation(BehaviorPtr behavior, float dt)
{
  UNREFERENCED_PARAMETER(dt);

  if (AEInputCheckCurr(VK_LEFT))
    PhysicsSetRotationalVelocity(GameObjectGetPhysics(behavior->parent), spaceshipTurnRateMax);

  else if (AEInputCheckCurr(VK_RIGHT))
    PhysicsSetRotationalVelocity(GameObjectGetPhysics(behavior->parent), -spaceshipTurnRateMax);

  else
    PhysicsSetRotationalVelocity(GameObjectGetPhysics(behavior->parent), 0.0f);
}

static void BehaviorSpaceshipUpdateVelocity(BehaviorPtr behavior, float dt)
{
  TransformPtr transform = GameObjectGetTransform(behavior->parent);
  PhysicsPtr physics = GameObjectGetPhysics(behavior->parent);

  if (transform == NULL || physics == NULL) return;

  float rotation = TransformGetRotation(transform);
  Vector2D unitRotation;
  Vector2DFromAngleRad(&unitRotation, rotation);
  Vector2DNormalize(&unitRotation, &unitRotation);

  Vector2D velocity = *PhysicsGetVelocity(physics);
  Vector2DScaleAdd(&velocity, &unitRotation, &velocity, spaceshipAcceleration * dt);

  float speed = Vector2DLength(&velocity);

  if (speed > spaceshipSpeedMax)
    Vector2DScale(&velocity, &velocity, (spaceshipSpeedMax / speed));

  PhysicsSetVelocity(physics, &velocity);
}

static void BehaviorSpaceshipUpdateWeapon(BehaviorPtr behavior, float dt)
{
  if (behavior->timer > 0)
  {
    behavior->timer -= dt;
    if (behavior->timer < 0)
      behavior->timer = 0;
  }

  if (AEInputCheckCurr(' '))
  {
    if (behavior->timer <= 0)
    {
      BehaviorSpaceshipSpawnBullet(behavior);
      behavior->timer = spaceshipWeaponCooldownTime;
    }
  }
}

static void BehaviorSpaceshipSpawnBullet(BehaviorPtr behavior)
{
  GameObjectPtr bullet = GameObjectFactoryBuild("Bullet");

  if (bullet == NULL) return;

  Vector2D position = *TransformGetTranslation(GameObjectGetTransform(behavior->parent));
  float rotation = TransformGetRotation(GameObjectGetTransform(behavior->parent));

  TransformSetTranslation(GameObjectGetTransform(bullet), &position);
  TransformSetRotation(GameObjectGetTransform(bullet), rotation);

  Vector2D unitRotation;
  Vector2DFromAngleRad(&unitRotation, TransformGetRotation(GameObjectGetTransform(behavior->parent)));
  
  Vector2D velocity = *PhysicsGetVelocity(GameObjectGetPhysics(bullet));
  Vector2DScale(&velocity, &unitRotation, spaceshipWeaponBulletSpeed);
  PhysicsSetVelocity(GameObjectGetPhysics(bullet), &velocity);

  GameObjectManagerAdd(bullet);
}

static void BehaviorSpaceshipCollisionHandler(GameObjectPtr collider, GameObjectPtr other)
{
  if (collider == NULL || other == NULL) return;

  if (GameObjectIsNamed(other, "Asteroid")) GameObjectGetBehavior(collider)->stateNext = cSpaceshipDead;
}
