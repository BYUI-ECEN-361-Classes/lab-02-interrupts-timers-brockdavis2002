/**
  ******************************************************************************
  * @file           : ReactionTester.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 BYU-Idaho
  * All rights reserved.
  *
  ******************************************************************************
  * @copyright  BYU-Idaho
  * @date       2023
  * @version    F23
  * @note       For course ECEN-361
  * @author     Lynn Watson
  ******************************************************************************
  */
/* This is for Part-3 of Lab-02, ECEN-361
 * Student to only change parts between the comment blocks:
	  ***** STUDENT TO FILL IN START
 *
 */

#include "main.h"
#include "stm32l4xx_it.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MultiFunctionShield.h"
extern TIM_HandleTypeDef htim3;  // Points to the timer structure   Timer3 is the Reaction Timer
extern void MX_TIM3_Init(void);	// To reset the timer
extern bool got_start_button;
extern bool got_stop_button;
extern bool got_fastest_button;
extern int best_reaction_time_in_millisec;
extern bool started_doing_reaction_timers;
extern bool ready_to_stop;


// Globals
#define upper_limit_millisec_to_wait  7000;  //Give the user up to 7 seconds to wonder

int rand_millisec;
int last_reaction_time_in_millisec = 0;


void show_a_random_number()
	{
	if (!started_doing_reaction_timers)
		{
		rand_millisec =  rand() % upper_limit_millisec_to_wait;
		MultiFunctionShield_Display(rand_millisec);
		HAL_Delay(2000);  // this is how long before the counter on the 7-Seg display
		}
	}

void got_start()
	{
	/* Here's the code to do when the Start Button is pushed
		 When Start is pressed:
		 1.) Display the Waiting "----"
		 2.) Wait for a random number of millisec's
		 3.) Turn on all the 7-Seg lights (that's "GO"
		 4.) Start the Reaction timer. (Hint: Use the same function you used to start the other timers)
		*/
    started_doing_reaction_timers = true;  // Timer has started
    ready_to_stop = false;  // User can't press STOP yet
    Clear_LEDs();  // Clear previous LEDs, ensuring clean display
    rand_millisec = rand() % upper_limit_millisec_to_wait;  // Generate random wait time

    /**************** STUDENT TO FILL IN START HERE ********************/

    // Step 1: Display "----" (waiting)
     MultiFunctionShield_Display(10000);  // Display "----" (out of range)

     // Step 2: Wait for a random time
     HAL_Delay(rand_millisec);  // Wait for the random delay time

     // Step 3: Turn on all Seven-Segment display lights (GO)
     MultiFunctionShield_Display(9999);  // Show "9999" (GO)
     ready_to_stop = true;  // Allow the user to press STOP now

     // Step 4: Start the reaction timer
     HAL_TIM_Base_Start_IT(&htim3);  // Start the timer (TIM3 for reaction timer)


    /**************** STUDENT TO FILL IN END HERE ********************/
	}
void got_stop()
{
		/* Here's the code for the STOP button --
		 * When pushed:
		 1.) Stop the random timer (Hint: There is a similar function you used to start the timer)
		 2.) Read the value of timer
		 3.) Display the value
		 */

		// 1.) Stop the timer

	  /**************** STUDENT TO FILL IN START HERE ********************/
    // Check if the STOP button was pressed prematurely
    // 1.) Check if the STOP button was pressed prematurely
    if (ready_to_stop == false) {
        // Premature STOP press: Display penalty
        MultiFunctionShield_Display(10000);  // Show "----" as a penalty
        HAL_Delay(3000);  // Show penalty for 3 seconds
        MultiFunctionShield_Clear();  // Clear the display after penalty
        HAL_Delay(3000);  // Show penalty for 3 seconds
        MultiFunctionShield_Display(10000);  // Show "----" as a penalty
        HAL_Delay(3000);  // Show penalty for 3 seconds
        MultiFunctionShield_Clear();  // Clear the display after penalty
        HAL_Delay(3000);  // Show penalty for 3 seconds
        MultiFunctionShield_Display(10000);  // Show "----" as a penalty
        started_doing_reaction_timers = false;  // Reset the timer state
        ready_to_stop = true;  // Reset stop readiness
        return;  // Exit without further processing
    }

    // 2.) Stop the reaction timer
    HAL_TIM_Base_Stop_IT(&htim3);  // Stop the timer (this is crucial)

    // 3.) Read the value of the timer (convert counter value to milliseconds)
    last_reaction_time_in_millisec = __HAL_TIM_GetCounter(&htim3) / 10;  // Convert microseconds to milliseconds

    // 4.) Display the reaction time on the 4-digit display
    if (last_reaction_time_in_millisec < 10000) {
        MultiFunctionShield_Display(last_reaction_time_in_millisec);  // Display reaction time
    }

    // 5.) Update the fastest reaction time if applicable
    if (last_reaction_time_in_millisec < best_reaction_time_in_millisec) {
        best_reaction_time_in_millisec = last_reaction_time_in_millisec;
    }

    // 6.) Reset the state for the next round
    started_doing_reaction_timers = false;
    ready_to_stop = false;  // Reset stop readiness for the next round

    // Debugging output (optional)
    printf("Random Delay was: %d\n\r", rand_millisec);
    printf("Reaction Time from Timer   : %d\n\r", last_reaction_time_in_millisec);

    // 7.) Reseed RNG for randomness in the next round
    srand((unsigned) last_reaction_time_in_millisec);

    /**************** STUDENT TO FILL IN END HERE ********************/
		// Keep the best time in a global variable
		if (last_reaction_time_in_millisec < best_reaction_time_in_millisec) best_reaction_time_in_millisec = last_reaction_time_in_millisec;
		// Show some stats
		printf("Random Delay was: %d\n\r", rand_millisec );
		printf("Reaction Time from Timer   : %d\n\r", last_reaction_time_in_millisec);
		// Just to keep things random -- reseed with the last reaction time
	    srand((unsigned) last_reaction_time_in_millisec );
}



void got_fastest()
{
    got_fastest_button = false;  // Reset the flag

    // Display the fastest reaction time on the 4-digit display
    MultiFunctionShield_Display(best_reaction_time_in_millisec);
}


