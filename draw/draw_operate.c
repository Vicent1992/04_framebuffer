#include "draw_operate.h"


int draw_fbmem_background(drawinfo dp_info)
{
	int i;
	unsigned char *draw_8;
	unsigned short *draw_16;
	ColorVect color_argb;

	if((dp_info.color == 0x0) || (dp_info.color == 0xFFFFFFFF)) {
		memset(dp_info.fb_buff, (unsigned char)dp_info.color, dp_info.fb_size);
		return 0;
	}

	draw_8  = dp_info.fb_buff;
	draw_16 = (unsigned short *)draw_8;

	color_argb = get_rgba_member(dp_info.color);

	switch(dp_info.fb_bpp){
		case 16:
			for (i = 0; i < (dp_info.fb_size >> 1); i++) {
				draw_16[i] = color_to_rgb565(color_argb);
			}
		break;
		case 24:
			for (i = 0; i < dp_info.fb_size; i += 3) {
				draw_8[i+2] = color_argb.color_r;
				draw_8[i+1] = color_argb.color_g;
				draw_8[i+0] = color_argb.color_b;
			}
		break;
		case 32:
			for (i = 0; i < dp_info.fb_size; i += 4) {
				draw_8[i+3] = color_argb.color_a;
				draw_8[i+2] = color_argb.color_r;
				draw_8[i+1] = color_argb.color_g;
				draw_8[i+0] = color_argb.color_b;
			}
		break;
		
	}
}

int draw_pixel(drawinfo dp_info)
{
	unsigned int hor_var;
	unsigned int ver_var;
	unsigned char *draw_8;
	unsigned short *draw_16;
	ColorVect color_argb;

	if (dp_info.x < 0 || dp_info.y < 0) {
		return -1;
	} 
	else if ((dp_info.x >= dp_info.fb_width) || (dp_info.y >= dp_info.fb_height)) 
	{
		dp_info.x %= dp_info.fb_width;
		dp_info.y %= dp_info.fb_height;
	}

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
			draw_8[3] = color_argb.color_a;
			draw_8[2] = color_argb.color_r;
			draw_8[1] = color_argb.color_g;
			draw_8[0] = color_argb.color_b;
		break;
		
	}
}


int draw_font_bitmap(drawinfo dp_info)
{
	int i, j;
	int left, top, right, bottom;
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

	left   = dp_info.x + dp_info.ft_left;
	top    = dp_info.y + (dp_info.ft_size - dp_info.ft_top);
	right  = dp_info.ft_width + left;
	bottom = dp_info.ft_rows + top;

	printf("vicent----- left %d right %d top %d bottom %d\n", left, right, top, bottom);

	for (j = top; j < bottom; j++) 
	{
		for (i = left; i < right; i++) 
		{
			if (dp_info.ft_buff[ft_offset++]) 
			{
				dp_info.x = i;
				dp_info.y = j;
				draw_pixel(dp_info);
			} 
		}
	}
}

