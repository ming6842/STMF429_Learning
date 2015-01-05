/* For direct write test */

#include "lcd_glitchless.h"

#define TRANSPARENCY_COLOR LCD_COLOR_WHITE

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

void DrawPartofBufferToScreen(uint16_t layer,uint16_t *buffer_ptr,uint16_t buffer_x_len,uint16_t buffer_y_len,rectangular_t *rect_to_draw){

	uint16_t i,j;

        for(i=(rect_to_draw->xpos);i< (rect_to_draw->xpos) +( rect_to_draw->xlen);i++){
          for(j=rect_to_draw->ypos;j< (rect_to_draw->ypos) +( rect_to_draw->ylen);j++)
          DrawDirectlyPixel(layer,i,j,buffer_ptr[j + buffer_y_len*i]);
        }


}

void PadRectangular(uint16_t *buffer_ptr,uint16_t buffer_x_len,uint16_t buffer_y_len,uint16_t color,rectangular_t *rect){

      //uint16_t (*array_ptr)[xlen][ylen] = buffer_ptr;

	uint16_t i,j;

        for(i= rect->xpos;i< (rect->xpos)+(rect->xlen);i++){
          for(j=rect->ypos;j< (rect->ypos) + (rect->ylen);j++)

          		//(*array_ptr)[i][j] = color;
				 buffer_ptr[j + buffer_y_len*i] = color;
        }

}


void MoveAndUpdateRectangular(uint16_t layer,uint16_t *buffer_ptr,uint16_t buffer_x_len,uint16_t buffer_y_len,uint16_t color,rectangular_t *old_rect, rectangular_t *new_rect){

	uint16_t i,j;


		/* Clean up old rect */
        PadRectangular(buffer_ptr,buffer_x_len,buffer_y_len,TRANSPARENCY_COLOR, old_rect);

 	 	/* Pad new rect */
        PadRectangular(buffer_ptr,buffer_x_len,buffer_y_len,color, new_rect);

        /* Update ONLY old rect area */
		DrawPartofBufferToScreen(layer,buffer_ptr,buffer_x_len,buffer_y_len,old_rect);
		DrawPartofBufferToScreen(layer,buffer_ptr,buffer_x_len,buffer_y_len,new_rect);

}