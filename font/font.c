

#include "font.h"


FT_Library	library;
FT_Face 	face;
FT_Glyph	glyph;
FT_BitmapGlyph	  bitmap_glyph ;
FT_Bitmap	bitmap;
FT_UInt 	glyph_index;

int initFreeType(char* font_path, int font_size)
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

	error = FT_Set_Pixel_Sizes(face, 0, font_size);
	//error = FT_Set_Char_Size(face, 16*64, 16*64, 96, 96);
	if (error)
	{
		printf("set font size error!\n");
		return -1;
	}

	return 0;
}

int deInitFreeType()
{
	if (glyph != NULL) {
    	FT_Done_Glyph(glyph);
    	glyph  =  NULL;
	}

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

int setFontSize(int font_size)
{
	int error;

	error = FT_Set_Pixel_Sizes(face, 0, font_size);
	if (error)
	{
		printf("set font size error!\n");
		return -1;
	}
}


int getFontBitmap(void*buffer, int *width, int *height, char wchar)
{
    int error;

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

    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0 , 1);
    bitmap_glyph = (FT_BitmapGlyph)glyph;
    bitmap = bitmap_glyph->bitmap;
 

	memcpy(buffer, bitmap.buffer, bitmap.rows * bitmap.width);
	*width = bitmap.width;
	*height = bitmap.rows;

	printf("vicent------------------bitmap.width %d\n", bitmap.width);
	printf("vicent------------------bitmap.rows  %d\n", bitmap.rows);

    return 0;
}
