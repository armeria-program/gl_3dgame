#ifndef _EVT_H_
#define _EVT_H_

//���¼�
void
evt_on(void* evtList,int id,void (*evtCallBack)(int,void*));

//����¼�
void
evt_off(void* evtList,int id,void (*evtCallBack)(int,void*));

//�ɷ��¼�
void
evt_dispatch(void* evtList,int evtID,void* sendData);

//�����¼����
void 
evt_dispose(void* evtList);
#endif