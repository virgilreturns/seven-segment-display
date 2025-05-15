/*
 * app_main.c
 *
 *  Created on: Apr 16, 2025
 *      Author: virgilreturns
 */

#include "seven_segment_driver.h"
#include "main.h"
#include <stdbool.h>




/* |||||||||||||||||||||  DECLARATIONS  ||||||||||||||||||||||||| */

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
}GPIO_PIN_TypeDef;

extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim3;

SEVSEG_DISPLAY_TypeDef sevseg;

volatile int UI_CURSOR_PRESSED = 0;
volatile int UI_COUNTUP_PRESSED = 0;
volatile int UI_COUNTDOWN_PRESSED = 0;
volatile int TIM2_UP = 0;
volatile int TIM3_UP = 0;
volatile static uint8_t temp;


volatile enum ENUM_SEVSEG_DIGIT cursor_selection = ENUM_SEVSEG_DIGIT_0;
GPIO_PIN_TypeDef DIGIT_SEL_PINS_ARRAY[8];

static void SEVSEG_Init();


/* |||||||||||||||||||||  MAIN  ||||||||||||||||||||||||| */

int app_main(){

	// this is used in sevseg_init
	uint8_t myDataa2[SEVSEG_DATA_BUF_SIZE] = {
				ENUM_SEVSEG_CHAR_H,
				ENUM_SEVSEG_CHAR_E,
				ENUM_SEVSEG_CHAR_L,
				ENUM_SEVSEG_CHAR_L,
				ENUM_SEVSEG_CHAR_0,
				ENUM_SEVSEG_CHAR_Blank,
				ENUM_SEVSEG_CHAR_A,
				ENUM_SEVSEG_CHAR_n,
				ENUM_SEVSEG_CHAR_D,
				ENUM_SEVSEG_CHAR_E,
				ENUM_SEVSEG_CHAR_r,
				ENUM_SEVSEG_CHAR_5,
				ENUM_SEVSEG_CHAR_o,
				ENUM_SEVSEG_CHAR_n,
				ENUM_SEVSEG_CHAR_Blank,
				ENUM_SEVSEG_CHAR_Blank,

		};

	uint8_t myDataa[4] = {
			ENUM_SEVSEG_CHAR_A,
			ENUM_SEVSEG_CHAR_n,
			ENUM_SEVSEG_CHAR_D,
			ENUM_SEVSEG_CHAR_Y,
	};

	SEVSEG_Init(myDataa);
	SEVSEG_StoreDataWindow(&sevseg, myDataa2); // will store the first 4 of data buffer myDataa
											  // after, only called with sevesg.data_window as 2nd arg

	// HAL_TIM_Base_Start_IT(&htim3);

/* |||||||||||||||||||||  LOOP  ||||||||||||||||||||||||| */

	while(1) {

		/* Task 0: Forced State Reconcilation */
		/* if (sevseg.state == SEVSEG_STATE_SCROLLING){ // set to scrolling at sevseg_init() simple solutions:
													 //	implement user controlled play button
													 // cursor button puts into edit mode, must go through all
													 // digit options to return to play mode !!

			TIM3->CR1 |= TIM_CR1_CEN; // 	constantly setting this bit
		}

		*/

		/* Task 1: Polling and Processing */


		if (UI_CURSOR_PRESSED) {
			UI_CURSOR_PRESSED = false; //acknowledge

			if (sevseg.state == SEVSEG_STATE_EDITING){ // if already in edit mode, increment digit selection

				if (cursor_selection == ( SEVSEG_QTY_DIGITS - 1)){ // return to scroll state if pressed on the last digit
					sevseg.state = SEVSEG_STATE_SCROLLING; }

				else {cursor_selection++;} // increment cursor

				TIM2->CR1 |= TIM_CR1_CEN; // enable debounce timer
				}

			else { // entering edit state
				sevseg.state = SEVSEG_STATE_EDITING;
				cursor_selection = 0;
			}

		}

		if (UI_COUNTUP_PRESSED){
			UI_COUNTUP_PRESSED = false;

			temp = sevseg.digit_select[cursor_selection].current_char_index;
			if (temp == ENUM_SEVSEG_CHAR_Blank) temp = ENUM_SEVSEG_CHAR_0;
			else temp++;
			sevseg.digit_select[cursor_selection].current_char_index = temp;

			TIM2->CR1 |= TIM_CR1_CEN; // enable debounce timer

		}

		if (UI_COUNTDOWN_PRESSED)	{
			UI_COUNTUP_PRESSED = false;
			temp = sevseg.digit_select[cursor_selection].current_char_index;
			if (temp == ENUM_SEVSEG_CHAR_0) temp = ENUM_SEVSEG_CHAR_Blank;
			else temp--;
			sevseg.digit_select[cursor_selection].current_char_index = temp;

			TIM2->CR1 |= TIM_CR1_CEN;// enable debounce timer
		}


		//scroll every 500 ms

		 if (TIM3_UP) {
			TIM3_UP = false; // acknowledge, timer will stay on regardless of state

			if(sevseg.state == SEVSEG_STATE_SCROLLING) { // only scroll if in proper state
				SEVSEG_ScrollDataWindow(&sevseg);} // seven_segment_driver.c
		 }

		/* Task 2: Render Display */

		HAL_SPI_Transmit(&hspi2, &SEVSEG_CHAR_ARRAY[sevseg.digit_select[sevseg.refresh_target].current_char_index] , 1, 100);
		HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(DIGIT_SEL_PINS_ARRAY[sevseg.refresh_target].port,
						  DIGIT_SEL_PINS_ARRAY[sevseg.refresh_target].pin, GPIO_PIN_SET);
		HAL_Delay(1);

		// consider sandwiching processing task during this delay(2) for optmization

		HAL_GPIO_WritePin(DIGIT_SEL_PINS_ARRAY[sevseg.refresh_target].port,
						  DIGIT_SEL_PINS_ARRAY[sevseg.refresh_target].pin, GPIO_PIN_RESET);

		if (sevseg.refresh_target == SEVSEG_QTY_DIGITS - 1) {
			sevseg.refresh_target = ENUM_SEVSEG_DIGIT_0;
		}
		else {
		sevseg.refresh_target += 1;
		}

	} //end while

} //end app_main

