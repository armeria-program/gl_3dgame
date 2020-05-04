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
#include "base.h"

#include "shader.h"

#define DEBUG

#define _Font_Size_ 512
//static GLbyte* g_bytes;//Ԥ�ƻ�����(Ԥ��1mb������)
typedef struct Rect{
	unsigned short x,y,w,h;
	char word;
}Rect;
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
	/* ��ǰ�ı����ݻ�����
	/************************************************************************/
	char* _cur;
	int _curLength;
	//int _pixelSize;//�����ֽ�
	
	//*******************************************************
	
	//�ַ���������
	void* wordList;
	char* words;
	//int wordSize;

	//�ı��Ƿ���
	int wordWrap;

	GLbyte* bufferbytes;
}FText;
/************************************************************************/
/* ����������ݵ���ͼ
/************************************************************************/
static void
f_draw_word(FText* txt,unsigned char* rgba,int x,int y,int w,int h){
	struct Sprite* spr = txt->spr;
	GMaterial* mat = spr->material;
	jsl_sub(tmat_getTextureByIndex(mat,0),rgba,GL_BGRA,GL_UNSIGNED_BYTE,x,y,w,h);
}

//����
static void
f_nextLne(struct FText* txt){
	txt->_py+=txt->fh;
}

/************************************************************************/
/* ��ʼ����ʱ������                                                                     */
/************************************************************************/
static void
f_init_buffer(int w,int h){
	//int length =sizeof(struct RGBA) *w * h;
	//int _maxSize = _Font_Size_*_Font_Size_*sizeof(struct RGBA);
	//if(length > _maxSize){
	//	//printf("������̫С\n");
	//	log_code(ERROR_BUFFER_NOT_ENOUGH);
	//	assert(0);
	//}
	//if(!g_bytes){
	//	g_bytes = (GLbyte*)tl_malloc(_maxSize);
	//}
}
//��wordlistת��Ϊһ��str����
char* 
get_wordlist_str(void *ptr){
	struct	FText* txt = (struct FText*)ptr;
	struct LStackNode* s = (struct LStackNode* )txt->wordList;
	int length = LStack_length(s);
	int index = length;
	if(txt->words){
		tl_free(txt->words);
	}
	txt->words = (char*)tl_malloc(length + 1);
	memset(txt->words,0,length + 1);
	{
		struct Rect* data;
		void* top,*p;
		top = s;
		p=top;
		while((int)LStack_next(p)){
			char _s[1];
			p=(void*)LStack_next(p);
			data = (struct Rect*)LStack_data(p);
			_s[0]=data->word;
			memcpy(txt->words+(index-1),_s,1);
			index--;
		}
	}
	return txt->words;
}
static int 
f_pCallBack(void* inParam,char* str){
	FText* txt = (FText*)inParam;
	int w,h;
	
	if (strlen(str)==1){
		switch(str[0])
		{
		case  10://�س�ռλ��
			if(txt->wordWrap){
				txt->_px = 0;
				f_nextLne(txt);
				return 1;
			}
		}
	}
	
	ftext_set(inParam,str,txt->_cx,txt->_cy,&w,&h);
	txt->_cx+=w;

	return txt->_stop == 0 ? 1 : 0;
}

void
ftext_vis(void* p,int vis){
	FText* txt = (FText*)p;
	/*int flag = FLAGS_VISIBLE;
	vis ? base_setv(txt->spr,flag) : base_resetv(txt->spr,flag);*/

	struct HeadInfo* base =	(struct HeadInfo*)txt->spr->base;
	base_set_visible(base,vis);
}
void 
ftext_set_wordWrap(void* p,int v){
	FText* txt = (FText*)p;
	txt->wordWrap = v;
}
void
ftext_open_wordlist(void* p){
	FText* txt = (FText*)p;
	if (!txt->wordList){
		txt->wordList = LStack_create();
	}
}

