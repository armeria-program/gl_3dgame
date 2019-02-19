#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include "tools.h"
#include "tween.h"
#include "tl_malloc.h"
#include "gettime.h"
#include "ex.h"
static struct LStackNode* g_list;


typedef struct {
	/*��ǰ����*/
	double* p;
	double s;//��ʼֵ
	double t;//Ŀ��ֵ
	

}TNode;
typedef struct{
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
	TNode* ptr;
}TweenNode;
/*
	��ֹtween
*/
static void
f_tween_stop(void* ptr){
	TweenNode* _node = (TweenNode*)ptr;
	tl_free(_node->ptr);
	LStack_delNode(g_list,(int)_node);
	//tl_free(_node);
}
/*
	����
*/
void* 
tween_to(int time,int cnt,...){
	TweenNode* _node = (TweenNode*)tl_malloc(sizeof(TweenNode));
	_node->length = cnt/2;
	_node->ptr = (TNode*)tl_malloc(sizeof(TNode) * cnt/2);
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
	LStack_push(g_list,(int)_node);
	return _node;
}
static void
f_nodeRun(TweenNode* _node,TNode* _nptr){
	double s = _nptr->s;
	double e = _nptr->t;
	int c = 20;//_node->needTime / ex_delay_time();
	double v = (e - s)/_node->needTime;
	/*double* ptr = (double*)_nptr->p;
	*ptr += v;*/
	//printf("%.3f\t%.3f\n",s,(double*)_nptr->p);
	//printf("delay_time = %ld\n",ex_delay_time());
	*_nptr->p+=v;
	printf("%ld %.3f %.3f %.3f\n",_node->useTime,v,*_nptr->p,e -s);
}

static void
f_tween_play(TweenNode* _node,long _longTime){
	
	_node->useTime =_longTime - _node->startTime;
	
	if( _node->useTime >= _node->needTime){
		f_tween_stop(_node);
	}else{
		int i;
		for(i = 0;i < _node->length;i++)	
			f_nodeRun(_node,(TNode*)&_node->ptr[i]);

		//printf("%ld\n",_node->useTime);
	}
}
void
tween_run(long _longTime){
	struct LStackNode* s = (struct LStackNode* )g_list;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//callBack(data,parm);
		f_tween_play((TweenNode*)data,_longTime);
	}
}