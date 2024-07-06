//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 4
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectManager.h"

#include "Collider.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static int managerMax = 1024;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct GameObjectManager
{
	// The current number of game objects currently in the list.
	unsigned int objectCount;

	// The maximum number of game objects ever in the list, at any given moment.
	unsigned int objectMax;

	// This list can be a fixed-length array (minimum size of 100 entries)
	// or a dynamically sized array, such as a linked list.
	GameObjectPtr	objectList[1024]; //objectListSize

} GameObjectManager;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static GameObjectManager gameObjectActiveList;
static GameObjectManager gameObjectArchetypes;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
void GameObjectManagerAddToList(GameObjectPtr gameObject, GameObjectPtr *list);
GameObjectPtr GameObjectManagerFindName(GameObjectPtr *list, const char* name);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game object manager.
// (NOTE: This function should initialize the lists for both active game objects
//    and the archetype objects.)
void GameObjectManagerInit(void)
{
	memset(&gameObjectActiveList, 0, sizeof(gameObjectActiveList));
	memset(&gameObjectArchetypes, 0, sizeof(gameObjectArchetypes));
}

// Update all objects in the active game objects list.
// (NOTE: After a game object has been updated, if it has been flagged as
//    destroyed, then it must be removed from the list and freed properly.
//    Additionally, the count of active objects must be reduced by 1.)
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectManagerUpdate(float dt)
{
	int i = 0;
	while (i < managerMax)
	{
		GameObjectUpdate(gameObjectActiveList.objectList[i], dt);
		if (GameObjectIsDestroyed(gameObjectActiveList.objectList[i]))
		{
			GameObjectFree(&gameObjectActiveList.objectList[i]);
			gameObjectActiveList.objectCount--;
		}

		i++;
	}
}

// Check collisions between all objects held by the game object manager.
void GameObjectManagerCheckCollisions(void)
{
	int i, j;
	for (i = 0; i < managerMax; i++)
	{
		if (GameObjectGetCollider(gameObjectActiveList.objectList[i]) == NULL) continue;

		for (j = i + 1; j < managerMax; j++)
		{
			if (GameObjectGetCollider(gameObjectActiveList.objectList[j]) == NULL) continue;

			ColliderCheck(GameObjectGetCollider(gameObjectActiveList.objectList[i]),
				GameObjectGetCollider(gameObjectActiveList.objectList[j]));
		}
	}
}

// Draw all game objects in the active game object list.
void GameObjectManagerDraw(void)
{
	int i = 0;
	while (i < managerMax)
	{
		GameObjectDraw(gameObjectActiveList.objectList[i]);

		i++;
	}
}

// Shutdown the game object manager.
// (NOTE: This means removing all game objects from both the active and
//	  archetype game object lists.  Make sure that the object counts are
//	  properly updated in both situations.)
void GameObjectManagerShutdown(void)
{
	int i = 0;
	while (i < managerMax)
	{
		if (gameObjectActiveList.objectList[i])
		{
			GameObjectFree(&gameObjectActiveList.objectList[i]);
			gameObjectActiveList.objectCount--;
		}
		
		if (gameObjectArchetypes.objectList[i])
		{
			GameObjectFree(&gameObjectArchetypes.objectList[i]);
			gameObjectArchetypes.objectCount--;
		}

		i++;
	}
}

// Add a game object to the active game object list.
// (Hint: This function and the GameObjectManagerAddArchetype functions require
//    the same code.  Consider creating a single function that adds an object
//	  to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManagerAdd(GameObjectPtr gameObject)
{
	if (gameObject == NULL) return;
	GameObjectManagerAddToList(gameObject, &gameObjectActiveList.objectList[0]);
	
	// Increment the associated values (Probably arbitrary)
	gameObjectActiveList.objectCount++;
}

// Add a game object to the game object archetype list.
// (Hint: This function and the GameObjectManagerAdd functions require
//    the same code.  Consider creating a single, private function that adds
//	 an object to a list that is passed as a parameter.)
// Params:
//	 gameObject = Pointer to the game object to be added to the list.
void GameObjectManagerAddArchetype(GameObjectPtr gameObject)
{
	if (gameObject == NULL) return;
	GameObjectManagerAddToList(gameObject, &gameObjectArchetypes.objectList[0]);

	// Increment the associated values (Probably arbitrary)
	gameObjectArchetypes.objectCount++;
}

// Returns a pointer to the first active game object matching the specified name.
// (Hint: This function and the GameObjectManagerGetObjectByName functions require
//    the same code.  Consider creating a single, private function that searches
//	  for the named game object in a list that is passed as a parameter.)
// Params:
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return NULL.
GameObjectPtr GameObjectManagerGetObjectByName(const char* name)
{
	return GameObjectManagerFindName(&gameObjectActiveList.objectList[0], name);
}

// Returns a pointer to the first game object archetype matching the specified name.
// (Hint: This function and the GameObjectManagerGetObjectByName functions require
//    the same code.  Consider creating a single, private function that searches
//	  for the named game object in a list that is passed as a parameter.)
// Params:
//	 name = The name of the archetype to be returned, if found.
// Returns:
//   If the named archetype is found,
//	   then return the pointer to the named game object archetype,
//	   else return NULL.
GameObjectPtr GameObjectManagerGetArchetype(const char* name)
{
	return GameObjectManagerFindName(&gameObjectArchetypes.objectList[0], name);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
void GameObjectManagerAddToList(GameObjectPtr gameObject, GameObjectPtr *list)
{
	if (gameObject == NULL) return;

	GameObjectPtr* stepper = list;

	int i = 0;
	while (i < managerMax)
	{
		if (*(stepper + i) == NULL)
		{
			*(stepper + i) = gameObject;
			return;
		}
		i++;
	}
}

GameObjectPtr GameObjectManagerFindName(GameObjectPtr *list, const char* name)
{
	if (list == NULL || name == NULL) return NULL;

	GameObjectPtr* stepper = list;

	int i = 0;
	while (i < managerMax)
	{
		if (*(stepper + i) == NULL)
		{
			i++;
			continue;
		}

		if (GameObjectIsNamed(*(stepper + i), name))
			return *(stepper + i);

		i++;
	}

	return NULL;
}