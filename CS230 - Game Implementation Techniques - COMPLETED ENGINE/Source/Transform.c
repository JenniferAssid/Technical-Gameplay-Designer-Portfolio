//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Trace.h"
#include "Stream.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Transform
{
	// The translation (or world position) of a game object.
	Vector2D	translation;

	// The rotation (or orientation) of a game object (in radians).
	float	rotation;

	// The scale (or size) of a game object.
	// (Hint: This should be initialized to (1, 1).)
	Vector2D	scale;

	// The transformation matrix resulting from concatenating the 
	//	 matrix = Translation*Rotation*Scale matrices.
	Matrix2D	matrix;

	// True if the transformation matrix needs to be recalculated.
	// (Hint: this should be initialized to true.)
	bool	isDirty;

} Transform;
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

// Dynamically allocate a new transform object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
TransformPtr TransformCreate(void)
{
	TransformPtr transform = (TransformPtr)calloc(1, sizeof(Transform));

	if (transform)
	{
		Vector2DSet(&transform->scale, 1.0f, 1.0f);
		transform->isDirty = true;
		return transform;
	}
	else
	{
		//TraceMessage("TransformCreate: Failed to allocate memory.");
		return NULL;
	}
}

// Dynamically allocate a clone of an existing transform.
// (Hint: Make sure to perform a shallow copy or deep copy, as appropriate.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
TransformPtr TransformClone(const TransformPtr other)
{
	if (other == NULL) return NULL;

	TransformPtr clone = (TransformPtr)calloc(1, sizeof(Transform));

	if (clone == NULL) return NULL;

	*clone = *other;
	return clone;
}

// Free the memory associated with a transform object.
// (Also, set the transform pointer to NULL.)
// Params:
//	 transform = Pointer to the transform pointer.
void TransformFree(TransformPtr* transform)
{
	free(*transform);
	*transform = NULL;
}

// Read the properties of a Transform component from a file.
// [NOTE: Read the translation value using StreamReadVector2D.]
// [NOTE: Read the rotation value using StreamReadFloat.]
// [NOTE: Read the scale value using StreamReadVector2D.]
// Params:
//	 physics = Pointer to the Transform component.
//	 stream = Pointer to the data stream used for reading.
void TransformRead(TransformPtr transform, Stream stream)
{
	StreamReadVector2D(stream, &transform->translation);
	transform->rotation = StreamReadFloat(stream);
	StreamReadVector2D(stream, &transform->scale);

	// Testing Code
	/*TraceMessage("TransformRead: Translation: x = %f, y = %f",
		transform->translation.x, transform->translation.y);
	TraceMessage("TransformRead: Rotation: %f", transform->rotation);
	TraceMessage("TransformRead: Scale: x = %f, y = %f",
		transform->scale.x, transform->scale.y);*/
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
Matrix2D* TransformGetMatrix(const TransformPtr transform)
{
	if (transform)
	{
		if (transform->isDirty)
		{
			// Calculates the individual matrices
			Matrix2D translation = { 0 }, rotation = { 0 }, scale = { 0 };
			Matrix2DTranslate(&translation, transform->translation.x, transform->translation.y);
			Matrix2DRotRad(&rotation, transform->rotation);
			Matrix2DScale(&scale, transform->scale.x, transform->scale.y);

			// tranform->matrix = translation * rotation * scale
			Matrix2DConcat(&transform->matrix, &rotation, &scale);
			Matrix2DConcat(&transform->matrix, &translation, &transform->matrix);

			transform->isDirty = false;
		}

		return &transform->matrix;
	}

	return NULL;
}


// Get the translation of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's translation structure,
//		else return a NULL pointer.
const Vector2D* TransformGetTranslation(const TransformPtr transform)
{
	if (transform)
	{
		const Vector2D* translation = &transform->translation;
		return translation;
	}

	//TraceMessage("TransformGetTranslation: Failed");
	return NULL;
}

// Get the rotation value of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float TransformGetRotation(const TransformPtr transform)
{
	if (transform)
	{
		return transform->rotation;
	}

	//TraceMessage("TransformGetRotation: Failed");
	return 0.0f;
}

// Get the scale of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's scale structure,
//		else return a NULL pointer.
const Vector2D* TransformGetScale(const TransformPtr transform)
{
	if (transform)
	{
		const Vector2D* scale = &transform->scale;
		return scale;
	}

	//TraceMessage("TransformGetScale: Failed");
	return NULL;
}

// Set the translation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a translation vector.
void TransformSetTranslation(TransformPtr transform, const Vector2D* translation)
{
	Vector2DSet(&transform->translation, translation->x, translation->y);
	transform->isDirty = true;
	
	// Test
	/*TraceMessage("TransformSetTranslation: Translation: x = %f, y = %f",
		transform->translation.x, transform->translation.y);*/
}

// Set the rotation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 rotation = The rotation value (in radians).
void TransformSetRotation(TransformPtr transform, float rotation)
{
	transform->rotation = rotation;
	transform->isDirty = true;

	// Test
	/*TraceMessage("TransformSetRotation: Rotation: %f",
		transform->rotation);*/
}

// Set the scale of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a scale vector.
void TransformSetScale(TransformPtr transform, const Vector2D* scale)
{
	Vector2DSet(&transform->scale, scale->x, scale->y);
	transform->isDirty = true;

	// Test
	/*TraceMessage("TransformSetScale: Scale: x = %f, y = %f",
		transform->scale.x, transform->scale.y);*/
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

