#include <gl/glew.h>
#include <gl/glut.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "tl_malloc.h"
#include "jgl.h"

#include "fbotex.h"
#include "tmat.h"
#include "base.h"
#include "sprite.h"
#include "camera.h"
#include "ex.h"
struct FBOTexNode
{
	void* _3dcam;

	GLuint              fboName;			//FBO��������	
	GLuint				tex;		//��ͼ
	GLuint              depthBufferName;	//��Ȼ�����
	int					texw,texh;			//��ͼ�Ŀ��
	
	//GLuint				tex;

	// ��Ⱦ�ڵ�Ļص�
	void (*callBack)();

	//��Ⱦ�Ľڵ��б�
	void* nodelist;
};
static void
f_renderList(int data,int parms){
	//printf("%d\n",data);
	ex_render3dNode(data);
}
void
fbo_render(void* ptr){
	struct FBOTexNode* fbo = (struct FBOTexNode*)ptr;
	const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->fboName);
	glDrawBuffers(1, fboBuffs);

	glViewport(0, 0, fbo->texw, fbo->texh);
	glClearColor(1,0,1,1);//���Ƴ��Ϻ�ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ǰ�Ļ�����
	
	ex_switch3dCam(fbo->_3dcam);
	if(fbo->callBack){
		fbo->callBack();
	}
	LStack_ergodic_t(fbo->nodelist,f_renderList,0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void fbo_pushNode(void* p,void* _node){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	if(_node){
		struct HeadInfo* b;
		b = base_get(_node);
		if( base_findByName(fbo->nodelist,b->name)){
			printf("fbo_pushNode,error! �����Ķ���_engine :%s\n",b->name);
			assert(0);
		}else{
			LStack_push(fbo->nodelist,_node);
		}
	}
}

void fbo_bind(void* ptr,void (*callBack)()){
	struct FBOTexNode* fbo = (struct FBOTexNode*)ptr;
	fbo->callBack = callBack;
}

void* 
fbo_init(int texW,int texH){
	struct FBOTexNode* fbo = (struct FBOTexNode*)tl_malloc(sizeof(struct FBOTexNode));
	
	GLint mirrorTexWidth  = texW;
	GLint mirrorTexHeight = texH;

	GLuint              fboName;
	GLuint				mirrorTexture;//������ͼ
	GLuint              depthBufferName; //��Ȼ�����
	
	memset(fbo,0,sizeof(struct FBOTexNode));

	fbo->nodelist = LStack_create();

	fbo->texw = mirrorTexWidth;
	fbo->texh = mirrorTexHeight;

	// Create and bind an FBO(����,��֡�������fbo)
	glGenFramebuffers(1,&fboName);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboName);

	// Create depth renderbuffer(������Ȼ���)
	glGenRenderbuffers(1, &depthBufferName);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferName);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, mirrorTexWidth, mirrorTexHeight);

	// Create the reflection texture(����ӳ����ͼ)
	glGenTextures(1, &mirrorTexture);
	glBindTexture(GL_TEXTURE_2D, mirrorTexture);
	//���ߵ�bug
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mirrorTexWidth, mirrorTexHeight, 0, GL_RGBA, GL_FLOAT, 0);
  
	// Attach texture to first color attachment and the depth RBO(��texture�󶨵�RBO)
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture, 0);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferName);

	fbo->depthBufferName = depthBufferName;
	fbo->fboName = fboName;
	fbo->tex = mirrorTexture;

	// Make sure all went well
	//gltCheckErrors();
	
	//fbo->_2dspr=f_createSprite(mirrorTexture,texW,texH);
	//fbo->tex = mirrorTexture;
	// Reset framebuffer binding
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//###################################
	fbo->_3dcam = cam_create();
	cam_setPerspect(fbo->_3dcam,45.0,fbo->texw / fbo->texh , 0.1, 10000);
	return fbo;
}

//����fbo����
void
fbo_dispose(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;

	//�ͷ�֮ǰҪ������Щpush��ȥ��3d�ڵ�
	LStack_delete(fbo->nodelist);
	fbo->nodelist = 0;
	if(fbo->tex){
		glDeleteTextures(1, &fbo->tex);
		fbo->tex = 0;
	}
	//sprite_dipose(fbo->_2dspr);
	//glDeleteTextures(1, textures);
	cam_dispose(fbo->_3dcam);

	// Cleanup RBOs
	glDeleteRenderbuffers(1, &fbo->depthBufferName);

	// Cleanup FBOs
	glDeleteFramebuffers(1, &fbo->fboName);
}

//void* fbo_get_spr(void* p){
//	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
//	return fbo->_2dspr;
//}
void* fbo_getCam(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return fbo->_3dcam;
}

void* fbo_getTex(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return (void*)fbo->tex;
}