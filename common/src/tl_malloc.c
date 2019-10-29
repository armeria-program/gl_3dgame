//#define BUILDING_DLL
#include <string.h>
#include <stdio.h>
#include <memory.h>    
#include <stdlib.h>

//#define DEBUG

#define BUILDING_DLL

//�Ƿ�ʹ���Զ�����ڴ����,�˷�ʽû��ԭ����malloc �� free�����ܸ��Ӻ�
#define CUST_MEMORY

#include "tools.h"
#include "gettime.h"

#define EMemoryDisable  0//δʹ��
#define	EMemoryEnable   1//�Ѿ�ʹ��

struct LStackNode* memList;//�ڴ�ڵ��б�

struct MemoryNode{
	/**
	* �ֽڳ���
	*/
	void* p;
	/**
	* �ֽڳ���                                                                  
	*/
	int length;
	/**
	* �Ƿ�ʹ����                                                                    
	*/
	int bUse;
};

//	���ֽڳ���
static int g_total;

//�洢��Ҫ������ڴ���С
static int g_size;

//��ʱ�ڵ�
static void* g_node;

//δʹ�õĽڵ����
static int g_disable_cnt;

//δʹ�õĽڵ������ֽ��ܴ�С
static int g_disable_bytes;
/************************************************************************/
/* �Ƿ���memory_free֮��ֱ��freeϵͳ����                         */
/************************************************************************/
static int g_bFreeClear = 1;

static int
f_getInfo(int data,int parm){
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		g_disable_cnt++;
		g_disable_bytes+=node->length;
		//log_color(0xffff00,"0x%x \t%d bytes\n",node,node->length);
	}
	else{
		//log_color(0x00ff00,"0x%x \t%d bytes\n",node,node->length);
	}
	return 1;
}
/*
	�ڵ���Ϣ����
*/
static void
f_calculate_mem(){
	g_disable_bytes = 0;
	g_disable_cnt = 0;
	if(memList){
		LStack_ergodic(memList,f_getInfo,0);
	}
}
void
memory_get_info(int* pDisable_bytes,int* pDisable_cnt,int* pg_total){
	f_calculate_mem();
	//log_color(0xffff00,"���ýڵ�����:%d,�ܴ�С:%d�ֽ�,�ڴ����ռ��:%d�ֽ�\n",g_disable_cnt,g_disable_bytes,g_total);
	if(pDisable_cnt)*pDisable_cnt = g_disable_cnt;

	if(pDisable_bytes)*pDisable_bytes = g_disable_bytes;

	if(pg_total)
		*pg_total = g_total;
}

static int
f_findnew(int data,int parm)
{
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		g_disable_cnt++;
		g_disable_bytes+=node->length;
		if(node->length >= g_size){
			g_node = node->p;
			//memset(node->p,0,node->length);
			node->bUse = EMemoryEnable;
			//log_color(0xff0000,"%d ȡ�ڴ���е�����:0x%x  %d bytes \ttotal = %.3f kb\n",g_cnt,node->p,node->length,f_getTotalKB());
			//g_cnt++;
			return 0;
		}
	}
	return 1;
}

static void*
memory_new(int size)
{
	struct MemoryNode* node;
	if(!memList){
		memList =	LStack_create();
	}
	
	//��һ���ֽ�,����\0���,�ַ������Ľ�����־
	size += 1;

	g_size = size;
	g_node = 0;

	LStack_ergodic(memList,f_findnew,0);


	if(g_node){
		return g_node;
	}

	///////////////////////////////////////////////////
	node = (struct MemoryNode*)MALLOC(sizeof(struct MemoryNode));
	//memset(node,0,sizeof(struct MemoryNode));

	node->bUse = EMemoryEnable;
	node->length = size;
	g_total += size;
	node->p = MALLOC(size);
	memset(node->p,0,size);

	LStack_push(memList,node);

	//log_color(0xff0000,"memory_new �ڴ����뿪�� %.3f kb\n",(float)(g_total)/1024);

	return node->p;
}

//ɾ��һ���ڵ�����freeϵͳ�����ڴ�,������ϵͳGC����
static void
f_freeNode(struct MemoryNode* node){
	g_total-=node->length;
	FREE(node->p);
	LStack_delNode(memList,(int)node);
}

//����ɾ��
static int 
f_findfree(int data,int parm){
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->p == g_node){
		node->bUse = EMemoryDisable;
		if(g_bFreeClear){
			f_freeNode(node);
		}
		return 0;
	}
	return 1;
}

