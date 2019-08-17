#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "tl_malloc.h"
#include "evt.h"
#include "ftext.h"

#include "input.h"
#include "ex.h"


void* gcur;
//****************************************************************//

typedef struct Input
{
	//0xcdcdcdcd
	/*�¼�����*/
	//void* evtList;

	//�Ƿ��ڽ�����
	int force;

	//ftext���
	void* t;

	//�ַ��������б�,���ڴ洢��Ҫɾ�����ı���Ϣ
	// 
}Input;

//�����¼�
static void
f_key2(int evtId,void* data){
	struct E_KeyBoard* pkey = (struct E_KeyBoard*)data;
	void* txt = gcur;
	char name[64];
	//printf("input key = %d\n",pkey->key);		//,sizeof(unsigned short)
	if(pkey->key == 8){
		//�˸�
		ftext_pop_word(txt);
		//ftext_clear(txt);
		return;		
	}



	memset(name,0,64);
	sprintf_s(name,64,"%c",pkey->key);

	ftext_parse(txt,name,0,0);
}

void* input_create(){
	struct Input* ptr=(struct Input*)tl_malloc(sizeof(struct Input));
	//�����¼��������
//	ptr->evtList = (void*)LStack_create();
	char name[64];
	memset(name,0,64);
	sprintf_s(name,64,"input%d",newid());
	//printf("%s\n",name);

	evt_on(ex_getInstance(),EVENT_ENGINE_KEYBOARD,f_key2);
	//printf("*\n");
	
	ptr->t = ftext_create(name,32,32,13,12);
	ftext_open_wordlist(ptr->t);
	gcur = ptr->t;
	return ptr;
}

void input_dispose(void* p){
	struct Input* ptr=(struct Input*)p;

	ftext_dispose(ptr->t);
	ptr->t = 0;

//	tl_free(ptr->buffer);
//	ptr->buffer = 0;

	tl_free(p);
}