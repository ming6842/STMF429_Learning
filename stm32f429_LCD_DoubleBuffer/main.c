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
  

float Buffer[6];

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

  float X_offset =0.0f,Y_offset =0.0f,Z_offset =0.0f;
  float GyX =0.0f, GyY =0.0f, GyZ =0.0f;

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

    /* MEMS Initialization */
    Demo_GyroConfig();

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



      while(1)
      {



        Demo_GyroReadAngRate (Buffer);

        /* MEMS Test area */
        #define LP_ALPHA 0.4f
        GyX = GyX*(1.0f - LP_ALPHA) + (Buffer[0] - X_offset)*LP_ALPHA;
        GyY = GyY*(1.0f - LP_ALPHA) + (Buffer[1] - Y_offset)*LP_ALPHA;
        GyZ = GyZ*(1.0f - LP_ALPHA) + (Buffer[2] - Z_offset)*LP_ALPHA;

        if(GyX > 130.0f) GyX = 130.0f;
        if(GyX < -130.0f) GyX = -130.0f;
        if(GyY > 100.0f) GyY = 100.0f;
        if(GyY < -100.0f) GyY = -100.0f;

        /* for debugging */
          // LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE-1);
          // sprintf(lcd_text_main," GyX :%f         ",(GyX *1.0f));
          // LCD_DisplayStringLine(LINE(1), (uint8_t*)lcd_text_main);

          // sprintf(lcd_text_main," y_runner :%d         ",y_runner++);
          // LCD_DisplayStringLine(LINE(2), (uint8_t*)lcd_text_main);

        PadRectangular(&buffer_screen,x_len,y_len,LCD_COLOR_WHITE, 0,0,x_len,y_len);
        PadRectangular(&buffer_screen,x_len,y_len,LCD_COLOR_BLACK, x_len/2+ (int16_t)(GyY)-10,y_len/2 + (int16_t)(GyX)-10,25,25);
         DrawBufferToScreen(LCD_FOREGROUND_LAYER,buf_ptr,0,0, x_len,y_len);

      }

    }


  while (1){


  }
  
}

