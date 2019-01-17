#ifndef _MD2PARSE_H_
#define _MD2PARSE_H_
/*
*	�ؼ�֡�ṹ��,����һ��md2�ؼ�֡����
*/
struct MD2_Frame{
	/*
	*	�ؼ�֡��
	*/
	char _name[G_BUFFER_16_SIZE];
	/*
	*	�ؼ�֡������
	*/
	int _index;
	/*
	*	��������
	*/
	int* _indices;
	//#####################################################
	/*
	*	���㶨���б�
	*/
	float* vertices;
	/*
	*	��������
	*/
	int vertCount;
};
/*
	����,����һ��md2���
*/
void* md2parse_load(const char* str,int len);
/*
	����
*/
void md2parse_dispose(void* md2);
/**
*	md2���ܵĹؼ�֡����
*/
int md2parse_totalFrames(void* md2);
/**
*	��ȡָ��������struct MD2_Frame*����,index��0��ʼ
*/
void* md2parse_getFrame(void* md2,int index);
/*
	��ȡ���������,��������
*/
//void md2parse_getFrameVertex(void* md2Frame,float* pVertices,int* pVertCnt);
#endif