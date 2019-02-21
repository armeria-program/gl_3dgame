#ifndef _STR_H_
#define _STR_H_
#include "def.h"
/*
����һ���ַ���
*/
DLLIMPORT void* str_create(const char* s);

/*
����
*/
DLLIMPORT void str_dispose(void* _str);
/*
���ַ���β��������ַ���
*/
DLLIMPORT void str_appand(void* _str,const char * s);
/*
���ַ���β��������ַ���,ָ����ȡ�ַ����ĳ���Ϊlen
*/
DLLIMPORT void str_appand_s(void* _str,const char * s,int len);

/*
�ַ�������
*/
DLLIMPORT int str_length(void* str);
/*
�ַ����滻
pSrc			-- Ҫ���滻�����ַ���, ���� <br>
pDst			-- Ҫ�滻�ɵ��ַ���, ����   /n
ע��:���ϵ��ַ�����Ҫ��'\0'��β.
*/
DLLIMPORT void str_replace(void* str,char *pSrc, char *pDst);

/*
	��dest�е��ַ�����copy��target��
*/
DLLIMPORT void str_copy(void* target,struct Str* dest);
/*
	��ȡ��ǰ���ַ���
*/
DLLIMPORT char* str_get(void* ptr);
#endif