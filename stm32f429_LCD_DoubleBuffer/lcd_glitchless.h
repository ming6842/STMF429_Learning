
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_GLITCHLESS_H
#define __LCD_GLITCHLESS_H

#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"

void DrawDirectlyPixel(uint16_t layer,uint16_t xpos,uint16_t ypos,uint16_t color);
void DrawBufferToScreen(uint16_t layer,uint16_t *buffer_ptr,uint16_t xpos,uint16_t ypos,uint16_t xlen,uint16_t ylen);
void DrawRectangular(uint16_t layer,uint16_t color,uint16_t xpos,uint16_t ypos,uint16_t xlen, uint16_t ylen);
void PadRectangular(uint16_t *buffer_ptr,uint16_t buffer_x_len,uint16_t buffer_y_len,uint16_t color, uint16_t xpos,uint16_t ypos,uint16_t xlen,uint16_t ylen);




#endif