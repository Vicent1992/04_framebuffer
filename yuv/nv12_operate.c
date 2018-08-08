#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>

#include "nv12_operate.h"

int get_yuyv_buffer(    char *filename, void* buffer, int size)
{
    int img_fd;
	int readbyte;

    img_fd = open(filename, O_RDONLY);
	if (img_fd < 0) {
		perror("open image");
		close(img_fd);
		return -1;
    }

	readbyte = read(img_fd, buffer, size);
	if (readbyte <= 0) {
		perror("read image buffer error\n");
		close(img_fd);
		return -1;
	}

	close(img_fd);
	return 0;
}



void prepare_nv12(void **nv12_buffer, int width, int height)
{
	static int cnt = 0;
	int y_size = width*height;
	int uv_size = width*height>>1;

	*nv12_buffer = malloc(y_size + uv_size);

    // YUV BLACK : 16 128 128
    memset(*nv12_buffer, 16 + cnt, y_size);
    memset(*nv12_buffer + y_size, 128 - cnt, uv_size);
	cnt+=2;

}

void release_nv12(void **nv12_buffer)
{
	if(*nv12_buffer != NULL) {
		free(*nv12_buffer);
		*nv12_buffer = NULL;
	}
}

