//------------------------------------------------------------------------------
//
// File Name:	ColliderLine.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 1
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ColliderLine.h"
#include "Vector2D.h"
#include "Stream.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static int cLineSegmentMax = 1024;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct ColliderLineSegment
{
	// A single line segment (P0 and P1).
	Vector2D	point[2];
} ColliderLineSegment;

typedef struct ColliderLine
{
	// Inherit the base collider structure.
	Collider	base;

	// The number of line segments in the list.
	unsigned int		lineCount;

	// The individual line segments.
	// (NOTE: Make sure to allocate enough memory for all line segments!)
	ColliderLineSegment	lineSegments[1024];

} ColliderLine, * ColliderLinePtr;
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

// Dynamically allocate a new (line) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
ColliderPtr ColliderLineCreate(void)
{
	ColliderLinePtr line = (ColliderLinePtr)calloc(1, sizeof(ColliderLine));
	if (line == NULL) return NULL;

	line->base.memorySize = sizeof(ColliderLine);
	line->base.type = ColliderTypeLine;
	return (ColliderPtr)line;
}

// Read the properties of a ColliderLine component from a file.
// (NOTE: First read an integer indicating the number of lines to be read.)
// (NOTE: For each line, read P0 and P1 using StreamReadVector2D.)
// (HINT: Call ColliderLineAddLineSegment() to add each line.)
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderLineRead(ColliderPtr collider, Stream stream)
{
	if (collider == NULL || stream == NULL) return;

	int numLines = StreamReadInt(stream);

	Vector2D p0, p1;

	int i;
	for (i = 0; i < numLines; i++)
	{
		StreamReadVector2D(stream, &p0);
		StreamReadVector2D(stream, &p1);

		ColliderLineAddLineSegment(collider, &p0, &p1);
	}
}

// Add a line segment to the line collider's line segment list.
// Params:
//	 collider = Pointer to the line collider component.
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLineAddLineSegment(ColliderPtr collider, const Vector2D* p0, const Vector2D* p1)
{
	if (collider == NULL) return;

	ColliderLinePtr line = (ColliderLinePtr)collider;

	ColliderLineSegment segment = { 0 };
	Vector2DSet(&segment.point[0], p0->x, p0->y);
	Vector2DSet(&segment.point[1], p1->x, p1->y);

	line->lineSegments[line->lineCount] = segment;
	line->lineCount++;
}

// Check for collision between a line collider and a circle collider.
// Params:
//	 collider1 = Pointer to the line collider component.
//	 collider2 = Pointer to the circle collider component.
// Returns:
//	 If the pointers are valid,
//	   then return the results of a line-circle collision check,
//	   else return false.
bool ColliderLineIsCollidingWithCircle(ColliderPtr collider, ColliderPtr other)
{
	if (collider == NULL || other == NULL) return false;

	Vector2D Bs = *PhysicsGetOldTranslation(GameObjectGetPhysics(other->parent));
	Vector2D Be = *TransformGetTranslation(GameObjectGetTransform(other->parent));

	Vector2D v, Bi, BiP1, BiP0, incidentV, s, r, Br, p0p1;
	Vector2DSub(&v, &Be, &Bs);

	ColliderLinePtr line = (ColliderLinePtr)collider;
	Vector2D p0, p1, e, n;

	unsigned int i;
	for (i = 0; i < line->lineCount; i++)
	{
		p0 = line->lineSegments[i].point[0];
		p1 = line->lineSegments[i].point[1];
		Vector2DSub(&e, &p1, &p0);
		Vector2DSet(&n, e.y, -e.x);
		Vector2DNormalize(&n, &n);

		if (Vector2DDotProduct(&n, &v) == 0)
			continue;

		if (Vector2DDotProduct(&n, &Bs) <= Vector2DDotProduct(&n, &p0) &&
			Vector2DDotProduct(&n, &Be) < Vector2DDotProduct(&n, &p0))
			continue;

		if (Vector2DDotProduct(&n, &Bs) >= Vector2DDotProduct(&n, &p0) &&
			Vector2DDotProduct(&n, &Be) > Vector2DDotProduct(&n, &p0))
			continue;

		float ti = (Vector2DDotProduct(&n, &p0) - Vector2DDotProduct(&n, &Bs)) / Vector2DDotProduct(&n, &v);
		
		Vector2DScaleAdd(&Bi, &v, &Bs, ti);
		Vector2DSub(&BiP1, &Bi, &p1);
		Vector2DSub(&BiP0, &Bi, &p0);
		Vector2DSub(&p0p1, &p0, &p1);

		if (Vector2DDotProduct(&e, &BiP0) < 0)
			continue;

		if (Vector2DDotProduct(&p0p1, &BiP1) < 0)
			continue;

		Vector2DSub(&incidentV, &Be, &Bi);
		Vector2DScale(&s, &n, Vector2DDotProduct(&incidentV, &n));
		Vector2DScale(&r, &s, 2.0f);
		Vector2DSub(&r, &incidentV, &r);

		Vector2DAdd(&Br, &Bi, &r);
		TransformSetTranslation(GameObjectGetTransform(other->parent), &Br);

		float angle = Vector2DToAngleRad(&r);
		TransformSetRotation(GameObjectGetTransform(other->parent), angle);

		Vector2D oldVel = *PhysicsGetVelocity(GameObjectGetPhysics(other->parent));
		float speed = Vector2DLength(&oldVel);
		Vector2D vel;
		Vector2DNormalize(&r, &r);
		Vector2DScale(&vel, &r, speed);
		PhysicsSetVelocity(GameObjectGetPhysics(other->parent), &vel);

		continue;
	}

	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

