#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "gettime.h"
#include "str.h"
#include "evt.h"

#include "tlgl.h"
#include "base.h"
#include "node.h"


//�ӿ�ʵ�ֲ���
//====================================================================================================

struct BaseInfo{
	struct HeadInfo* base;
};
//void base_get(void* p,struct HeadInfo* base){
//	memcpy(base,((struct BaseInfo*)p)->base,sizeof(struct HeadInfo));
//}
struct HeadInfo* 
base_get(void* p){
	
	struct BaseInfo* b;
	if(!p){
		return 0;
	}
	b = (struct BaseInfo*)p;
	return b->base;
}

static void f_base_drawBoundBox(struct HeadInfo* base,float* vertices,int vertCount);
static void f_outline(struct HeadInfo* base);

static void f_renderLine(struct HeadInfo* base);

static void f_outlineByGLSL(struct HeadInfo* base,float r,float g,float b);

static
void f_base_custRender(struct HeadInfo* base)
{
	if(base->renderCallBack!=NULL)
	{
		base->renderCallBack(base);
	}
}

/*
	��λ�ã�����,��ת�����仯��ʱ����¾���
*/
void 
base_updateMat4x4(struct HeadInfo* base){
	Matrix44f xyz,scale,rx,ry,rz;
	
	mat4x4_zero(base->m);
	
	//x,y,z����
	mat4x4_identity(xyz);
	mat4x4_translate(xyz,base->x,base->y,base->z);

	//scale
	mat4x4_identity(scale);
	mat4x4_scale(scale,base->scale,base->scale,base->scale);

	//��ת===================================================
	//rx
	mat4x4_identity(rx);
	//mat4x4_rotateX(rx,PI-base->rx);
	mat4x4_rotateX(rx,base->rx);

	//ry
	mat4x4_identity(ry);
	mat4x4_rotateY(ry,base->ry);

	//rz
	mat4x4_identity(rz);
	mat4x4_rotateZ(rz,base->rz);
	
	mat4x4_mult(5,base->m,xyz,ry,rx,rz,scale);	//����base->m = xyz * ry * rx * rz * scale
	//mat4x4_mult(6,base->m,xyz,ry,rx,rz,base->quat_m,scale);
	//mat4x4_mult(6,base->m,xyz,base->quat_m,ry,rx,rz,scale);

/*
	if(base->curType == TYPE_SPRITE_FLIE || base->curType==TYPE_TEXT_FILE)
	{
		//2d����
		multi2(base->m,xyz,result);//λ�ƾ���
	}
	else
	{
		
		if(!base->lookat){
			//���ڳ���Ŀ��
			multi2(base->m,xyz,result);

		}else{
			struct EX* p =ex_getInstance();
			Matrix44f _look,tt;
			struct Vec3 eye;
			struct Vec3 center;
			struct Vec3 up;
			eye.x = -p->camx;//base->target.x;
			eye.y = -p->camy;//base->target.y;
			eye.z = -p->camz;//base->target.z;

			center.x = base->x;
			center.y = base->y;
			center.z = base->z;

			up.x = 0;
			up.y = 1;
			up.z = 0;
			mat4x4_zero(tt);
			mat4x4_lookAt(_look,&eye,&center,&up);
			
			mat4x4_transpose(_look);
			multi2(tt,_look,result);
			multi2(base->m,xyz,tt);
		}
	}
*/
}

/*
	��������
*/
void 
base_setPos(struct HeadInfo* base,float x,float y,float z)
{
	base->x = x;
	base->y = y;
	base->z = z;
	base_updateMat4x4(base);
}

void
base_set_position(struct HeadInfo* base,struct Vec3* pos){
	base_setPos(base,pos->x,pos->y,pos->z);
}

void 
base_set_scale(void* p,float scale){
	struct HeadInfo* b = (struct HeadInfo*)p;
	b->scale = scale;
}
/************************************************************************/
/* ���ú�׺                                                    */
/************************************************************************/
void base_set_suffix(struct HeadInfo* base,const char* str){
	int pos = str_pos(str,".",1);
	int n = strlen(".");
	tl_strsub(str,base->suffix,pos + n,strlen(str) - n);
	//printf("[%s]\n",base->suffix);
}


