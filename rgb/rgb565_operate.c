#include <stdio.h>
#include <stdlib.h>

#include "rgb565_operate.h"


void prepare_rgb565(void **rgb565_buffer, int width, int height, int *bpp, int *size)
{
	int i = 0, offset = 0, pos = 0;
	unsigned short color = 0;
	unsigned short rgb565_colors[3] = {
		(0x1f<<11), //red
		(0x3f<<5),  //green
		(0x1f)      //blue
	};
	unsigned short* rgbbuff;

	if(*rgb565_buffer != NULL) {
		printf("vicent----prepare_rgb565 buff is not null!\n");
		return;
	}
	printf("vicent----prepare_rgb565!\n");

	*size = width*height*2;
	*bpp  = (2<<3);
	*rgb565_buffer = malloc(*size);
	rgbbuff = *rgb565_buffer;

	offset = 0;
	pos = (width*height) >> 2;
	color = rgb565_colors[0];
	for (i = offset; i < pos; i++)
		rgbbuff[i] = color;

	offset = pos;
	pos = (width*height) >> 1;
	color = rgb565_colors[1];
	for (i = offset; i < pos; i++)
		rgbbuff[i] = color;

	offset = pos;
	pos = (width*height);
	color = rgb565_colors[2];
	for (i = offset; i < pos; i++)
		rgbbuff[i] = color;
}

void release_rgb565(void **rgb565_buffer)
{
	if(*rgb565_buffer != NULL) {
		free(*rgb565_buffer);
		*rgb565_buffer = NULL;
	}
}

