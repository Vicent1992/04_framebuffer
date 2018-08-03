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


typedef struct rkfb_test {
		int fb_bpp;
		int fb_format;
		int fb_width;
		int fb_height;
		int fb_size;

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
    struct win * ui_win;
	void* framebuff = NULL;
	void* rgbbuff =NULL;
	int width, height, size, bpp;
	printf("vicent------------------paint_rgbbuff\n");

	bpp = fb_test.fb_bpp;
	width = fb_test.fb_width;
	height = fb_test.fb_height;
	size = fb_test.fb_size;
	prepare_rgbbuff(&rgbbuff, width, height, bpp, size);

	printf("vicent --- rgbbuff bits %d width %d height %d size %d\n", bpp, width, height, size);
	
	ui_win = rk_fb_getuiwin();
	if (ui_win == NULL)
		printf("vicent----rk_fb_getuiwin error\n");
	framebuff = (void*)ui_win->buffer;
	memcpy(framebuff, rgbbuff, size);
	sync();
	rk_fb_ui_disp(ui_win);

	while(!loop_exit){
		usleep(1000*1000);
		printf("vicent------------------looping\n");
	}

	release_rgbbuff(&rgbbuff);
}

void paint_pixel()
{
    struct win * ui_win;
	unsigned char* framebuff = NULL;

	int x =100, y = 100; 
	unsigned int color = 0xFFFFFFFF;
	drawinfo dp_info;
	printf("vicent------------------paint_char\n");
	
	ui_win = rk_fb_getuiwin();
	if (ui_win == NULL)
		printf("vicent----rk_fb_getuiwin error\n");
	framebuff = ui_win->buffer;

	while(!loop_exit)
	{
		dp_info.x = x+=2;
		dp_info.y = y+=10;
		dp_info.fb_bpp = fb_test.fb_bpp;
		dp_info.fb_width = fb_test.fb_width;
		dp_info.fb_height = fb_test.fb_height;
		dp_info.fb_size   = fb_test.fb_size;
		dp_info.fb_buff = framebuff;
		dp_info.color = color;
		draw_pixel(dp_info);

		sync();
		rk_fb_ui_disp(ui_win);
		usleep(100*1000);
		printf("vicent------------------looping\n");

	}
}

int paint_fbmem_colors()
{
	struct win * ui_win;
	unsigned char* framebuff = NULL;

	int cnt = 0;
	unsigned int colors[5] = {
		PIXEL_COLOR_WHITE, PIXEL_COLOR_RED, PIXEL_COLOR_BLUE, PIXEL_COLOR_GREEN, PIXEL_COLOR_BLACK
	};
	drawinfo dp_info;
	printf("vicent------------------paint_fbmem_colors\n");
	
	ui_win = rk_fb_getuiwin();
	if (ui_win == NULL)
		printf("vicent----rk_fb_getuiwin error\n");
	framebuff = ui_win->buffer;

	while(!loop_exit)
	{
		dp_info.fb_bpp = fb_test.fb_bpp;
		dp_info.fb_width = fb_test.fb_width;
		dp_info.fb_height = fb_test.fb_height;
		dp_info.fb_size   = fb_test.fb_size;
		dp_info.fb_buff = framebuff;
		dp_info.color = colors[cnt];

		draw_fbmem_background(dp_info);
		
		(cnt > 3) ? (cnt = 0) : (cnt++);
		sync();
		rk_fb_ui_disp(ui_win);
		usleep(1000*1000);
		printf("vicent------------------looping\n");
	}
}

void paint_string()
{
	int i;
    struct win * ui_win;
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
	
	font_buffer = malloc(font_size*font_size);
	
	ui_win = rk_fb_getuiwin();
	if (ui_win == NULL)
		printf("vicent----rk_fb_getuiwin error\n");
	framebuff = ui_win->buffer;

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
			get_fontbitmap((void*)font_buffer, &font_rect, word);
					
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
		rk_fb_ui_disp(ui_win);
		usleep(500*1000);
		printf("vicent------------------looping\n");
	}

	free(font_buffer);

	deinit_freetype();
}


int main(int argc, char*argv[])
{
    int opt;

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
			break;
	}
	rk_fb_deinit();

	return 0;
}
