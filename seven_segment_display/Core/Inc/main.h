/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

int app_main();
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define UI_COUNTDOWN_Pin GPIO_PIN_0
#define UI_COUNTDOWN_GPIO_Port GPIOA
#define UI_COUNTDOWN_EXTI_IRQn EXTI0_IRQn
#define UI_COUNTUP_Pin GPIO_PIN_1
#define UI_COUNTUP_GPIO_Port GPIOA
#define UI_COUNTUP_EXTI_IRQn EXTI1_IRQn
#define UI_CURSOR_Pin GPIO_PIN_2
#define UI_CURSOR_GPIO_Port GPIOA
#define UI_CURSOR_EXTI_IRQn EXTI2_IRQn
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SPI_RESET_Pin GPIO_PIN_14
#define SPI_RESET_GPIO_Port GPIOB
#define DIGIT_SEL_3_Pin GPIO_PIN_6
#define DIGIT_SEL_3_GPIO_Port GPIOC
#define DIGIT_SEL_1_Pin GPIO_PIN_8
#define DIGIT_SEL_1_GPIO_Port GPIOC
#define DIGIT_SEL_0_Pin GPIO_PIN_9
#define DIGIT_SEL_0_GPIO_Port GPIOC
#define SPI_LATCH_Pin GPIO_PIN_9
#define SPI_LATCH_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define DIGIT_SEL_2_Pin GPIO_PIN_8
#define DIGIT_SEL_2_GPIO_Port GPIOB
#define DIGIT_SEL_4_Pin GPIO_PIN_9
#define DIGIT_SEL_4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

// SPI MOSI B15
// SPI CLK B10

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
