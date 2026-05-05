#include "MicroGFX.h"

extern Font_st font6X8;
extern Font_st font11X18;

#define CYRILIC_BYTE              0xD0

#define CYRILIC_OFFSET_ASCII      0x90

#define CYRILIC_OFFSET_FONT_TABLE 96
#define LATIN_OFFSET_FONT_TABLE   32

static uint8_t IsLatinChar(char c);

static void MicroGFX_DrawLine_Slow(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
static void MicroGFX_DrawCircleHelper(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t radius, int8_t quadrantMask, uint16_t color);

void MicroGFX_Init(MicroGFX_st* gfx, MicroGFX_Descriptor_st* descriptor){
	gfx->descriptor.setPixel = descriptor->setPixel;
	gfx->descriptor.setPixelArray = descriptor->setPixelArray;
	gfx->descriptor.height = descriptor->height;
	gfx->descriptor.width = descriptor->width;
}

void MicroGFX_DrawLine(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color){
	if(y0 == y1){
	    if (x0 > x1){
	    	uint16_t tx = x0;
	    	x0 = x1;
	    	x1 = tx;
		}
	    gfx->descriptor.setPixelArray(gfx, x0, y0, x1 - x0 + 1, 1, color);
	}
	else if(x0 == x1){
	    if (y0 > y1){
	    	uint16_t ty = y0;
	    	y0 = y1;
	    	y1 = ty;
		}
	    gfx->descriptor.setPixelArray(gfx, x0, y0, 1, y1 - y0 + 1, color);
	}

	else{
		MicroGFX_DrawLine_Slow(gfx, x0, y0, x1, y1, color);
	}
}

void MicroGFX_DrawRect(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color){
	MicroGFX_DrawLine(gfx, x0, y0, x0 + width - 1, y0, color);
	MicroGFX_DrawLine(gfx, x0 + width - 1, y0, x0 + width - 1, y0 + height - 1, color);
	MicroGFX_DrawLine(gfx, x0 + width - 1, y0 + height - 1, x0, y0 + height - 1, color);
	MicroGFX_DrawLine(gfx, x0, y0 + height - 1, x0, y0, color);
}

void MicroGFX_DrawCircle(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color){
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;

	while (y >= 0){
		gfx->descriptor.setPixel(gfx, x0 + x, y0 + y, color);
		gfx->descriptor.setPixel(gfx, x0 + x, y0 - y, color);
		gfx->descriptor.setPixel(gfx, x0 - x, y0 + y, color);
		gfx->descriptor.setPixel(gfx, x0 - x, y0 - y, color);
		error = 2 * (delta + y) - 1;

		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}

		error = 2 * (delta - x) - 1;

		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}

		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void MicroGFX_DrawRoundRect(MicroGFX_st* gfx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t cornerRadius, uint16_t color){
	int16_t max_radius = ((width < height) ? width : height) / 2;
	if (cornerRadius > max_radius){
		cornerRadius = max_radius;
	}

	MicroGFX_DrawLine(gfx, x + cornerRadius, y, x + cornerRadius + width -1 - 2 * cornerRadius, y, color);
	MicroGFX_DrawLine(gfx, x + cornerRadius, y + height - 1, x + cornerRadius + width - 1 - 2 * cornerRadius, y + height - 1, color);
	MicroGFX_DrawLine(gfx, x, y + cornerRadius, x, y + cornerRadius + height - 1 - 2 * cornerRadius, color);
	MicroGFX_DrawLine(gfx, x + width - 1, y + cornerRadius, x + width - 1, y + cornerRadius + height - 1 - 2 * cornerRadius, color);

	MicroGFX_DrawCircleHelper(gfx, x + cornerRadius, y + cornerRadius, cornerRadius, 1, color);
	MicroGFX_DrawCircleHelper(gfx, x + width - cornerRadius - 1, y + cornerRadius, cornerRadius, 2, color);
	MicroGFX_DrawCircleHelper(gfx, x + width - cornerRadius - 1, y + height - cornerRadius - 1, cornerRadius, 4, color);
	MicroGFX_DrawCircleHelper(gfx, x + cornerRadius, y + height - cornerRadius - 1, cornerRadius, 8, color);
}

void MicroGFX_FillScreen(MicroGFX_st* gfx, uint16_t color){
	gfx->descriptor.setPixelArray(gfx, 0, 0, gfx->descriptor.width, gfx->descriptor.height, color);
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

static void MicroGFX_DrawLine_Slow(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint16_t deltaX = abs(x1 - x0);
	uint16_t deltaY = abs(y1 - y0);
	int16_t signX = x0 < x1 ? 1 : -1;
	int16_t signY = y0 < y1 ? 1 : -1;

	int16_t error = deltaX - deltaY;

	gfx->descriptor.setPixel(gfx, x1, y1, color);

	while (x0 != x1 || y0 != y1) {
		gfx->descriptor.setPixel(gfx, x0, y0, color);
		int16_t error2 = error * 2;

		if (error2 > -deltaY) {
			error -= deltaY;
			x0 += signX;
		}
		if (error2 < deltaX){
			error += deltaX;
			y0 += signY;
		}
	}
}

static void MicroGFX_DrawCircleHelper(MicroGFX_st* gfx, uint16_t x0, uint16_t y0, uint16_t radius, int8_t quadrantMask, uint16_t color){
    int16_t f = 1 - radius ;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    while (x <= y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;

        if (quadrantMask & 0x4) {
            gfx->descriptor.setPixel(gfx, x0 + x, y0 + y, color);
            gfx->descriptor.setPixel(gfx, x0 + y, y0 + x, color);;
        }
        if (quadrantMask & 0x2) {
			gfx->descriptor.setPixel(gfx, x0 + x, y0 - y, color);
            gfx->descriptor.setPixel(gfx, x0 + y, y0 - x, color);
        }
        if (quadrantMask & 0x8) {
			gfx->descriptor.setPixel(gfx, x0 - y, y0 + x, color);
            gfx->descriptor.setPixel(gfx, x0 - x, y0 + y, color);
        }
        if (quadrantMask & 0x1) {
            gfx->descriptor.setPixel(gfx, x0 - y, y0 - x, color);
            gfx->descriptor.setPixel(gfx, x0 - x, y0 - y, color);
        }
    }
}
