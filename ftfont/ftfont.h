#ifndef _FT_FONT_
#define _FT_FONT_

//https://blog.csdn.net/wesleyluo/article/details/7432063
#include "def.h"
/*
 *	tl_malloc����һ��rgba���ֽ�����,ȡ��֮��ǵ�tl_free������
 */
DLLIMPORT void* ft_load(unsigned char* buffer,int fontw,int fonth,int *iWidth, int *iHeight,char* str);

#endif