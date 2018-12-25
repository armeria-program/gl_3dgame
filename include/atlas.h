#ifndef _ATALS_
#define _ATALS_
#include "tools.h"
struct Atals
{
	//ͼ����
	char name[G_BUFFER_64_SIZE];
	
	//ͼ�����
	float width,height;

	//����
	void* material;

	//����
	struct XMLSList* xml;
};
/************************************************************************/
/* һ��ͼ���ṹ��,����,���,��һ��λͼ��ȡһ���������
/************************************************************************/
struct AtlasNode
{
	float x,y,width,height;
};
/*
	����ͼ��
*/
struct Atals* atals_load(const char* path,const char* name);

/*
	����ͼ��
*/
void atals_dispose(struct Atals* ptr);

/*
	��������
*/
void atals_tex(struct Atals* atals,const char* name,struct AtlasNode* ptrOut);


#endif