
#include "gyro.h"
#include "main.h"

float Buffer[6];

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void LED_Initialization(void){

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE); //LED3/4 GPIO Port

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;  // LED is connected to PG13/PG14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
}




int main(void)
{

  float X_offset =0.0f,Y_offset =0.0f,Z_offset =0.0f;
  float GyX =0.0f, GyY =0.0f, GyZ =0.0f;

      uint16_t x_len=240;
      uint16_t y_len=320;

      uint16_t i=0;
      uint16_t buffer_screen[x_len][y_len];

      uint16_t *buf_ptr = &buffer_screen;


      rectangular_t rect1;
      rectangular_t prev_rect;
      rectangular_t rect_screen;


    char lcd_text_main[100];

    /* LCD Initialization */
    lcd_init();
    lcd_drawBackground(20,60,250);
    lcd_drawBGPersimmon(20, 60, 250);

    LCD_SetLayer(LCD_FOREGROUND_LAYER);

    /* LED Initialization */
    LED_Initialization();

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

        /* MEMS Calibration */
        #define LP_ALPHA 0.1f
        GyX = GyX*(1.0f - LP_ALPHA) + (Buffer[0] - X_offset)*LP_ALPHA;
        GyY = GyY*(1.0f - LP_ALPHA) + (Buffer[1] - Y_offset)*LP_ALPHA;
        GyZ = GyZ*(1.0f - LP_ALPHA) + (Buffer[2] - Z_offset)*LP_ALPHA;

        if(GyX > 130.0f) GyX = 130.0f;
        if(GyX < -130.0f) GyX = -130.0f;
        if(GyY > 100.0f) GyY = 100.0f;
        if(GyY < -100.0f) GyY = -100.0f;


        /* Start drawing rectangular */
        prev_rect = rect1;

        rect1.xlen = 25;
        rect1.ylen = 5;
        rect1.xpos = x_len/2+ (int16_t)(GyY)-10;
        rect1.ypos = y_len/2 + (int16_t)(GyX)-10;

        rect_screen.xlen = x_len;
        rect_screen.ylen = y_len;
        rect_screen.xpos = 0;
        rect_screen.ypos = 0;

        /* Faster method */
        MoveAndUpdateRectangular(LCD_FOREGROUND_LAYER,&buffer_screen,x_len,y_len,LCD_COLOR_BLACK,&prev_rect, &rect1);
        
       
        GPIO_ToggleBits(GPIOG,GPIO_Pin_13);

        /* Regular method */
        // PadRectangular(&buffer_screen,x_len,y_len,LCD_COLOR_WHITE, &prev_rect);
        // PadRectangular(&buffer_screen,x_len,y_len,LCD_COLOR_BLACK, &rect1);
        // DrawBufferToScreen(LCD_FOREGROUND_LAYER,buf_ptr,0,0, x_len,y_len);

      }

    
  
}

