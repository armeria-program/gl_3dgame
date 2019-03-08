#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "tools.h"
#include "tl_malloc.h"
#include "ex.h"
#include "tmat.h"
#include "obj_vbo.h"
#include "ftext.h"
#include "sprite.h"
#include "jgl.h"
#include "ftfont.h"

#include "str.h"

#define DEBUG

/*
*[0]: width: 6  height: 12
[1]: width: 7  height: 13
[2]: width: 7  height: 14
[3]: width: 8  height: 15
[4]: width: 8  height: 16
[5]: width: 9  height: 17
 *
 **/
typedef struct FText
{
	struct Sprite* spr;	
	int size;
	int fw,fh;
	//===========================================================
	//����Ԥ�ƻ�����,���ڴ洢��ʱ�������ֽ�������
	void* _buffer;
	//��������С
	int _bufferLength;

	float n;//��������
	
	int _cx,_cy;//��ʱ����
	
	int _px,_py;//��ǰ�ı�����������ı�������Ҫ���� _px + fw <= spr.w ,py + fh <= spr.h
	
	int _stop;//�޷����ƵĲ��ֽ�������
	
//#ifdef DEBUG
//	struct Sprite* debugbg;
//#endif
	int w,h;//�ı���Ⱦ�Ŀ��

}FText;

static int 
f_pCallBack(void* inParam,char* str){
	FText* txt = (FText*)inParam;
	int w,h;
	ftext_set(inParam,str,txt->_cx,txt->_cy,&w,&h);
	txt->_cx+=w;
	//printf("%s\n",str);
	return txt->_stop == 0 ? 1 : 0;
}
void
ftext_parse(void* p,const char* str,int *w,int *h){
	//void 
	//	str_parse_eg_cn(const char* str,void* inParam,
	//	void(pCallBack)(void*,char*))
	//	
	FText* txt = (FText*)p;
	txt->_cx = 0;
	txt->_cy = 0;

	str_parse_eg_cn(str,p,f_pCallBack);
	//printf("%d %d\n",txt->w,txt->_py);
	*w = txt->w;// + txt->fw;
	*h = txt->h;// + txt->fh;
}
void
ftext_clear(void* p){
	FText* txt = (FText*)p;
	//int length;
	struct Sprite* spr = txt->spr;
	GMaterial* mat = spr->material;
	txt->_cx = 0;
	txt->_cy = 0;
	txt->_px = 0;
	txt->_py = 0;
	txt->_stop = 0;
	//jgl_create_opengl_RGBA_Tex(txt->w,txt->h,GL_BGRA);
	
	//����������ݵ�alphaֵ = 0
	//length = sizeof(struct RGBA) * txt->w * txt->h;
	memset(txt->_buffer,0x00,txt->_bufferLength);
	///*GLbyte* image = jgl_createRGBA_buffer(txt->w,txt->h);

	jsl_sub(tmat_getTextureByIndex(mat,0),txt->_buffer,GL_BGRA,GL_UNSIGNED_BYTE,0,0,txt->w,txt->h);
	
	txt->w = 0;
	txt->h = 0;
}

void*
ftext_create(char* txtName,int txtWidth,int txtHeight,int fw,int fh){
	//int txtWidth = 64;
	//int txtHeight= 64;
	//struct Sprite* sp = sprite_create("text",0,0,32,32,0);
	struct Sprite* spr;
	float n;

	FText* txt = (FText*)tl_malloc(sizeof(FText));
	memset(txt,0,sizeof(FText));
	//txt->size = 20;//18
	txt->n = 1;
	n = txt->n;
//6 12
//����ʹ��12,11,��ĸ����ʹ���κγߴ������
	txt->fw = fw*n;
	txt->fh = fh*n;
	//txt->fw = 18*n;
	//txt->fh = 18*n;
//#ifdef DEBUG
//	{
//		char _name[G_BUFFER_64_SIZE];
//		tl_newName(_name,G_BUFFER_64_SIZE);
//
//		txt->debugbg = sprite_create(_name,0,0,txtWidth,txtHeight,0);
//		txt->debugbg->material = tmat_create("spritevbo",1,"\\resource\\texture\\b.bmp");//spr->material;
//		//ex_setv(txt->debugbg,FLAGS_DRAW_PLOYGON_LINE);
//		ex_setv(txt->debugbg,FLAGS_VISIBLE);
//		sprite_rotateZ(txt->debugbg,-PI/2);
//		sprite_set_scale_z(txt->debugbg,1/n);
//	}
//#endif
	txt->spr = sprite_create(txtName,0,0,txtWidth,txtHeight,0);
	
	txt->_bufferLength = txt->fw * txt->fh*4;//������Ҫ�Ļ������Ĵ�С
	txt->_buffer = tl_malloc(txt->_bufferLength);

	spr = txt->spr;
	
	sprite_rotateZ(spr,-PI/2);
	
	sprite_set_scale_z(spr,1/n);

	//spr->material = tmat_create_rgba("font1",64,64,GL_BGRA);//"font"
	
	spr->material = tmat_create_rgba("font1",txtWidth,txtHeight,GL_BGRA);//"font"
	
	//���ñ�����͸��
	tmat_set_discardAlpha(spr->material,1);

	/*
	{
		int cw=0;
		int w,h;
		ftext_set(txt,"R",0,0,&w,&h);
		cw+=w;
	//	ftext_set(txt,"A",cw,0,&w,&h);
		cw+=w;
	//	ftext_set(txt,"r",cw,0,&w,&h);
		
		ftext_setpos(txt,100,100);
	}
	*/
	return txt;
}

