#ifndef _STR_H_
#define _STR_H_

/*
����һ���ַ���
*/
void* str_create(const char* s);

/*
����
*/
void str_dispose(void* _str);
/*
���ַ���β��������ַ���
*/
void str_appand(void* _str,const char * s);
/*
���ַ���β��������ַ���,ָ����ȡ�ַ����ĳ���Ϊlen
*/
void str_appand_s(void* _str,const char * s,int len);

/*
�ַ�������
*/
int str_length(void* str);
/*
�ַ����滻
pSrc			-- Ҫ���滻�����ַ���, ���� <br>
pDst			-- Ҫ�滻�ɵ��ַ���, ����   /n
ע��:���ϵ��ַ�����Ҫ��'\0'��β.
*/
void str_replace(void* str,char *pSrc, char *pDst);

/*
	��dest�е��ַ�����copy��target��
*/
void str_copy(void* target,struct Str* dest);
/*
	��ȡ��ǰ���ַ���
*/
char* str_get(void* ptr);
#endif