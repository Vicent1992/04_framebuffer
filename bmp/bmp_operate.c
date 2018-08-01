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
#include "rgb565_operate.h"
#include "rgb888_operate.h"
#include "argb8888_operate.h"


int save_rgbbuff_to_bmp(void* rgbbuff, int bpp, int width, int height, int size)
{  
    int img_fd;
	char img_name[48];
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

	sprintf(img_name,"res_img/rgb_%dx%d_bpp%d.bmp", width, height, bpp);
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


#if 0
int main(int argc, char * argv[])
{
	
	int opt;
	void* rgbbuff =NULL;
	int width = 1280, height = 720;
	int size, bpp;
	
	prepare_rgb565(&rgbbuff, width, height, &bpp, &size);
	save_rgbbuff_to_bmp(rgbbuff, bpp, width, height, size);
	release_rgb565(&rgbbuff); 
	
	prepare_rgb888(&rgbbuff, width, height, &bpp, &size);
	save_rgbbuff_to_bmp(rgbbuff, bpp, width, height, size);
	release_rgb888(&rgbbuff); 

	prepare_argb8888(&rgbbuff, width, height, &bpp, &size);
	save_rgbbuff_to_bmp(rgbbuff, bpp, width, height, size);
	release_argb8888(&rgbbuff);
}
#endif