void 
base_createRayVertex(struct VertexData* rayVertexData,
					 float* inputVertex,int inputVertexCount)
{
	rayVertexData->vertLen = BOX_SIZE;
	rayVertexData->vertex = (float*)tl_malloc(sizeof(float)*rayVertexData->vertLen);
	tlgl_aabb(inputVertex,inputVertexCount,rayVertexData->vertex);
}



struct HeadInfo* base_create(int curType,const char* name,float x,float y,float z)
{
	struct HeadInfo* base = (struct HeadInfo*)tl_malloc(sizeof(struct HeadInfo));
	memset(base,0,sizeof(struct HeadInfo));
	memset(&base->rData,0,sizeof(struct VertexData));

	//�����¼��������
	base->evtList = (void*)LStack_create();

	//��������
	base->curType = curType;

	//��������
	tl_set_vec(&base->x,x,y,z);

	//����ģ������
	memset(base->name,0,G_BUFFER_32_SIZE);
	memcpy(base->name,name,strlen(name));

	base->scale = 1.0;

	base->custframe = -1;

	//����Ĭ�ϵ�ʰȡ����ɫ
	base->boxR = 1.0f;

	//��ʼ����Ԫ������
	mat4x4_identity(base->quat_m);

	return base;
}

void base_setLuaPick(struct HeadInfo* base,const char* luaFunctionName){
	char* str;
	int len;
	if(base->luaPickCallBack){
		tl_free(base->luaPickCallBack);
	}
	len = strlen(luaFunctionName) + 1;
	str = tl_malloc(len);
	memset(str,0,len);
	memcpy(str,luaFunctionName,len - 1);
	base->luaPickCallBack = str;
}

void base_dispose(struct HeadInfo* base){

	if(base->tmat){
		tmat_dispose(base->tmat);
	}
	base->tmat = NULL;

	evt_dispose(base);

	//���ٰ�Χ��
	if(base->boxVertPtr){
		tl_free(base->boxVertPtr);
		base->boxVertPtr = NULL;
	}
	////���پ�̬��Χ��
	//if(base->staticBoxVert){
	//	tl_free(base->staticBoxVert);
	//	base->staticBoxVert = NULL;
	//}

	//����ʰȡ����
	if(base->rayVertexData.vertex!=NULL){
		tl_free(base->rayVertexData.vertex);
		base->rayVertexData.vertex=NULL;
	}

	//���ٶ���������
	if(base->frameAnim){
		tl_free(base->frameAnim);
		base->frameAnim = NULL;
	}

	//����Lua�ص�
	if(base->luaPickCallBack){
		tl_free(base->luaPickCallBack);
		base->luaPickCallBack = NULL;
	}

	//����VBO 
	//base->vboPtr

	tl_free(base);
}
void 
base_drawLineByColor(struct HeadInfo* base,GLfloat* vertex,int vertLen,float r,float g,float b)
{
	struct GMaterial* m = (struct GMaterial*)base->tmat;
	m->_lineColor.x = r;
	m->_lineColor.y = g;
	m->_lineColor.z = b;
	tmat_render(base->tmat,"diffuseStateBox",base->m);
	tlgl_triangles(vertex,vertLen,GL_LINE);
}
//void f_closeRenderState(){
//	//�����ɫ������
//	glUseProgram(0);
//	glActiveTexture(0);
//	glBindTexture(GL_TEXTURE_2D,0);
//}
void base_renderFill(struct HeadInfo* base)
{
	struct VertexData* node=&base->rData;

	//���ƾ�̬��Χ��
	base_staticBox(base);

	if(!getv(&(base->flags),FLAGS_VISIBLE))
	{
		return;
	}

	/***�����Ǹ�����Ⱦģʽ����	***/

	//��Ⱦģ��
	base_renderByMaterial(base);

	if(getv(&(base->flags),FLAGS_DRAW_NORMAL))
	{
		tlgl_drawNormal(&base->rData,1.0f);//���ƶ���η���
	}

	//��Ⱦ����
	f_outlineByGLSL(base,1.0f,0.0f,1.0f);

	//====================================================

	//��̬��Χ��,�ð�Χ�л����Ŷ��㶯���仯���仯
	f_base_drawBoundBox(base,node->vertex,node->vertLen);

	//�߿���Ⱦ
	f_renderLine(base);

	//������Ⱦ
	f_outline(base);

	//�Զ���Ļص���Ⱦ��ʽ
	f_base_custRender(base);

	//������Ҫ�ر�������ɫ���е�״̬...

}


