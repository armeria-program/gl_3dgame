#include <string.h>
#include "tools.h"
#include "node.h"
#include "anim.h"
#include "ex.h"
#include "collide.h"
#include "tmat.h"
//����
struct Node* node_create(const char* name)
{
	struct Node* ptr = (struct Node*)tl_malloc(sizeof(struct Node));
	memset(ptr,0,sizeof(struct Node));

	ptr->base = base_create(TYPE_OBJ_VBO_FILE,name,0,0,0);
	updateMat4x4(ptr->base);

	return ptr;
}
//����
void node_dispose(struct Node* p)
{
	//long time = get_longTime();
	//char* name = p->base->name;

	base_dispose(p->base);
	objVBO_dispose(p->ptrVBO);
	LStack_delNode(ex_getInstance()->renderList,(int)p);//����Ⱦ�ڵ��б����Ƴ�
	if(p->anim)
	{
		anim_dispose(p->anim);
	}

	//����ʰȡ����
	//if(p->collideBox.vertex)
	//{
	//	tl_free(p->collideBox.vertex);
	//	p->collideBox.vertex=0;
	//}

	if(p->ptrCollide){
		collide_dispose(p->ptrCollide);
		p->ptrCollide = 0;
	}

	tl_free(p);

	//log_color(0xff0000,"node_disposeɾ��%s����%ld����\n",name,get_longTime() - time);
}

void 
node_bindAnim(struct Node* p)
{
	//////////////////////////////////////////////////////////////////////////
	//��������������
	p->anim=anim_create(p->ptrVBO->ptrList);
}

static void 
drawCollion(struct Node* p)
{
	struct HeadInfo* b = base_get2(p);
	struct VertexData* vd =collide_cur(p->ptrCollide);
	drawLineByColor(b,vd->vertex,vd->vertLen,b->boxR,b->boxG,b->boxB);
}

/*
	����һ����ɫ����Ⱦһ������
*/
static void 
addRenderNode(struct Node* p)
{
	anim_playByFPS(p->anim,objVBO_render,(int)p);
}

static void 
f_setShader(struct HeadInfo* base,const char* shader)
{
	struct GMaterial* tmat = base->tmat;
	memset(tmat->curGlslType,0,G_BUFFER_32_SIZE);
	memcpy(tmat->curGlslType,shader,strlen(shader));
}
/*
	����Ĭ�ϵĲ���
*/
static void 
setDefaultShader(struct Node* ptr)
{
	struct HeadInfo* base =	base_get2(ptr);
	char _shaderName[G_BUFFER_32_SIZE];
	tmat_getShader(base->tmat,_shaderName,G_BUFFER_32_SIZE);
	f_setShader(base,_shaderName);
}

static int 
checkLineShader(struct ObjVBO* vbo)
{
	if(!vbo->normalID)
	{
		//�������ӵ�з�������,û�з������ݾͱ��쳣,�޳�����α���
		printf("��ɫ������Ҫ��normal����\n");
		//assert(0);
		return 0;
	}

	return 1;
}

static void 
lineVBO_CallBack(int input,struct ObjVBO* vbo)
{
	if(!checkLineShader(vbo))
	{
		return;
	}
	if(input)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}
/*
 *ͳһ����Ⱦ���ýӿ�
 */
void
node_render(struct Node* p){
	struct HeadInfo* base =	base_get2(p);

	if(!getv(&(base->flags),FLAGS_VISIBLE))
		return;

	//////////////////////////////////////////////////////////////////////////
	//objVBO_render��Ϊ�ڵ���Ⱦ�����ӿ�
	//�ж����Ĳ��Ŷ���,�޶�����,��Ⱦ��̬ģ��
	setDefaultShader(p);
	p->renderCallBack = NULL;

	addRenderNode(p);

	if(getv(&(base->flags),FLAGS_GLSL_OUTLINE))
	{
		//������Ե��Ⱦ
		f_setShader(base,"linevbo");
		p->renderCallBack = lineVBO_CallBack;
		addRenderNode(p);
	}

	//////////////////////////////////////////////////////////////////////////
	//������ײ��
	if(getv(&(base->flags),FLAGS_RAY) && p->ptrCollide && getv(&(base->flags),FLAGS_DRAW_RAY_COLLISION))
	{
		drawCollion(p);	
	}
}

//void 
//node_setPos(struct Node* p,float x,float y,float z)
//{
//	base_setPos(p->base,x,y,z);
//}