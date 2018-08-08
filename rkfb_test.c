#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "rk_fb.h"
#include "rgb_management.h"
#include "font.h"
#include "draw_operate.h"
#include "bmp_operate.h"
#include "nv12_operate.h"


typedef struct rkfb_test {
		int fb_bpp;
		int fb_format;
		int fb_width;
		int fb_height;
		int fb_size;
		void* fb_mem;
		void* win1_fbmem;

        int paint_style;
}rkfbtest;

static rkfbtest fb_test = {
	.paint_style = 0,
	.fb_format = FB_FORMAT_BGRA_8888,
};

int loop_exit = 0;

void sigint_handler(int sig)
{
    loop_exit = 1;
}

void paint_rgbbuff_loop()
{
	void* framebuff = NULL;
	void* rgbbuff =NULL;
	int width, height, size, bpp;
	printf("vicent------------------paint_rgbbuff\n");

	bpp = fb_test.fb_bpp;
	width = fb_test.fb_width;
	height = fb_test.fb_height;
	size = fb_test.fb_size;
	framebuff = fb_test.fb_mem;
	prepare_rgbbuff(&rgbbuff, width, height, bpp, size);

	printf("vicent --- rgbbuff bits %d width %d height %d size %d\n", bpp, width, height, size);
	memcpy(framebuff, rgbbuff, size);

	sync();
	rk_fb_ui_disp_ext();

	while(!loop_exit){
		usleep(1000*1000);
		printf("vicent------------------looping\n");
	}

	release_rgbbuff(&rgbbuff);
}

void paint_yuv()
{
	struct win* win1;
	void* win0_fbmem = NULL;
	void* win1_fbmem = NULL;
	void* nv12buff =NULL;
	void* rgbbuff =NULL;
	int width, height, yuv_size, rgb_size, bpp;
	printf("vicent------------------paint_yuv\n");


	if(fb_test.fb_format == FB_FORMAT_RGB_565) 
	{
	    struct color_key key;
		key.blue = 0x0;
	    key.green = 0x0;
	    key.red = 0x0;
		key.enable = 1;
		if (rk_fb_set_color_key(key) == -1) {
	        printf("rkfb_set_color_key err\n");
	    }
	}
	
	
	win1 = rk_fb_getvideowin();
	fb_test.win1_fbmem = (void*)win1->buffer;

	bpp = fb_test.fb_bpp;
	width = fb_test.fb_width;
	height = fb_test.fb_height;
	yuv_size = width*height*3>>1;
	rgb_size = fb_test.fb_size;
	win1_fbmem = fb_test.win1_fbmem;
	win0_fbmem = fb_test.fb_mem;
	
	prepare_nv12(&nv12buff, width, height);
	get_yuyv_buffer("720x1280.yuv", nv12buff, yuv_size);
	memcpy(win1_fbmem, nv12buff, yuv_size);
	sync();
	rk_fb_video_disp(win1);
	
	printf("vicent --- rgbbuff bits %d width %d height %d\n", bpp, width, height);

	while(!loop_exit){

		prepare_rgbbuff(&rgbbuff, width, height, bpp, rgb_size);
		sync();
		memcpy(win0_fbmem, rgbbuff, rgb_size);
		sync();
		rk_fb_ui_disp_ext();
		release_rgbbuff(&rgbbuff);

		usleep(100*1000);
		printf("vicent------------------paint_yuv looping\n");
	}
	release_nv12(&nv12buff);

}


void paint_pixel()
{
	int x =100, y = 100; 
	unsigned int color = 0xFFFFFFFF;
	drawinfo dp_info;
	printf("vicent------------------paint_char\n");

	while(!loop_exit)
	{
		dp_info.x = x+=2;
		dp_info.y = y+=10;
		dp_info.fb_bpp = fb_test.fb_bpp;
		dp_info.fb_width = fb_test.fb_width;
		dp_info.fb_height = fb_test.fb_height;
		dp_info.fb_size   = fb_test.fb_size;
		dp_info.fb_buff = fb_test.fb_mem;
		dp_info.color = color;
		draw_pixel(dp_info);

		sync();
		rk_fb_ui_disp_ext();
		usleep(100*1000);
		printf("vicent------------------looping\n");

	}
}

