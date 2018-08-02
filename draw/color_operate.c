#include "color_operate.h"


ColorVect get_rgba_member(unsigned int color)
{
	ColorVect rgba;

	rgba.color_a = (unsigned char)((color>>24) & 0xFF);
	rgba.color_r = (unsigned char)((color>>16) & 0xFF);
	rgba.color_g = (unsigned char)((color>>8)  & 0xFF);
	rgba.color_b = (unsigned char)(color       & 0xFF);

	return rgba;
}

short color_to_rgb565(ColorVect rgba)
{
	short color_565;

	color_565  = (rgba.color_r << 11) & 0xF800;
	color_565 += (rgba.color_g << 5)  & 0x7E0;
	color_565 +=  rgba.color_b 	      & 0x1F;

	return color_565;
}

