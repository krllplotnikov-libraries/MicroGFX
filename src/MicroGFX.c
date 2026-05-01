#include "MicroGFX.h"

void MicroGFX_Init(MicroGFX_st* gfx, MicroGFX_Descriptor_st* descriptor){
	gfx->descriptor.setPixel = descriptor->setPixel;
	gfx->descriptor.height = descriptor->height;
	gfx->descriptor.width = descriptor->width;
}

void MicroGFX_DrawLine(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color){
	if(y0 == y1){
		for(uint16_t i = x0; i < x1; i++){
			gfx->descriptor.setPixel(gfx, i, y0, color);
		}
	}
}
