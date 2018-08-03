#ifndef _FONT_H_
#define _FONT_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include "gb2312_unicode.h"


typedef struct Font_Rect{
	int width;
	int rows;
	int left;
	int top;
} FontRect;

extern int init_freetype(char* font_path, int font_size);
extern int deinit_freetype();
extern int set_font_pixelsize(int font_size);
extern int set_font_charsize(int font_size);
extern int set_font_angle(float angle);
extern int get_font_bitmap(void*buffer, FontRect *font_rect, FT_ULong wchar);

#endif

