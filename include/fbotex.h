#ifndef _FBO_TEX_
#define _FBO_TEX_
//��ʼ��
void* fbo_init(int texW,int texH);

//��Ⱦ�ص�
void fbo_render(void* ptr);

//����Ⱦ�ص�
void fbo_bind(void* ptr,void (*callBack)());

//����fbo����
void fbo_dispose(void* p);

//��ȡ��ǰ��cam
void* fbo_getCam(void* p);

//��ȡfbo������texture���
void* fbo_getTex(void* p);

//���һ������Ⱦ�Ľڵ�
void fbo_pushNode(void* p,void* node);

#endif