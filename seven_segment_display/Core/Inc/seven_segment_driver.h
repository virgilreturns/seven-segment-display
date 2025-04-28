#ifndef __SEVEN_SEGMENT_DRIVER_H
#define __SEVEN_SEGMENT_DRIVER_H


#define SEVSEG_QTY_DIGITS 4
#define SEVSEG_DATA_BUF_SIZE 16

#include "stm32f4xx_hal.h"


typedef enum __DEBOUNCE_STATE {
	DEBOUNCE_FALSE = 0,
	DEBOUNCE_TRUE = 1
} DEBOUNCE_STATE;

typedef enum {
	SEVSEG_STATE_SCROLLING = 0, // for scrolling text, need defined data buffer length
	SEVSEG_STATE_STATIC = 1,
} SEVSEG_STATE;

enum ENUM_SEVSEG_CHAR {
	ENUM_SEVSEG_CHAR_0 = 0x40,
	ENUM_SEVSEG_CHAR_1 = 0xF9,
	ENUM_SEVSEG_CHAR_2 = 0xA4,
	ENUM_SEVSEG_CHAR_3 = 0xB0,
	ENUM_SEVSEG_CHAR_4 = 0x99,
	ENUM_SEVSEG_CHAR_5 = 0x92,
	ENUM_SEVSEG_CHAR_6 = 0x82,
	ENUM_SEVSEG_CHAR_7 = 0xF8,
	ENUM_SEVSEG_CHAR_8 = 0x80,
	ENUM_SEVSEG_CHAR_9 = 0x98,
	ENUM_SEVSEG_CHAR_A = 0x88,
	ENUM_SEVSEG_CHAR_B = 0x80,
	ENUM_SEVSEG_CHAR_C = 0xC6,
	ENUM_SEVSEG_CHAR_D = 0xA1,
	ENUM_SEVSEG_CHAR_E = 0x86,
	ENUM_SEVSEG_CHAR_F = 0x8E,
	ENUM_SEVSEG_CHAR_h = 0x89,
	ENUM_SEVSEG_CHAR_H = 0x09,
	ENUM_SEVSEG_CHAR_L = 0xC7,
	ENUM_SEVSEG_CHAR_n = 0xAB,
	ENUM_SEVSEG_CHAR_o = 0x23,
	ENUM_SEVSEG_CHAR_P = 0x8C,
	ENUM_SEVSEG_CHAR_r = 0xAF,
	ENUM_SEVSEG_CHAR_t = 0x87,
	ENUM_SEVSEG_CHAR_U = 0xC1,
	ENUM_SEVSEG_CHAR_Y = 0x91,
	ENUM_SEVSEG_CHAR_Dash = 0xBF,
	ENUM_SEVSEG_CHAR_Blank = 0xFF
};

enum ENUM_SEVSEG_DIGIT { //digit index
	ENUM_SEVSEG_DIGIT_0 = 0,
	ENUM_SEVSEG_DIGIT_1 = 1,
	ENUM_SEVSEG_DIGIT_2 = 2,
	ENUM_SEVSEG_DIGIT_3 = 3,
	ENUM_SEVSEG_DIGIT_4 = 4,
	ENUM_SEVSEG_DIGIT_5 = 5,
	ENUM_SEVSEG_DIGIT_6 = 6,
	ENUM_SEVSEG_DIGIT_7 = 7, // max = 8 total
};


typedef struct {
	GPIO_TypeDef* 	DS_port;
	uint16_t 		DS_pin;
	uint8_t 		current_char_index;
	uint8_t 		state;
} SEVSEG_DIGIT_TypeDef;

typedef struct {
	SPI_HandleTypeDef* 		spi_handler;
	SEVSEG_DIGIT_TypeDef 	digit_select[SEVSEG_QTY_DIGITS]; // for cursor
	enum ENUM_SEVSEG_CHAR 	data_buf[SEVSEG_DATA_BUF_SIZE];
	enum ENUM_SEVSEG_CHAR 	data_window[SEVSEG_QTY_DIGITS];
	uint8_t 				scroll_head;
	enum ENUM_SEVSEG_DIGIT 	refresh_target; // for rendering
	SEVSEG_STATE			state;

} SEVSEG_DISPLAY_TypeDef;

extern enum ENUM_SEVSEG_CHAR SEVSEG_CHAR_ARRAY[];
extern const uint16_t INDEX_FROM_ENUM[];

void SEVSEG_StoreDataWindow(SEVSEG_DISPLAY_TypeDef* seveg, enum ENUM_SEVSEG_CHAR data[]);
enum ENUM_SEVSEG_CHAR SEVSEG_ReadDigitData(SEVSEG_DISPLAY_TypeDef*, enum ENUM_SEVSEG_DIGIT);
HAL_StatusTypeDef SEVSEG_DigitTx(SEVSEG_DISPLAY_TypeDef* sevseg);
HAL_StatusTypeDef SEVSEG_Write(SEVSEG_DISPLAY_TypeDef* sevseg);
void SEVSEG_Cycle();

#endif
