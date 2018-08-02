#include <stdio.h>
#include <stdlib.h>

#include "rgb888_operate.h"


void prepare_rgb888(void **rgb888_buffer, int width, int height, int size)
{
	int i = 0, offset = 0, pos = 0;
	unsigned int color = 0;
	unsigned int color_r = 0;
	unsigned int color_g = 0;
	unsigned int color_b = 0;
	unsigned int rgb888_colors[3] = {
		(0xff<<16), //red
		(0xff<<8),  //green
		(0xff)      //blue
	};
	unsigned char* rgbbuff;

	if(*rgb888_buffer != NULL) {
		printf("vicent----prepare_rgb888 buff is not null!\n");
		return;
	}
	printf("vicent----prepare_rgb888!\n");

	*rgb888_buffer = malloc(size);
	rgbbuff = (unsigned char*)*rgb888_buffer;

	offset = 0;
	pos = (size) >> 2;
	color = rgb888_colors[0];
	color_r = (color) & 0xFF;
	color_g = (color >> 8) & 0xFF;
	color_b = (color >> 16) & 0xFF;
	for (i = offset; i < pos; i+=3) {
		rgbbuff[i]   = color_r;
		rgbbuff[i+1] = color_g;
		rgbbuff[i+2] = color_b;
	}

	offset = pos;
	pos = (size) >> 1;
	color = rgb888_colors[1];
	color_r = (color) & 0xFF;
	color_g = (color >> 8) & 0xFF;
	color_b = (color >> 16) & 0xFF;
	for (i = offset; i < pos; i+=3) {
		rgbbuff[i]   = color_r;
		rgbbuff[i+1] = color_g;
		rgbbuff[i+2] = color_b;
	}

	offset = pos;
	pos = (size);
	color = rgb888_colors[2];
	color_r = (color) & 0xFF;
	color_g = (color >> 8) & 0xFF;
	color_b = (color >> 16) & 0xFF;
	for (i = offset; i < pos; i+=3) {
		rgbbuff[i]   = color_r;
		rgbbuff[i+1] = color_g;
		rgbbuff[i+2] = color_b;
	}
}

void release_rgb888(void **rgb888_buffer)
{
	if(*rgb888_buffer != NULL) {
		free(*rgb888_buffer);
		*rgb888_buffer = NULL;
	}
}