int paint_fbmem_colors()
{
	int cnt = 0;
	unsigned int colors[5] = {
		PIXEL_COLOR_WHITE, PIXEL_COLOR_RED, PIXEL_COLOR_BLUE, PIXEL_COLOR_GREEN, PIXEL_COLOR_BLACK
	};
	drawinfo dp_info;
	printf("vicent------------------paint_fbmem_colors\n");

	while(!loop_exit)
	{
		dp_info.fb_bpp = fb_test.fb_bpp;
		dp_info.fb_width = fb_test.fb_width;
		dp_info.fb_height = fb_test.fb_height;
		dp_info.fb_size   = fb_test.fb_size;
		dp_info.fb_buff = fb_test.fb_mem;
		dp_info.color = colors[cnt];

		draw_fbmem_background(dp_info);
		
		(cnt > 3) ? (cnt = 0) : (cnt++);
		sync();
		rk_fb_ui_disp_ext();
		usleep(1000*1000);
		printf("vicent------------------looping\n");
	}
}

void paint_string()
{
	int i;
	unsigned char* framebuff = NULL;

	int x = 100, y = 100, cnt = 0;
	unsigned int colors[4] = {
		PIXEL_COLOR_WHITE, PIXEL_COLOR_RED, PIXEL_COLOR_BLUE, PIXEL_COLOR_GREEN
	};
	unsigned char wchar[40] = "一二三四五六七, vicent";
	unsigned long word;
	int font_size = 64;
	FontRect font_rect;
	unsigned char* font_buffer = NULL;

	drawinfo dp_info;

	printf("vicent------------------paint_string\n");

	init_freetype("/tmp/yhgk.ttf", font_size);

	framebuff = fb_test.fb_mem;
	font_buffer = malloc(font_size*font_size);

	while(!loop_exit)
	{
		x += 10;
		y += 20;
		dp_info.x = x;
		dp_info.y = y;
		dp_info.ft_size = font_size;
		memset(framebuff, 0x0, fb_test.fb_size);

		for (i = 0; wchar[i] != '\0'; i++) 
		{
			if (!isascii(wchar[i])) {
				word = gb2312_conv_unicode(&wchar[i]);
				i++;
			} else {
				word = wchar[i];
			}
			get_font_bitmap((void*)font_buffer, &font_rect, word);
					
			dp_info.fb_bpp    = fb_test.fb_bpp;
			dp_info.fb_width  = fb_test.fb_width;
			dp_info.fb_height = fb_test.fb_height;
			dp_info.fb_size   = fb_test.fb_size;
			dp_info.fb_buff   = framebuff;
	
			dp_info.ft_left  = font_rect.left;
			dp_info.ft_width  = font_rect.width;
			dp_info.ft_rows   = font_rect.rows;
			dp_info.ft_top 	  = font_rect.top;
			dp_info.ft_buff   = font_buffer;
			
			dp_info.color     = colors[cnt];

			draw_font_bitmap(dp_info);

			dp_info.x += font_rect.left + font_rect.width;
			sync();
			(cnt > 2) ? (cnt = 0) : (cnt++);
		}
		rk_fb_ui_disp_ext();
		usleep(500*1000);
		printf("vicent------------------looping\n");
	}

	free(font_buffer);

	deinit_freetype();
}

