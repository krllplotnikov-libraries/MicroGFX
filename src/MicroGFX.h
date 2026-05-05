#ifndef MICROGFX_H_
#define MICROGFX_H_

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "math.h"

#include "Fonts.h"

typedef struct MicroGFX            MicroGFX_st;
typedef struct MicroGFX_Descriptor MicroGFX_Descriptor_st;

typedef void (*MicroGFX_SetPixel)(MicroGFX_st* gfx, uint16_t x, uint16_t y, uint16_t color);
typedef void (*MicroGFX_SetPixelArray)(MicroGFX_st* gfx, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

struct MicroGFX_Descriptor{
	MicroGFX_SetPixel setPixel;
	MicroGFX_SetPixelArray setPixelArray;
	uint16_t height;
	uint16_t width;
};

struct MicroGFX{
	MicroGFX_Descriptor_st descriptor;
};

void MicroGFX_Init(MicroGFX_st* gfx, MicroGFX_Descriptor_st* descriptor);

void MicroGFX_DrawLine(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void MicroGFX_DrawRect(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color);
void MicroGFX_DrawCircle(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);
void MicroGFX_DrawRoundRect(MicroGFX_st* gfx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t cornerRadius, uint16_t color);
void MicroGFX_FillScreen(MicroGFX_st* gfx, uint16_t color);

void MicroGFX_DrawChar(MicroGFX_st* gfx, uint16_t x, uint16_t y, char c, uint16_t color, Font_st* font);
void MicroGFX_DrawString(MicroGFX_st* gfx, uint16_t x, uint16_t y, char* str, uint16_t color, Font_st* font);

Font_st* MicroGFX_GetFontByType(MicroGFX_st* gfx, Font_et fontType);

#endif
