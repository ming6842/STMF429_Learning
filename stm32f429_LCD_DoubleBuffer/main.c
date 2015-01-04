#include "main.h"

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

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





int main(void)
{

      uint16_t xpos=0;
      uint16_t ypos=0;

      uint16_t x_len=240;
      uint16_t y_len=320;
      uint16_t i=0,j=0;

      uint16_t buffer_screen[x_len][y_len];
      //uint32_t buffer_screen2[x_len][y_len];
      uint16_t *buf_ptr = &buffer_screen;
      uint16_t (*array_ptr)[x_len][y_len] = &buffer_screen;

      uint16_t y_runner = 50;

  char lcd_text_main[100];
  uint32_t runner=0;

    /* MEMS Initialization */
    Demo_GyroConfig();

    /* LCD Initialization */
    lcd_init();
    lcd_drawBackground(20,60,250);
    lcd_drawBGPersimmon(20, 60, 250);
    LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_WHITE);

    LCD_SetFont(&Font8x12); 






    {
      
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

    DrawThickCircle(122,222,22,   5,LCD_COLOR_BLACK,LCD_COLOR_WHITE);

  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);


      // for(i=0;i<x_len;i++){

      //   for(j=0;j<y_len;j++){

      //     buffer_screen[i][j] = LCD_COLOR_WHITE;
      //   }

      // }

      // for(i=1;i<x_len-1;i++){

      //   DrawDirectlyPixel(LCD_FOREGROUND_LAYER,i,1,LCD_COLOR_BLACK);
      //   DrawDirectlyPixel(LCD_FOREGROUND_LAYER,i,310,LCD_COLOR_BLACK);

      // }

      // for(j=1;j<y_len-1;j++){

      //   DrawDirectlyPixel(LCD_FOREGROUND_LAYER,1,j,LCD_COLOR_BLACK);
      //   DrawDirectlyPixel(LCD_FOREGROUND_LAYER,10,j,LCD_COLOR_BLACK);

      // }


       // for(i=0;i<x_len;i++){

       //    (*array_ptr)[i][y_len/2-1] = LCD_COLOR_BLACK;
       //    (*array_ptr)[i][y_len/2] = LCD_COLOR_BLACK;
       //    (*array_ptr)[i][y_len/2+1] = LCD_COLOR_BLACK;

       //  }


      // for(i=0;i<x_len;i++){

      //   buffer_screen[i][y_len/2] = LCD_COLOR_BLACK;

      // }
      // for(i=0;i<x_len;i++){

      //   buffer_screen[i][y_len/2 + 1] = LCD_COLOR_BLACK;

      // }

      // for(i=0;i<x_len;i++){

      //   buffer_screen[i][y_len/2 + 2] = LCD_COLOR_BLACK;

      // }


      while(y_runner--)
      {

        // ypos = y_runner;
        // for(i=0;i< x_len;i++){
        //   for(j=0;j< y_len;j++)
        //   DrawDirectlyPixel(LCD_FOREGROUND_LAYER,i+xpos,j+ypos,buf_ptr[j + y_len*i]);
        // }

        PadRectangular(&buffer_screen,x_len,y_len,LCD_COLOR_WHITE, 0,0,x_len,y_len);
        PadRectangular(&buffer_screen,x_len,y_len,LCD_COLOR_BLACK, 0,y_runner,20,20);
         DrawBufferToScreen(LCD_FOREGROUND_LAYER,buf_ptr,0,0, x_len,y_len);

        if(y_runner <2){

          y_runner = 120;
        }

      }

    }


  while (1){


  }
  
}

