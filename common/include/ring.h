#ifndef _RING_H_
#define _RING_H_
#include "def.h"
//������
DLLIMPORT void* ring_create();
//ѹ������
DLLIMPORT void ring_push(void* ptr,void* node);
//��һ������
DLLIMPORT void* ring_next(void* ptr);
//ǰһ������
DLLIMPORT void* ring_pre(void* ptr);
//���ٻ�
DLLIMPORT void ring_dispose(void* ptr);
//��ȡ��ǰ�Ļ�ѡ�������
DLLIMPORT void* ring_cur(void* ptr);
#endif