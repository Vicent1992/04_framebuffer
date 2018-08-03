#ifndef __DRAW_OPERATE_H_
#define __DRAW_OPERATE_H_

#include "color_operate.h"


typedef struct draw_info {
	int x;
	int y;

	int fb_width;
	int fb_height;
	int fb_bpp;
	unsigned char *fb_buff;
	int fb_size;

	unsigned int color;

	int ft_width;
	int ft_rows;
	int ft_left;
	int ft_top;
	int ft_size;
	unsigned char *ft_buff;

} drawinfo;

extern int draw_fbmem_background(drawinfo dp_info);
extern int draw_pixel(drawinfo dp_info);
extern int draw_font_bitmap(drawinfo dp_info);

#endif

