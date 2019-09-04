#ifndef _FBO_TEX_
#define _FBO_TEX_

struct FBOTex
{
	GLuint              fboName;
	//GLuint				textures[1];	//����ʯ��ͼ
	GLuint				mirrorTexture;		//������ͼ
	GLuint              depthBufferName;	//��Ȼ�����
	int					texw,texh;			//��ͼ�Ŀ��
	//struct Sprite*		_2dspr;				//2dSprite,��fbo��2dSprite
	void*				mat;//���ʶ���
	
	// ��Ⱦ�ڵ�
	void (*callBack)();
};
void* fbo_init();

void fbo_render(void* ptr);

//����Ⱦ�ص�
void fbo_bind(void* ptr,void (*callBack)());

//����fbo����
void fbo_dispose(void* p);
#endif