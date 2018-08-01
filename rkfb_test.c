#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "rk_fb.h"
#include "rgb565_operate.h"
#include "rgb888_operate.h"
#include "argb8888_operate.h"

typedef struct rkfb_test {
		struct fb *rk_fb;
        int paint_style;
		int fb_format;
		int fb_bpp;
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
	void* dist_buff = NULL;
	void* rgbbuff =NULL;
	int width, height, size, bpp;
	printf("vicent------------------paint_rgbbuff\n");

	rk_fb_get_out_device(&width, &height);

	switch(fb_test.fb_bpp){
	case 16:
		prepare_rgb565(&rgbbuff, width, height, &bpp, &size);
		break;
	case 24:
		prepare_rgb888(&rgbbuff, width, height, &bpp, &size);
		break;
	case 32:
		prepare_argb8888(&rgbbuff, width, height, &bpp, &size);
		break;
	}

	printf("vicent --- rgbbuff bits %d width %d height %d size %d\n", bpp, width, height, size);

	while(!loop_exit){

		ui_win = rk_fb_getuiwin();
		if (ui_win == NULL)
			printf("vicent----rk_fb_getuiwin error\n");

		dist_buff = (void*)ui_win->buffer;

		memcpy(dist_buff, rgbbuff, size);
		sync();
		rk_fb_ui_disp(ui_win);
		usleep(1000*1000);

	}
	
	switch(fb_test.fb_bpp){
	case 16:
		release_rgb565(&rgbbuff);
		break;
	case 24:
		release_rgb888(&rgbbuff);
		break;
	case 32:
		release_argb8888(&rgbbuff);
		break;
	}
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
	
    fb_test.rk_fb = rk_fb_init(fb_test.fb_format);

	 switch (fb_test.paint_style) {
	    case 0:
			paint_rgbbuff_loop();
			break;
	    case 1:
			break;
	    case 2:
			break;
	}
	rk_fb_deinit();

	return 0;
}
