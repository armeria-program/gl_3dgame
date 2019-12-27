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

struct FBOTexNode{
	void* _3dcam;			//3d͸��camera
	void* _2dcam;			//2d͸��camera
	GLuint              fboName;			//FBO��������	
	GLuint				tex;				//��ͼ������,fbo��Ⱦ��֡��������������������
	GLuint              depthBufferName;	//��Ȼ�����
	int					texw,texh;			//��ͼ�Ŀ��
	GLbyte enable;								//�Ƿ��ڼ���״̬
};

static void fbo_resize(void*p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	cam_setOrtho(fbo->_2dcam,fbo->texw,fbo->texh,-ex_getIns()->allzBuffer);
}

void
fbo_render(void* ptr){
	struct FBOTexNode* fbo = (struct FBOTexNode*)ptr;

	if(fbo->enable){
		const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

		ex_switch3dCam(fbo->_3dcam);
		ex_switch2dCam(fbo->_2dcam);

		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, fbo->fboName);//glBindFramebuffer
		glDrawBuffers(1, fboBuffs);

		{//���fbo��״̬
			GLenum state =	glCheckFramebufferStatusEXT(GL_DRAW_FRAMEBUFFER);//glCheckFramebufferStatus
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
		//ex_renderlistCall(ex_render3dNode);//��Ⱦ�ڵ�
		ex_renderAll3dNode();


		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, 0);//glBindFramebuffer
	}
}

void* 
fbo_init(int texW,int texH){
	struct FBOTexNode* fbo = (struct FBOTexNode*)tl_malloc(sizeof(struct FBOTexNode));
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

		fbo->texw = mirrorTexWidth;
		fbo->texh = mirrorTexHeight;
		//printf("glGenFramebuffers\n");
		// Create and bind an FBO(����,��֡�������fbo)
		glGenFramebuffersEXT(1,&fboName);//glGenFramebuffers
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, fboName);//glBindFramebuffer

		// Create depth renderbuffer(������Ȼ���)
		glGenRenderbuffersEXT(1, &depthBufferName);//glGenRenderbuffers
		glBindRenderbufferEXT(GL_RENDERBUFFER, depthBufferName);//glBindRenderbuffer
		glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, mirrorTexWidth, mirrorTexHeight);//glRenderbufferStorage

		// Create the reflection texture(����ӳ����ͼ)
		glGenTextures(1, &mirrorTexture);
		glBindTexture(GL_TEXTURE_2D, mirrorTexture);
		//���ߵ�bug
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mirrorTexWidth, mirrorTexHeight, 0, GL_RGBA, GL_FLOAT, 0);
	  
		// Attach texture to first color attachment and the depth RBO(��texture�󶨵�RBO)
		glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture, 0);//glFramebufferTexture2D
		glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferName);//��Ȼ�����	glFramebufferRenderbuffer

		//printf("glFramebufferRenderbuffer\n");
		fbo->depthBufferName = depthBufferName;
		fbo->fboName = fboName;
		fbo->tex = mirrorTexture;

		// Make sure all went well
		//gltCheckErrors();
		
		// Reset framebuffer binding
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, 0);//glBindFramebuffer

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
	glDeleteRenderbuffersEXT(1, &fbo->depthBufferName);//glDeleteRenderbuffers

	// Cleanup FBOs
	glDeleteFramebuffersEXT(1, &fbo->fboName);//glDeleteFramebuffers

	tl_free(fbo);
}

void
fbo_enable(void*p,int v){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	fbo->enable = v;
}

void fbo_info(void* ptr,struct FboInfo* info){
	struct FBOTexNode* fbo = (struct FBOTexNode*)ptr;
	info->cam2d = fbo->_2dcam;
	info->cam3d = fbo->_3dcam;
	info->tex = fbo->tex;
}