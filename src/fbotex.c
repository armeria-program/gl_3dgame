#include <gl/glew.h>
#include <gl/glut.h>
#include <assert.h>
#include <stdio.h>

#include "common.h"
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
	struct Sprite*		_2dspr;				//2dSprite,��fbo��2dSprite
	void* _3dcam;


	GLuint              fboName;			//FBO��������	
	GLuint				mirrorTexture;		//������ͼ
	GLuint              depthBufferName;	//��Ȼ�����
	int					texw,texh;			//��ͼ�Ŀ��
	

	// ��Ⱦ�ڵ�Ļص�
	void (*callBack)();

	//��Ⱦ�Ľڵ��б�
	void* nodelist;
};
static void
f_renderList(int data,int parms){
	ex_render3dNode(data);
}
void
fbo_render(void* ptr){
	struct FBOTexNode* fbo = (struct FBOTexNode*)ptr;
	const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->fboName);
	glDrawBuffers(1, fboBuffs);

	glViewport(0, 0, fbo->texw, fbo->texh);

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
		if( base_findByName(fbo->nodelist,b->name) != NULL){
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
//#define _TestTex_
void* 
fbo_init(int texW,int texH){
	struct FBOTexNode* fbo = (struct FBOTexNode*)tl_malloc(sizeof(struct FBOTexNode));
	
	GLint mirrorTexWidth  = texW;
	GLint mirrorTexHeight = texH;

	GLuint              fboName;
	//GLuint				textures[1];//����ʯ��ͼ
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
	fbo->mirrorTexture = mirrorTexture;

	// Make sure all went well
	//gltCheckErrors();

	
	//����2dSprite
	/*fbo->_2dspr = sprite_create("fbo1",0,0,256,256,0);
	*/
	{
		struct Sprite* spr = 0;
#ifdef _TestTex_
		GLuint tex =jgl_loadTex("//resource//texture//1.tga");
#endif
		void* mat= tmat_create_empty("_spritevbo");
		
		char buffer[64];
		tl_newName(buffer,64);
		//tmat_setTexFromGPU(mat,0,mirrorTexture);
		tmat_setID(mat,1);
		
#ifdef _TestTex_
		tmat_pushTex(mat,tex);//mirrorTexture
#else
		tmat_pushTex(mat,mirrorTexture);//mirrorTexture
#endif
		//printf("mirrorTexture:%d\n",mirrorTexture);
		//fbo->mat = mat;
		
		spr = sprite_create(buffer,0,0,texW,texH,0);
		sprite_rotateZ(spr,-PI/2);//sprite��ת90��
		sprite_rotateX(spr,PI);
		base_setv(spr,FLAGS_REVERSE_FACE);

		spr->material = mat;
		fbo->_2dspr=spr;

	}
	
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
	
	glDeleteTextures(1, &fbo->mirrorTexture);

	sprite_dipose(fbo->_2dspr);
	//glDeleteTextures(1, textures);
	cam_dispose(fbo->_3dcam);

	// Cleanup RBOs
	glDeleteRenderbuffers(1, &fbo->depthBufferName);

	// Cleanup FBOs
	glDeleteFramebuffers(1, &fbo->fboName);
}


void* fbo_get_spr(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return fbo->_2dspr;
}
void* fbo_get_cam(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return fbo->_3dcam;
}