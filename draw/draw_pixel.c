#include "draw_pixel.h"

typedef struct ColorVectors {
	unsigned char color_a;
	unsigned char color_r;
	unsigned char color_g;
	unsigned char color_b;
	
}ColorVect;

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

int draw_pixel(drawinfo dp_info)
{
	unsigned int hor_var;
	unsigned int ver_var;
	unsigned char *draw_8;
	unsigned short *draw_16;
	ColorVect color_argb;

	if ((dp_info.x > dp_info.fb_width) || (dp_info.y > dp_info.fb_height))
		return 0;

	hor_var  = dp_info.fb_bpp >> 3;
	ver_var  = dp_info.fb_width * hor_var;
	draw_8 = dp_info.fb_buff + dp_info.y * ver_var + dp_info.x * hor_var;

	color_argb = get_rgba_member(dp_info.color);

	switch(dp_info.fb_bpp){
		case 16:
			draw_16 = (short*)draw_8;
			draw_16[0] = color_to_rgb565(color_argb);
		break;
		case 24:
			draw_8[0] = color_argb.color_r;
			draw_8[1] = color_argb.color_g;
			draw_8[2] = color_argb.color_b;
		break;
		case 32:
			draw_8[0] = color_argb.color_a;
			draw_8[1] = color_argb.color_r;
			draw_8[2] = color_argb.color_g;
			draw_8[3] = color_argb.color_b;
		break;
		
	}
}


int drawFontBitmap(drawinfo dp_info)
{
	int i, j;
	int fb_offset = 0, ft_offset = 0;
	int  hor_var,  ver_var;

	unsigned char color_a;
	unsigned char color_r;
	unsigned char color_g;
	unsigned char color_b;

	if (!(dp_info.fb_bpp && dp_info.fb_height && dp_info.fb_width)){
		printf("parameter error, please check drawinfo\n");
		return -1;
	}
	hor_var = dp_info.fb_bpp>>3;
	ver_var = dp_info.fb_width*hor_var;
	
	color_b = (dp_info.color >> 0)  & 0xFF;
	color_g = (dp_info.color >> 8)  & 0xFF;
	color_r = (dp_info.color >> 16) & 0xFF;
	color_a = (dp_info.color >> 24) & 0xFF;

	for (i = dp_info.y; i < dp_info.y + dp_info.ft_height; i++) 
	{
		for (j = dp_info.x; j < dp_info.x + dp_info.ft_width; j++) 
		{
			fb_offset = i*ver_var + j*hor_var;
			if (dp_info.ft_buff[ft_offset]) 
			{
				dp_info.fb_buff[fb_offset]   = color_b;
				dp_info.fb_buff[fb_offset+1] = color_g;
				dp_info.fb_buff[fb_offset+2] = color_r;
				dp_info.fb_buff[fb_offset+3] = color_a;
			} 
			ft_offset++;
		}
	}
}

