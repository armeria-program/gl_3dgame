#ifndef _RENDER_NODE_H_
#define _RENDER_NODE_H_

#include "tlgl.h"
#include "obj_vbo.h"
#include "anim.h"
#include "collide.h"


struct Node
{
	struct HeadInfo* base;
	struct Obj_vbo_model* ptrVBO;
	/*
	*	����������
	*/
	void* anim;	
	struct CollideBox* ptrCollide;//��ײ������
	void (*renderCallBack)(int,struct ObjVBO*);//��Ⱦ�ص�
};

struct Node* node_create(const char* name);
/*
 *����node
 */
void node_dispose(struct Node* p);

/*
	û�ж����ľͲ����øýӿ�,������������ʼ��
*/
void node_bindAnim(struct Node* p);
/*
	��Ⱦ
*/
void node_render(struct Node* p);
///*
//	��������
//*/
//void node_setPos(struct Node* p,float x,float y,float z);

#endif
