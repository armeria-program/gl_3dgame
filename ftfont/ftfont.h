#ifndef _FT_FONT_
#define _FT_FONT_

//https://blog.csdn.net/wesleyluo/article/details/7432063
#include "def.h"

/*
 *	tl_malloc����һ��rgba���ֽ�����,ȡ��֮��ǵ�tl_free������,�ýӿڵ������֮����Զ�������Դ���
 */
//DLLIMPORT void ft_load(unsigned char* buffer,int fontw,int fonth,int *iWidth, int *iHeight,char* str);

/************************************************************************/
/* ������������豸���                                                                     */
/************************************************************************/
DLLIMPORT
void* ft_create(const char* fileName);
/************************************************************************/
/* ���������������                                                                     */
/************************************************************************/
DLLIMPORT
void ft_parse(void* pt,unsigned char* outBuffer,int fontw,int fonth,int *iWidth, int *iHeight,int* iTop,char* str);

/*������������豸*/
DLLIMPORT
void ft_free(void* pt);
#endif