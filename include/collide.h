#ifndef _COLLIDE_H_
#define _COLLIDE_H_
#include "tlgl.h"

/*
 *����ʰȡ��ײ��
 **/
struct CollideBox
{
	struct LStackNode* list;
	int index;//��ǰ������
};
/*
 *����һ����ײ����
 *int frameCount:0��ʱ��������еĹؼ�֡��Ϊ��ײ����,2�������2���ؼ�֡��Ϊ��ײ��
 */
struct CollideBox* collide_load(const char* url,int frameCount);
/*
 *������ײ��
 */
void collide_dispose(struct CollideBox* ptr);
/*
 *��ȡ������Ӧ������,index��0��ʼ
 */
struct VertexData*  collide_getIndex(struct CollideBox* ptr,int index);
/*
 *��ǰ��VertexData
 */
struct VertexData* collide_cur(struct CollideBox* ptr);

#endif