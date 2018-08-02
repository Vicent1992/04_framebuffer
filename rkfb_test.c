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
#include "draw_pixel.h"


typedef struct rkfb_test {
		int fb_bpp;
		int fb_format;
		int fb_wight;
		int fb_height;
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
	width = fb_test.fb_wight;
	height = fb_test.fb_height;
	size = width*height*(bpp>>3);
	prepare_rgbbuff(&rgbbuff, width, height, bpp, size);

	printf("vicent --- rgbbuff bits %d width %d height %d size %d\n", bpp, width, height, size);

	while(!loop_exit){

		ui_win = rk_fb_getuiwin();
		if (ui_win == NULL)
			printf("vicent----rk_fb_getuiwin error\n");
		framebuff = (void*)ui_win->buffer;

		memcpy(framebuff, rgbbuff, size);
		sync();
		rk_fb_ui_disp(ui_win);
		usleep(1000*1000);

	}

	release_rgbbuff(&rgbbuff);
}

void paint_char()
{
	int i,j;

    struct win * ui_win;
	unsigned char* framebuff = NULL;

	int x = 100, y = 100;
	unsigned char wchar = 40;
	int font_size = 64;
	int font_width, font_height;
	unsigned char* font_buffer = NULL;

	drawinfo dp_info;

	printf("vicent------------------paint_char\n");


	initFreeType("/tmp/yhgk.ttf", font_size);
	
	font_buffer = malloc(font_size*font_size);

	while(!loop_exit){

		ui_win = rk_fb_getuiwin();
		if (ui_win == NULL)
			printf("vicent----rk_fb_getuiwin error\n");
		framebuff = ui_win->buffer;

		memset(framebuff, 0x88, ui_win->video_ion.size);

		getFontBitmap((void*)font_buffer, &font_width, &font_height, wchar++);

		dp_info.x = x;
		dp_info.y = y;
		dp_info.fb_bpp = fb_test.fb_bpp;
		dp_info.fb_width = fb_test.fb_wight;
		dp_info.fb_height = fb_test.fb_height;
		dp_info.ft_width = font_width;
		dp_info.ft_height = font_height;
		dp_info.fb_buff = framebuff;
		dp_info.ft_buff = font_buffer;
		dp_info.color = 0x200000FF;
		drawFontBitmap(dp_info);

		sync();
		rk_fb_ui_disp(ui_win);
		usleep(1000*1000);
		printf("vicent------------------looping\n");
	}

	free(font_buffer);

	deInitFreeType();
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
	rk_fb_get_out_device(&fb_test.fb_wight, &fb_test.fb_height);

	 switch (fb_test.paint_style) {
	    case 0:
			paint_rgbbuff_loop();
			break;
	    case 1:
			break;
	    case 2:
			paint_char();
			break;
	}
	rk_fb_deinit();

	return 0;
}
