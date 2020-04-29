#include <stdio.h>
#include <assert.h>
#include <string.h>


#include "common.h"
#include "tools.h"
#include "tl_malloc.h"
#include "atlas.h"
#include "tmat.h"
#include "xml.h"
#include "fbotex.h"
#include "ex.h"
#include "sprite.h"
#include "map.h"
#include "gettime.h"
#include "evt.h"
//#define DEBUG_SHOW_TIME

/*
	����ͼ����Դid��ȡ����

	struct AtlasNode* ptrOut:����name��ȡ�������ݻ���䵽ptrOut��
*/
static int
getNode(struct XMLSList* xml,const char* name,struct AtlasNode* ptrOut){
	struct XmlNode* node=0;
	//printf("xml=%0x,ptrOut=%0x\n",xml,ptrOut);

	node = xml_getrow(xml,"n",name);
	if(!node){
		return 0;//δ�ҵ��ڵ�
	}

	ptrOut->x=	xml_getfloat(node,"x");
	ptrOut->y=	xml_getfloat(node,"y");
	ptrOut->width=	xml_getfloat(node,"w");
	ptrOut->height=	xml_getfloat(node,"h");

	return 1;
}
static void
getTextureInfo(struct XMLSList* xml,const char* name,float* pWidth,float* pHeight)
{
	char bufferXmlURL[G_BUFFER_128_SIZE];
	struct XmlNode* node;

	memset(bufferXmlURL,0,G_BUFFER_128_SIZE);
	sprintf_s(bufferXmlURL,G_BUFFER_128_SIZE,"%s.tga",name);
	node=xml_getrow(xml,"imagePath",name);
	*pWidth=xml_getfloat(node,"width");
	*pHeight=xml_getfloat(node,"height");
}
struct Atals* 
atals_load(const char* path,const char* name){
	struct Atals* ptr = tl_malloc(sizeof(struct Atals));

	char bufferXmlURL[G_BUFFER_128_SIZE];
	char tgaURL[G_BUFFER_128_SIZE];
	//char alphaURL[G_BUFFER_128_SIZE];

	memset(bufferXmlURL,0,G_BUFFER_128_SIZE);
	sprintf_s(bufferXmlURL,G_BUFFER_128_SIZE,"%s%s.xml",path,name);

	memset(tgaURL,0,G_BUFFER_128_SIZE);
	sprintf_s(tgaURL,G_BUFFER_128_SIZE,"%s%s.tga",path,name);

	//memset(alphaURL,0,G_BUFFER_128_SIZE);
	//sprintf_s(alphaURL,G_BUFFER_128_SIZE,"%s%s_alpha.tga",path,name);

	memset(ptr,0,sizeof(struct Atals));
	
	if(strlen(name)>G_BUFFER_64_SIZE){
		assert(0);//printf("�ļ���(%s)̫��\n",name);
	}
	memcpy(ptr->name,name,strlen(name));
	{

		int len;
		char* _s = tl_loadfile(bufferXmlURL,&len);
		ptr->xml =  xml_parse(_s,len);
		tl_free(_s);
	}
	getTextureInfo(ptr->xml,name,&ptr->width,&ptr->height);
	//log_color(0,"����ͼ������:%s\n",tgaURL);
	//����һ������
	ptr->material = tmat_create("spritevbo",1,tgaURL);

	return ptr;
}

void
atals_tex(struct Atals* atals,const char* name,struct AtlasNode* ptrOut)
{
	//printf("===============>atals_tex %0x %0x %0x\n",atals->xml,name,ptrOut);
	if(!getNode(atals->xml,name,ptrOut))
	{
		printf("δ�ҵ�ͼ��(%s)�е���Դ(%s)\n",atals->name,name);
		assert(0);
	}
}


#define _ICON_SIZE_  32
struct Atals_params
{
	void* sprite;
	GLuint tex;
	char icon[_ICON_SIZE_];
	void* fbo;
	void (*callBack)(void*,void*);
	void* parms;
};
#ifdef DEBUG_SHOW_TIME
static long time;
#endif
//
//static void 
//f_onceCallBack(void* fbo,void* atals_params){
//
//}

static void
f_render1(int id,void* p,void* thisObj){
	//f_callLater(thisObj);
	struct Atals_params* ap = (struct Atals_params*)thisObj;
	if(ap->callBack)
		ap->callBack((void*)ap->tex,ap->parms);

	tl_free(thisObj);//���ٻص���������
}

