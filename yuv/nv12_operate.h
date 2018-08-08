#ifndef _NV12_OPERATE_H_
#define _NV12_OPERATE_H_


extern void prepare_nv12(void **nv12_buffer, int width, int height);
extern void release_nv12(void **nv12_buffer);
extern int get_yuyv_buffer(    char *filename, void* buffer, int size);


#endif


