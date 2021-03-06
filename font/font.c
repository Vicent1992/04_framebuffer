#define _GNU_SOURCE

#include <math.h>

#include "font.h"


FT_Library	library;
FT_Face 	face;

int init_freetype(char* font_path, int font_size)
{
	
    int error;
	printf("vicent------------------Init_FreeType\n");

	error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("can not init free type library!\n");
		return -1;
	}

	error = FT_New_Face(library, font_path, 0, &face);
	if (error)
	{
		printf("create new face falied!\n");
		return -1;
	}

	error = FT_Set_Pixel_Sizes(face, font_size, font_size);
	//error = FT_Set_Char_Size(face, 16*64, 16*64, 96, 96);
	if (error)
	{
		printf("set font size error!\n");
		return -1;
	}

	return 0;
}

int deinit_freetype()
{
	if (face != NULL) {
    	FT_Done_Face(face);
    	face  =  NULL;
	}

	if (library != NULL) {
    	FT_Done_FreeType(library);
    	library  =  NULL;
	}
	printf("vicent------------------DeInit_FreeType\n");
}

/**********************************************/
/* set_font_charmap            				  */
/* charmap:                        			  */
/* 	FT_ENCODING_UNICODE						  */
/* 	FT_ENCODING_GB2312 and so on			  */
/**********************************************/
int set_font_charmap(int charmap)
{
	int error;

	error = FT_Select_Charmap(face, charmap);
	if (error)
	{
		printf("set font charmap error!\n");
		return -1;
	}
}

int set_font_pixelsize(int font_size)
{
	int error;

	error = FT_Set_Pixel_Sizes(face, font_size, font_size);
	if (error)
	{
		printf("set font size error!\n");
		return -1;
	}
}

int set_font_charsize(int font_size)
{
	int error;

	error = FT_Set_Char_Size(face, font_size << 6, font_size << 6, 96, 96);
	if (error)
	{
		printf("set font size error!\n");
		return -1;
	}
}

/**********************************************/
/* set_font_angle            				  */
/*                           				  */
/* Depend On set_font_charsize				  */
/* and fontsize just be set 24/32 and so on.  */
/**********************************************/
int set_font_angle(float angle)
{
	FT_Matrix matrix;
	FT_Vector delta;

	angle = (angle/180.)* 3.14; 
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L ); 
    matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L ); 
    matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L ); 
    matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

	FT_Set_Transform( face, &matrix, &delta);
}

int get_font_bitmap(void*buffer, FontRect *font_rect, FT_ULong wchar)
{
    int error;

	FT_Glyph	glyph;
	FT_BitmapGlyph	  bitmap_glyph ;
	FT_Bitmap	bitmap;
	FT_UInt 	glyph_index;

 	glyph_index = FT_Get_Char_Index(face, wchar); 

    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error)
    {
        printf("Load char error!\n");
        return -1;
    }

    error  =  FT_Get_Glyph(face->glyph,  &glyph);
    if(error)
    {
        printf("get the flyph error\n");
        return -1;
    }

    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1); 
    bitmap_glyph = (FT_BitmapGlyph)glyph;
    bitmap = bitmap_glyph->bitmap;

	font_rect->width = bitmap.width;
	font_rect->rows = bitmap.rows;
	font_rect->left = bitmap_glyph->left;
	font_rect->top = bitmap_glyph->top;
	memcpy(buffer, bitmap.buffer, bitmap.rows * bitmap.width);

	if (glyph != NULL) {
    	FT_Done_Glyph(glyph);
    	glyph  =  NULL;
	}

    return 0;
}