static void
f_callLater(void*p){
	struct Atals_params* ap = (struct Atals_params*)p;
	struct FBOTexNode* fbo;
	//printf("ap��ַ=0x%0x\n",ap);
	fbo = ap->fbo;
	//fbo->enable=0;          parms);
	//fbo->onceCallBack(fbo,fbo->parms);
	//f_onceCallBack(fbo,fbo->parms);
	ex_ptr_remove(ap->sprite);//����Ⱦ�б����Ƴ�
	map_set(ex_getIns()->mapptr,ap->icon,(void*)ap->tex); 
#ifdef DEBUG_SHOW_TIME
	printf("atals_new_tex���� : %ld ����\n",get_longTime()-time);
#endif
	ex_remove_fbo(fbo);
	fbo_dispose(fbo,0);
	// 
	//printf("f_callLater ����%s����%d����\n",ap->icon,(void*)ap->tex);

	evt_once(ex_getIns(),EVENT_ENGINE_RENDER_3D,f_render1,ap);	
}

static void
f_render(int id,void* p,void* thisObj){
	//printf("��ʼ����֮��callback f_render\n");
	f_callLater(thisObj);
}

//��ͼ���д���һ����������,������
GLuint
atals_new_tex(struct Atals* atals,const char* icon,
			  void (*callBack)(void*,void*),void* parms){
	GLuint tex;
	//long t = get_longTime();
	struct MapNode * node ;
	
	struct Atals_params* ap;
	//printf("��ʼatals_new_tex  %0x %0x,%s\n",ex_getIns()->texmap,icon,icon);
	node = map_get(ex_getIns()->mapptr,icon);
	//printf("map_get���� : %ld ����\n",get_longTime()-t);
	//printf("node=%0x\n",node);
	if(node){
		
		//log_color(0,"���ü�ֵ:%0x,%s\n",node,icon);

		//printf("====%s,%0x\n",icon,parms);
		if(callBack){
			callBack((void*)icon,parms);
		}
		return (GLuint)node->value;
	}
	else{
		void* fbo = 0;
		void* _2dfbo = 0;
		//void* spr;//����չʾ��Sprite,���ڹ۲���Ⱦ�Ķ����Ƿ���ȷ
		//void* material;

		//struct Atals* atals = ex_get_ui_atals();//ͼ��
		//struct AtlasNode p;
		struct AtlasNode _pNode;

		struct AtlasNode* pnode;
		void* sprite;//������frame buffer object����Ⱦ
		
		struct FboInfo info;
		
		char tname[32];
		
		if(strlen(icon)>_ICON_SIZE_){
			printf("icon���ı�����̫��\n");
			assert(0);
		}
		//pnode = tl_malloc(sizeof(struct AtlasNode));
		pnode = &_pNode;
#ifdef DEBUG_SHOW_TIME
		time = get_longTime();
#endif

		
		atals_tex(atals,icon,pnode);
		
		//printf("p = %0x %.3f %.3f\n",pnode,pnode->width,pnode->height);
		fbo = fbo_init(pnode->width,pnode->height);
		//printf("fbo = %0x  %.3f %.3f\n",fbo,pnode->width,pnode->height);
		fbo_info(fbo,&info);
		tex = (GLuint)info.tex; //fbo_getTex(fbo);
		
		//printf("tex = %0x\n",tex);

		tl_newName(tname,32,0);

		_2dfbo = info.cam2d;//fbo_get2dCam(fbo);
		//printf("tname = %s _2dfbo = %d\n",tname,_2dfbo);

		sprite = (void*)sprite_create(tname,0,0,pnode->width,pnode->height,0,_2dfbo);

		//tl_free(pnode);

		//printf("sprite = %0x\n",sprite);
		//sprite_bindAtals(sprite,atals);
		sprite_set(sprite,SPRITE_ATALS,atals);
		sprite_texName(sprite,icon,0);
		ex_add(sprite);
		
		log_color(0,"���� atals===>%0x,(%s) tex=0x%0x\n",atals,icon,tex);
		//ex_add(spr);
		ex_add_fbo(fbo);
		ap = (struct Atals_params*)tl_malloc(sizeof(struct Atals_params));

		//printf("ap = %0x sprite = %0x\n",ap,sprite);

		memset(ap,0,sizeof(struct Atals_params));
		ap->sprite = sprite;
		ap->tex = tex;
		ap->fbo = fbo;
		ap->parms = parms;
		ap->callBack = callBack;
		memset(ap->icon,0,_ICON_SIZE_);
		memcpy(ap->icon,icon,strlen(icon));
		//printf("icon = %s\n",ap->icon);
		//fbo_set_once(fbo,f_onceCallBack,ap);
		//printf("��ʼ����tex:%d\n",tex);
		
		//callLater(f_callLater,ap);
		evt_once(ex_getIns(),EVENT_ENGINE_RENDER_3D,f_render,ap);
	}
	return tex;
}

void 
atals_dispose(struct Atals* ptr)
{
	xml_del(ptr->xml);
	tmat_dispose(ptr->material);
	tl_free(ptr);
}
