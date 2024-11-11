#include <stdint.h>

#ifndef __SSD1306_FONTS_H__
#define __SSD1306_FONTS_H__
// Include only needed fonts

#define SSD1306_INCLUDE_FONT_12x8
#define SSD1306_INCLUDE_FONT_12x12
// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
typedef struct
{
	const uint8_t FontWidth;   // font width in pixels, width of word has double width
	const uint8_t FontHeight;  // font height in pixels
	const uint8_t BytePreChar; // byte of per char
	const uint8_t CharStart;   // ascii char start from
	const uint8_t *AscIIData;  // pointer to font data array
	const uint8_t DWCharStart; // pointer to font data array
	const uint8_t *DWCharData; // pointer to font data array
} ZFontDef;

#ifdef SSD1306_INCLUDE_FONT_12x8
extern ZFontDef * Font_12x8;
#endif


#ifdef SSD1306_INCLUDE_FONT_12x12
extern ZFontDef * Font_12x12;
#endif


#define ni (uint8_t)0xf0
#define hao (uint8_t)0xf1
#define shi (uint8_t)0xf2
#define jie (uint8_t)0xf3


#endif
