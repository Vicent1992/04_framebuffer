#include <stdio.h>
#include <stdlib.h>

#include "argb8888_operate.h"


void prepare_argb8888(void **argb8888_buffer, int width, int height, int size)
{
	int i = 0, offset = 0, pos = 0;
	static unsigned char alpha = 0xFF;
	unsigned int color = 0;
	unsigned int argb8888_colors[3] = {
		((alpha<<24) | (0xff<<16)) & 0xffff0000,   //red
		((alpha<<24) | (0xff<<8))  & 0xff00ff00,   //green
		((alpha<<24) | (0xff<<0))  & 0xff0000ff    //blue
	};
	unsigned int* rgbbuff;
	alpha--;

	if(*argb8888_buffer != NULL) {
		printf("vicent----prepare_argb8888 buff is not null!\n");
		return;
	}
	printf("vicent----prepare_argb8888!\n");

	*argb8888_buffer = malloc(size);
	memset(*argb8888_buffer, 0, size);
	rgbbuff = *argb8888_buffer;

	offset = 0;
	pos = (width*height) >> 2;
	color = argb8888_colors[0];
	for (i = offset; i < pos; i++)
		rgbbuff[i] = color;

	offset = pos;
	pos = (width*height) >> 1;
	color = argb8888_colors[1];
	for (i = offset; i < pos; i++)
		rgbbuff[i] = color;

	offset = pos;
	pos = (width*height);
	color = argb8888_colors[2];
	for (i = offset; i < pos; i++)
		rgbbuff[i] = color;
}

void release_argb8888(void **argb8888_buffer)
{
	if(*argb8888_buffer != NULL) {
		free(*argb8888_buffer);
		*argb8888_buffer = NULL;
	}
}

