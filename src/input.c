#include <stdio.h>

#include "tl_malloc.h"
#include "evt.h"
#include "ex.h"
#include "input.h"

typedef struct Input
{
	//0xcdcdcdcd
	/*�¼�����*/
	//void* evtList;

	//�Ƿ��ڽ�����
	int force;
}Input;

//�����¼�
static void
f_key2(int evtId,void* data){
	struct E_KeyBoard* pkey = (struct E_KeyBoard*)data;

	
	printf("input key = %d\n",pkey->key);
	
	
	//switch(pkey->key){
	//}
}

void* input_create(){
	struct Input* ptr=(struct Input*)tl_malloc(sizeof(struct Input));
	//�����¼��������
//	ptr->evtList = (void*)LStack_create();

	evt_on(ex_getInstance(),EVENT_ENGINE_KEYBOARD,f_key2);
	//printf("*\n");

	return ptr;
}