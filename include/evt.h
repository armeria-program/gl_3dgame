#ifndef _EVT_H_
#define _EVT_H_

//�¼��ṹ��
struct EvtInfo{
	int evt;
	int ptr;
};

//���¼�
void
evt_on(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*));

//����¼�
void
evt_off(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*));
#endif