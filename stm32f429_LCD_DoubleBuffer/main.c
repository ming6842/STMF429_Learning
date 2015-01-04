#include "main.h"

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}


int main(void)
{

  char lcd_text_main[100];
  uint32_t runner=0;

    lcd_init();
    lcd_drawBackground(20,60,250);
    lcd_drawBGPersimmon(20, 60, 250);
    LCD_SetColors(LCD_COLOR_WHITE-1,LCD_COLOR_WHITE);

    LCD_SetFont(&Font8x12); 



    {
      uint16_t xpos=0;
      uint16_t ypos=0;

      uint16_t xlen=100;
      uint16_t ylen=100;
      uint16_t i=0,j=0;

      uint32_t buffer_screen[100][100];
      uint32_t *buf_ptr = &buffer_screen;
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

    DrawThickCircle(122,222,22,   5,LCD_COLOR_BLACK,LCD_COLOR_WHITE);

  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LINE(1), (uint8_t*)" LCD text print example ");
  LCD_DisplayStringLine(LINE(2), (uint8_t*)" LCD text print example ");
  LCD_DisplayStringLine(LINE(3), (uint8_t*)" LCD text print example ");

      for(i=0;i<100;i++){

        for(j=0;j<100;j++){

          buffer_screen[i][j] = LCD_COLOR_WHITE;
        }

      }

      for(i=0;i<100;i++){

        buffer_screen[i][50] = LCD_COLOR_BLACK;

      }


      for(i=xpos;i<xpos + xlen;i++){
        for(j=ypos;j<ypos + ylen;j++)
        DrawDirectlyPixel(LCD_FOREGROUND_LAYER,i,j,buffer_screen[i][j]);
      }

    }


  while (1){


  }
  
}