static void f_base_drawBoundBox(struct HeadInfo* base,float* vertices,int vertCount){

	int dataLength;
	if(!getv(&base->flags,FLAGS_RENDER_BOUND_BOX)){
		return;
	}

	/*glPushMatrix();
	glLoadIdentity();
	glTranslatef(base->x,base->y,base->z);*/
	
	//���ɰ�Χ�ж�������
	//f_base_boxVert_init(base);

	if(!base->boxVertPtr)
	{
		base->boxVertPtr = tl_malloc(sizeof(float)*BOX_SIZE);
	}
	//���ɰ�Χ������,��������䵽base->boxVert��
	dataLength=tlgl_aabb(vertices,vertCount,base->boxVertPtr);

	//��Ⱦ�������߿�
	//tlgl_drawTrianglesLine(base->boxVertPtr,dataLength,0.0f,1.0f,0.0f);		//base->boxVertLength

	//tmat_render(base->tmat,"diffuseStateBox",base->m,0);
	//tlgl_triangles(base->boxVertPtr,dataLength,GL_LINE);

	base_drawLineByColor(base,base->boxVertPtr,dataLength,base->boxR,base->boxG,base->boxB);


	//glPopMatrix();
}
/*
	��Ⱦ���������
*/
static void f_drawTriangles(struct HeadInfo* base)
{
	tlgl_render_triangles(base->rData.vertex,base->rData.vertLen);
}
/*
*	������Ⱦ(�д��Ż�)
*/
static void f_outline(struct HeadInfo* base){
	if(!getv(&base->flags,FLAGS_OUTLINE)){
		return;
	}
	
	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(base->x,base->y,base->z);
	glScalef(base->scale,base->scale,base->scale);
	//�����߿�ߴ�
	glLineWidth(2.0f);

	glEnable(GL_CULL_FACE);
	glPolygonMode (GL_BACK,GL_LINE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_FRONT);
	glColor3f(0.0,0.0,1.0);
	glUseProgram(0);

	f_drawTriangles(base);

	glPopMatrix();
	glDisable(GL_CULL_FACE);

}

//�߿���Ⱦ
static void f_renderLine(struct HeadInfo* base){
	struct VertexData* node = NULL;
	//if(!base->isLineMode)
	//	return;
	if(!getv(&base->flags,FLAGS_LINE_RENDER)){
		return;
	}

	node=&base->rData;

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(base->x,base->y,base->z);
	glScalef(base->scale,base->scale,base->scale);
	//�߿�ģʽ
	tlgl_drawPloyLine();

	//f_xrender(e);
	//tlgl_renderMode(NULL,node->vertex,node->vertLen);
	f_drawTriangles(base);

	glPopMatrix();
}


/*
	���ƾ�̬��Χ��,�ð�Χ�����Զ��峤���ߵ�
*/
void base_staticBox(struct HeadInfo* base){

	struct VertexData* ray = &base->rayVertexData;
	if(!getv(&base->flags,FLAGS_DRAW_RAY_COLLISION) || !ray->vertex || !ray->vertLen){
		return;
	}
	base_drawLineByColor(base,ray->vertex,ray->vertLen,base->boxR,base->boxG,base->boxB);
}

