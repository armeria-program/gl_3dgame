#ifndef _TIME_LATER_H_
#define _TIME_LATER_H_

void* timelater_new(int ms,void (*callBack)(void*),void* param);
//ms�����һ���ص�
//void  timelatar_push((*callBack)(void* timelater),void* param,int ms);

/*
 *���ټ�ʱ��
 */
void timelater_remove(void* timer);

/************************************************************************/
/* ���м�ʱ��                                                                     */
/************************************************************************/
void timelater_run();

#endif