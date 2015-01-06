/* CAN bus driver */
#include "can.h"
#include "main.h"
void CANx_Config(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* CAN GPIOs configuration **************************************************/
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);
	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
	GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT);
	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);
	/* CAN configuration ********************************************************/
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
	/* CAN register init */
	CAN_DeInit(CANx);
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	/* CAN Baudrate = 1 MBps (CAN clocked at 30 MHz) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 10;
	CAN_Init(CANx, &CAN_InitStructure);
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	/* USE_CAN1 */
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CANx, CAN_IT_FF0, ENABLE);
}

void CANx_Transmit(void){
	CanTxMsg TxMessage;
	/* Transmit Structure preparation */
	TxMessage.StdId = 0x321;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;
	TxMessage.Data[0] = 0xAA;
	TxMessage.Data[1] = 4;
	TxMessage.Data[2] = 4;
	TxMessage.Data[3] = 4;
	TxMessage.Data[4] = 4;
	TxMessage.Data[5] = 4;
	TxMessage.Data[6] = 4;
	TxMessage.Data[7] = 4;
	CAN_Transmit(CANx, &TxMessage);
}

void CANx_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	/* USE_CAN1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

CanRxMsg RxMessage;
char can_buff[100];
void CAN1_RX0_IRQHandler(void)
{
	CAN_Receive(CANx, CAN_FIFO0, &RxMessage);
	//GPIO_ToggleBits(LED3);
	// if ((RxMessage.StdId == 0x123)&&(RxMessage.IDE == CAN_ID_STD) && (RxMessage.DLC == 8))
	// {
	// 	// sprintf(can_buff," RxMessage.Data[0] :%d ",RxMessage.Data[0]);
	// 	// LCD_DisplayStringLine(LINE(1), (uint8_t*)can_buff);
	// 	GPIO_ToggleBits(LED3);
	// }
}

void CAN2_RX0_IRQHandler(void)
{
	CAN_Receive(CANx, CAN_FIFO0, &RxMessage);
	//GPIO_ToggleBits(LED3);
	// if ((RxMessage.StdId == 0x123)&&(RxMessage.IDE == CAN_ID_STD) && (RxMessage.DLC == 8))
	// {
	// 	// sprintf(can_buff," RxMessage.Data[0] :%d ",RxMessage.Data[0]);
	// 	// LCD_DisplayStringLine(LINE(1), (uint8_t*)can_buff);
	// 	GPIO_ToggleBits(LED3);
	// }
}