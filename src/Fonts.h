#ifndef FONTS_H_
#define FONTS_H_

#include "stdint.h"

#define PRINTABLE_CHARACTERS_OFFSET_FONT_TABLE         32
#define PRINTABLE_CHARACTERS_OFFSET_FONT_TABLE_EXT_1   88

#define LAST_PRINTABLE_CHARACTER                       127
#define LAST_PRINTABLE_CHARACTER_EXT_1                 255

typedef enum{
	FONT_6X8,
	FONT_7X9,
	FONT_11X18
}Font_et;

typedef struct{
	uint8_t width;
	uint8_t height;
	const uint16_t *data;
}Font_st;

Font_st* Font_GetFontByType(Font_et fontType);

#endif
