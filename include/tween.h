#ifndef _TWEEN_H_
#define _TWEEN_H_

/*
	����
*/
void* 
tween_to(void* obj,int time,
		 void (*endCallBack)(void*),
		 void (*updateCallBack)(void*),
		 int cnt,...);

/*
	�Ƿ��ڲ���(1:�ڲ���,0:û�в���)
*/
int 
tween_is_play(void* ptr);
/*
	ֹͣtween
*/
void
tween_stop(void* ptr);
//���˷���������Ⱦѭ����
void
tween_run(long _longTime);
#endif