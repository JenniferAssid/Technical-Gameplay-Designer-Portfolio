//---------------------------------------------------------
// file:	gamestate_template.c
// author:	[NAME]
// email:	[DIGIPEN EMAIL ADDRESS]
//
// brief:	template file for holding gamestate functions
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "collisions.h"
#include "gamestate_menu.h"

CP_Image spriteCharacter;
CP_Image cursorFlame;

CP_Color worldPrimary, worldSecondary;
int worldState, worldChange;

float offsetRestartButton, offsetMenuButton, offsetExitButton;

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_gameover_init(void)
{
	CP_Settings_Background(CP_Color_Create(0, 0, 0, 255));

	CP_System_ShowCursor(0);

	spriteCharacter = CP_Image_Load("./Assets/worldstateChibi-SpriteSheet.png");
	cursorFlame = CP_Image_Load("./Assets/Flame.png");

	offsetRestartButton = CP_System_GetWindowHeight() / 24.0f;
	offsetMenuButton = CP_System_GetWindowHeight() / 24.0f;
	offsetExitButton = CP_System_GetWindowHeight() / 24.0f;

	worldPrimary = CP_Color_Create(0, 0, 0, 255);
	worldSecondary = CP_Color_Create(255, 255, 255, 255);

	worldState = 0;
	worldChange = 0;

}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_gameover_update(void)
{

	if (CP_Input_KeyTriggered(KEY_C))
	{
		worldChange += 1;
	}

	if (worldChange % 2 == 0)
	{
		worldState = 0;
		worldChange = 0;
	}
	else {
		worldState = 1;
	}
	
	if (worldState == 0)
	{
		worldPrimary = CP_Color_Create(0, 0, 0, 255);
		worldSecondary = CP_Color_Create(255, 255, 255, 255);

	}

	if (worldState == 1)
	{
		worldPrimary = CP_Color_Create(255, 255, 255, 255);
		worldSecondary = CP_Color_Create(0, 0, 0, 255);

	}

	CP_Settings_Background(worldPrimary);

	//idleRightDEFAULT_SubDraw(chibiSUB_1, chibiSUB_2);
	
	// Start Button
	CP_Settings_Fill(worldSecondary);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect((CP_System_GetWindowWidth() / 24.0f),
		-offsetRestartButton,
		(CP_System_GetWindowWidth() / 4.0f),
		(CP_System_GetWindowHeight() / 6.0f));

	if (CP_Input_GetMouseX() >= (CP_System_GetWindowWidth() / 24.0f) &&
		CP_Input_GetMouseX() <= ((CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= -offsetRestartButton &&
		CP_Input_GetMouseY() <= ((0.0f + offsetRestartButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		offsetRestartButton = 0.0;
	}
	else
	{
		offsetRestartButton = CP_System_GetWindowHeight() / 12.0f;
	}

	if (CP_Input_MouseClicked() &&
		CP_Input_GetMouseX() >= (CP_System_GetWindowWidth() / 24.0f) &&
		CP_Input_GetMouseX() <= ((CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= -offsetRestartButton &&
		CP_Input_GetMouseY() <= ((0.0f + offsetRestartButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		CP_Engine_SetNextGameState(gamestate_game_init, gamestate_game_update, gamestate_game_exit);
	}

	// Credits Button
	CP_Settings_Fill(worldSecondary);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect(9.0f * (CP_System_GetWindowWidth() / 24.0f),
		-(offsetMenuButton),
		(CP_System_GetWindowWidth() / 4.0f),
		(CP_System_GetWindowHeight() / 6.0f));

	if (CP_Input_GetMouseX() >= (9.0f * (CP_System_GetWindowWidth() / 24.0f)) &&
		CP_Input_GetMouseX() <= (9.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= 0.0f &&
		CP_Input_GetMouseY() <= ((0.0f + offsetMenuButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		offsetMenuButton = 0.0;
	}
	else
	{
		offsetMenuButton = CP_System_GetWindowHeight() / 12.0f;
	}

	if (CP_Input_MouseClicked() &&
		CP_Input_GetMouseX() >= (9.0f * (CP_System_GetWindowWidth() / 24.0f)) &&
		CP_Input_GetMouseX() <= (9.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= 0.0f &&
		CP_Input_GetMouseY() <= ((0.0f + offsetMenuButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		CP_Engine_SetNextGameStateForced(gamestate_menu_init, gamestate_menu_update, gamestate_menu_exit);
	}

	// Exit Button
	CP_Settings_Fill(worldSecondary);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect(17.0f * (CP_System_GetWindowWidth() / 24.0f),
		-(offsetExitButton),
		(CP_System_GetWindowWidth() / 4.0f),
		(CP_System_GetWindowHeight() / 6.0f));

	if (CP_Input_GetMouseX() >= (17.0f * (CP_System_GetWindowWidth() / 24.0f)) &&
		CP_Input_GetMouseX() <= (17.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= 0.0f &&
		CP_Input_GetMouseY() <= ((0.0f + offsetExitButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		offsetExitButton = 0.0;
	}
	else
	{
		offsetExitButton = CP_System_GetWindowHeight() / 12.0f;
	}


	if (CP_Input_MouseClicked() &&
		CP_Input_GetMouseX() >= (17.0f * (CP_System_GetWindowWidth() / 24.0f)) &&
		CP_Input_GetMouseX() <= (17.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= 0.0f &&
		CP_Input_GetMouseY() <= ((0.0f + offsetExitButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		CP_Engine_Terminate();
	}
	

	// Menu Text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(CP_System_GetWindowHeight() / 12.0f);
	CP_Settings_Fill(worldPrimary);
	CP_Font_DrawText("Restart",
		(CP_System_GetWindowWidth() / 6.0f),
		(CP_System_GetWindowHeight() / 8.0f) - offsetRestartButton);

	CP_Font_DrawText("Menu",
		(CP_System_GetWindowWidth() / 2.0f),
		(CP_System_GetWindowHeight() / 8.0f) - offsetMenuButton);

	CP_Font_DrawText("Exit",
		5.0f * (CP_System_GetWindowWidth() / 6.0f),
		(CP_System_GetWindowHeight() / 8.0f) - offsetExitButton);

	CP_Image_Draw(cursorFlame,
		CP_Input_GetMouseX(),
		CP_Input_GetMouseY(),
		CP_System_GetWindowWidth() / 12.0f,
		CP_System_GetWindowWidth() / 12.0f,
		255);
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_gameover_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
