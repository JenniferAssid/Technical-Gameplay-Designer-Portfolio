//------------------------------------------------------------------------------
//
// File Name:	Collider.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright � 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Collider.h"

#include "Vector2D.h"
#include "Transform.h"
#include "GameObject.h"

#include "ColliderCircle.h"
#include "ColliderLine.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
//typedef struct Collider
//{
//	// Pointer to the collider's parent game object.
//	GameObjectPtr parent;
//
//	// Pointer to a function that handles collisions between two objects.
//	CollisionEventHandler	handler;
//
//} Collider;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a clone of an existing Collider component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
ColliderPtr ColliderClone(const ColliderPtr other)
{
	if (other == NULL) return NULL;

	ColliderPtr collider = (ColliderPtr)calloc(1, other->memorySize);
	if (collider == NULL) return NULL;
	memcpy_s(collider, other->memorySize, other, other->memorySize);
	collider->parent = NULL;

	return collider;
}

// Free the memory associated with a Collider component.
// (Also, set the collider pointer to NULL.)
// Params:
//	 collider = Pointer to the collider component.
void ColliderFree(ColliderPtr* collider)
{
	if (*collider == NULL) return;

	(*collider)->parent = NULL;
	free(*collider);
	*collider = NULL;
}

// Read the properties of a Collider component from a file.
// [NOTE: No values need to be read at this time.]
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderRead(ColliderPtr collider, Stream stream)
{
	UNREFERENCED_PARAMETER(collider);
	UNREFERENCED_PARAMETER(stream);
}

// Set the parent game object for a Collider component.
// Params:
//	 collider = Pointer to the Collider component.
//	 parent = Pointer to the parent game object.
void ColliderSetParent(ColliderPtr collider, GameObjectPtr parent)
{
	if (parent == NULL || collider == NULL) return;

	collider->parent = parent;
}

// Check if two objects are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent game object pointers in the correct order!)
// Params:
//	 collider1 = Pointer to the first collider component.
//	 collider2 = Pointer to the second collider component.
void ColliderCheck(ColliderPtr collider, ColliderPtr other)
{
	if (collider == NULL || other == NULL) return;

	if (ColliderIsColliding(collider, other))
	{
		if (other->handler)
			other->handler(other->parent, collider->parent);

		if (collider->handler)
			collider->handler(collider->parent, other->parent);
	}
}

// Set the collision event handler for a collider.
// (Hint: This allows other components, such as behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
// Params:
//	 collider = Pointer to the collider component.
//	 handler = Pointer to the collision event handler (may be NULL).
void ColliderSetCollisionHandler(ColliderPtr collider, CollisionEventHandler handler)
{
	if (collider == NULL) return;

	collider->handler = handler;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2)
{
	if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeCircle)
		return ColliderCircleIsCollidingWithCircle(collider1, collider2);
	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeLine)
		return false;
	else if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeLine)
		return ColliderLineIsCollidingWithCircle(collider2, collider1);
	else if (collider2->type == ColliderTypeCircle && collider1->type == ColliderTypeLine)
		return ColliderLineIsCollidingWithCircle(collider1, collider2);
	else
		return false;
}
