#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vmath.h"

void* cam_create();

void cam_dispose(void* cam);

//��ȡcamera������������
void* cam_getPerctive(void* cam);
//��ȡģ����ͼ����
void* cam_getModel(void* cam);
//��ȡcam��Ӧ��֡�������Ŀ��
void cam_get_wh(void *p,float *w,float *h);

void cam_setX(void* cam,float x);
void cam_setY(void* cam,float y);
void cam_setZ(void* cam,float z);
void cam_setRX(void* cam,float rx);
void cam_setRY(void* cam,float ry);
void cam_setRZ(void* cam,float rz);
float cam_getX(void *p);
float cam_getY(void *p);
float cam_getZ(void *p);
float cam_getRX(void *p);
float cam_getRY(void* p);
float cam_getRZ(void* p);
void cam_offsetX(void* p,float v);
void cam_offsetY(void* p,float v);
void cam_offsetZ(void* p,float v);

//���ڻ�ȡ��ǰ��cam����ռ���Ⱦ������,�������֡�������������Ͻǵ�x,yƫ��
void cam_set_2dxy(void* p,int x,int y);
void cam_get_2dxy(void* p,int* px,int* py);

//ˢ��cam��ģ����ͼ����,��������x,y,z,rx,ry,rz�����
void cam_refreshModel(void *p);

//����camera���������ת
void cam_reset(void* p);

//void cam_refreshPerspect(void* cam);
/************************************************************************/
/* ������ξ���                                                          
/************************************************************************/
void cam_setPerspect(void*p,float fov, float aspectRatio,
					  float zNear, float zFar);

/************************************************************************/
/* ����ģ����ͼ������                                                                     */
/************************************************************************/
void cam_setModePosition(void* p,float x,float y,float z,
						 float rx,float ry,float rz);

/************************************************************************/
/* ������������                                                                     */
/************************************************************************/
void cam_setOrtho(void* p,float sw,float sh,float zbuffer);

#endif