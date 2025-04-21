/*
 * app_main.c
 *
 *  Created on: Apr 16, 2025
 *      Author: virgilreturns
 */

#include "main.h"
#include <stdbool.h>
#include "seven_segment_driver.h"

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
}GPIO_PIN_TypeDef;

extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern SEVSEG_DISPLAY_TypeDef sevseg;

volatile static bool UI_CURSOR_PRESSED = false;
volatile static bool UI_COUNTUP_PRESSED = false;
volatile static bool UI_COUNTDOWN_PRESSED = false;
volatile static bool debounce = true;

volatile enum ENUM_SEVSEG_DIGIT cursor_selection = ENUM_SEVSEG_DIGIT_0; //maybe add a UI handler
GPIO_PIN_TypeDef DIGIT_SEL_PINS_ARRAY[];

static void SEVSEG_Init();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef*);


int app_main(){

SEVSEG_Init();

	uint8_t myDataa[5] = {ENUM_SEVSEG_CHAR_H,
					  ENUM_SEVSEG_CHAR_E,
					  ENUM_SEVSEG_CHAR_L,
					  ENUM_SEVSEG_CHAR_L,
					  ENUM_SEVSEG_CHAR_o};

	SEVSEG_StoreDataBuf(&sevseg, myDataa);

	volatile enum ENUM_SEVSEG_CHAR test1;

	/* |||||||||||||||||||||  LOOP  ||||||||||||||||||||||||| */

	while(1) {

		/* Task 1: Process Inputs

		if (debounce) {
			debounce = false;
			if (UI_CURSOR_PRESSED) {
				UI_CURSOR_PRESSED = false;


			} else if (UI_COUNTUP_PRESSED){
				UI_COUNTUP_PRESSED = false;



			} else if (UI_COUNTDOWN_PRESSED){
				UI_COUNTUP_PRESSED = false;


			}

		} /* debounce */

		/* Task 2: Render Display */

		test1 = SEVSEG_ReadDigitData(&sevseg, sevseg.refresh_target);
		HAL_SPI_Transmit(&hspi2, &SEVSEG_CHAR_ARRAY[sevseg.digit_select[sevseg.refresh_target].current_char_index] , 1, 100);
		HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SPI_LATCH_GPIO_Port, SPI_LATCH_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(DIGIT_SEL_PINS_ARRAY[sevseg.refresh_target].port,
						  DIGIT_SEL_PINS_ARRAY[sevseg.refresh_target].pin, GPIO_PIN_SET);
		HAL_Delay(2);
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


static void SEVSEG_Init(){

	// contiguous array of GPIO pins, used for multiplexing displays
	GPIO_PIN_TypeDef DIGIT_SEL_PINS_ARRAY[8] = {
			[ENUM_SEVSEG_DIGIT_0].port = DIGIT_SEL_0_GPIO_Port,
			[ENUM_SEVSEG_DIGIT_0].pin = DIGIT_SEL_0_Pin,
			[ENUM_SEVSEG_DIGIT_1].port = DIGIT_SEL_1_GPIO_Port,
			[ENUM_SEVSEG_DIGIT_1].pin = DIGIT_SEL_1_Pin,
			[ENUM_SEVSEG_DIGIT_2].port = DIGIT_SEL_2_GPIO_Port,
			[ENUM_SEVSEG_DIGIT_2].pin = DIGIT_SEL_2_Pin,
			[ENUM_SEVSEG_DIGIT_3].port = DIGIT_SEL_3_GPIO_Port,
			[ENUM_SEVSEG_DIGIT_3].pin = DIGIT_SEL_3_Pin,
			[ENUM_SEVSEG_DIGIT_4].port = DIGIT_SEL_4_GPIO_Port,
			[ENUM_SEVSEG_DIGIT_4].pin = DIGIT_SEL_4_Pin,
			[ENUM_SEVSEG_DIGIT_5].port = DIGIT_SEL_1_GPIO_Port,
			[ENUM_SEVSEG_DIGIT_5].pin = DIGIT_SEL_1_Pin,
			[ENUM_SEVSEG_DIGIT_6].port = DIGIT_SEL_2_GPIO_Port,
			[ENUM_SEVSEG_DIGIT_6].pin = DIGIT_SEL_2_Pin,
			[ENUM_SEVSEG_DIGIT_7].port = DIGIT_SEL_3_GPIO_Port,
			[ENUM_SEVSEG_DIGIT_7].pin = DIGIT_SEL_3_Pin,
	};

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

	sevseg.digit_select[4].DS_pin = DIGIT_SEL_4_Pin;
	sevseg.digit_select[4].DS_port = DIGIT_SEL_4_GPIO_Port;
	sevseg.digit_select[4].current_char_index = 0;

	sevseg.refresh_target = ENUM_SEVSEG_DIGIT_0;

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
}

void HAL_GPIO_EXTI_Callback(uint16_t pin){
	return;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi){
}