void 
base_seachPick(struct LStackNode* s,struct Vec3* nearPoint,struct Vec3* farPoint,struct HitResultObject* last){

	void* top,*p;

	struct HitResultObject hit;
	struct HitResultObject* pHit=&hit;

	//struct HeadInfo __base;
	struct HeadInfo* base = NULL;//= &__base;

	struct VertexData* vd;

	float maxDistance = INF;

	top = s;
	p=top;
	//printf("==================================\n");
	//printf("nearPoint %.3f,%.3f,%.3f farPoint: %.3f,%.3f,%.3f\n",nearPoint->x,nearPoint->y,nearPoint->z,farPoint->x,farPoint->y,farPoint->z);

	memset(pHit,0,sizeof(struct HitResultObject));
	memset(last,0,sizeof(struct HitResultObject));

	while((int)LStack_next(p)){
		int data;
		float* tri = NULL;//��̬��Χ�ж�������
		struct Vec3 pos; 

		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//=========================================
		//base_get((void*)data,&__base);
		base = base_get((void*)data);
		vd = &base->rayVertexData;
		//if(base->curType == TYPE_OBJ_VBO_FILE)
		if(base->isNode){
			struct Node* node = (struct Node*)data;
			if(node->ptrCollide){
				vd = collide_cur(node->ptrCollide);
			}
		}

		tri = vd->vertex;

		if(tri && base){
			int rayStat = getv(&base->flags,FLAGS_RAY);
			if(rayStat){
				float newTri[BOX_SIZE];
				memcpy(newTri,tri,vd->vertLen * sizeof(float));

				if(vd->vertLen && base->scale!=1.0f)
				{
					int i;

					//�Ծ�̬��ײ�н�������
					for( i = 0;i< vd->vertLen;i++)
						newTri[i] *= base->scale;

				}

				pos.x = base->x;
				pos.y = base->y;
				pos.z = base->z;

				tl_pickTriangle(newTri,vd->vertLen,nearPoint,farPoint,&pos,pHit);

				if(pHit->isHit && pHit->distance < maxDistance){
					memset(pHit->name,0,G_BUFFER_32_SIZE);
					memcpy(pHit->name,base->name,strlen(base->name));
					maxDistance = pHit->distance;

					memcpy(last,pHit,sizeof(struct HitResultObject));
				}
			}
		}
		//=========================================
	}
	//f_pickLog(last);
}

/*
	��ȡ��Ⱦģʽ�Ƿ����߿���ʵ��ģʽ
*/
int base_get_ploygonLineMode(struct HeadInfo* base){
	if(getv(&base->flags,FLAGS_DRAW_PLOYGON_LINE)){
		return GL_LINE;
	}
	if(getv(&base->flags,FLAGS_DRAW_PLOYGON_POINT)){
		return GL_POINT;
	}
	return GL_FILL;
}

//#define _RenderTest_

GLboolean 
base_cullface(struct HeadInfo* base){
	//int mode = base_get_ploygonLineMode(base);

//	if(mode != GL_FILL){
		if(getv(&base->flags,FLAGS_DISABLE_CULL_FACE)){
			glDisable(GL_CULL_FACE);
		}else{
			//�޳�����α���
			glEnable(GL_CULL_FACE);//GL_FRONT	//GL_BACK
			//ǰ���洦��
			glCullFace(GL_FRONT);
			//glCullFace(f_check(base->curType) ? GL_FRONT : GL_BACK);
			return GL_TRUE;
		}
//	}

	return GL_FALSE;
}


/*
	��Ⱦһ�������ʵ�ģ��
*/
void base_renderByMaterial(struct HeadInfo* base)
{	
	char _shaderName[G_BUFFER_32_SIZE];

	int mode = base_get_ploygonLineMode(base);

	GLboolean _cull = base_cullface(base);
	//===================================================
	if(!base->tmat)//û�в��ʾͷ���
		return;

	tmat_getShader(base->tmat,_shaderName,G_BUFFER_32_SIZE);
	
	glLineWidth(1.0f);

	//ָ����ɫ������ͼ,��������(�����괫�ݵ���ɫ��������)
	tmat_render(base->tmat,_shaderName,base->m);

	//������Ⱦģʽ
	glPolygonMode (GL_FRONT_AND_BACK,mode);


	//��VBO,ֱ�Ӵ�cpu�������л�����
	f_drawTriangles(base);

	if(_cull){
		glDisable(GL_CULL_FACE);
	}
}


