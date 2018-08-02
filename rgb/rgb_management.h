#ifndef __RGB_MANAGEMENT_H_
#define __RGB_MANAGEMENT_H_


#include "rgb565_operate.h"
#include "rgb888_operate.h"
#include "argb8888_operate.h"


extern int prepare_rgbbuff(void **rgbbuff, int width, int height, int bpp, int size);
extern int release_rgbbuff(void **rgbbuff);


#endif