/* |||||||||||||||||||||  DEFINITIONS  ||||||||||||||||||||||||| */

static void SEVSEG_Init(uint8_t data_buf[]){

	for (int i = 0; i<SEVSEG_DATA_BUF_SIZE; i++) {
		sevseg.data_buf[i] = data_buf[i];
	}

	sevseg.state = SEVSEG_STATE_SCROLLING;

	// contiguous array of GPIO pins, used for multiplexing displays

	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_0].port = DIGIT_SEL_0_GPIO_Port;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_0].pin = DIGIT_SEL_0_Pin;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_1].port = DIGIT_SEL_1_GPIO_Port;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_1].pin = DIGIT_SEL_1_Pin;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_2].port = DIGIT_SEL_2_GPIO_Port;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_2].pin = DIGIT_SEL_2_Pin;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_3].port = DIGIT_SEL_3_GPIO_Port;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_3].pin = DIGIT_SEL_3_Pin;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_4].port = DIGIT_SEL_4_GPIO_Port;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_4].pin = DIGIT_SEL_4_Pin,
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_5].port = DIGIT_SEL_1_GPIO_Port;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_5].pin = DIGIT_SEL_1_Pin;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_6].port = DIGIT_SEL_2_GPIO_Port;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_6].pin = DIGIT_SEL_2_Pin,
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_7].port = DIGIT_SEL_3_GPIO_Port;
	DIGIT_SEL_PINS_ARRAY[ENUM_SEVSEG_DIGIT_7].pin = DIGIT_SEL_3_Pin;


	sevseg.digit_select[0].DS_pin = DIGIT_SEL_0_Pin;
	sevseg.digit_select[0].DS_port = DIGIT_SEL_0_GPIO_Port;
	sevseg.digit_select[0].current_char_index = 0;

	sevseg.digit_select[1].DS_pin = DIGIT_SEL_1_Pin;
	sevseg.digit_select[1].DS_port = DIGIT_SEL_1_GPIO_Port;
	sevseg.digit_select[1].current_char_index = 0;

	sevseg.digit_select[2].DS_pin = DIGIT_SEL_2_Pin;
	sevseg.digit_select[2].DS_port = DIGIT_SEL_2_GPIO_Port;
	sevseg.digit_select[2].current_char_index = 0;

	sevseg.digit_select[3].DS_pin = DIGIT_SEL_3_Pin;
	sevseg.digit_select[3].DS_port = DIGIT_SEL_3_GPIO_Port;
	sevseg.digit_select[3].current_char_index = 0;

	sevseg.refresh_target = ENUM_SEVSEG_DIGIT_0;

}

static const uint16_t UI_ALL_BITS = 7; // 0b0111

void HAL_GPIO_EXTI_Callback(uint16_t pin){
	if (pin == UI_CURSOR_Pin) {
		UI_CURSOR_PRESSED = 1; //invoke
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		EXTI->EMR &=~ UI_ALL_BITS; // disable exti lines 0:2
	} else if (pin == UI_COUNTDOWN_Pin) {
		UI_COUNTDOWN_PRESSED = 1;
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		EXTI->EMR &=~ UI_ALL_BITS;
	} else if (pin == UI_COUNTUP_Pin) {
		UI_COUNTUP_PRESSED = 1;
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		EXTI->EMR &=~ UI_ALL_BITS;
	}
	else {
		__NOP();
	}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (htim -> Instance == TIM3){ // scrolling update timer
	  //HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  TIM3_UP = true;
  }
  if (htim -> Instance == TIM2){ //debounce timer 10 kHz -> 50 ms UP One-Pulse-Mode
	  EXTI->EMR |= UI_ALL_BITS;
  }
}



