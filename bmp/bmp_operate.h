#ifndef _BMP_OPERATE_H_
#define _BMP_OPERATE_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;


typedef struct tagBITMAPFILEHEADER {
  WORD  bfType; 		//文件类型，必须是0x4d42，即字符串"BM"。
  DWORD bfSize; 		//整个文件大小
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;		//从文件头到实际的位图图像数据的偏移字节数。
}__attribute__((packed)) BITMAPFILEHEADER, *PBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;			//本结构的长度，值为40
  DWORD  biWidth;		//图像的宽度是多少象素
  DWORD  biHeight;		//图像的高度是多少象素
  WORD  biPlanes;		//必须是1
  WORD  biBitCount;		//表示位数，常用的值为1(黑白二色图)、4(16色图)、8(256色图)、24(真彩色图)。
  DWORD biCompression;	//指定位图是否压缩，有效值为BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS。Windows位图可采用RLE4和RLE8的压缩格式，BI_RGB表示不压缩
  DWORD biSizeImage;	//指定实际的位图图像数据占用的字节数，可用以下的公式计算出来：
  						//图像数据 = Width' * Height * 表示每个象素颜色占用的byte数(即颜色位数/8,24bit图为3，256色为1）
  						//要注意的是：上述公式中的biWidth'必须是4的整数倍(不是biWidth，而是大于或等于biWidth的最小4的整数倍)。
  						//如果biCompression为BI_RGB，则该项可能为0。
  DWORD  biXPelsPerMeter;//目标设备的水平分辨率。
  DWORD  biYPelsPerMeter;//目标设备的垂直分辨率。
  DWORD biClrUsed;		//本图像实际用到的颜色数，如果该值为0，则用到的颜色数为2的(颜色位数)次幂,如颜色位数为8，2^8=256,即256色的位图
  DWORD biClrImportant; //指定本图像中重要的颜色数，如果该值为0，则认为所有的颜色都是重要的。
}__attribute__((packed)) BITMAPINFOHEADER, *PBITMAPINFOHEADER;


//对于256色BMP位图，颜色位数为8，需要2^8 = 256个调色盘；
//对于24bitBMP位图，各象素RGB值直接保存在图像数据区，不需要调色盘，不存在调色盘区
typedef struct tagRGBQUAD {
  BYTE rgbBlue;			//该颜色的蓝色分量。
  BYTE rgbGreen;		//该颜色的绿色分量。
  BYTE rgbRed;			//该颜色的红色分量。
  BYTE rgbReserved;		
}__attribute__((packed)) RGBQUAD;


typedef struct bmp_operate {
		int width;
		int height;
        int savefile;
		int readfile;
        char *savefilepath;
        char *readfilepath;
		int rgb565_size;
		void* rgb565buff;
		int rgb888_size;
		void* rgb888buff;
}bmpoperate;

extern int save_rgbbuff_to_bmp(void* rgbbuff, int bpp, int width, int height);

extern int get_bmp_fileinfo(    char *filename, BITMAPFILEHEADER *file_head, 
							 BITMAPINFOHEADER *info_head);
extern int get_bmp_buffer(    char *filename, void* buffer, int size);



#endif

