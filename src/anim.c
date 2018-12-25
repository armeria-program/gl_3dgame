#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "tools.h"
#include "gettime.h"
#include "anim.h"
#include "animtor.h"

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
anim_get_animtor(struct VBOAnim* p)
{
	if(!p->pAnimtor)
		p->pAnimtor=animtor_create();
	
	return p->pAnimtor;
}

/*
	����
*/
void
anim_dispose(struct VBOAnim* anim)
{
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
struct VBOAnim*
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
anim_play(struct VBOAnim* anim,int index,void (*callBack)(int,int),int param)
{
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
anim_playByFPS(struct VBOAnim* p,void (*callBack)(int,int),int parm)
{
	if(p->fpsInterval!=0)
	{
		calculateFrame(p);
	}
	anim_play(p,p->curIndex,callBack,parm);
}

void
anim_set_fps(struct VBOAnim* p,int fps)
{
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