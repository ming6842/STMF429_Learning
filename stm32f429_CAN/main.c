/**
  ******************************************************************************
  * @file    Touch_Panel/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example describes how to configure and use the touch panel 
  *          mounted on STM32F429I-DISCO boards.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "drawing.h"
float Buffer[6];

/* Private macro -------------------------------------------------------------*/
#define ABS(x)                     (x < 0) ? (-x) : x
#define L3G_Sensitivity_250dps     (float)114.285f        /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_500dps     (float)57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_2000dps    (float)14.285f         /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */
  

static void Demo_GyroConfig(void);
static void Demo_GyroReadAngRate (float* pfData);


static void Demo_GyroConfig(void)
{
  L3GD20_InitTypeDef L3GD20_InitStructure;
  L3GD20_FilterConfigTypeDef L3GD20_FilterStructure;

  /* Configure Mems L3GD20 */
  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
  L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
  L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
  L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
  L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
  L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
  L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_500; 
  L3GD20_Init(&L3GD20_InitStructure);
  
  L3GD20_FilterStructure.HighPassFilter_Mode_Selection =L3GD20_HPM_NORMAL_MODE_RES;
  L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
  L3GD20_FilterConfig(&L3GD20_FilterStructure) ;
  
  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
}

static void Demo_GyroReadAngRate (float* pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  int i =0;
  
  L3GD20_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  
  L3GD20_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
  
  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & 0x40))
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
    }
  }
  else
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i] << 8) + tmpbuffer[2*i+1]);
    }
  }
  
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & 0x30)
  {
  case 0x00:
    sensitivity=L3G_Sensitivity_250dps;
    break;
    
  case 0x10:
    sensitivity=L3G_Sensitivity_500dps;
    break;
    
  case 0x20:
    sensitivity=L3G_Sensitivity_2000dps;
    break;
  }
  /* divide by sensitivity */
  for(i=0; i<3; i++)
  {
  pfData[i]=(float)RawData[i]/sensitivity;
  }
}


uint32_t L3GD20_TIMEOUT_UserCallback(void)
{
  return 0;
}


static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void GPIO_Configuration(void)
{
    /* GPIOA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration for Push Button ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

      /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // USART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // USART1_RX

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE); //LED3/4 GPIO Port

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;  // LED is connected to PG13/PG14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

}

void USART1_Configuration(void)
{
    /* USART1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 115200 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 921600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}


int main(void)
{
  uint8_t colorR =0 ,colorG =0 ,colorB =0 ;
  uint8_t colorR_dir =0 ,colorG_dir =0 ,colorB_dir =0 ;
  char lcd_text_buff[100];

  float GyX =0.0f, GyY =0.0f, GyZ =0.0f;
  float X_offset =0.0f,Y_offset =0.0f,Z_offset =0.0f;
  uint32_t i=0;


  GPIO_Configuration();
  USART1_Configuration();
  CANx_Config();
  CANx_NVIC_Config();

  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */      

  /* LCD initialization */
  // LCD_Init(); 
  // /* LCD Layer initialization */
  // LCD_LayerInit();
  // /* configure the color Keying */
  // LCD_SetColorKeying(0xFFFFFF);
  // /* Enable the LTDC */
  // LTDC_Cmd(ENABLE);
  
  // /* Clear the LCD */ 
  // LCD_Clear(LCD_COLOR_WHITE);
  // LCD_SetFont(&Font12x12);


  // LCD_SetLayer(LCD_FOREGROUND_LAYER);
  // LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
  // //LCD_DrawFullRect(0,0,240,320);
  
  // Meter(120,160,100,10,-10);


  //LCD_SetColors(ASSEMBLE_RGB(colorR, colorG, colorB),LCD_COLOR_BLACK);
  //LCD_DrawFullRect(0,0,240,320);
  // LCD_SetLayer(LCD_BACKGROUND_LAYER);
  // LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
  // LCD_DrawFullRect(0,0,240,320);

  // LCD_Clear(LCD_COLOR_WHITE);



  // LCD_SetLayer(LCD_BACKGROUND_LAYER);
  // LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
  // DrawThickCircle(120,160,100,5);

  // LCD_SetLayer(LCD_BACKGROUND_LAYER);

  /* MEMS Initialization */
  // Demo_GyroConfig();

  // #define CALIBRATE_COUNT 200
  // for (i=0;i<CALIBRATE_COUNT ;i++){


  //   Demo_GyroReadAngRate (Buffer);
  //   X_offset+= Buffer[0];
  //   Y_offset+= Buffer[1];
  //   Z_offset+= Buffer[2];

  // }
  // X_offset = X_offset/ (float)CALIBRATE_COUNT;
  // Y_offset = Y_offset/ (float)CALIBRATE_COUNT;
  // Z_offset = Z_offset/ (float)CALIBRATE_COUNT;

  
  // CANx_Transmit();
  while (1)
  {
    CANx_Transmit();
    GPIO_ToggleBits(LED4);
    // GPIO_ToggleBits(GPIOD,GPIO_Pin_0);
    // GPIO_ToggleBits(GPIOD,GPIO_Pin_1);

    // LCD_SetLayer(LCD_FOREGROUND_LAYER);
    // //LCD_SetTransparency(122);
    // LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
    // Demo_GyroReadAngRate (Buffer);

    // /* MEMS Test area */
    // #define LP_ALPHA 0.3f
    // GyX = GyX*(1.0f - LP_ALPHA) + (Buffer[0] - X_offset)*LP_ALPHA;
    // GyY = GyY*(1.0f - LP_ALPHA) + (Buffer[1] - Y_offset)*LP_ALPHA;
    // GyZ = GyZ*(1.0f - LP_ALPHA) + (Buffer[2] - Z_offset)*LP_ALPHA;

    // sprintf(lcd_text_buff," GyX :%f         ",(GyX *1.0f));
    // LCD_DisplayStringLine(LINE(1), (uint8_t*)lcd_text_buff);
    // sprintf(lcd_text_buff," GyY :%f          ",(GyY *1.0f));
    // LCD_DisplayStringLine(LINE(2), (uint8_t*)lcd_text_buff);

    // DrawNeedle(120,160,75,10,-10,GyX);
    // Delay_1us(100000);
    // LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
    // DrawNeedle(120,160,75,10,-10,GyX);


   //  LCD_SetLayer(LCD_BACKGROUND_LAYER);

   // if(colorR_dir){

   //        colorR += 1;

   //    if(colorR > 250) colorR_dir=0;
      
   //  }else{

   //    colorR -= 1;

   //    if(colorR<20) colorR_dir=1;
   //  }

   //  if(colorG_dir){

   //        colorG += 2;

   //    if(colorG > 250) colorG_dir=0;
      
   //  }else{

   //    colorG -= 2;

   //    if(colorG<25) colorG_dir=1;
   //  }

   //  if(colorB_dir){

   //        colorB += 3;

   //    if(colorB > 250) colorB_dir=0;
      
   //  }else{

   //    colorB -= 3;

   //    if(colorB<25) colorB_dir=1;
   //  }


   //  LCD_SetColors(ASSEMBLE_RGB(colorR, colorG, colorB),LCD_COLOR_BLACK);
   //  LCD_DrawFullRect(0,0,240,320);


     

  }
  
}

/**
* @brief  Configure the IO Expander and the Touch Panel.
* @param  None
* @retval None
*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
