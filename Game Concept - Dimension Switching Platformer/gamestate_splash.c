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
#include "gamestate_splash.h"
#include "gamestate_menu.h"
#include <math.h>

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void gamestate_splash_init(void)
{
	CP_System_SetWindowSize(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight());
	// initialize variables and CProcessing settings for this gamestate
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void gamestate_splash_update(void)
{
	// sets background to black at each update
	CP_Settings_Background(CP_Color_Create(0, 0, 0, 255));

	// define variable with DigiPen Logo (WHITE) from assets folder
	CP_Image DigiPen_Logo = CP_Image_Load("./Assets/DigiPen_WHITE.png");

	// define time variables
	float time_max = 3.0; // desired total time for animation - 2 seconds - 2000 millis
	float time_actual = CP_System_GetSeconds(); // time transpired
	float time_ratio = (time_actual / time_max); // percent of time transpired to time total

	// defines the notes for the sound portion
	CP_Sound G_Note = CP_Sound_Load("./Assets/Piano/g.wav");
	CP_Sound E_Note = CP_Sound_Load("./Assets/Piano/e.wav");
	CP_Sound C_Note = CP_Sound_Load("./Assets/Piano/c.wav");

	if (time_ratio < 1.0f)
	{
		if (0.99f < time_ratio && time_ratio < 1.0f) // sets the last note
		{
			CP_Sound_Play(C_Note);
		}
		else if (0.60f < time_ratio && time_ratio < 0.61f)  // sets the second note
		{
			CP_Sound_Play(E_Note);
		}
		else if (0.20f < time_ratio && time_ratio < 0.21f) // sets the last note
		{
			CP_Sound_Play(G_Note);
		}


		// define math variables
		float magnitude = 1.0f - (time_ratio * 0.5f);
		double pi = 3.14159265359;

		// defining the x-movement of the image
		float ImageX = CP_System_GetWindowWidth() * 0.5f * (sqrtf(time_ratio));

		int alpha = 0 + (int)(255 * time_ratio); // has the image fade in

		CP_Image_Draw(DigiPen_Logo, // defines the image that is being modified
			ImageX, // applies the x-movement of the image
			(CP_System_GetWindowHeight() * 0.5f) - fabsf(cosf(time_ratio * 5.0f * (float)(pi / 2.0))) * (CP_System_GetWindowHeight() * 0.5f) * magnitude, // coordinates the "bounce" movement
			CP_Image_GetWidth(DigiPen_Logo) * time_ratio, // expands the image along with the time ratio
			CP_Image_GetHeight(DigiPen_Logo) * time_ratio,
			alpha);
	}
	else if (time_ratio > 1.0f) // stops the logo when it reached half-screen
	{
		time_ratio = 1.0f; // set time_ratio to 1 
		CP_Image_Draw(DigiPen_Logo, // defines the image that is being modified
			CP_System_GetWindowWidth() * 0.5f, // has logo stationary at the cetner of the screen
			CP_System_GetWindowHeight() * 0.5f,
			CP_Image_GetWidth(DigiPen_Logo) * time_ratio,
			CP_Image_GetHeight(DigiPen_Logo) * time_ratio,
			255);


		float wait_time = CP_System_GetSeconds() - time_max; // sets wait time variable

		if (wait_time > 2.0f) // indicates that two seconds have passed
		{
			wait_time = 2.0f;

			// replicating the original time variables to use for the 'exit' sequence
			float time_growth_max = 1.0f;
			float time_growth_actual = CP_System_GetSeconds() - time_max - 2.0f;
			float time_growth_ratio = time_growth_actual / time_growth_max;

			// draws a white circle with no stroke at the center of the screen which expands based off of the growth ratio
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
			CP_Settings_NoStroke();
			CP_Graphics_DrawCircle(CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f, CP_System_GetWindowWidth() * time_growth_ratio);

			if (time_growth_ratio > 1.0f)
			{
				time_growth_ratio = 1.0f; // stops the circle from continuing the grow
				CP_Engine_SetNextGameState(gamestate_menu_init, gamestate_menu_update, gamestate_menu_exit);
			}

		}
	} // check input, update simulation, render etc.
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void gamestate_splash_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
}
