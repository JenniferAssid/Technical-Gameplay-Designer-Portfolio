//------------------------------------------------------------------------------
//
// File Name:	ColliderCircle.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderCircle.h"

#include "Transform.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct ColliderCircle
{
	// Inherit the base collider structure.
	Collider	base;

	// Radius of the circle collider.
	float radius;

} ColliderCircle, * ColliderCirclePtr;
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

// Dynamically allocate a new (circle) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
// (Note: Set the radius to a non-zero value.  Typical values are 0.5f or 1.0f.)
ColliderPtr ColliderCircleCreate(void)
{
	ColliderCirclePtr circle = (ColliderCirclePtr)calloc(1, sizeof(ColliderCircle));
	if (circle == NULL) return NULL;

	circle->base.memorySize = sizeof(ColliderCircle);
	circle->base.type = ColliderTypeCircle;
	circle->radius = 1.0f;

	return (ColliderPtr)circle;
}

// Read the properties of a ColliderCircle component from a file.
// (NOTE: Read the radius value using StreamReadFloat.)
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderCircleRead(ColliderPtr collider, Stream stream)
{
	if (collider == NULL || stream == NULL) return;

	ColliderCirclePtr circle = (ColliderCirclePtr)collider;
	circle->radius = StreamReadFloat(stream);
}

// Get the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
// Returns:
//	 If the pointer is valid,
//	   then return the circle collider's radius,
//	   else return 0.0f.
float ColliderCircleGetRadius(ColliderPtr collider)
{
	ColliderCirclePtr circle = (ColliderCirclePtr)collider;

	if (circle)
		return circle->radius;

	return 0.0f;
}

// Set the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
//   radius = the circle collider's new radius.
void ColliderCircleSetRadius(ColliderPtr collider, float radius)
{
	if (collider == NULL) return;

	ColliderCirclePtr circle = (ColliderCirclePtr)collider;
	circle->radius = radius;
}

// Check for collision between two circle colliders.
// Params:
//	 collider1 = Pointer to the first circle collider component.
//	 collider2 = Pointer to the second circle collider component.
bool ColliderCircleIsCollidingWithCircle(ColliderPtr collider, ColliderPtr other)
{
	if (collider == NULL || other == NULL) return false;

	ColliderCirclePtr c1 = (ColliderCirclePtr)collider;
	ColliderCirclePtr c2 = (ColliderCirclePtr)other;

	Vector2D c1Pos = *TransformGetTranslation(GameObjectGetTransform(collider->parent));
	Vector2D c2Pos = *TransformGetTranslation(GameObjectGetTransform(other->parent));

	Vector2D v = { 0 };
	v.x = c1Pos.x - c2Pos.x;
	v.y = c1Pos.y - c2Pos.y;

	float distance = Vector2DLength(&v);

	if (distance <= (c1->radius + c2->radius))
		return true;

	return false;
}


//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

