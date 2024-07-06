//------------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct Physics
{
  // Previous position.  May be used for resolving collisions.
  Vector2D	oldTranslation;

  // Acceleration = inverseMass * (sum of forces)
  Vector2D	acceleration;

  // Velocity may be stored as a direction vector and speed scalar, instead.
  Vector2D	velocity;

  // Rotational velocity (in radians).
  float rotationalVelocity;

  // Used when calculating acceleration due to forces.
  // Used when resolving collision between two dynamic objects.
  //float		inverseMass;


} Physics;

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

// Dynamically allocate a new Physics component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
PhysicsPtr PhysicsCreate(void)
{
  PhysicsPtr newPhysics = (PhysicsPtr)calloc(1, sizeof(Physics));

  if (newPhysics)
    return newPhysics;
  else
    return NULL;
}

// Dynamically allocate a clone of an existing physics component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
PhysicsPtr PhysicsClone(const PhysicsPtr other)
{
  if (other == NULL) return NULL;

  PhysicsPtr clone = (PhysicsPtr)calloc(1, sizeof(Physics));

  if (clone == NULL) return NULL;

  *clone = *other;
  return clone;
}

// Free the memory associated with a Physics component.
// (Also, set the physics pointer to NULL.)
// Params:
//	 physics = Pointer to the Physics component pointer.
void PhysicsFree(PhysicsPtr* physics)
{
  free(*physics);
  *physics = NULL;
}

// Read the properties of a Physics component from a file.
// [NOTE: Read the acceleration and velocity values using StreamReadVector2D.]
// Params:
//	 physics = Pointer to the Physics component.
//	 stream = Pointer to the data stream used for reading.
void PhysicsRead(PhysicsPtr physics, Stream stream)
{
  StreamReadVector2D(stream, &physics->acceleration);
  StreamReadVector2D(stream, &physics->velocity);
}

// Get the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetAcceleration(const PhysicsPtr physics)
{
  if (physics)
  {
    Vector2D* result = &physics->acceleration;
    return result;
  }

  return NULL;
}

// Get the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetVelocity(const PhysicsPtr physics)
{
  if (physics)
  {
    Vector2D* result = &physics->velocity;
    return result;
  }

  return NULL;
}

// Get the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return the component's rotational velocity value,
//		else return 0.0f.
float PhysicsGetRotationalVelocity(PhysicsPtr physics)
{
  if (physics == NULL) return 0.0f;

  return physics->rotationalVelocity;
}

// Get the old translation (position) of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
const Vector2D* PhysicsGetOldTranslation(PhysicsPtr physics)
{
  if (physics)
  {
    Vector2D* result = &physics->oldTranslation;
    return result;
  }

  return NULL;
}

// Set the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 acceleration = Pointer to an acceleration vector.
void PhysicsSetAcceleration(PhysicsPtr physics, const Vector2D* acceleration)
{
  Vector2DSet(&physics->acceleration, acceleration->x, acceleration->y);
}

// Set the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 velocity = Pointer to a velocity vector.
void PhysicsSetVelocity(PhysicsPtr physics, const Vector2D* velocity)
{
  Vector2DSet(&physics->velocity, velocity->x, velocity->y);
}

// Set the rotational velocity of a physics component.
// Params:
//	 physics = Pointer to the physics component.
//	 rotationalVelocity = The new rotational velocity.
void PhysicsSetRotationalVelocity(PhysicsPtr physics, float rotationalVelocity)
{
  if (physics == NULL) return;

  physics->rotationalVelocity = rotationalVelocity;
}

// Set the "drag" value of a Physics component.
// (NOTE: This value should be clamped between [0.0f, 1.0f].
// Params:
//	 physics = Pointer to the physics component.
//	 drag = The new drag value.
void PhysicsSetDrag(PhysicsPtr physics, float drag)
{
  UNREFERENCED_PARAMETER(physics);
  UNREFERENCED_PARAMETER(drag);
}

// Update the state of a Physics component using the Semi-Implicit Euler method,
//	 as outlined in the "Dynamics" lecture slides and the project instructions.
// (NOTE: This function must verify that the physics and transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void PhysicsUpdate(PhysicsPtr physics, TransformPtr transform, float dt)
{
  if (physics == NULL || transform == NULL)
    return;
  
  Vector2D translation = *TransformGetTranslation(transform);
  Vector2DSet(&physics->oldTranslation, translation.x, translation.y);
  Vector2DScaleAdd(&physics->velocity, &physics->acceleration, &physics->velocity, dt);

  Vector2D newTranslation = { 0 };
  Vector2DScaleAdd(&newTranslation, &physics->velocity, &physics->oldTranslation, dt);
  TransformSetTranslation(transform, &newTranslation);

  float rotation = TransformGetRotation(transform);
  rotation += PhysicsGetRotationalVelocity(physics) * dt;
  TransformSetRotation(transform, rotation);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

