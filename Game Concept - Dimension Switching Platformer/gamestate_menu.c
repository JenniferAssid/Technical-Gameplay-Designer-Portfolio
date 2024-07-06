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

CP_Image spriteCharacter;
CP_Image cursorFlame;

CP_Color worldPrimary, worldSecondary;
int worldState, worldChange;

float offsetStartButton, offsetCreditsButton, offsetExitButton, offsetBackButton;
float offsetCreditsScreen, offsetMainScreen;

float animation_timeDelay, animation_timeElapsed, animation_timeLastTick;

float cellwidth, cellheight;
float chibiSUB_1, chibiSUB_2;

void idleRightDEFAULT_SubDraw(float row, float column)
{
	
	CP_Image_DrawSubImage(spriteCharacter,
		(CP_System_GetWindowWidth() / 2.0f) - (CP_System_GetWindowWidth() / 48.0f) - offsetMainScreen,
		(2.0f * CP_System_GetWindowHeight() / 3.0f) - (CP_System_GetWindowWidth() / 12.0f),
		(CP_System_GetWindowWidth() / 6.0f),
		(CP_System_GetWindowWidth() / 6.0f),
		0.0f,
		row,
		240.0f,
		column,
		255);
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_menu_init(void)
{
	CP_Settings_Background(CP_Color_Create(0, 0, 0, 255));

	CP_System_ShowCursor(0); // makes cursor invisiible

	// loads images for character and cursor
	spriteCharacter = CP_Image_Load("./Assets/worldstateChibi-SpriteSheet.png");
	cursorFlame = CP_Image_Load("./Assets/Flame.png");

	// sets the offset for each button
	offsetStartButton = CP_System_GetWindowHeight() / 24.0f;
	offsetCreditsButton = CP_System_GetWindowHeight() / 24.0f;
	offsetExitButton = CP_System_GetWindowHeight() / 24.0f;
	offsetBackButton = CP_System_GetWindowHeight() / 24.0f;

	// sets default offset for each screen
	offsetCreditsScreen = CP_System_GetWindowWidth() * 1.0f;
	offsetMainScreen = 0.0f;

	// sets colors
	worldPrimary = CP_Color_Create(0, 0, 0, 255);
	worldSecondary = CP_Color_Create(255, 255, 255, 255);

	// sets world state determining variables
	worldState = 0;
	worldChange = 0;

	// gives subimage cell width / height
	cellwidth, cellheight = 240.0f;

}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_menu_update(void)
{
	// checks if world state is changed and determines which one needs to be shown
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
		// sets default variable values
		worldPrimary = CP_Color_Create(0, 0, 0, 255);
		worldSecondary = CP_Color_Create(255, 255, 255, 255);
		chibiSUB_1 = 480.0f;
		chibiSUB_2 = 720.0f;
	}

	if (worldState == 1)
	{
		// sets change variable values
		worldPrimary = CP_Color_Create(255, 255, 255, 255);
		worldSecondary = CP_Color_Create(0, 0, 0, 255);
		chibiSUB_1 = 0.0f;
		chibiSUB_2 = 240.0f;
	}

	CP_Settings_Background(worldPrimary);

	// sets the shadow underneath the player
	CP_Settings_Fill(CP_Color_Create(127, 127, 127, 255));
	CP_Settings_NoStroke();
	CP_Graphics_DrawEllipse(CP_System_GetWindowWidth() / 2.0f - offsetMainScreen,
		2.0f * CP_System_GetWindowHeight() / 3.0f,
		CP_System_GetWindowWidth() / 6.0f,
		1.5f * CP_System_GetWindowHeight() / 12.0f);

	// draws the character based off of world state values
	idleRightDEFAULT_SubDraw(chibiSUB_1, chibiSUB_2);
	
	// Start Button
	CP_Settings_Fill(worldSecondary);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect((CP_System_GetWindowWidth() / 24.0f) - offsetMainScreen,
		-offsetStartButton,
		(CP_System_GetWindowWidth() / 4.0f),
		(CP_System_GetWindowHeight() / 6.0f));

	// have button pop out when cursor hovering over it
	if (CP_Input_GetMouseX() >= (CP_System_GetWindowWidth() / 24.0f) &&
		CP_Input_GetMouseX() <= ((CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= -offsetStartButton &&
		CP_Input_GetMouseY() <= ((0.0f + offsetStartButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		offsetStartButton = 0.0;
	}
	else
	{
		offsetStartButton = CP_System_GetWindowHeight() / 12.0f;
	}

	// starts game with button is clicked
	if (CP_Input_MouseClicked() &&
		CP_Input_GetMouseX() >= (CP_System_GetWindowWidth() / 24.0f) &&
		CP_Input_GetMouseX() <= ((CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= -offsetStartButton &&
		CP_Input_GetMouseY() <= ((0.0f + offsetStartButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		CP_Engine_SetNextGameState(gamestate_game_init, gamestate_game_update, gamestate_game_exit);
	}

	// Credits Button
	CP_Settings_Fill(worldSecondary);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect(9.0f * (CP_System_GetWindowWidth() / 24.0f) - offsetMainScreen,
		-(offsetCreditsButton),
		(CP_System_GetWindowWidth() / 4.0f),
		(CP_System_GetWindowHeight() / 6.0f));

	// have button pop out when cursor hovering over it
	if (CP_Input_GetMouseX() >= (9.0f * (CP_System_GetWindowWidth() / 24.0f)) &&
		CP_Input_GetMouseX() <= (9.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= 0.0f &&
		CP_Input_GetMouseY() <= ((0.0f + offsetCreditsButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		offsetCreditsButton = 0.0;
	}
	else
	{
		offsetCreditsButton = CP_System_GetWindowHeight() / 12.0f;
	}

	// changes offset values for the screen and swaps screens
	if (CP_Input_MouseClicked() &&
		CP_Input_GetMouseX() >= (9.0f * (CP_System_GetWindowWidth() / 24.0f)) &&
		CP_Input_GetMouseX() <= (9.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= 0.0f &&
		CP_Input_GetMouseY() <= ((0.0f + offsetCreditsButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		offsetMainScreen = CP_System_GetWindowWidth() * 1.0f;
		offsetCreditsScreen = 0.0f;
	}

	// Exit Button
	CP_Settings_Fill(worldSecondary);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect(17.0f * (CP_System_GetWindowWidth() / 24.0f) - offsetMainScreen,
		-(offsetExitButton),
		(CP_System_GetWindowWidth() / 4.0f),
		(CP_System_GetWindowHeight() / 6.0f));

	// have button pop out when cursor hovering over it
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

	// terminates game when button clicked
	if (CP_Input_MouseClicked() &&
		CP_Input_GetMouseX() >= (17.0f * (CP_System_GetWindowWidth() / 24.0f)) &&
		CP_Input_GetMouseX() <= (17.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= 0.0f &&
		CP_Input_GetMouseY() <= ((0.0f + offsetExitButton) + (CP_System_GetWindowHeight() / 6.0f)))
	{
		CP_Engine_Terminate();
	}
	
	// Credits Screen

	// Back Button
	CP_Settings_Fill(worldSecondary);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect(9.0f * (CP_System_GetWindowWidth() / 24.0f) + offsetCreditsScreen,
		(10.5f * CP_System_GetWindowHeight() / 12.0f) + offsetBackButton,
		(CP_System_GetWindowWidth() / 4.0f),
		(CP_System_GetWindowHeight() / 6.0f));

	// have button pop out when cursor hovering over it
	if (CP_Input_GetMouseX() >= 9.0f * (CP_System_GetWindowWidth() / 24.0f) &&
		CP_Input_GetMouseX() <= (9.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= ((10.5f * CP_System_GetWindowHeight() / 12.0f) + offsetBackButton) &&
		CP_Input_GetMouseY() <= CP_System_GetWindowHeight() * 1.0f)
	{
		offsetBackButton = 0.0;
	}
	else
	{
		offsetBackButton = CP_System_GetWindowHeight() / 24.0f;
	}

	// returns offest values to default and swaps screens
	if (CP_Input_MouseClicked() &&
		CP_Input_GetMouseX() >= (9.0f * (CP_System_GetWindowWidth() / 24.0f)) &&
		CP_Input_GetMouseX() <= (9.0f * (CP_System_GetWindowWidth() / 24.0f) + (CP_System_GetWindowWidth() / 4.0f)) &&
		CP_Input_GetMouseY() >= ((10.5f * CP_System_GetWindowHeight() / 12.0f) + offsetBackButton) &&
		CP_Input_GetMouseY() <= CP_System_GetWindowHeight() * 1.0f)
	{
		offsetCreditsScreen = CP_System_GetWindowWidth() * 1.0f;
		offsetMainScreen = 0.0f;
	}

	// Menu Text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(CP_System_GetWindowHeight() / 12.0f);
	CP_Settings_Fill(worldPrimary);
	CP_Font_DrawText("Start",
		(CP_System_GetWindowWidth() / 6.0f) + offsetMainScreen,
		(CP_System_GetWindowHeight() / 8.0f) - offsetStartButton);

	CP_Font_DrawText("Credits",
		(CP_System_GetWindowWidth() / 2.0f) + offsetMainScreen,
		(CP_System_GetWindowHeight() / 8.0f) - offsetCreditsButton);

	CP_Font_DrawText("Exit",
		5.0f * (CP_System_GetWindowWidth() / 6.0f) + offsetMainScreen,
		(CP_System_GetWindowHeight() / 8.0f) - offsetExitButton);

	CP_Font_DrawText("Back",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		(11.0f * CP_System_GetWindowHeight() / 12.0f) + offsetBackButton);

	// Credits Text
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_Fill(worldSecondary);
	CP_Settings_TextSize(CP_System_GetWindowHeight() / 24.0f);

	CP_Font_DrawText("made by",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		CP_System_GetWindowHeight() / 6.0f);

	CP_Settings_TextSize(CP_System_GetWindowHeight() / 12.0f);
	CP_Font_DrawText("Jennifer Assid",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		CP_System_GetWindowHeight() / 4.0f);

	CP_Settings_TextSize(CP_System_GetWindowHeight() / 24.0f);
	CP_Font_DrawText("special thanks to",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		CP_System_GetWindowHeight() / 3.0f);
	
	CP_Settings_TextSize(CP_System_GetWindowHeight() / 10.0f);
	CP_Font_DrawText("Justin Chambers",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		5.0f * CP_System_GetWindowHeight() / 12.0f);

	CP_Settings_TextSize(CP_System_GetWindowHeight() / 10.0f);
	CP_Font_DrawText("Adam De Broeck",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		CP_System_GetWindowHeight() / 2.0f);

	CP_Settings_TextSize(CP_System_GetWindowHeight() / 10.0f);
	CP_Font_DrawText("Wyley Frank",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		7.0f * CP_System_GetWindowHeight() / 12.0f);

	CP_Settings_TextSize(CP_System_GetWindowHeight() / 10.0f);
	CP_Font_DrawText("Dillon Goicoechea",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		2.0f * CP_System_GetWindowHeight() / 3.0f);

	CP_Settings_TextSize(CP_System_GetWindowHeight() / 24.0f);
	CP_Font_DrawText("Copyright (c) 2020 DigiPen, All rights reserved.",
		(CP_System_GetWindowWidth() / 2.0f) + offsetCreditsScreen,
		5.0f * CP_System_GetWindowHeight() / 6.0f);

	CP_Image_Draw(cursorFlame,
		CP_Input_GetMouseX(),
		CP_Input_GetMouseY(),
		CP_System_GetWindowWidth() / 12.0f,
		CP_System_GetWindowWidth() / 12.0f,
		255);
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_menu_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
