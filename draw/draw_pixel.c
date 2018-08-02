#include "draw_pixel.h"


int drawFontBitmap(drawinfo dp_info)
{
	int i, j;
	int fb_offset = 0, ft_offset = 0;

	unsigned char color_a;
	unsigned char color_r;
	unsigned char color_g;
	unsigned char color_b;

	if (!(dp_info.fb_width && dp_info.fb_height && dp_info.fb_width)){
		printf("parameter error, please check drawinfo\n");
		return -1;
	}
	color_b = (dp_info.color >> 0)  & 0xFF;
	color_g = (dp_info.color >> 8)  & 0xFF;
	color_r = (dp_info.color >> 16) & 0xFF;
	color_a = (dp_info.color >> 24) & 0xFF;

	for (i = dp_info.y; i < dp_info.y+dp_info.ft_height; i++) 
	{
		for (j = dp_info.x; j < dp_info.x+dp_info.ft_width; j++) 
		{
			fb_offset = (i*dp_info.fb_width + j)*dp_info.fb_bitdeep;
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



