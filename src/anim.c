#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "tools.h"
#include "tl_malloc.h"
#include "gettime.h"
#include "anim.h"
#include "animtor.h"

/************************************************************************
*	����������
*	�޹غ��ײ�����VBO����ֱ��ģʽ,ֻ�ǹ���������,fps
/************************************************************************/
typedef struct VBOAnim{
	int allLength;//�����ܳ���
	int _tempLength;
	int curIndex;//��ǰ��������

	int* nodeList;//int ���鴢��struct ObjVBO*

	/*
	 * ֻ��>0��ʱ������ж�����
	 * ÿһ֡�ļ��ʱ��
	 * fps = 60;
	 * fpsInterval = 1000 / fps = 16.6666;
	 */
	long fpsInterval;
	
	/*
	*	��ֵ
	*/
	long interpolation;

	/*
	*	����������
	*/
	struct Animtor* pAnimtor;
};
static void
addListCallBack(int data,int parms)
{
	struct VBOAnim* anim=(struct VBOAnim*)parms;
	int i = anim->allLength-anim->_tempLength-1;
	anim->nodeList[i] = data;
	//anim->allLength++;
	anim->_tempLength++;
}

struct Animtor* 
anim_get_animtor(void* ptr){
	struct VBOAnim* p = (struct VBOAnim*)ptr;
	if(!p->pAnimtor)
		p->pAnimtor=animtor_create();
	
	return p->pAnimtor;
}
int
anim_total(void* p){
	struct VBOAnim* ptr=(struct VBOAnim*)(p);
	return ptr->allLength;
}
/*
	����
*/
void
anim_dispose(void* p){
	struct VBOAnim* anim = (struct VBOAnim*)p;
	if(anim->pAnimtor)
	{
		animtor_dispose(anim->pAnimtor);
	}

	tl_free(anim->nodeList);
	tl_free(anim);
}

/*
	��VBOת��Ϊ�����б�
*/
void*
anim_create(struct LStackNode* list)
{
	int length;
	struct VBOAnim* anim = tl_malloc(sizeof(struct VBOAnim));
	memset(anim,0,sizeof(struct VBOAnim));
	
	length = LStack_length(list);
	
	anim->nodeList = tl_malloc(sizeof(int)*length);
	anim->allLength = length;
	LStack_ergodic_t(list,addListCallBack,(int)anim);
	
	return anim;
}

/*
	����ָ�������ŵĶ���
*/
void
anim_play(void* p,int index,void (*callBack)(int,int),int param){
	struct VBOAnim* anim = (struct VBOAnim*)p;
	int data=anim->nodeList[index];
	callBack(data,param);
}

/*
	��ת��һ֡
*/
static void 
nextFrame(struct VBOAnim* p)
{
	if(p->pAnimtor)
	{
		animtor_calculateFrame(p->pAnimtor,&p->curIndex);
	}
	else
	{
		p->curIndex++;
		//û���и���Ķ����������йؼ�֡
		if(p->curIndex>=p->allLength)
		{
			p->curIndex=0;
		}
	}
	//printf("%d,",p->curIndex);
}
/*
	����ؼ�֡
*/
static void
calculateFrame(struct VBOAnim* p)
{
	long cur =  get_longTime();

	if(p->fpsInterval >0 &&	cur - p->interpolation > p->fpsInterval)
	{
		nextFrame(p);
		p->interpolation = cur;
	}
}
/*
	�������ж���

	struct VBOAnim* p,��Ҫ����p->curIndex������
*/
void
anim_playByFPS(void* ptr,void (*callBack)(int,int),int parm){
	struct VBOAnim* p = (struct VBOAnim*)ptr;
	if(p->fpsInterval!=0)
	{
		calculateFrame(p);
	}
	anim_play(p,p->curIndex,callBack,parm);
}

void
anim_set_fps(void* ptr,int fps){
	struct VBOAnim* p = (struct VBOAnim*)ptr;
	if(fps==0)
	{
		//printf("fps = %d ,�����˴����֡��\n",fps);
		//assert(0);
		//��������ǰ�ؼ�֡,��ֹ����
		p->fpsInterval = 0;
	}
	else
	{
		p->fpsInterval = 1000 / fps;
	}
}