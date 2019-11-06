#include <memory.h>
#include "tools.h"
#include "tree.h"

struct TreeNode
{
	void* data;//�洢һ�����ݽڵ�
	void* childlist;//treeNode list
};

void* tree_new(){
	struct TreeNode* p = (struct TreeNode*)tl_malloc(sizeof(struct TreeNode));
	memset(p,0,sizeof(struct TreeNode));
	return p;
}

//�������Ľڵ�����
void tree_set_data(void* t,void* data){
	struct TreeNode* p = (struct TreeNode*)t;
	p->data = data;
}
//��ȡ���Ľڵ�����
void* tree_get_data(void* t){
	struct TreeNode* p = (struct TreeNode*)t;
	return p->data;
}
//Ϊ���ڵ���������ڵ�
void tree_addchild(void* t,void* data){
	struct TreeNode* p = (struct TreeNode*)t;
	if(!p->childlist){
		p->childlist = (void*)LStack_create();
	}
	{
		void* tree = tree_new();
		struct TreeNode* ptree = (struct TreeNode*)tree;
		ptree->data = data;
		LStack_push(p->childlist,tree);
	}
}

//Ϊ���ڵ��Ƴ�һ���ӽڵ�
int tree_removeChild(void* t,void* node){
	struct TreeNode* p = (struct TreeNode*)t;
	if(!p->childlist){
		return 0;
	}
	if(LStack_delNode((struct LStackNode*)p->childlist,(int)node)){
		tree_dispose(node);
		return 1;
	}
	//ɾ��ʧ��
	return 0;
}

//�������ڵ�
void tree_foreach(void* t,void (*callBack)(void*,void*),void* param){//treenode*,param*
	struct TreeNode* p = (struct TreeNode*)t;
	callBack(p,param);
	{
		struct LStackNode* s = (struct LStackNode*)p->childlist;
		if(!s){
			void* top,*p;
			top = s;
			p=top;
			while((int)LStack_next(p)){
				int data;
				p=(void*)LStack_next(p);
				data = LStack_data(p);
				tree_foreach((void*)data,callBack,param);
			}
		}
	}
}
static void
f_del(void* n,void* p){
	tree_dispose(n);
}
void tree_dispose(void* p){
	tree_foreach(p,f_del,0);
}