#ifndef _FTEXT_H_
#define _FTEXT_H_

/*
 *����һ���ı�
 */
void*
ftext_create(char* txtName,int fw,int fh);
/************************************************************************/
/* ��ָ����������һ���ַ�����									        */
/************************************************************************/
void 
ftext_set(void* p,char* s,int x,int y,int* pw,int* ph);

/*
 *	�����ı�����Ļ����
 */
void 
ftext_setpos(void* p,int x,int y);
#endif