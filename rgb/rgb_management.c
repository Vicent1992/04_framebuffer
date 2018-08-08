#include <stdio.h>

#include "rgb_management.h"


int prepare_rgbbuff(void **rgbbuff, int width, int height, int bpp, int size)
{

	switch(bpp){
	case 16:
		prepare_rgb565(rgbbuff, width, height, size);
		break;
	case 24:
		prepare_rgb888(rgbbuff, width, height, size);
		break;
	case 32:
		prepare_argb8888(rgbbuff, width, height, size);
		break;
	}

}

int release_rgbbuff(void **rgbbuff)
{
	if(*rgbbuff != NULL) {
		free(*rgbbuff);
		*rgbbuff = NULL;
	}
}

