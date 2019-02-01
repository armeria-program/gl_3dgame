#ifndef _ANIMTOR_H_
#define _ANIMTOR_H_
#include "tools.h"

/*
	����Ƭ���и���
*/
struct Animtor
{
	struct LStackNode* list;

	//��ǰ�Ķ���
	char cur_anim[G_BUFFER_64_SIZE];
	int cur_start;
	int cur_end;
	//�Ƿ��ڲ���
	int isPlaying;
};
struct Animtor* animtor_create();

void animtor_dispose(struct Animtor* p);
/*
	��һ������
*/
void animtor_push(struct Animtor* p,const char* animKey,const int start,const int end);

/*
	���õ�ǰ�Ķ���
*/
int animtor_setcur(struct Animtor* p,char* const animKey);

/*
	����ؼ�֡
*/
void animtor_calculateFrame(struct Animtor* p,int* const pCurIndex);

/*
	����
*/
void animtor_play(struct Animtor* p);
/*
	��ͣ
*/
void animtor_pause(struct Animtor* p);

/*
	�Ƿ��ڲ���
*/
int animtor_isPlaying(struct Animtor* p);
/*
	ָ�������������Ŷ���
*/
void animtor_play_start_end(struct Animtor* p,const int s,int e);
#endif