void 
ftext_setpos(void* p,int x,int y){
	FText* txt = (FText*)p;
	struct Sprite* spr = txt->spr;
	float n = txt->n;
	/*x = (float)x/txt->n;
	y = (float)y/txt->n;*/
	/*if(n!=1){
		x = x - spr->mWidth*(spr->zScale);
		y = y - spr->mHeight*(spr->zScale);
	}*/
	sprite_setpos(spr,x,y);
//#ifdef DEBUG
//	sprite_setpos(txt->debugbg,x,y);
//#endif
}

/************************************************************************/
/* �����ı��ǲ�����Ҫ���д���                                                                     */
/************************************************************************/
static int
f_reset_xy(FText* txt,int *px,int *py,int cw,int ch){
	//��ǰcw�ı���Ⱦ�Ŀ��,ch �ı���Ⱦ�ĸ߶�

	int maxWidth = txt->spr->mWidth;//���Ŀ��
	int tw = cw;//txt->fw;
	if(txt->_px+ tw<=maxWidth){
		txt->_px+=tw;
	}
	else{
		//����
		txt->_px=tw;
		txt->_py+=txt->fh;
	}
	
	*px = txt->_px-tw;
	*py = txt->_py + (txt->fh - ch);	//y = txt->fh - ch;//�׶���
	//if(*py > txt->spr->mHeight-txt->fh){
	if(*py + ch > txt->spr->mHeight){
		txt->_stop = 1;//���������ı����ػ�ȡ����
		return 0;
	}else{
		//�����ı�����Ŀ��
	//	printf("py=%d\n",txt->_py);
	}
	if (txt->w<txt->_px){
		txt->w=txt->_px;
	}
	txt->h = *py + ch;
	
	return 1;
}

void 
ftext_set(void* p,char* s,int x,int y,int* pw,int* ph){
	FText* txt = (FText*)p;
	if(txt->_stop)
	{
		//��ֹ����
		//printf("%s\n",s);
	}
	else{
		struct Sprite* spr = txt->spr;
		GMaterial* mat = spr->material;

		//int _size = txt->size;
		int iWidth,iHeight;
		unsigned char* rgba = txt->_buffer;//(unsigned char*)tl_malloc(_size*_size*4);
		//*eFormat = GL_BGRA;
		//*iComponents = GL_RGBA;
		memset(txt->_buffer,0,txt->_bufferLength);

		//ft_load(rgba,txt->fw,txt->fh,&iWidth,&iHeight,s);
		ft_parse(ex_getInstance()->ft,rgba,txt->fw,txt->fh,&iWidth,&iHeight,s);
#ifdef DEBUG
		//	printf("ft_load:%s:%d %d\n",s,iWidth,iHeight);
#endif

		*pw = iWidth;
		*ph = iHeight;

		//y = txt->fh - iHeight;//�׶���

		if(f_reset_xy(txt,&x,&y,iWidth,iHeight)){
			//printf("==>%s\n",s);
			jsl_sub(tmat_getTextureByIndex(mat,0),rgba,GL_BGRA,GL_UNSIGNED_BYTE,x,y,iWidth,iHeight);
		}
	}
}

void
ftext_dispose(void* p){
	FText* txt = (FText*)p;
	sprite_dipose(txt->spr);
	
	tl_free(p);
}