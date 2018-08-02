#ifndef __DRAW_PIXEL_H_
#define __DRAW_PIXEL_H_

typedef struct draw_info {
	int x;
	int y;

	int fb_width;
	int fb_height;
	int fb_bpp;
	unsigned char *fb_buff;

	unsigned int color;

	int ft_width;
	int ft_height;
	unsigned char *ft_buff;

} drawinfo;


extern int draw_pixel(drawinfo dp_info);
extern int drawFontBitmap(drawinfo dp_info);

#endif

