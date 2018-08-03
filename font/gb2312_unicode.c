#include "gb2312_unicode_table.h"

unsigned short gb2312_conv_unicode(unsigned char* wchar)
{
	unsigned short word;
	int area = wchar[0] - 0xA1;

	if (area < 9)
		area = area * 94 + (wchar[1] -0xA1);
	else if (area >= 15)
		area = (area - 6)* 94 + (wchar[1] -0xA1);

	return word = gb2312_unicode_table[area];
} 
	

