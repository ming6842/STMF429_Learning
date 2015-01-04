
#ifndef FILE_CAN_H
#define FILE_CAN_H
#include "stm32f4xx_conf.h"
#include "stm32f4xx_can.h"


#define CANx CAN2
#define CAN_CLK RCC_APB1Periph_CAN2
#define CAN_RX_PIN GPIO_Pin_5
#define CAN_TX_PIN GPIO_Pin_6
#define CAN_GPIO_PORT GPIOB
#define CAN_GPIO_CLK RCC_AHB1Periph_GPIOB
#define CAN_AF_PORT GPIO_AF_CAN2
#define CAN_RX_SOURCE GPIO_PinSource5
#define CAN_TX_SOURCE GPIO_PinSource6



void CANx_Config(void);
void CANx_Transmit(void);
void CANx_NVIC_Config(void);
void CAN1_RX0_IRQHandler(void);
#endif