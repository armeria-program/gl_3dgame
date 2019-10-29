#ifndef _TL_MALLOC_H_
#define _TL_MALLOC_H_
/************************************************************************/
/* �ڴ����                                                             */
/************************************************************************/

#include "def.h"
/*
	�����ڴ�
*/
//DLLIMPORT void* memory_new(int size);
/*
	�ͷ��ڴ�,ֻ���ͷ�����,����ϵͳ�ڴ�鲢û�л���.
*/
//DLLIMPORT void memory_free(void* p);

/*
	ֱ��ϵͳ����,ʹ�õ�free���ж��ڴ����,���������ϵĻ���,
	�������ܲ���memory_free�ڴ�ػ������ĺ�
*/
//DLLIMPORT void memory_retrieve(void* p);
/*
	��ȡ��Ϣ
*/
DLLIMPORT void memory_get_info(int* pDisable_bytes,int* pDisable_cnt,int* pg_total);

/*
	Garbage Collection	��������
*/
DLLIMPORT void memory_gc();

/************************************************************************/
/* �ܹ���ϵͳ��������ڴ��С(�ֽ�)                                                                     */
/************************************************************************/
//int memory_get_total();

/*����һ���ڴ�*/
DLLIMPORT void* tl_malloc(int size);

/*�ͷ�һ���ڴ�*/
DLLIMPORT void tl_free(void* s);

#endif