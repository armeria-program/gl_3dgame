#ifndef _FBO_TEX_
#define _FBO_TEX_
//��ʼ��
void* fbo_init(int texW,int texH);

//��Ⱦ�ص�
void fbo_render(void* ptr);

//����Ⱦ�ص�
//void fbo_bind(void* ptr,void (*callBack)());

//����fbo����
void fbo_dispose(void* p);

//��ȡ��ǰ��3d cam
void* fbo_get3dCam(void* p);
//��ȡ2d cam
void* fbo_get2dCam(void* p);

//��ȡfbo������texture���
void* fbo_getTex(void* p);

//��fbo���ڵ�֡�����������ñ��ʱ�����
void 
fbo_resize(void*p);


/*
 *	������߹رյ�ǰ��FBO,��FBO�ر�״̬�µ�ʱ��,FBO�󶨵���Ⱦ�ڵ��ǲ���Ⱦ��
 *	0 �ر� 1 ����
 */
void
fbo_enable(void*p,int v);

#endif