#ifndef _FONT_H_
#define _FONT_H_


#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>


extern int Init_FreeType(char* font_path, int font_size);
extern int DeInit_FreeType();
extern int Get_FreeType_Bitmap(void*buffer, int *width, int *height, char wchar);

#endif

