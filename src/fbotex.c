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

//#define _2D_Z_POS -90000

struct FBOTexNode{
	void* _3dcam;			//3d͸��camera
	void* _2dcam;			//2d͸��camera
	GLuint              fboName;			//FBO��������	
	GLuint				tex;				//��ͼ������,fbo��Ⱦ��֡��������������������
	GLuint              depthBufferName;	//��Ȼ�����
	int					texw,texh;			//��ͼ�Ŀ��
	int enable;								//�Ƿ��ڼ���״̬
	//int once;								//�Ƿ�ֻ��Ⱦ�ص�һ��
	//int disposeTexStatus;							//�Ƿ������������
	//void (*onceCallBack)(void*,void*);
	void* parms;
};
//static void
//f_callLater(void*p){
//	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
//	//fbo->enable=0;          parms);
//	fbo->onceCallBack(fbo,fbo->parms);
//	ex_remove_fbo(fbo);
//	fbo_dispose(fbo,0);
//	// 
//	printf("f_callLater ��������[%d]����\n",fbo->tex);	
//}
void
fbo_render(void* ptr){
	struct FBOTexNode* fbo = (struct FBOTexNode*)ptr;

	if(fbo->enable){
		const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

		ex_switch3dCam(fbo->_3dcam);

		{
			//This is Test code for debug to equal 2 mat4x4
			/*struct EX* p = ex_getIns();
			cam_setOrtho(p->_2dcam,256,256,-p->allzBuffer);*/

			//void* a= cam_getPerctive(fbo->_2dcam);
			//void* b= cam_getPerctive(ex_getIns()->_2dCurCam);

			//mat4x4_copy(b,a);//��_2dCurCam�����ֵ��ֵ��fbo->_2dcam

			//printf("�Ƚ�:%d\n",mat4x4_equal(a,b));
			//if(!mat4x4_equal(a,b,1)){
			//	mat4x4_printf("_2dCurCam",b);
			//	mat4x4_printf("fbocam",a);
			//	getchar();
			//}
		}
		ex_switch2dCam(fbo->_2dcam);


		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->fboName);
		glDrawBuffers(1, fboBuffs);

		{//���fbo��״̬
			GLenum state =	glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
			if(state != GL_FRAMEBUFFER_COMPLETE){
				printf("fbo state = %0x\n",state);
				return;
			}
		}

		glViewport(0, 0, fbo->texw, fbo->texh);
		glClearColor(1,0,1,1);//���Ƴ��Ϻ�ɫ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ǰ�Ļ�����


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		//LStack_ergodic_t(fbo->nodelist,f_renderList,0);
		ex_renderlistCall(ex_render3dNode);//��Ⱦ�ڵ�

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	//printf("%d,",fbo->wait);
	/*if(fbo->onceCallBack){
		callLater(f_callLater,fbo);
	}*/
}
//void 
//fbo_set_once(void* ptr,void (*onceCallBack)(void*,void*),void* parms){
//	struct FBOTexNode* fbo = (struct FBOTexNode*)ptr;
//	//fbo->once = 1;
//	fbo->onceCallBack= onceCallBack;
//	fbo->parms = parms;
//}
void* 
fbo_init(int texW,int texH){
	struct FBOTexNode* fbo = (struct FBOTexNode*)tl_malloc(sizeof(struct FBOTexNode));
	//printf("tl_malloc %0x\n",fbo);
	if(!fbo){
		printf("fbo ����ʧ�� %0x\n",fbo);
		return 0;
	}else{

		GLint mirrorTexWidth  = texW;
		GLint mirrorTexHeight = texH;

		GLuint              fboName;
		GLuint				mirrorTexture;//������ͼ
		GLuint              depthBufferName; //��Ȼ�����
		
		memset(fbo,0,sizeof(struct FBOTexNode));
		
		fbo->enable = 1;//Ĭ�ϴ��ڼ���״̬

		//fbo->nodelist = LStack_create();

		fbo->texw = mirrorTexWidth;
		fbo->texh = mirrorTexHeight;
		//printf("glGenFramebuffers\n");
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
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferName);//��Ȼ�����

		//printf("glFramebufferRenderbuffer\n");
		fbo->depthBufferName = depthBufferName;
		fbo->fboName = fboName;
		fbo->tex = mirrorTexture;
		//printf("****fbo %0x\n",fbo);


		// Make sure all went well
		//gltCheckErrors();
		
		//fbo->_2dspr=f_createSprite(mirrorTexture,texW,texH);
		//fbo->tex = mirrorTexture;
		// Reset framebuffer binding
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		//###################################
		fbo->_3dcam = cam_create();
		cam_setPerspect(fbo->_3dcam,45.0,fbo->texw / fbo->texh , 0.1, 10000);
		//printf("****cam_setPerspect %0x\n",fbo->_3dcam);
		fbo->_2dcam = cam_create();
		//printf("****_2dcam %0x\n",fbo->_2dcam);
		fbo_resize(fbo);
		//printf("****fbo_resize %0x\n",fbo);
	}
	return fbo;
}

void 
fbo_resize(void*p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	cam_setOrtho(fbo->_2dcam,fbo->texw,fbo->texh,-ex_getIns()->allzBuffer);
}

//����fbo����
void
fbo_dispose(void* p ,int deltex){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;

	if(deltex && fbo->tex){
		glDeleteTextures(1, &fbo->tex);
		fbo->tex = 0;
	}

	cam_dispose(fbo->_3dcam);

	// Cleanup RBOs
	glDeleteRenderbuffers(1, &fbo->depthBufferName);

	// Cleanup FBOs
	glDeleteFramebuffers(1, &fbo->fboName);

	tl_free(fbo);

	//{
	//	struct EX* p = ex_getIns();

	//	//�л���Ӧ��cam�ľ���ռ�
	//	ex_switch3dCam(p->_3dcam);
	//	ex_switch2dCam(p->_2dcam);
	//}
}

void* fbo_get3dCam(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return fbo->_3dcam;
}
void* fbo_get2dCam(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return fbo->_2dcam;
}
void* fbo_getTex(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return (void*)fbo->tex;
}

void
fbo_enable(void*p,int v){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	fbo->enable = v;
}