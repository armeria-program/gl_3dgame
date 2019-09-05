#ifndef _FBO_TEX_
#define _FBO_TEX_

void* fbo_init(int texW,int texH);

void fbo_render(void* ptr);

//����Ⱦ�ص�
void fbo_bind(void* ptr,void (*callBack)());

//����fbo����
void fbo_dispose(void* p);

void* fbo_get_spr(void* p);
void* fbo_get_cam(void* p);

#endif