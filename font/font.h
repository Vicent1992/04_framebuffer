#ifndef _FONT_H_
#define _FONT_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

typedef struct Font_Rect{
	int width;
	int rows;
	int left;
	int top;
} FontRect;

extern int init_freetype(char* font_path, int font_size);
extern int deinit_freetype();
extern int set_fontsize(int font_size);
extern int get_fontbitmap(void*buffer, FontRect *font_rect, char wchar);

#endif

