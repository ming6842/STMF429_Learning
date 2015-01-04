/* For direct write test */

#include "lcd_glitchless.h"


void DrawDirectlyPixel(uint16_t layer,uint16_t xpos,uint16_t ypos,uint32_t color){

       *(__IO uint16_t*) (LCD_FRAME_BUFFER + BUFFER_OFFSET*layer +(2*((xpos) + LCD_PIXEL_WIDTH*(ypos)))) = color; 

}

void DrawBufferToScreen(uint8_t *buffer_ptr,uint16_t xpos,uint16_t ypos,uint16_t xlen,uint16_t ylen){




}