//����ɾ��
static int 
f_gc(int data,int parm){
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		f_freeNode(node);
	}
	return 1;
}

/*
int memory_get_total(){
	return g_total;
}
*/

//Garbage Collection
void memory_gc(){
	int n = g_total;
	int m = get_longTime();
	if(memList){
		LStack_ergodic(memList,f_gc,0);
		log_color(0xffff00,"gc֮ǰ%d�ֽ�(%.3fkb),����%d����,gc֮��%d�ֽ�(%.3fkb),������%d�ֽ�(%.3fkb)\n",n,n/1024.0f,(get_longTime() - m), g_total,g_total / 1024.0f , (n - g_total), (float)(n - g_total) / 1024.0f);
	}else{
		printf("δʹ���Զ�����ڴ����\n");
	}
}

static void 
f_free(void *p,int _stat){
	g_bFreeClear = _stat;
	g_node = p;
	LStack_ergodic(memList,f_findfree,0);
}

static void 
memory_retrieve(void* p){
	f_free(p,1);
}

static void  
memory_free(void* p){
/*
	int stat;
	va_list ap; 
	va_start(ap, p);
	stat = va_arg(ap, int);
	g_bFreeClear = stat == TRUE ? TRUE : FALSE;
	log_color(0xff0000,"%d %d\n",stat,g_bFreeClear);
*/
	f_free(p,0);
}

//static int m;
void* tl_malloc(int size){
#ifdef CUST_MEMORY
	void* p = memory_new(size);
#else
	void*p =malloc(size);
	if(p == 0){
		printf("tl_malloc size %d false",size);
	}
#endif
	//m++;
#ifdef DEBUG
	printf("+ \t%0x\n",p);
#endif
	return p;
/*

	struct MemHandle* instance = GetInstance();
	if(instance->stat){
		int needSize;	
		//�˷�������ʹ����������

		int fSize = 0;
		N* _node = NULL;
		int _stat= getNextSize(size,instance->bufferList,instance->bufferCount,&needSize);

		if(_stat==MEM_NOT_FOUNT)
		{
			printf("������ڴ�Ϊ:%d�ֽ�,���������ڴ�ش�С\n",size);
			assert(0);
		}
		else if(_stat == MEM_NEED_MORE)
		{
			printf("��Ҫ������ڴ�� %d\n",size);
			assert(0);
		}

		fSize = needSize;

		_node=findNode(instance,fSize);
		if(_node != NULL){
			_node->uesd = MEM_USED;
#ifdef _SHOW_MALLOC_LOG_
			printf("���� 0x%x %d/%d\n",n,_node->len,tl_memsize());
#endif
			return _node->p;
		}else{
			//�ڴ���ڴ治��,Ϊ�ڴ����ӽڵ�
			
			//printf("�����ڴ�%d�ֽ�δ�ҵ�,��������������ڴ��,�����ڴ�:%d�ֽ�\n",size,fSize);
			
			
			//assert(0);
		
		
		//printf("�������ڴ�:%d�ֽ�\n",fSize);
		//	instance->maxSize+=fSize;
		//	realloc(instance->buffer,instance->maxSize);
		//	{
		//		struct MemNode* _n=	ex_addNode(instance ,fSize);
		//		return _n->p;
		//	}
			
			
			return tl_malloc(fSize);
		}
		printf("tl_malloc (%d bytes) fail!\n",size);
		return NULL;
	}
	//printf("MALLOC ");
	return MALLOC(size);
*/
}
//static int c;
void tl_free(void* p){
	#ifdef CUST_MEMORY
	memory_free(p);
#else
	free(p);
#endif
#ifdef DEBUG
	printf("\tfree %0x\n",p); 
	//c++;
#endif
	return;
/*
	struct MemHandle* t = GetInstance();
	if(t->stat){	

		struct MemDelNode delNode;
		memset(&delNode,0,sizeof(struct MemDelNode));
		delNode.stat = FALSE;
		delNode.target = p;
		LStack_ergodic(GetInstance()->list,freeDelNode,(int)&delNode);

		if(delNode.stat== FALSE){
			printf("tl_free 0x%x ʧ��,�Ҳ����ͷŵĽڵ�!\n",(int)p);
			assert(0);
		}else{
			struct MemNode* n = delNode.result;
			#ifdef _SHOW_MALLOC_LOG_
			printf("        �ͷ� 0x%x %d/%d\n",n,n->len,tl_memsize());
			#endif
		}
		p = NULL;

		return;
	}
	FREE(p);
	*/
}