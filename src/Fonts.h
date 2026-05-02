#ifndef FONTS_H_
#define FONTS_H_

#include "stdint.h"

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

#endif
