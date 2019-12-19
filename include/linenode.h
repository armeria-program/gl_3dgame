#ifndef _LINE_NODE_H_
#define _LINE_NODE_H_
/*
vertcnt ��������
int point 1:�ǵ�ģʽ 0:����ģʽ
���������ʱ����Ҫ��������Դcomplete���֮����ǰ�ȫ��
*/
void* linenode_create(const char* name,int vertcnt,void* cam,int point);

void linenode_push(void* p,float x,float y,float z);
void linenode_end(void* p);
void linenode_render(void* p);

//����
void linenode_dispose(void* p);

//Ϊlinenode�����߶ε���ɫ
void linenode_set_color(void* p,float r,float g,float b);
#endif