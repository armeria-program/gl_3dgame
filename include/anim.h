#ifndef _ANIM_
#define _ANIM_

#include "animtor.h"

/************************************************************************
*	����������
*	�޹غ��ײ�����VBO����ֱ��ģʽ,ֻ�ǹ���������,fps
/************************************************************************/
struct VBOAnim
{
	int allLength;//�����ܳ���
	int _tempLength;
	int curIndex;//��ǰ��������

	int* nodeList;//int ���鴢��struct ObjVBO*

	/*
	 * ֻ��>0��ʱ������ж�����
	 * ÿһ֡�ļ��ʱ��
	 * fps = 60;
	 * fpsInterval = 1000 / fps = 16.6666;
	 */
	long fpsInterval;
	
	/*
	*	��ֵ
	*/
	long interpolation;

	/*
	*	����������
	*/
	struct Animtor* pAnimtor;
};


/*
	��VBOת��Ϊ�����б�
*/
struct VBOAnim* anim_create(struct LStackNode* list);
/*
	����
*/
void anim_dispose(struct VBOAnim* anim);

/*
	����ָ�������ŵĶ���
*/
void anim_play(struct VBOAnim* anim,int index,void (*callBack)(int,int),int param);

/*
	����FPSѡ�񲥷�����Ӧ��VBO�ڵ�
*/
void anim_playByFPS(struct VBOAnim* p,void (*callBack)(int,int),int param);

/*
	�Զ������ö�����fps,��ֵ������֮���������õ�֡�ʼ���ؼ�֡
*/
void anim_set_fps(struct VBOAnim* p,int fps);

/*
	��ȡ����������
*/
struct Animtor* anim_get_animtor(struct VBOAnim* p);
#endif