#include "tools.h"
#include "evt.h"
//�¼��ṹ��
struct EvtInfo{
	/*
	 *  �¼� id
	 */
	int evtId;
	/*
	 *	�ص���������
	 */
	void (*ptr)(int evtId,void* data);
};
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
evt_on(void* evtList,int evtId,void (*evtCallBack)(int,void*)){
	struct EvtInfo* data;
	struct LStackNode* list;
	if(!f_check_evt(evtList,evtId,evtCallBack)){
		return;
	}
	data = (struct EvtInfo*)tl_malloc(sizeof(struct EvtInfo));
	data->evtId = evtId;
	data->ptr = evtCallBack;
	list =(struct LStackNode*)evtList;
	LStack_push(list,data);
}

//����¼�
void
evt_off(void* evtList,int event,void (*evtCallBack)(int,void*)){
	struct LStackNode* s = (struct LStackNode* )evtList;
	struct EvtInfo* node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;

		p=(void*)LStack_next(p);
		data = LStack_data(p);

		node = (struct EvtInfo*)data;

		
		if(node->evtId == event && node->ptr == evtCallBack){
			node->evtId = 0;
			node->ptr = 0;
			LStack_delNode(s,data);
			tl_free(node);
		}
	}

}

void
evt_dispatch(void* evtList,int evtID,void* sendData){
	struct LStackNode* s = (struct LStackNode* )evtList;
	struct EvtInfo* node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;

		p=(void*)LStack_next(p);
		data = LStack_data(p);
		
		node = (struct EvtInfo*)data;

		if(node->evtId == evtID){
			node->ptr(evtID,sendData);
		}
	}

}

void 
evt_dispose(void* evtList){
	struct LStackNode* s = (struct LStackNode* )evtList;
	struct EvtInfo* node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;

		p=(void*)LStack_next(p);
		data = LStack_data(p);

		node = (struct EvtInfo*)data;

		node->evtId = 0;
		node->ptr = 0;
		LStack_delNode(s,data);
		tl_free(node);
	}
	LStack_delete(s);
}