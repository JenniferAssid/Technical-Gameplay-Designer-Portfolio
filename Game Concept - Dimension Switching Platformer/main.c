//---------------------------------------------------------
// file:	main.c
// author:	Jennifer Assid
// email:	jennifer.assid@digipen.edu
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include <stdlib.h>
#include "collisions.h"
#include "gamestate_splash.h"


// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void)
{
	CP_Engine_SetNextGameState(gamestate_splash_init, gamestate_splash_update, gamestate_splash_exit);
	CP_Engine_Run();
	return 0;
}