void* 
ftext_get_container(void* p){
	FText* txt = (FText*)p;
	return txt->spr;
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
	if(w!=0)	*w = txt->w;// + txt->fw;
	if(h!=0)	*h = txt->h;// + txt->fh;
	
	memset(txt->_cur,0,txt->_curLength);
	memcpy(txt->_cur,str,strlen(str));
}
char* 
ftext_get_str(void* p){
	FText* txt = (FText*)p;
	if(txt->wordList){
		return get_wordlist_str(txt);
	}
	return txt->_cur;
}
void
ftext_get_wordpos(void* p,int* w,int *h){
	FText* txt = (FText*)p;
	//*w = txt->w;
	//*h = txt->h;
	*w = txt->_px;
	*h = txt->_py;
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
	
	memset(txt->_cur,0,txt->_curLength);
	
	//����������ݵ�alphaֵ = 0
	//length = sizeof(struct RGBA) * txt->w * txt->h;
	
	///*GLbyte* image = jgl_createRGBA_buffer(txt->w,txt->h);
	if(txt->w > 0 && txt->h > 0){
		int length;
		GLbyte* bytes;
		
		//jsl_sub(tmat_getTextureByIndex(mat,0),txt->_buffer,GL_BGRA,GL_UNSIGNED_BYTE,0,0,txt->w,txt->h);//txt->w,txt->h	txt->spr->mWidth,txt->spr->mHeight
		length =sizeof(struct RGBA) *txt->w * txt->h;
		
		txt->bufferbytes = tl_malloc(length);
		
		bytes = txt->bufferbytes;
		memset(bytes,0x00,length);

		//jsl_sub(tmat_getTextureByIndex(mat,0),bytes,GL_BGRA,GL_UNSIGNED_BYTE,0,0,txt->w,txt->h);
		f_draw_word(txt,bytes,0,0,txt->w,txt->h);

	}
	txt->w = 0;
	txt->h = 0;
	if(txt->wordList){
		LStack_clear(txt->wordList);
	}
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

static void
f_rotate(struct Sprite* spr){
	//sprite_rotateZ(spr,0);
	sprite_set(spr,SPRITE_RZ,0);
	//sprite_rotateX(spr,PI);
	sprite_set(spr,SPRITE_RX,PI);
	base_setv(spr,FLAGS_REVERSE_FACE);
}

void*
ftext_create(char* txtName,int txtWidth,int txtHeight,int fw,int fh,void* cam){
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

	txt->spr = sprite_create(txtName,0,0,txtWidth,txtHeight,0,cam);
	ex_add(txt->spr);
	txt->_bufferLength = txt->fw * txt->fh*4;//����һ���ı�������Ҫ�Ļ������Ĵ�С
	txt->_buffer = tl_malloc(txt->_bufferLength);

	spr = txt->spr;
	
	//sprite_rotateZ(spr,-PI/2);
	//sprite_setUV(spr,0,0,1,1);

	//sprite_set_scale_z(spr,1/n);
	sprite_set((void*)spr,SPRITE_SCALE_Z,1/n);
	
	f_rotate(spr);
	//spr->material = tmat_create_rgba("font1",64,64,GL_BGRA);//"font"
	
	spr->material = tmat_create_rgba("simple;font1",txtWidth,txtHeight,GL_BGRA);//"font"
	
	{
		struct GMaterial* m =  (struct GMaterial*)spr->material;
		m->updateVarCallback = vboSimpleCallBack;//������ɫ�������Զ���ص�
	}


	//���ñ�����͸��
	//tmat_set_discardAlpha(spr->material,1);
	
	txt->wordWrap = 1;

	ftext_set_buffer(txt,G_BUFFER_8_SIZE);//����Ĭ�ϵ��ı���������С
	

	return txt;
}

void 
ftext_setpos(void* p,int x,int y){
	FText* txt = (FText*)p;
	struct Sprite* spr = txt->spr;
	float n = txt->n;
	sprite_setpos(spr,x,y);
}

/************************************************************************/
/* �����ı��ǲ�����Ҫ���д���   top�ǵ����ı��ַ���y��ƫ�� 
/************************************************************************/
static int
f_reset_xy(FText* txt,int *px,int *py,int cw,int ch,int top){
	//��ǰcw�ı���Ⱦ�Ŀ��,ch �ı���Ⱦ�ĸ߶�

	int maxWidth = txt->spr->mWidth;//���Ŀ��
	int tw = cw;//txt->fw

	int _st = 0;
	if(txt->_px+ tw<=maxWidth){
		txt->_px+=tw;
	}
	else{
		if(txt->wordWrap){
			//����
			txt->_px=tw;
			if(txt->_py+txt->fh < txt->spr->mHeight){
				//txt->_py+=txt->fh;
				f_nextLne(txt);
				_st = 1;
			}
		}else{
			txt->_stop = 1;
			return 0;
		}
	}
	
	*px = txt->_px-tw;	
	*py = txt->_py - top;
	 
	if(txt->_py + txt->fh >  txt->spr->mHeight){
		//�ı���Ⱦy������ + ��λ����߶� > �����߶�
		if(_st)	txt->_py-=txt->fh;
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
static void
f_del_wordlist(FText* txt){
	//�ͷ�ÿһ���ڵ�

	struct LStackNode* s = (struct LStackNode* )txt->wordList;
	struct Rect* data;
	void* top,*p;

	top = s;
	p=top;

	while((int)LStack_next(p)){
		p=(void*)LStack_next(p);
		data = (struct Rect*)LStack_data(p);
		tl_free((void*)data);
	}
	LStack_delete(txt->wordList);
	txt->wordList = 0;
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
		//printf("==>[%s]\n",s);

		ft_parse(ex_getIns()->ft,rgba,txt->fw,txt->fh,&iWidth,&iHeight,&top,s);
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
			
			f_draw_word(txt,rgba,x,y,iWidth,iHeight);

			if(txt->wordList){
				struct Rect* rect = (struct Rect*)tl_malloc(sizeof(struct Rect));
				rect->x = x;	rect->y = y;	rect->w = iWidth;	rect->h = iHeight;
				rect->word = s[0];
				LStack_push(txt->wordList,(void*)rect);
			}

			//printf("��λ��%d,%d,����һ������Ϊ%d,%d�ľ�����������\n",x,y,iWidth,iHeight);
		}
	}
}

/*ɾ����һ���ַ�*/
static void
f_clear_word(FText* txt,struct Rect* r){
	unsigned char* rgba = txt->_buffer;
	memset(txt->_buffer,0,txt->_bufferLength);
	if(txt->_px == 0){
		txt->_py -= txt->fh;
	}
	txt->_px = r->x;
	if(txt->_py+txt->fh <= txt->spr->mHeight)	txt->_stop = 0;

	//����ftext��w,h����
	//txt->w = r->x - r->w;
	//txt->h = r->y - r->h;

	//printf("del one word:%d,%d,%d,%d w = %d h = %d\n",r->x,r->y,r->w,r->h,txt->w,txt->h);
	f_draw_word(txt,rgba,r->x,r->y,r->w,r->h);
}

void
ftext_pop_word(void* p){
	FText* txt = (FText*)p;
	int e;
	struct Rect* rect;
	struct LStackNode* _l = (struct LStackNode*)txt->wordList;
	if (!txt->wordList){
		printf("ftext_pop_word txt->wordList=0\n");
		assert(0);
	}

	if(LStack_length(_l) == 0){
		return;
	}
	LStack_top(_l,&e);
	rect = (struct Rect*)e;
	f_clear_word(txt,rect);

	LStack_pop(_l,0);
}
//���õ������ͻص�
void
ftext_set_hit(void* p,
			  void (*clickCallBack)(void* ,int ,int ),
			  int x,int y,int w,int h){
	FText* txt = (FText*)p;
	sprite_set(txt->spr,SPRITE_HIT_RECT,x,y,w,h);
	sprite_set_clickHandle(txt->spr,clickCallBack);
}

void
ftext_dispose(void* p){
	FText* txt = (FText*)p;
	if(txt->bufferbytes){
		tl_free(txt->bufferbytes);
	}
	if(txt->wordList)
		f_del_wordlist(txt);
	if(txt->words){
		tl_free(txt->words);
		txt->words = 0;
	}
	sprite_dipose(txt->spr);
	tl_free(txt->_cur);
	tl_free(p);
}									