/*
	��������
*/
static void f_outlineByGLSL(struct HeadInfo* base,float r,float g,float b)
{
	if(!getv(&(base->flags),FLAGS_GLSL_OUTLINE))
	{
		return;
	}

	//�޳�����α���
	glEnable(GL_CULL_FACE);

	/*
	glCullFace(GL_FRONT);

	if(f_check(base->curType))
	{
		glCullFace(GL_BACK);
	}
	else
	{
		glCullFace(GL_FRONT);
	}
	*/
	glCullFace(GL_FRONT);
	//===================================================

	//ָ����ɫ������ͼ,��������(�����괫�ݵ���ɫ��������)
	tmat_render(base->tmat,"line",base->m);

	//������Ⱦģʽ
	glPolygonMode (GL_FRONT_AND_BACK,GL_FILL);

	//��VBO,ֱ�Ӵ�cpu�������л�����
	f_drawTriangles(base);	
}




void 
base_hit_mouse(GLint xMouse, GLint yMouse,float screenWidth,float screenHeight,
struct LStackNode *renderList,Matrix44f perspectiveMatrix,Matrix44f modelViewMatrix,
	void (*mRayPickCallBack)(struct HitResultObject*))
{
	//GLdouble model_view[16];
	//��������ĵ�����
	struct Vec3 nearPoint;

	//������Զ�ĵ�����
	struct Vec3 farPoint;

	struct Vec3 n_vector;
	//struct Vec3 offset;
	struct HitResultObject last;
	//int i;

	GLfloat posX, posY, posZ;
	//float _posX,_posY,_posZ;
	int bResult;
	//===================================================

	Matrix44f modelview;
	Matrix44f projection;

	GLint viewport[4];

	//�任Ҫ��ͼ�������˳��һ������������ת�����������
	GLfloat winX;
	GLfloat winY;

	//xMouse = 249;yMouse = 157;

	winX = xMouse;
	winY = screenHeight - yMouse;

	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = screenWidth;
	viewport[3] = screenHeight;
	//////////////////////////////////////////////////////////////////////////

	//modelview = p->modelViewMatrix;
	//projection = p->perspectiveMatrix;


	mat4x4_copy(modelViewMatrix,modelview);
	mat4x4_transpose(modelview);

	mat4x4_copy(perspectiveMatrix,projection);
	mat4x4_transpose(projection);

	//2Dת3D
	//��ȡ���ض�Ӧ��ǰ�ü���ĵ�����
	bResult = tlgl_UnProject(winX, winY, 0.0, modelview, projection, viewport, &posX, &posY, &posZ);//gluUnProject


	//printf("0ʰȡ������%f,%f,%f\n",posX,posY,posZ);

	if(bResult == GL_FALSE){
		printf("gluUnProject false 0.0");
		return;
	}

	nearPoint.x = posX; 
	nearPoint.y = posY; 
	nearPoint.z = posZ;

	//��ȡ���ض�Ӧ�ĺ�ü���ĵ�����
	bResult = tlgl_UnProject(winX, winY, 1.0, modelview, projection, viewport, &posX, &posY, &posZ); //gluUnProject


	//printf("1ʰȡ������%f,%f,%f\n",posX,posY,posZ);


	if(bResult == GL_FALSE){
		printf("gluUnProject false 1.0");
		return;
	}

	farPoint.x = posX; 
	farPoint.y = posY; 
	farPoint.z = posZ;

	//cal the vector of ray
	//n_vector���߷���

	n_vector.x=farPoint.x-nearPoint.x;
	n_vector.y=farPoint.y-nearPoint.y;
	n_vector.z=farPoint.z-nearPoint.z;

	base_seachPick(renderList,&nearPoint,&farPoint,&last);

	if(last.isHit && mRayPickCallBack!=NULL){
		//printf("%s ����:%.3f,%.3f,%.3f\n",last.name,last.x,last.y,last.z);
		mRayPickCallBack(&last);
	}
}
