#ifndef _FTEXT_H_
#define _FTEXT_H_

/*
 *����һ���ı�
 */
void*
ftext_create(char* txtName,int texw,int texh,int fw,int fh);
/************************************************************************/
/* ��ָ����������һ���ַ�����(һ�����ֻ�����һ��Ӣ��)
/************************************************************************/
void 
ftext_set(void* p,char* s,int x,int y,int* pw,int* ph);

/*
 *	�����ı�����Ļ����
 */
void 
ftext_setpos(void* p,int x,int y);

/*
 *	��str�ַ���ת������ͼ����չʾ����
 */
void
ftext_parse(void* p,const char* str);
#endif