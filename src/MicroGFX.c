#include "MicroGFX.h"

extern Font_st font6X8;
extern Font_st font11X18;

#define CYRILIC_BYTE              0xD0

#define CYRILIC_OFFSET_ASCII      0x90

#define CYRILIC_OFFSET_FONT_TABLE 96
#define LATIN_OFFSET_FONT_TABLE   32

static uint8_t IsLatinChar(char c);

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

void MicroGFX_DrawChar(MicroGFX_st* gfx, uint16_t x, uint16_t y, char c, uint16_t color, Font_st* font){
	uint16_t tx = x;
	uint16_t ty = y;

	uint8_t fontTableOffset = 0;

	if(IsLatinChar(c)){
		fontTableOffset = LATIN_OFFSET_FONT_TABLE;
	}
	else{
		fontTableOffset = CYRILIC_OFFSET_ASCII - CYRILIC_OFFSET_FONT_TABLE;
	}

	for(uint8_t i = 0; i < font->height; i++){
		for(uint8_t j = 0; j < font->width; j++){
			if((font->data[(c - fontTableOffset) * font->height + i] << j) & 0x8000){
				gfx->descriptor.setPixel(gfx, tx, ty, color);
			}
			tx++;
		}
		ty++;
		tx = x;
	}
}

void MicroGFX_DrawString(MicroGFX_st* gfx, uint16_t x, uint16_t y, char* str, uint16_t color, Font_st* font){
	uint16_t tx = x;

	do{
		if(*str == CYRILIC_BYTE){
			continue;
		}

		MicroGFX_DrawChar(gfx, tx, y, *str, color, font);

		tx += font->width;
	}while(*(++str));
}

Font_st* MicroGFX_GetFontByType(MicroGFX_st* gfx, Font_et fontType){
	return Font_GetFontByType(fontType);
}

static uint8_t IsLatinChar(char c){
	if(c < 0x7F){
		return 1;
	}
	return 0;
}