void paint_bmp()
{
	int ret, offset, h_size;
	int i, j, h_var, v_var;
	int x = 10, y = 100;
	unsigned char* framebuff = NULL;
	
	int bmp_bpp, bmp_width, bmp_height, bmp_size;
	unsigned char* bmp_buff = NULL;
	printf("vicent------------------paint_bmp\n");
	
	framebuff = (unsigned char*)fb_test.fb_mem;

	BITMAPFILEHEADER file_head;
	BITMAPINFOHEADER info_head;

	while(!loop_exit)
	{
		ret = get_bmp_fileinfo("abc.bmp", &file_head, &info_head);
		if(ret == 0) {
			bmp_bpp 	= info_head.biBitCount;
			bmp_width 	= info_head.biWidth;
			bmp_height 	= info_head.biHeight;
			bmp_size 	= bmp_width * abs(bmp_height) * bmp_bpp>>3;

			printf("vicent ---bpp %d width %d height %d size %d\n", 
						 bmp_bpp, bmp_width, bmp_height, bmp_size);
			bmp_buff = malloc(bmp_size);
			memset(bmp_buff, 0x0F, bmp_size);
		}

		ret = get_bmp_buffer("abc.bmp", bmp_buff, bmp_size);
		if(ret == 0) {
			bmp_height = abs(bmp_height);
			h_var = fb_test.fb_bpp >> 3;
			v_var = fb_test.fb_width * h_var;
			h_size = bmp_width * h_var;
			printf("vicent ---h_var %d v_var %d\n", h_var, v_var);
			
			offset = 0;
			for (j = y; j < y+bmp_height; j++) 
			{
				memcpy(&framebuff[j*v_var + x*h_var], &bmp_buff[offset], h_size);
				offset+= h_size;
			}
			sync();
			free(bmp_buff);
		}

		rk_fb_ui_disp_ext();
		usleep(1000*1000);

		printf("vicent------------------looping\n");
	}
}


int main(int argc, char*argv[])
{
    int opt;
	struct win * ui_win;

	while (1) 
	{
	    static struct option opts[] = {
	        {"fb format", 	    required_argument, 	0, 'f'},
	        {"paint style", 	required_argument, 	0, 'p'},
	    };

	    int i = 0;
	    opt = getopt_long(argc, argv, "f:p:", opts, &i);
	    if (opt == -1)
	        break;

	    switch (opt) {
	    case 'p':
	        fb_test.paint_style = atoi(optarg);
			break;
	    case 'f':
			fb_test.fb_format = atoi(optarg);
			(fb_test.fb_format == 1) ?  (fb_test.fb_format = FB_FORMAT_RGB_565) : 
										(fb_test.fb_format = FB_FORMAT_BGRA_8888);
			break;
		}
	}

	printf("vicent----rkfb test\n");
	printf("        paint_style  %d\n",fb_test.paint_style); 
	if (fb_test.fb_format == FB_FORMAT_RGB_565) {
		fb_test.fb_bpp = 16;
		printf("        fb_format  FB_FORMAT_RGB_565\n");
	} else if (fb_test.fb_format == FB_FORMAT_BGRA_8888) {
		fb_test.fb_bpp = 32;
		printf("        fb_format  FB_FORMAT_BGRA_8888\n");
	}

	signal(SIGINT, sigint_handler);
	
    rk_fb_init(fb_test.fb_format);
	rk_fb_get_out_device(&fb_test.fb_width, &fb_test.fb_height);
	fb_test.fb_size = fb_test.fb_width * fb_test.fb_height;
	fb_test.fb_size *= fb_test.fb_bpp>>3;
	printf("        fb_bpp     %d\n", fb_test.fb_bpp);
	printf("        fb_width   %d\n", fb_test.fb_width);
	printf("        fb_height  %d\n", fb_test.fb_height);
	printf("        fb_size    %x\n", fb_test.fb_size);

	ui_win = rk_fb_getuiwin();
	if (ui_win == NULL)
		printf("vicent----rk_fb_getuiwin error\n");
	fb_test.fb_mem = (void*)ui_win->buffer;
	printf("        fb_mem     %8x\n\n", fb_test.fb_mem);
	

	 switch (fb_test.paint_style) {
	    case 0:
			paint_rgbbuff_loop();
			break;
	    case 1:
			paint_pixel();
			break;
	    case 2:
			paint_fbmem_colors();
			break;
	    case 3:
			paint_string();
			break;
	    case 4:
			paint_bmp();
			break;
	    case 5:
			paint_yuv();
			break;
	}
	rk_fb_deinit();

	return 0;
}
