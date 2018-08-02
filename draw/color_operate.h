#ifndef __COLOR_OPERATE_H_
#define __COLOR_OPERATE_H_

#define PIXEL_COLOR_WHITE 0xFFFFFFFF
#define PIXEL_COLOR_BLACK 0x00000000
#define PIXEL_COLOR_RED   0xFFFF0000
#define PIXEL_COLOR_GREEN 0xFF00FF00
#define PIXEL_COLOR_BLUE  0xFF0000FF

typedef struct ColorVectors {
	unsigned char color_a;
	unsigned char color_r;
	unsigned char color_g;
	unsigned char color_b;
	
}ColorVect;

extern ColorVect get_rgba_member(unsigned int color);
extern short color_to_rgb565(ColorVect rgba);



#endif

