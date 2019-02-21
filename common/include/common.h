#ifndef _COMMON_H_
#define _COMMON_H_

#include "def.h"

//DLLIMPORT 
//int x_md2load(const char* path);

/*
 *	�ж��Ƿ�����ļ�file,���ڷ���1 �����ڷ���0
 *	19:35 2017/7/8
 */
DLLIMPORT 
int tl_is_file_exist(const char* file);
/*
	������buffer ����д��filePath��
	-------------------------------------------------
	wb ֻд�򿪻��½�һ���������ļ���ֻ����д���ݡ� 
	wb+ ��д�򿪻���һ���������ļ����������д�� 
	wt+ ��д�򿪻��Ž���һ���ı��ļ��������д�� 
	at+ ��д��һ���ı��ļ�������������ı�ĩ׷�����ݡ� 
	ab+ ��д��һ���������ļ�������������ļ�ĩ׷�����ݡ� 
*/
DLLIMPORT 
void  tl_writeAppandFile(const char* filePath,char* buffer);
/*
	����һ��������
*/
DLLIMPORT
void tl_newName(char* buffer,int bufferSize);

//DLLIMPORT int dllGlobalVar;

/*
	����ڳ����·��
 */
DLLIMPORT char* g_local_path[256];

/*·��ת��*/
DLLIMPORT int tl_convert_path(char* in,char* out,int outBufferSize);


#endif