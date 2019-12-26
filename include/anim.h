#ifndef _ANIM_
#define _ANIM_

#include "animtor.h"

/*
	��VBOת��Ϊ�����б�
*/
void* anim_create(struct LStackNode* list);
/*
	����
*/
void anim_dispose(void* anim);

/*
	����ָ�������ŵĶ���
*/
void anim_play(void* anim,int index,void (*callBack)(int,int),int param);

/*
	����FPSѡ�񲥷�����Ӧ��VBO�ڵ�
*/
void anim_playByFPS(void* p,void (*callBack)(int,int),int param);

/*
	�Զ������ö�����fps,��ֵ������֮���������õ�֡�ʼ���ؼ�֡
*/
void anim_set_fps(void* p,int fps);

/*
	��ȡ����������
*/
struct Animtor* anim_get_animtor(void* p);
/*
	��ȡ�ؼ�֡��������
*/
int
anim_total(void* p);

/*
	��ȡ��ǰ�Ĺؼ�֡������ 0��ʼ
*/
int
anim_curFrame(void* p);
#endif