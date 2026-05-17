/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32wbxx_hal.h"

#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

#include "stm32wbxx_ll_dma.h"
#include "stm32wbxx_ll_tim.h"
#include "stm32wbxx_ll_bus.h"
#include "stm32wbxx_ll_cortex.h"
#include "stm32wbxx_ll_rcc.h"
#include "stm32wbxx_ll_system.h"
#include "stm32wbxx_ll_utils.h"
#include "stm32wbxx_ll_pwr.h"
#include "stm32wbxx_ll_gpio.h"

#include "stm32wbxx_ll_exti.h"

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
#define RPM /* delete if you do not use an ignition probe */
#define ADC_range 4095.0
#define MPU_SCL_Pin GPIO_PIN_8
#define MPU_SCL_GPIO_Port GPIOB
#define MPU_SDA_Pin GPIO_PIN_9
#define MPU_SDA_GPIO_Port GPIOB
#define RPM_timer_Pin GPIO_PIN_0
#define RPM_timer_GPIO_Port GPIOA
#define MAP_Pin GPIO_PIN_1
#define MAP_GPIO_Port GPIOA
#define NTC_Pin GPIO_PIN_2
#define NTC_GPIO_Port GPIOA
#define MAP_VCC_Pin GPIO_PIN_3
#define MAP_VCC_GPIO_Port GPIOA
#define TC_VCC_Pin GPIO_PIN_4
#define TC_VCC_GPIO_Port GPIOA
#define LS_PWM_Pin GPIO_PIN_6
#define LS_PWM_GPIO_Port GPIOA
#define LS_UBat_sense_Pin GPIO_PIN_7
#define LS_UBat_sense_GPIO_Port GPIOA
#define CJ125_UA_Pin GPIO_PIN_8
#define CJ125_UA_GPIO_Port GPIOA
#define CJ125_UR_Pin GPIO_PIN_9
#define CJ125_UR_GPIO_Port GPIOA
#define CJ125_RST_Pin GPIO_PIN_2
#define CJ125_RST_GPIO_Port GPIOB
#define LS_LED1_Pin GPIO_PIN_0
#define LS_LED1_GPIO_Port GPIOB
#define LS_LED2_Pin GPIO_PIN_1
#define LS_LED2_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOE
#define TC_NSS_Pin GPIO_PIN_10
#define TC_NSS_GPIO_Port GPIOA
#define CJ125_NSS_Pin GPIO_PIN_15
#define CJ125_NSS_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
