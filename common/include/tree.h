#ifndef _TREE_H_
#define _TREE_H_

//����һ����
void* tree_new();

//Ϊ��ǰ�����ڵ���������
void tree_set_data(void* t,void* data);

//��ȡ���Ľڵ�����
void* tree_get_data(void* t);

//Ϊ���ڵ���������ڵ�,��data��ֵ���������ڵ��data�ֶ�
void tree_addchild(void* t,void* data);

//Ϊ���ڵ��Ƴ�һ���ӽڵ�
int tree_removeChild(void* t,void* node);

//�������ڵ�
void tree_foreach(void* t,void (*callBack)(void*,void*),void* param);//treenode*,param*
//������
void tree_dispose(void* p);
#endif
