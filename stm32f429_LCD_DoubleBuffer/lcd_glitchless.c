/* For direct write test */

#include "lcd_glitchless.h"


void DrawDirectlyPixel(uint16_t xpos,uint16_t ypos,uint32_t color){

       *(__IO uint16_t*) (LCD_FRAME_BUFFER + (2*((xpos) + LCD_PIXEL_WIDTH*(ypos)))) = color; 

}

void DrawBufferToScreen(uint8_t *buffer_ptr,uint16_t xpos,uint16_t ypos,uint16_t xlen,uint16_t ylen){




}



  //   {
  //     uint16_t xpos=0;
  //     uint16_t ypos=0;

  //     uint16_t xlen=100;
  //     uint16_t ylen=100;
  //     uint16_t i=0,j=0;

  //     uint32_t buffer_screen[100][100];
  //     uint32_t *buf_ptr = &buffer_screen;
  // // LCD_SetLayer(LCD_FOREGROUND_LAYER);
  // //     for(i=0;i<100;i++){

  // //       for(j=0;j<100;j++){

  // //         buffer_screen[i][j] = 0xFFFFFF;
  // //       }

  // //     }

  // //     for(i=0;i<100;i++){

  // //       buffer_screen[i][50] = LCD_COLOR_BLACK;

  // //     }


  // //     for(i=xpos;i<xpos + xlen;i++){
  // //       for(j=ypos;j<ypos + ylen;j++)
  // //       DrawDirectlyPixel(i,j,buffer_screen[i][j]);
  // //     }

  //   }