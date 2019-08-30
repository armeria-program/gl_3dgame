#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vmath.h"

void* cam_create();

void cam_dispose(void* cam);

//��ȡcamera������������
void* cam_getPerctive(void* cam);
//��ȡģ����ͼ����
void* cam_getModel(void* cam);

//void cam_setX(void* cam,float x);
//void cam_setY(void* cam,float y);
//void cam_setZ(void* cam,float z);

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