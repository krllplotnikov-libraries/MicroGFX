#ifndef MICROGFX_H_
#define MICROGFX_H_

#include "stdio.h"
#include "stdint.h"

typedef struct MicroGFX            MicroGFX_st;
typedef struct MicroGFX_Descriptor MicroGFX_Descriptor_st;

typedef void (*MicroGFX_SetPixel)(MicroGFX_st* gfx, uint16_t x, uint16_t y, uint16_t color);

struct MicroGFX_Descriptor{
	MicroGFX_SetPixel setPixel;
	uint16_t height;
	uint16_t width;
};

struct MicroGFX{
	MicroGFX_Descriptor_st descriptor;
};

void MicroGFX_Init(MicroGFX_st* gfx, MicroGFX_Descriptor_st* descriptor);

void MicroGFX_DrawLine(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

#endif
