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
#include "gettime.h"
#include "str.h"

#define DEBUG

#define _Font_Size_ 512
static GLbyte* g_bytes;//Ԥ�ƻ�����(Ԥ��1mb������)

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
	
	int w,h;//�ı���Ⱦ�Ŀ��
	
	/************************************************************************/
	/* ��ǰ�ı����ݻ�����                                                                     */
	/************************************************************************/
	char* _cur;
	int _curLength;
	//int _pixelSize;//�����ֽ�

}FText;

/************************************************************************/
/* ��ʼ����ʱ������                                                                     */
/************************************************************************/
static void
f_init_buffer(int w,int h){
	int length =sizeof(struct RGBA) *w * h;
	int _maxSize = _Font_Size_*_Font_Size_*sizeof(struct RGBA);
	if(length > _maxSize){
		//printf("������̫С\n");
		log_code(ERROR_BUFFER_NOT_ENOUGH);
		assert(0);
	}
	if(!g_bytes){
		g_bytes = (GLbyte*)tl_malloc(_maxSize);
	}
}

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
ftext_vis(void* p,int vis){
	FText* txt = (FText*)p;
	vis ? ex_setv(txt->spr,FLAGS_VISIBLE) : ex_resetv(txt->spr,FLAGS_VISIBLE);
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
	
	memset(txt->_cur,0,txt->_curLength);
	memcpy(txt->_cur,str,strlen(str));
}
char* 
ftext_get_str(void* p){
	FText* txt = (FText*)p;
	return txt->_cur;
}
void
ftext_get_size(void* p,int* w,int *h){
	FText* txt = (FText*)p;
	*w = txt->w;
	*h = txt->h;
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
	//if(txt->cur){
		//tl_free(txt->cur);
		//txt->cur = 0;
		memset(txt->_cur,0,txt->_curLength);
	//}
	//jgl_create_opengl_RGBA_Tex(txt->w,txt->h,GL_BGRA);
	
	//����������ݵ�alphaֵ = 0
	//length = sizeof(struct RGBA) * txt->w * txt->h;
	
	///*GLbyte* image = jgl_createRGBA_buffer(txt->w,txt->h);
	if(txt->w > 0 && txt->h > 0){
		int length;
		GLbyte* bytes;
		
		//memset(txt->_buffer,0x00,txt->_bufferLength);
		//jsl_sub(tmat_getTextureByIndex(mat,0),txt->_buffer,GL_BGRA,GL_UNSIGNED_BYTE,0,0,txt->w,txt->h);//txt->w,txt->h	txt->spr->mWidth,txt->spr->mHeight
		length =sizeof(struct RGBA) *txt->w * txt->h;	//txt->_pixelSize;
		
		bytes = g_bytes;//(GLbyte*)tl_malloc(length);
		memset(bytes,0x00,length);
		jsl_sub(tmat_getTextureByIndex(mat,0),bytes,GL_BGRA,GL_UNSIGNED_BYTE,0,0,txt->w,txt->h);
	}
	txt->w = 0;
	txt->h = 0;
	//txt->_pixelSize = 0;
}

void
ftext_set_buffer(void* p,int bufferSize){
	FText* txt = (FText*)p;
	if(txt->_cur){
		tl_free(txt->_cur);
		txt->_cur = 0;
	}
	txt->_cur = tl_malloc(bufferSize);
	memset(txt->_cur,0,bufferSize);
	txt->_curLength = bufferSize;
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

	f_init_buffer(txtWidth,txtHeight);

	txt->spr = sprite_create(txtName,0,0,txtWidth,txtHeight,0);
	
	txt->_bufferLength = txt->fw * txt->fh*4;//����һ���ı�������Ҫ�Ļ������Ĵ�С
	txt->_buffer = tl_malloc(txt->_bufferLength);

	spr = txt->spr;
	
	sprite_rotateZ(spr,-PI/2);
	
	sprite_set_scale_z(spr,1/n);

	//spr->material = tmat_create_rgba("font1",64,64,GL_BGRA);//"font"
	
	spr->material = tmat_create_rgba("font1",txtWidth,txtHeight,GL_BGRA);//"font"
	
	//���ñ�����͸��
	//tmat_set_discardAlpha(spr->material,1);

	ftext_set_buffer(txt,G_BUFFER_8_SIZE);//����Ĭ�ϵ��ı���������С
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
f_reset_xy(FText* txt,int *px,int *py,int cw,int ch,int top){
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
	
	//*py = txt->_py + (txt->fh - ch);	//y = txt->fh - ch;//�׶���
	*py = txt->_py - top;
	 
	//if(*py > txt->spr->mHeight-txt->fh){
	if(*py + ch > txt->spr->mHeight){
		//�ı���Ⱦy������ + ��λ����߶� > �����߶�

		txt->_stop = 1;//���������ı����ػ�ȡ����
		return 0; 
	}else{
		//�����ı�����Ŀ��
	//	printf("py=%d\n",txt->_py);
	}
	if (txt->w<txt->_px){
		txt->w=txt->_px;
	}
	{
		//====================================================================
		//�����ı��߶�
		int real_h = *py + ch;
		if(real_h> txt->h){
			txt->h = real_h;
		}
		txt->h = real_h> txt->h ? real_h : txt->h;
	}
	
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
		int top;
		//int _size = txt->size;
		int iWidth,iHeight;
		unsigned char* rgba = txt->_buffer;//(unsigned char*)tl_malloc(_size*_size*4);
		//*eFormat = GL_BGRA;
		//*iComponents = GL_RGBA;
		memset(txt->_buffer,0,txt->_bufferLength);

		//ft_load(rgba,txt->fw,txt->fh,&iWidth,&iHeight,s);
		ft_parse(ex_getInstance()->ft,rgba,txt->fw,txt->fh,&iWidth,&iHeight,&top,s);
#ifdef DEBUG
		//	printf("ft_load:%s:%d %d\n",s,iWidth,iHeight);
#endif

		*pw = iWidth;
		*ph = iHeight;

		//y = txt->fh - iHeight;//�׶���

		if(f_reset_xy(txt,&x,&y,iWidth,iHeight,top)){
			//printf("==>%s\n",s);
			//txt->_pixelSize+=iWidth*iHeight*sizeof(struct RGBA);
#ifdef DEBUG
//			printf("%s\t->top = %d iHeight = %d fh = %d\n",s,top,iHeight,txt->fh);
#endif
			jsl_sub(tmat_getTextureByIndex(mat,0),rgba,GL_BGRA,GL_UNSIGNED_BYTE,x,y,iWidth,iHeight);
		}
	}
}

void
ftext_dispose(void* p){
	FText* txt = (FText*)p;
	sprite_dipose(txt->spr);
	tl_free(txt->_cur);
	tl_free(p);
}