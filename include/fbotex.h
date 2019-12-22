#ifndef _FBO_TEX_
#define _FBO_TEX_
#include <gl/glew.h>
struct FboInfo{
	void* cam2d;
	void* cam3d;
	GLuint tex;
};
//��ȡfbo����Ϣ
void fbo_info(void* ptr,struct FboInfo* info);
//��ʼ��
void* fbo_init(int texW,int texH);

//��Ⱦ�ص�
void fbo_render(void* ptr);

//����fbo����
// deltex 1 ����������� 0 �������������
void fbo_dispose(void* p,int deltex);

////��fbo���ڵ�֡�����������ñ��ʱ�����
//void fbo_resize(void*p);

/*
 *	������߹رյ�ǰ��FBO,��FBO�ر�״̬�µ�ʱ��,FBO�󶨵���Ⱦ�ڵ��ǲ���Ⱦ��
 *	0 �ر� 1 ����
 */
void
fbo_enable(void*p,int v);
//����fboֻ��Ⱦһ��,���Ҳ������������
//void 
//fbo_set_once(void* ptr);
//void fbo_set_once(void* ptr,void (*onceCallBack)(void*,void*),void* parms);
#endif