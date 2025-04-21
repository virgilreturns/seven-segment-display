/**
 ******************************************************************************
  * @file    seven_segment_driver.c
  * @author  Anderson Nguyen
  * @brief   Seven Segment Display Driver.
  * 
  *          This file provides firmware functions to use SPI with a single
			 SIPO shift register 74HC595 to interface with a Seven Segment 
			 Display of up to 8 digits.
  *          
  *
  ******************************************************************************
  @virgilreturns
  ==============================================================================
                 ##### Seven Segment Display Driver Features #####
  ==============================================================================

  [..]
  This driver comes with customized mode functionality for user interface with 
  a seven segment display. The purpose of this driver is to provide a simple way
  for a user to change values on a seven segment display using only a few discrete
  buttons. This would function as a basic text editor.

  Such UI implementations include:
  (+) One button config, where a single button is used to cycle through
	  the characters on the display; only compatible with a single digit.
  (+) Two button config, where one button is used to increment the character and
	  another to decrement the character.
  (+) Two button config plus, where a single button is used to cycle through
	  the characters and another to cycle through the digits.
  (+) Three button config, where one button is used to increment the character,
	  another to decrement the character, and a third to cycle digits.
  (+) Rotary Encoder config, where a rotary encoder can cycle digits or character, 
	  and a switch multiplexes which to cycle through
  
					   ##### How to use this driver #####
  ==============================================================================

	(#) define SEVSEG_QTY_DIGITS to the number of digits on the display

	(#) Create a SEVSEG_DISPLAY_TypeDef object and initialize it with the SPI handler
		and the GPIO digit select pins for each digit.

	(#) Initialize buttons/switches and other UI elements for external interrupts

	(#) create a data buffer of ENUM_SEVSEG_CHAR and initialize it with ENUM_SEVSEG_CHAR
		characters that you wish to display.

	(#) Call SEVSEG_StoreDataBuf() to store the data buffer in the display struct





  @endvirgilreturns
  ******************************************************************************
  */

#include "seven_segment_driver.h"


