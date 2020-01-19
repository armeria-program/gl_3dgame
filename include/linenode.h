#ifndef _LINE_NODE_H_
#define _LINE_NODE_H_
/*
vertcnt ��������
int point 1:�ǵ�ģʽ 0:����ģʽ
���������ʱ����Ҫ��������Դcomplete���֮����ǰ�ȫ��
*/
void* linenode_create(const char* name,int vertcnt,void* cam,int point);
/*
	Ϊ�߶�����һ����
*/
void linenode_push(void* p,float x,float y,float z);
/*
	linenode_push ѹ�붥�����������ʱ�����
*/
void linenode_end(void* p);

/*
	��Ⱦ
*/
void linenode_render(void* p);

/*
	����
*/
void linenode_dispose(void* p);

/*
	Ϊlinenode�����߶ε���ɫ
*/
void linenode_set_color(void* p,float r,float g,float b);

/*
	�޸Ķ���������ϵĶ�����������,������0��ʼ����
*/
void linenode_mod(void* p,int index,float x,float y,float z);
#endif  