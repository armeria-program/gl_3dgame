#ifndef _TWEEN_H_
#define _TWEEN_H_

/*
	����
*/
void* 
tween_to(int time,void (*callBack)(),int cnt,...);


//���˷���������Ⱦѭ����
void
tween_run(long _longTime);
#endif