const uint16_t INDEX_FROM_ENUM[0x102] = {
    [ENUM_SEVSEG_CHAR_0] = 0,
    [ENUM_SEVSEG_CHAR_1] = 1,
    [ENUM_SEVSEG_CHAR_2] = 2,
    [ENUM_SEVSEG_CHAR_3] = 3,
    [ENUM_SEVSEG_CHAR_4] = 4,
    [ENUM_SEVSEG_CHAR_5] = 5,
    [ENUM_SEVSEG_CHAR_6] = 6,
    [ENUM_SEVSEG_CHAR_7] = 7,
    [ENUM_SEVSEG_CHAR_8] = 8,
    [ENUM_SEVSEG_CHAR_9] = 9,
    [ENUM_SEVSEG_CHAR_A] = 10,
    [ENUM_SEVSEG_CHAR_B] = 11,
    [ENUM_SEVSEG_CHAR_C] = 12,
    [ENUM_SEVSEG_CHAR_D] = 13,
    [ENUM_SEVSEG_CHAR_E] = 14,
    [ENUM_SEVSEG_CHAR_F] = 15,
    [ENUM_SEVSEG_CHAR_h] = 16,
    [ENUM_SEVSEG_CHAR_H] = 17,
    [ENUM_SEVSEG_CHAR_L] = 18,
    [ENUM_SEVSEG_CHAR_n] = 19,
    [ENUM_SEVSEG_CHAR_o] = 20,
    [ENUM_SEVSEG_CHAR_P] = 21,
    [ENUM_SEVSEG_CHAR_r] = 22,
    [ENUM_SEVSEG_CHAR_t] = 23,
    [ENUM_SEVSEG_CHAR_U] = 24,
    [ENUM_SEVSEG_CHAR_Y] = 25,
    [ENUM_SEVSEG_CHAR_Dash] = 26,
    [ENUM_SEVSEG_CHAR_Blank] = 27
};

 enum ENUM_SEVSEG_CHAR SEVSEG_CHAR_ARRAY[] = {
    ENUM_SEVSEG_CHAR_0,
    ENUM_SEVSEG_CHAR_1,
    ENUM_SEVSEG_CHAR_2,
    ENUM_SEVSEG_CHAR_3,
    ENUM_SEVSEG_CHAR_4,
    ENUM_SEVSEG_CHAR_5,
    ENUM_SEVSEG_CHAR_6,
    ENUM_SEVSEG_CHAR_7,
    ENUM_SEVSEG_CHAR_8,
    ENUM_SEVSEG_CHAR_9,
    ENUM_SEVSEG_CHAR_A,
    ENUM_SEVSEG_CHAR_B,
    ENUM_SEVSEG_CHAR_C,
    ENUM_SEVSEG_CHAR_D,
    ENUM_SEVSEG_CHAR_E,
    ENUM_SEVSEG_CHAR_F,
    ENUM_SEVSEG_CHAR_h,
    ENUM_SEVSEG_CHAR_H,
    ENUM_SEVSEG_CHAR_L,
    ENUM_SEVSEG_CHAR_n,
    ENUM_SEVSEG_CHAR_o,
    ENUM_SEVSEG_CHAR_P,
    ENUM_SEVSEG_CHAR_r,
    ENUM_SEVSEG_CHAR_t,
    ENUM_SEVSEG_CHAR_U,
    ENUM_SEVSEG_CHAR_Y,
    ENUM_SEVSEG_CHAR_Dash,
    ENUM_SEVSEG_CHAR_Blank
    };

 const enum ENUM_SEVSEG_CHAR ASCII_to_SEVSEG_CHAR[255] = {
     ['0'] = ENUM_SEVSEG_CHAR_0,
     ['1'] = ENUM_SEVSEG_CHAR_1,
     ['2'] = ENUM_SEVSEG_CHAR_2,
     ['3'] = ENUM_SEVSEG_CHAR_3,
     ['4'] = ENUM_SEVSEG_CHAR_4,
     ['5'] = ENUM_SEVSEG_CHAR_5,
     ['6'] = ENUM_SEVSEG_CHAR_6,
     ['7'] = ENUM_SEVSEG_CHAR_7,
     ['8'] = ENUM_SEVSEG_CHAR_8,
     ['9'] = ENUM_SEVSEG_CHAR_9,
     ['A'] = ENUM_SEVSEG_CHAR_A,
     ['B'] = ENUM_SEVSEG_CHAR_B,
     ['C'] = ENUM_SEVSEG_CHAR_C,
     ['D'] = ENUM_SEVSEG_CHAR_D,
     ['E'] = ENUM_SEVSEG_CHAR_E,
     ['F'] = ENUM_SEVSEG_CHAR_F,
     ['h'] = ENUM_SEVSEG_CHAR_h,
     ['H'] = ENUM_SEVSEG_CHAR_H,
     ['L'] = ENUM_SEVSEG_CHAR_L,
     ['n'] = ENUM_SEVSEG_CHAR_n,
     ['o'] = ENUM_SEVSEG_CHAR_o,
     ['P'] = ENUM_SEVSEG_CHAR_P,
     ['r'] = ENUM_SEVSEG_CHAR_r,
     ['t'] = ENUM_SEVSEG_CHAR_t,
     ['U'] = ENUM_SEVSEG_CHAR_U,
     ['Y'] = ENUM_SEVSEG_CHAR_Y,
     ['-'] = ENUM_SEVSEG_CHAR_Dash,
     [' '] = ENUM_SEVSEG_CHAR_Blank,
 };

//stores look-up indexes (user-defined-pointers) into each DIGIT in sevseg.digits_select[DIGIT]
void SEVSEG_StoreDataBuf(SEVSEG_DISPLAY_TypeDef* sevseg, enum ENUM_SEVSEG_CHAR data[]) {
	for (int i = 0; i < SEVSEG_QTY_DIGITS; i++) {
		sevseg->digit_select[i].current_char_index = INDEX_FROM_ENUM[data[i]];
	}
};

enum ENUM_SEVSEG_CHAR SEVSEG_ReadDigitData(SEVSEG_DISPLAY_TypeDef *sevseg, enum ENUM_SEVSEG_DIGIT target) {
	return SEVSEG_CHAR_ARRAY[sevseg->digit_select[target].current_char_index];
}

HAL_StatusTypeDef SEVSEG_DigitTx(SEVSEG_DISPLAY_TypeDef* sevseg) {
	enum ENUM_SEVSEG_CHAR temp = SEVSEG_CHAR_ARRAY[(sevseg->digit_select[sevseg->refresh_target].current_char_index)];
	// write data through MOSI (SPI2 handler)
    HAL_StatusTypeDef success = HAL_SPI_Transmit(sevseg->spi_handler, &(SEVSEG_CHAR_ARRAY[(sevseg->digit_select[sevseg->refresh_target].current_char_index)]), 1, 1000); //send data through MOSI pin
	if (success != HAL_OK) return success; //if error, return error code

	return success;
};

//FIRST DEMO IMPLREMENTATION: NO 'PLAY' MODE, JUST WRITE 5 CHAR TO 5 DIGIT DISPLAY

// Array of all ENUM_SEVSEG_CHAR values








