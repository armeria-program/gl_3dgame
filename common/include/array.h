#ifndef _ARR_
#define _ARR_

#include "def.h"

#define  ARRAY_HEAP 1	//���ڴ���Ҫtl_free�ͷ�
#define  ARRAY_STACK 0 //ջ�ڴ�,�Զ��ͷ�

/*
	�ɱ߳�����
*/
typedef struct Array{
	int* ptr;
	int length;	//���鳤��
	int cur;	//��ǰ�α�
	int mode;	//����ģʽ
}Array;
/*
	���鴴��
*/
DLLIMPORT struct Array* array_create(int length,int mode);

/*
	�ͷ�����
*/
DLLIMPORT void array_free(struct Array* ptr);
/*
	�洢���ݾ�
*/
DLLIMPORT void array_put(struct Array* ptr,void* d);
/*
	���鳤��
*/
DLLIMPORT int array_len(struct Array* ptr);
/*
	��ȡ�����ҵ����ݷ���1,û�����ݾ�0
*/
DLLIMPORT int array_get(struct Array* ptr,int index,int* outData);
/*
	��������
*/
DLLIMPORT int array_set(struct Array* p,int index,int data);
/*
	��ӡ��������
*/
//DLLIMPORT void array_log(struct Array* p);

/*
	����key�и�ɶ���ַ��ļ�,������������,�ַ����и���ַ�������
*/
DLLIMPORT struct Array* array_split(const char* str,char key);

#endif