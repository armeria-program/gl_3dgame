#ifndef _TWEEN_H_
#define _TWEEN_H_
#include "def.h"
/*
	����
*/
DLLIMPORT void* 
tween_to(void* obj,int time,
		 void (*endCallBack)(void*),
		 void (*updateCallBack)(void*),
		 int cnt,...);

/*
	�Ƿ��ڲ���(1:�ڲ���,0:û�в���)
*/
DLLIMPORT int 
tween_is_play(void* ptr);
/*
	ֹͣtween
*/
DLLIMPORT void
tween_stop(void* ptr);
//���˷���������Ⱦѭ����
DLLIMPORT void
tween_run(long _longTime,long delayTime);
#endif