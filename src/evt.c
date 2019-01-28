#include "tools.h"
#include "evt.h"
/************************************************************************/
/* ����Ƿ��Ѿ����˺���                                               */
/************************************************************************/
static int 
f_check_evt(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*)){

	struct LStackNode* s = (struct LStackNode* )evtList;
	struct EvtInfo* node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;

		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//if(!callBack(data,parm)){
		//	//callBack����0��ʱ���жϱ���
		//	return;
		//}

		node = (struct EvtInfo*)data;

		//if(data == (int)evtCallBack){
		if(node->evtId == event && node->ptr == evtCallBack){
			//��ע��ú���
			return 0;
		}
	}

	return 1;
}
//���¼�
void
evt_on(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*)){
	if(!f_check_evt(evtList,event,evtCallBack)){
		return;
	}

}

//����¼�
void
evt_off(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*)){

}