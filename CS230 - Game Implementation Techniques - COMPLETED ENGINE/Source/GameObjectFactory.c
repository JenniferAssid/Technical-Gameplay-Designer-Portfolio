//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.c
// Author(s):	Jennifer Assid (jennifer.assid)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectFactory.h"
#include "Stream.h"
#include "GameObject.h"
#include "GameObjectManager.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Build a single instance of the specified game object.
// Params:
//	 filename = The name of the file to be deserialized.
// Returns:
//	 If the filename is valid
//	   then return a pointer to a new instance of the specified game object,
//	   else NULL.
GameObjectPtr GameObjectFactoryBuild(const char* objectName)
{
  if (objectName == NULL) return NULL;
  
  GameObjectPtr archetype = GameObjectManagerGetArchetype(objectName);

  if (archetype == NULL)
  {
    char pathName[FILENAME_MAX] = "";
    sprintf_s(pathName, _countof(pathName), "Data/%s.txt", objectName);

    Stream file = StreamOpen(pathName);

    if (file == NULL) return NULL;

    const char* token = StreamReadToken(file);

    if (strncmp(token, "GameObject", sizeof(*token)) == 0)
    {
      GameObjectPtr newArchetype = GameObjectCreate();
      GameObjectRead(newArchetype, file);
      GameObjectManagerAddArchetype(newArchetype);

      GameObjectPtr archetypeClone = GameObjectClone(newArchetype);
      return archetypeClone;
    }
  }
  else
  {
    GameObjectPtr archetypeClone = GameObjectClone(archetype);
    return archetypeClone;
  }

  return NULL;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

