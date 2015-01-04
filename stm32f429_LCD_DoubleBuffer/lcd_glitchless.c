/* For direct write test */

#include "lcd_glitchless.h"


void DrawDirectlyPixel(uint16_t layer,uint16_t xpos,uint16_t ypos,uint16_t color){

       *(__IO uint16_t*) (LCD_FRAME_BUFFER + BUFFER_OFFSET*layer +(2*((xpos) + LCD_PIXEL_WIDTH*(ypos)))) = color; 

}

void DrawRectangular(uint16_t layer,uint16_t color,uint16_t xpos,uint16_t ypos,uint16_t xlen, uint16_t ylen){


	uint16_t i,j;

        for(i=0;i< xlen;i++){
          for(j=0;j< ylen;j++)
          DrawDirectlyPixel(layer,i+xpos,j+ypos,color);
        }

}

void DrawBufferToScreen(uint16_t layer,uint16_t *buffer_ptr,uint16_t xpos,uint16_t ypos,uint16_t xlen,uint16_t ylen){

	uint16_t i,j;

        for(i=0;i< xlen;i++){
          for(j=0;j< ylen;j++)
          DrawDirectlyPixel(layer,i+xpos,j+ypos,buffer_ptr[j + ylen*i]);
        }


}

void PadRectangular(uint16_t *buffer_ptr,uint16_t buffer_x_len,uint16_t buffer_y_len,uint16_t color, uint16_t xpos,uint16_t ypos,uint16_t xlen,uint16_t ylen){

      uint16_t (*array_ptr)[xlen][ylen] = buffer_ptr;

	uint16_t i,j;

        for(i=xpos;i< xpos+xlen;i++){
          for(j=ypos;j< ypos + ylen;j++)

          		//(*array_ptr)[i][j] = color;
				 buffer_ptr[j + buffer_y_len*i] = color;
        }

}

