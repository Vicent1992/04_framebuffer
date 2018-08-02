#ifndef _FONT_H_
#define _FONT_H_


#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>


extern int initFreeType(char* font_path, int font_size);
extern int deInitFreeType();
extern int setFontSize(int font_size);
extern int getFontBitmap(void*buffer, int *width, int *height, char wchar);

#endif

