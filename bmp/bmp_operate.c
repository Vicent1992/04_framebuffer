#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>

#include "bmp_operate.h"

int save_rgbbuff_to_bmp(void* rgbbuff, int bpp, int width, int height)
{  
    int img_fd;
	char img_name[48];
	int size = width * abs(height) * bpp >> 3;
    BITMAPFILEHEADER file_head;
    BITMAPINFOHEADER info_head;
    //RGBQUAD rgb_quad;
	
	printf("vicent --- save bits %d width %d height %d size %d\n", bpp, width, height, size);
    
	/*initialize bmp structs*/
	file_head.bfType = 0x4d42;
	file_head.bfSize = sizeof(file_head) + sizeof(info_head) + size;
	file_head.bfReserved1 = 0;
	file_head.bfReserved2 = 0;
	file_head.bfOffBits = sizeof(file_head) + sizeof(info_head);

	info_head.biSize = sizeof(info_head);
	info_head.biWidth = width;
	info_head.biHeight = height;
	info_head.biPlanes = 1;
	info_head.biBitCount = bpp;
	info_head.biCompression = 0;
	info_head.biSizeImage = 0;
	info_head.biXPelsPerMeter = 0;
	info_head.biYPelsPerMeter = 0;
	info_head.biClrUsed = 0;
	info_head.biClrImportant = 0;

	sprintf(img_name,"rgb_%dx%d_bpp%d.bmp", width, height, bpp);
    img_fd = open(img_name, O_RDWR | O_CREAT, 0644);
    if (img_fd < 0) {
		perror("open image");
		close(img_fd);
		return -1;
    }
	write(img_fd, &file_head, sizeof(file_head));
	write(img_fd, &info_head, sizeof(info_head));
	write(img_fd, rgbbuff, size);
	fsync(img_fd);
	close(img_fd);

	return 0;
}


int get_bmp_fileinfo(    char *filename, BITMAPFILEHEADER *file_head, 
							 BITMAPINFOHEADER *info_head)
{
    int img_fd;
	int readbyte;
	char heads[128];

    img_fd = open(filename, O_RDONLY);
	if (img_fd < 0) {
		perror("open image");
		close(img_fd);
		return -1;
    }

	readbyte = read(img_fd, file_head, sizeof(BITMAPFILEHEADER));
	if (readbyte < 0) {
		perror("read image error!");
		close(img_fd);
		return -1;
	}
		
	readbyte = read(img_fd, info_head, sizeof(BITMAPINFOHEADER));
	if (readbyte < 0) {
		perror("read image error!");
		close(img_fd);
		return -1;
	}
	
	close(img_fd);
	
	return 0;
}

int get_bmp_buffer(    char *filename, void* buffer, int size)
{
    int img_fd;
	int readbyte;
	long offset;

    img_fd = open(filename, O_RDONLY);
	if (img_fd < 0) {
		perror("open image");
		close(img_fd);
		return -1;
    }

	offset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	lseek(img_fd, offset, SEEK_CUR);

	readbyte = read(img_fd, buffer, size);
	if (readbyte <= 0) {
		perror("read image buffer error\n");
		close(img_fd);
		return -1;
	}

	close(img_fd);
	return 0;
}



#if 0
int main(int argc, char * argv[])
{
	
	int opt;
	void* bmpbuff =NULL;
	unsigned int width = 1280, height = 720;
	unsigned int size, bpp;
#if 1
	BITMAPFILEHEADER file_head;
	BITMAPINFOHEADER info_head;

	get_bmp_fileinfo("abc.bmp", &file_head, &info_head);
	printf("vicent ---------file-bfType %x\n", file_head.bfType);
	printf("vicent ---------file-bfSize %x\n", file_head.bfSize);
	printf("vicent ---------file-bfOffBits %d\n", file_head.bfOffBits);
	printf("vicent ---------info-biHeight %d\n", info_head.biHeight);
	printf("vicent ---------info-biWidth %d\n", info_head.biWidth);
	printf("vicent ---------info-biBitCount %d\n", info_head.biBitCount);
	printf("vicent ---------info-biSizeImage %d\n", info_head.biSizeImage);

	bpp 	= info_head.biBitCount;
	width 	= info_head.biWidth;
	height 	= info_head.biHeight;
	size 	= width * abs(height) * bpp>>3;

	printf("vicent ---bpp %d width %d height %d size %d\n", 
										bpp, width, height, size);

	bmpbuff = malloc(size);
	
	get_bmp_buffer("abc.bmp", bmpbuff, size);

	save_rgbbuff_to_bmp(bmpbuff, bpp, width, height);

	free(bmpbuff);

#else
	prepare_rgb565(&rgbbuff, width, height, &bpp, &size);
	save_rgbbuff_to_bmp(rgbbuff, bpp, width, height, size);
	release_rgb565(&rgbbuff); 
	
	prepare_rgb888(&rgbbuff, width, height, &bpp, &size);
	save_rgbbuff_to_bmp(rgbbuff, bpp, width, height, size);
	release_rgb888(&rgbbuff); 

	prepare_argb8888(&rgbbuff, width, height, &bpp, &size);
	save_rgbbuff_to_bmp(rgbbuff, bpp, width, height, size);
	release_argb8888(&rgbbuff);
#endif
}
#endif
