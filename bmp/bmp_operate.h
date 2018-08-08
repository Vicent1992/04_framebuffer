#ifndef _BMP_OPERATE_H_
#define _BMP_OPERATE_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;


typedef struct tagBITMAPFILEHEADER {
  WORD  bfType; 		//�ļ����ͣ�������0x4d42�����ַ���"BM"��
  DWORD bfSize; 		//�����ļ���С
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;		//���ļ�ͷ��ʵ�ʵ�λͼͼ�����ݵ�ƫ���ֽ�����
}__attribute__((packed)) BITMAPFILEHEADER, *PBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;			//���ṹ�ĳ��ȣ�ֵΪ40
  DWORD  biWidth;		//ͼ��Ŀ���Ƕ�������
  DWORD  biHeight;		//ͼ��ĸ߶��Ƕ�������
  WORD  biPlanes;		//������1
  WORD  biBitCount;		//��ʾλ�������õ�ֵΪ1(�ڰ׶�ɫͼ)��4(16ɫͼ)��8(256ɫͼ)��24(���ɫͼ)��
  DWORD biCompression;	//ָ��λͼ�Ƿ�ѹ������ЧֵΪBI_RGB��BI_RLE8��BI_RLE4��BI_BITFIELDS��Windowsλͼ�ɲ���RLE4��RLE8��ѹ����ʽ��BI_RGB��ʾ��ѹ��
  DWORD biSizeImage;	//ָ��ʵ�ʵ�λͼͼ������ռ�õ��ֽ������������µĹ�ʽ���������
  						//ͼ������ = Width' * Height * ��ʾÿ��������ɫռ�õ�byte��(����ɫλ��/8,24bitͼΪ3��256ɫΪ1��
  						//Ҫע����ǣ�������ʽ�е�biWidth'������4��������(����biWidth�����Ǵ��ڻ����biWidth����С4��������)��
  						//���biCompressionΪBI_RGB����������Ϊ0��
  DWORD  biXPelsPerMeter;//Ŀ���豸��ˮƽ�ֱ��ʡ�
  DWORD  biYPelsPerMeter;//Ŀ���豸�Ĵ�ֱ�ֱ��ʡ�
  DWORD biClrUsed;		//��ͼ��ʵ���õ�����ɫ���������ֵΪ0�����õ�����ɫ��Ϊ2��(��ɫλ��)����,����ɫλ��Ϊ8��2^8=256,��256ɫ��λͼ
  DWORD biClrImportant; //ָ����ͼ������Ҫ����ɫ���������ֵΪ0������Ϊ���е���ɫ������Ҫ�ġ�
}__attribute__((packed)) BITMAPINFOHEADER, *PBITMAPINFOHEADER;


//����256ɫBMPλͼ����ɫλ��Ϊ8����Ҫ2^8 = 256����ɫ�̣�
//����24bitBMPλͼ��������RGBֱֵ�ӱ�����ͼ��������������Ҫ��ɫ�̣������ڵ�ɫ����
typedef struct tagRGBQUAD {
  BYTE rgbBlue;			//����ɫ����ɫ������
  BYTE rgbGreen;		//����ɫ����ɫ������
  BYTE rgbRed;			//����ɫ�ĺ�ɫ������
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

