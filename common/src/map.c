#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "map.h"
#include "tools.h"

struct Map{
	struct LStackNode* list;
};
//������map�ڵ�
static void
f_delNode(struct MapNode* node){
	tl_free(node->key);
	tl_free(node);
}

void*
map_create(){
	struct Map* map = (struct Map*)tl_malloc(sizeof(struct Map));
	map->list = LStack_create();;
	return map;
}

void*
map_getNodeByValue(void* mapptr,void* value){
	struct Map* map = (struct Map*)mapptr;
	struct LStackNode* s = map->list;
	struct MapNode* result = 0;
	struct MapNode* _node = 0;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		_node = (struct MapNode*)data;
		if(_node->value == value){
			return _node;
		}
	}
	return 0;
}

void*
map_get(void* mapptr,const char* key){
	struct Map* map = (struct Map*)mapptr;

	struct LStackNode* s = map->list;

	int cnt = 0;
	struct MapNode* result = 0;
	struct MapNode* _node = 0;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);

		_node = (struct MapNode*)data;
		if (!strcmp(_node->key,key))
		{
			cnt++;
			//map�ڵ㸳ֵ
			//_node->value = value;
			//return _node;
			result = _node;
		}
	}
	if(cnt >1){
		printf("%d����ͬ�ļ�\n");
		assert(0);
	}
	return result;
}
void
map_del_key(void* mapptr,const char* key){
	struct Map* map = (struct Map*)mapptr;

	struct LStackNode* s = map->list;

	int cnt = 0;

	struct MapNode* _node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);

		_node = (struct MapNode*)data;
		if (!strcmp(_node->key,key))
		{
			//cnt++;
			//map�ڵ㸳ֵ
			//_node->value = value;
			f_delNode(_node);

			LStack_delNode(map->list,data);
		}
	}
}

void
map_set(void* mapptr,const char* key,void* value){
	struct Map* map = (struct Map*)mapptr;

	struct LStackNode* s = map->list;

	int isfind = 0;

	struct MapNode* _node;
	void* top,*p;
	top = s;
	p=top;
	//printf("***=======>%0x\n",p);
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//printf("=======>%0x\n",p);
		_node = (struct MapNode*)data;
		if (_node!=0 && !strcmp(_node->key,key))
		{
			isfind = 1;
			//map�ڵ㸳ֵ
			_node->value = value;
		}
	}

	if(!isfind){
		struct MapNode* node = (struct MapNode*)tl_malloc(sizeof(struct MapNode));
		char* name = tl_malloc(sizeof(key+1));
		memset(name,0,strlen(key)+1);
		memcpy(name,key,strlen(key));
		node->key = name;
		node->value=value;

		LStack_push(map->list,(void*)node);
		//printf("����map�ڵ�0x%0x,key=%s,value=%d\n",node,node->key,node->value);
	}else{
		//printf("�ҵ��ڵ�%0x\n",_node);
	}
}

void
map_dispose(void* map){
	//����map��ֵ����
}