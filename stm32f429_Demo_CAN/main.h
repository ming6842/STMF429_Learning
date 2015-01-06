/**
  ******************************************************************************
  * @file    Touch_Panel/main.h 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"
#include "stm32f429i_discovery_l3gd20.h"
#include "can.h"

/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE(led_name) GPIO_ToggleBits(led_name)
#define LED_ON(led_name) GPIO_ResetBits(led_name)
#define LED_OFF(led_name) GPIO_SetBits(led_name)
#define LED1 GPIOE, GPIO_Pin_8
#define LED2 GPIOE, GPIO_Pin_10
#define LED3 GPIOG, GPIO_Pin_13
#define LED4 GPIOG, GPIO_Pin_14
#define TOGGLE_DEBUG GPIOC, GPIO_Pin_9
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/ 
/* Exported functions ------------------------------------------------------- */
void GPIO_Configuration(void);
void USART1_Configuration(void);
static inline void Delay_1us(uint32_t nCnt_1us);


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
