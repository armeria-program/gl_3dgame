#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#define BUILDING_DLL

#include "tools.h"
#include "tween.h"
#include "tl_malloc.h"
#include "gettime.h"
//#include "ex.h"

//#define _DEBUG_

static struct LStackNode* g_list;

typedef struct {
	/*��ǰ����*/
	double* p;
	double s;//��ʼֵ
	double t;//Ŀ��ֵ
}TNode;
typedef struct{
	void* obj;
	/*
		�ڵ㳤��
	*/
	int length;

	/*
		��Ҫ��ʱ��(����)
	*/
	int needTime;
	int useTime;
	/*
		��ʼ��ʱ��
	*/
	int startTime;
	void (*updateCallBack)();
	void (*endCallBack)();
	TNode* ptr;
}TweenNode;
//ִ�н����ص�����
static void
f_call_end(void* ptr){
	TweenNode* _node = (TweenNode*)ptr;
	if(_node->endCallBack)	_node->endCallBack(_node->obj);//ִ�н����¼�
}

/*
	��ֹtween
*/
void
tween_stop(void* ptr){
	if(!tween_is_play(ptr))	{
		return;
	}
	{
		TweenNode* _node = (TweenNode*)ptr;
		tl_free(_node->ptr);
		_node->ptr = 0;
		LStack_delNode(g_list,(int)_node);
		tl_free(_node);

		log_color(0,"tw ���ٽڵ�%0x\n",_node);
	}
	
}
/*
	����
*/
void* 
tween_to(void* obj,
		 int time,
		 void (*end)(void*),
		 void (*update)(void*),
		 int cnt,...){
	TweenNode* _node = (TweenNode*)tl_malloc(sizeof(TweenNode));
	_node->obj = obj;
	_node->length = cnt/2;
	_node->ptr = (TNode*)tl_malloc(sizeof(TNode) * cnt/2);
	_node->updateCallBack = update;
	_node->endCallBack = end;
	_node->startTime = get_longTime();
	_node->needTime = time;
	//###############################################
	{
		int i;
		double* pv;
		int n = 0;
		int k = 0;
		//double* dptr;
		va_list ap;
		va_start(ap, cnt);
		for(i = 0; i < cnt; i++){
			if(n % 2 == 1){
				double* dptr;
				TNode* _nptr = &_node->ptr[k];
				
				dptr = (double*)pv;
				_nptr->p = pv;
				_nptr->s = *dptr;
				_nptr->t = va_arg(ap, double);
				//printf("%d\t%d\n",_nptr,sizeof(TNode));
				k++;
			}else{
				//printf("%d\t%.3f\n",p,(float)p);
				//printf("%d\n",p);
				pv = va_arg(ap, double*);
			}
			n++;
		}
		va_end(ap);
	}

	if(g_list==0){
		g_list = LStack_create();
	}
	LStack_push(g_list,_node);
	return _node;
}
//ÿһ�λص���ʱ����
static long g_delay = 0;
static long g_last= 0;
static void
f_nodeRun(TweenNode* _node,TNode* _nptr,int i){
	double s = _nptr->s;
	double e = _nptr->t;
	//int c = 20;//_node->needTime / ex_delay_time();
	double v = _node->needTime/g_delay;//v������Ҫִ�д�ѭ���Ĵ���
	double f = (e-s)/v;//f����ÿ���ƶ��ı仯ֵ
	if(v <= 0) return;
	
	
	//assert(0);
	/*double* ptr = (double*)_nptr->p;
	*ptr += v;*/
	//printf("%.3f\t%.3f\n",s,(double*)_nptr->p);
	//printf("delay_time = %ld\n",ex_delay_time());


	if(f > 0 && *_nptr->p+f > e||f<0 && *_nptr->p+f < e){
		*_nptr->p = e;
	}else{
		*_nptr->p+=f;
	}
//	printf("%d** %d %.3f\n\n",i,_nptr->p,*_nptr->p);
//	printf("%ld v = %.3f p = %.3f %.3f \t%ld %.3f\n",_node->useTime,v,*_nptr->p,e,g_delay,f);
//	
#ifdef _DEBUG_
	printf("ֵ�仯:%d f = %.3f,e = %.3f,s = %.3f,v = %.3f \t needtime = %d\t g_delay = %ld r = %.3f\n",
		_nptr,f,e,s,v,
		_node->needTime,
		g_delay,*_nptr->p);
#endif
}
//����һ���ڵ�,ɾ���ڵ�ɹ�����1,��֮����0
static int
f_tween_play(TweenNode* _node,long _longTime){

	_node->useTime = _longTime - _node->startTime;

	if( _node->useTime >= _node->needTime){
		f_call_end(_node);
		tween_stop(_node);
		return 1;
	}else{
		int i;
		for(i = 0;i < _node->length;i++)	
			f_nodeRun(_node,(TNode*)&_node->ptr[i],i);

		if( _node->updateCallBack)	_node->updateCallBack(_node->obj);
		//printf("%ld\n",_node->useTime);
	}
	return 0;
}

int 
tween_is_play(void* ptr){
	struct LStackNode* s = (struct LStackNode* )g_list;
	void* top,*p;
	if(!g_list)	return 0;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		if((int)ptr == data)	return 1;
	}
	return 0;
}
void
tween_run(long _longTime,long delayTime){
	struct LStackNode* s = (struct LStackNode* )g_list;
	void* top,*p;

	if(!g_list)	return;

	g_delay = delayTime;//_longTime - g_last;
	
	if(g_delay < 1 )	return;

	g_last = _longTime;
	
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//callBack(data,parm);
		if(f_tween_play((TweenNode*)data,_longTime)){
			//ɾ���ڵ�
			//struct LStackNode* n = (struct LStackNode*)p;
			//memset(n,0,sizeof(struct LStackNode));//���������Ұָ��,ʹ���ڴ���ƻ���
			//printf("%0x %0x %0xɾ���ɹ�!\n",n->next,n->pre,n->data);

			//��ת��topλ�ñ���while((int)LStack_next(p))ȡ�������ͷŶ��ڴ������
			p = top;
		}
	}
	
}