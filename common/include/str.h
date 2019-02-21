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

/*ɾ��str�е�ch�ַ�*/
DLLIMPORT 
void str_delchar(char *str,char ch);


/*
*	str.h
*	�ַ�������ӿ�
*/
/*�����ַ����״γ��ֵ�λ�ã�û���ҵ����� -1�������ַ�����ȷ��� 0*/
DLLIMPORT 
int str_pos(const char *haystack,const char *needle,int ignorecase);

 //int (*pCallBack)(struct StrCutSplit*)
/*
	����:

	
	//	int* inputParms	����Ĳ�������,inputParms������Ϊ�����ַ�������ҵ���߼�����
	//	char* splitStr	���и���ַ���Ƭ��
	
	static void f_obj_animsSplit2(int* inputParms,char* splitStr){
		parseOneLine(inputParms,splitStr);
	}

	T t;
	tl_splitByStr((const char*)data,OBJ_SIGN_CHAR_BR,f_obj_animsSplit2,&t);

*/
DLLIMPORT 
void str_split(const char* dest,const char sign,void (*pCallBack)(int*,char*),int* parms);


/*
	�ַ����и�,��tl_split_s���ܸ���,��ʱ�ַ���ʹ�õ�ջ�ռ��е�����
	ʹ�÷���:


	int animsSplitExample(char* point)
	{
		struct StrCutSplit* p = (struct StrCutSplit*)point;
		char* str = p->str;
		if(!strcmp(str,(char*)p->parms))
		{
			//�ҵ��ַ���
			printf("�ҵ��ַ���{%s}\n",str);
		}
		return 1;
	}
	tl_split_cut("run,walk,dead,",',',animsSplitExample,(int*)"walk");//Ѱ���и��ַ������Ƿ���"walk"�ֶ�

	
	void (*pCallBack)(char*,void*),
	char* �ָ�����ַ���
	void* ���ݵĲ�������
*/
DLLIMPORT 
void str_split_cut(
				  const char* dest,const char sign,
				  void (*pCallBack)(char*,void*),
				  void* parms);

#endif