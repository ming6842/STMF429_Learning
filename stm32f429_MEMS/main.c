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

void DrawThickCircle(uint32_t x,uint32_t y,uint32_t radius, uint32_t thickness){


    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DrawFullCircle(x, y, radius);
    LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_WHITE);
    LCD_DrawFullCircle(x, y, radius-thickness);


}

  Point needlePoints[4];
void DrawNeedle(int32_t x_center,int32_t y_center,float direction,int32_t radius, int32_t base_width){

  direction += 180.0f;

  needlePoints[0].X = x_center + (int32_t)((float) base_width * cosf(direction * 0.0174444444444f));
  needlePoints[0].Y = y_center + (int32_t)((float) base_width * sinf(direction * 0.0174444444444f));

  needlePoints[1].X = x_center - (int32_t)((float) radius * sinf(direction * 0.0174444444444f));
  needlePoints[1].Y = y_center + (int32_t)((float) radius * cosf(direction * 0.0174444444444f));

  needlePoints[2].X = x_center - (int32_t)((float) base_width * cosf(direction * 0.0174444444444f));
  needlePoints[2].Y = y_center - (int32_t)((float) base_width * sinf(direction * 0.0174444444444f));


    // LCD_ClosedPolyLine(needlePoints,3); //this one work

LCD_FillTriangle(needlePoints[0].X,needlePoints[1].X,needlePoints[2].X
              ,  needlePoints[0].Y,needlePoints[1].Y,needlePoints[2].Y);
    //LCD_FillPolyLine(needlePoints, 3); // this one bugged

}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // USART1_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // USART1_RX
}



void USART1_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 57600 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 57600;
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

  char lcd_text_buff[100];
  float GyX =0.0f, GyY =0.0f, GyZ =0.0f;
  float AngY = 0.0f;
  float GyX_prev =0.0f, GyY_prev =0.0f, GyZ_prev =0.0f;
  float X_offset =0.0f,Y_offset =0.0f,Z_offset =0.0f;
  uint32_t i=0;
  uint8_t chksum = 0;

   uint8_t  toPushPosition=0;

  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f429_439xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */      

  /* LCD initialization */
  LCD_Init();
  
  /* LCD Layer initialization */
  LCD_LayerInit();
    
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_SetColorKeying(0xFFFFFF);

  /* Need to reload */
  LTDC_ReloadConfig(LTDC_IMReload);

  /* Enable the LTDC */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer */
  
  /* MEMS Initialization */
  Demo_GyroConfig();

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_Configuration();
  USART1_Configuration();

  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);
  LCD_SetFont(&Font16x24);

  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);

  LCD_DisplayStringLine(LINE(1), (uint8_t*)" LCD text print example ");
  LCD_DisplayStringLine(LINE(2), (uint8_t*)" Ming6842 @ github");


  //LCD_SetColors(ASSEMBLE_RGB(colorR, colorG, colorB),LCD_COLOR_BLACK);
  //LCD_DrawFullRect(0,0,240,320);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
  LCD_DrawFullRect(0,0,240,320);

  LCD_Clear(LCD_COLOR_WHITE);

  LCD_SetLayer(LCD_BACKGROUND_LAYER);

  #define CALIBRATE_COUNT 1000
  for (i=0;i<CALIBRATE_COUNT ;i++){


    Demo_GyroReadAngRate (Buffer);
    X_offset+= Buffer[0];
    Y_offset+= Buffer[1];
    Z_offset+= Buffer[2];

  }
  X_offset = X_offset/ (float)CALIBRATE_COUNT;
  Y_offset = Y_offset/ (float)CALIBRATE_COUNT;
  Z_offset = Z_offset/ (float)CALIBRATE_COUNT;


    LCD_SetColors(ASSEMBLE_RGB(22, 150, 255),LCD_COLOR_BLACK);
    LCD_DrawFullRect(0,0,240,320);


#define X_MIDDLE 120
#define Y_MIDDLE 190




    DrawThickCircle(X_MIDDLE,Y_MIDDLE,100,7);


  while (1)
  {


  LCD_SetLayer(LCD_FOREGROUND_LAYER);
    Demo_GyroReadAngRate (Buffer);

    /* MEMS Test area */
    #define LP_ALPHA 0.3f
    GyX = GyX*(1.0f - LP_ALPHA) + (Buffer[0] - X_offset)*LP_ALPHA;
    GyY = GyY*(1.0f - LP_ALPHA) + (Buffer[1] - Y_offset)*LP_ALPHA;
    GyZ = GyZ*(1.0f - LP_ALPHA) + (Buffer[2] - Z_offset)*LP_ALPHA;

    AngY += (Buffer[1] - Y_offset)*0.045;


    if(AngY >= 180.0f){

      AngY = 180.0f;
    }else if(AngY <= -180.0f){

      AngY = -180.0f;
    }

    toPushPosition = (uint8_t)((AngY/180.0f * 126.0f) +126.0f);

    chksum =0;
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, 0x91);
    chksum += 0x91;

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, 0x71);
    chksum += 0x71;

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, 0x01);
    chksum += 0x01;

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, toPushPosition);
    chksum += toPushPosition;

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1,chksum );



    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE-1);
    sprintf(lcd_text_buff," GyX :%f         ",(GyX *1.0f));
    LCD_DisplayStringLine(LINE(1), (uint8_t*)lcd_text_buff);
    sprintf(lcd_text_buff," GyY :%f          ",(GyY *1.0f));
    LCD_DisplayStringLine(LINE(2), (uint8_t*)lcd_text_buff);

    sprintf(lcd_text_buff," AngY :%f          ",(AngY *1.0f));
    LCD_DisplayStringLine(LINE(3), (uint8_t*)lcd_text_buff);





  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);

  /* drew quite ok but glitch */
   DrawNeedle(X_MIDDLE,Y_MIDDLE,GyY_prev,100,16);


    LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE-1);
   DrawNeedle(X_MIDDLE,Y_MIDDLE,GyY,80, 10);

//   {
//     int pix_test = 0;

//     while(pix_test < 150){

//       *(__IO uint16_t*) (LCD_FRAME_BUFFER + (2*((120) + LCD_PIXEL_WIDTH*(pix_test)))) = LCD_COLOR_WHITE; 

//     pix_test++;
//     }



//     pix_test = 0;
//     while(pix_test < 150){

// *(__IO uint16_t*) (LCD_FRAME_BUFFER + (2*((120) + LCD_PIXEL_WIDTH*(pix_test)))) = LCD_COLOR_BLACK; 

//     pix_test++;
//     }

//   }



     GyX_prev = GyX;
   GyY_prev = GyY;
   GyZ_prev = GyZ;


    // LCD_SetTextColor(LCD_COLOR_WHITE);
    // LCD_SetLayer(LCD_FOREGROUND_LAYER);
    // //LCD_DrawFullCircle(120, 240, 40);
    // Delay_1us(10000);

    // LCD_SetColors(ASSEMBLE_RGB(colorR, colorG, colorB),LCD_COLOR_BLACK);
    // //sLCD_DrawFullCircle(120, 240, 30);


    Delay_1us(1000);

     // LCD_Clear(ASSEMBLE_RGB(colorR, colorG, colorB));


      //sprintf(lcd_text_buff," LOLOLOL %ld",test_int32);
      //LCD_DisplayStringLine(LINE(7), (uint8_t*)lcd_text_buff);

      //LCD_DisplayStringLine(LINE(8), (uint8_t*)lcd_text_buff);

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
