#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "tools.h"
#include "gettime.h"
#include "tl_malloc.h"
#include "bmp.h"

typedef struct BitmapFileHead{
	/*λͼ��𣬸��ݲ�ͬ�Ĳ���ϵͳ����ͬ����Windows�У����ֶε�ֵ��Ϊ��BM��*/
	unsigned short bfType;			
	/*BMPͼ���ļ��Ĵ�С*/
	unsigned long bfSize;			
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	/*BMPͼ�����ݵĵ�ַ �˵�ַ��ʼλͼ���ݵĲ�ɫ��/��ɫ��*/
	unsigned short bfOffBits;
}BitmapFileHead;
/*	��ɫ�ṹ��*/
typedef struct Color{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Color;
typedef struct BitmapInfoHead{
	unsigned long biSize;
	unsigned long biWidth;			//λͼ���
	unsigned long biHeight;			//λͼ�߶�
	unsigned short biPlanes;		
	unsigned short biBitCount;		//λͼλ�� 24,32   8λ��256ɫ�������£�4λ��1λ������ɫ���ɫ�壬����û��
	unsigned long biCompression;
	unsigned long biSizeImage;
	unsigned long biXPelsPerMeter;
	unsigned long biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
}BitmapInfoHead;
/*�����ļ�ͷ*/
void parseBmpInfo(const char* _data,struct BitmapFileHead* p_bitmapheader,
struct BitmapInfoHead* p_bitmapinfo)
{
	int l = sizeof(unsigned long);	//4
	int s = sizeof(unsigned short);	//2
	int pos = 0;
	/*�ļ�ͷ*/
	memcpy(&p_bitmapheader->bfType,_data + pos,s);		pos+=s;
	memcpy(&p_bitmapheader->bfSize,_data+pos,l);		pos+=l;
	memcpy(&p_bitmapheader->bfReserved1,_data+pos,s);	pos+=s;
	memcpy(&p_bitmapheader->bfReserved2,_data+pos,s);	pos+=s;
	memcpy(&p_bitmapheader->bfOffBits,_data+pos,l);		pos+=l;
	/*�ļ���Ϣ*/
	memcpy(&p_bitmapinfo->biSize,_data+pos,l);			pos+=l;
	memcpy(&p_bitmapinfo->biWidth,_data+pos,l);			pos+=l;
	memcpy(&p_bitmapinfo->biHeight,_data+pos,l);		pos+=l;
	memcpy(&p_bitmapinfo->biPlanes,_data+pos,s);		pos+=s;
	memcpy(&p_bitmapinfo->biBitCount,_data+pos,s);		pos+=s;
	memcpy(&p_bitmapinfo->biCompression,_data+pos,l);	pos+=l;
	memcpy(&p_bitmapinfo->biSizeImage,_data+pos,l);		pos+=l;
	memcpy(&p_bitmapinfo->biXPelsPerMeter,_data+pos,l);	pos+=l;
	memcpy(&p_bitmapinfo->biYPelsPerMeter,_data+pos,l);	pos+=l;
	memcpy(&p_bitmapinfo->biClrUsed,_data+pos,l);		pos+=l;
	memcpy(&p_bitmapinfo->biClrImportant,_data+pos,l);	pos+=l;
}

void static
parseBmp24(const char* _data,struct Color* bs,struct BitmapFileHead* p_bitmapheader,struct BitmapInfoHead *p_bitmapinfo)
{
	int	bitOffset=p_bitmapheader->bfOffBits;
	int bfSize = (int)p_bitmapheader->bfSize;
	int _sl = (int)sizeof(struct Color);
	int w = (int)p_bitmapinfo->biWidth;
	int h = (int)p_bitmapinfo->biHeight;
	int offset = h * w+p_bitmapheader->bfOffBits;/*�ܵ�����ƫ��*/
	while(bitOffset<bfSize){
		int j;
		if(bitOffset >= offset){
			break;
		}
		for(j=0;j<w;j++){
			struct Color _color;
			int k;
			for(k=0;k<h;k++){
				memcpy(&_color,_data + bitOffset,_sl);
				bs[j * w + k].b = _color.r;
				bs[j * w + k].g = _color.g;
				bs[j * w + k].r = _color.b;
				bitOffset=bitOffset+_sl;
			}
		}
	}
}

unsigned char* tl_bmp24(const char* bmppath,int* biWidth,int* biHeight){
	long ct=get_longTime();
	long sub;
	char outName[G_BUFFER_128_SIZE];

	char* fileName=0;

	int bytesLength;//�ֽڴ�С
	struct Color* bs = 0;
	struct BitmapFileHead bitmapheader ;
	struct BitmapInfoHead bitmapinfo ;
	struct BitmapFileHead * p_bitmapheader = &bitmapheader;
	struct BitmapInfoHead *p_bitmapinfo = &bitmapinfo;
	//char* _path = (char*)tl_malloc(strlen(bmppath)+1);
	char* _data = 0;

	//memset(_path,0,strlen(bmppath)+1);
	//memcpy(_path,bmppath,strlen(bmppath));
	_data =  tl_loadfile(bmppath,0);

	parseBmpInfo(_data,p_bitmapheader,p_bitmapinfo);

	//fileName = tl_getFileName((const char*)bmppath);
	tl_getPathName((const char*)bmppath,outName);

	//tl_free(_path);

	//_path = NULL;
	*biWidth = p_bitmapinfo->biWidth;
	*biHeight= p_bitmapinfo->biHeight;
	bytesLength = sizeof(struct Color)* 3 * p_bitmapinfo->biWidth * p_bitmapinfo->biHeight;
	bs=(struct Color*)tl_malloc(bytesLength);
	/*	bmp24����	*/
	if(p_bitmapinfo->biBitCount == 24){
		parseBmp24(_data,bs,p_bitmapheader,p_bitmapinfo);
	}
	tl_free(_data);
	_data = 0;
	sub = get_longTime()-ct;

	//printf("bmp24������ͼ [%s](width %d,height %d) ʹ�� %ld ����\n",outName,*biWidth,*biHeight,sub);//��ӡ����bmp��ʱ 2s

	return (unsigned char*)bs;
}