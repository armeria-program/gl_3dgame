#ifndef _TEXT_H_
#define _TEXT_H_

struct TextField
{
	void* base;

	//�ı�����
	char* textPtr;

	//�ı���Ļ����
	int x,y;

	//�ı���ɫ
	float r,g,b;

	//�ı��ĸ߶�
	int height;
	
	//��������С
	int bufferSize;
	
	//�ı��ܳ���
	int textWidth;
};

struct TextField* tf_create(int bufferSize,int x,int y,float r,float g,float b);
void tf_dispose(struct TextField* tf);
void tf_setPos(struct TextField* tf,int x,int y);
void tf_setText(struct TextField* tf,const char* str);
/*
 *	��ȡ��ǰ�ı��Ŀ��
 */
int tf_getWidth(struct TextField* tf);
void tf_copy(struct TextField *tf_source,struct TextField *tf_dest);
void tf_render(int data);
/*
	��ȡ�ı�
*/
//void tf_getStr(struct TextField* tf,char* const buffer,const int bufferSize);
#endif