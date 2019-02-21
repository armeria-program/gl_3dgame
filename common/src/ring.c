#include <memory.h>
#include "tl_malloc.h"
#include "tools.h"
#include "ring.h"

typedef struct 
{
	void* list;
	int index;
}Ring;

//������
void* ring_create(){
	Ring* r = (Ring*)tl_malloc(sizeof(Ring));
	memset(r,0,sizeof(Ring));
	r->list = LStack_create();
	return (void*)r;
}
void* 
ring_cur(void* ptr){
	Ring * r = (Ring*)ptr;
	int out;
	LStatk_getNodeByIndex(r->list,r->index,&out);
	return (void*)out;
}

//ѹ������
void ring_push(void* ptr,void* node){
	Ring * r = (Ring*)ptr;
	LStack_push(r->list,node);
}
//��һ������
void* ring_next(void* ptr){
	Ring * r = (Ring*)ptr;
	int out;

	if(r->index+1<LStack_length(r->list)){
		r->index++;
	}
	else{
		r->index = 0;
	}
	LStatk_getNodeByIndex(r->list,r->index,&out);
	return (void*)out;
}
//ǰһ������
void* ring_pre(void* ptr){
	Ring * r = (Ring*)ptr;
	int out;

	if(r->index-1 >= 0){
		r->index--;
	}
	else{
		r->index = LStack_length(r->list)-1;
	}
	LStatk_getNodeByIndex(r->list,r->index,&out);
	return (void*)out;

}

void ring_dispose(void* ptr){
	Ring * r = (Ring*)ptr;

	LStack_delete(r->list);
	r->list = 0;
}