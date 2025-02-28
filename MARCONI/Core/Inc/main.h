/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define GNSS_TX_Pin GPIO_PIN_2
#define GNSS_TX_GPIO_Port GPIOA
#define GNSS_RX_Pin GPIO_PIN_3
#define GNSS_RX_GPIO_Port GPIOA
#define MAIN_COM_RX_Pin GPIO_PIN_0
#define MAIN_COM_RX_GPIO_Port GPIOB
#define MAIN_COM_CLK_Pin GPIO_PIN_1
#define MAIN_COM_CLK_GPIO_Port GPIOB
#define MAIN_COM_TX_Pin GPIO_PIN_2
#define MAIN_COM_TX_GPIO_Port GPIOB
#define LoRa_AUX_Pin GPIO_PIN_5
#define LoRa_AUX_GPIO_Port GPIOB
#define LoRa_TX_Pin GPIO_PIN_6
#define LoRa_TX_GPIO_Port GPIOB
#define LoRa_RX_Pin GPIO_PIN_7
#define LoRa_RX_GPIO_Port GPIOB
#define LoRa_M1_Pin GPIO_PIN_8
#define LoRa_M1_GPIO_Port GPIOB
#define LoRa_M0_Pin GPIO_PIN_9
#define LoRa_M0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
