
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_GLITCHLESS_H
#define __LCD_GLITCHLESS_H

#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"

void DrawDirectlyPixel(uint16_t layer,uint16_t xpos,uint16_t ypos,uint32_t color);



#endif