#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "tools.h"
#include "ex.h"
#include "tmat.h"
#include "sprite.h"
#include "atlas.h"
#include "gettime.h"
#include "obj.h"
#include "obj_vbo.h"
#include "tl_malloc.h"
#include "base.h"


//������������������Ҫ�����ݴ�С(Glfloat���ݵĸ���)
enum {
	//uv vertex
	SPRITE_VERTEX_COUNT_30=30,

	//uv normal vertex
	SPRITE_VERTEX_COUNT_48=48,
	
	//uv��ҪGLfloat����
	UV_COUNT_6 = 6,

	UV_COUNT_4 = 4,
};

//�����������
enum
{
	Type_6Vertex = 1,
	Type_4Vertex = 2
};

/*
 *��ȡ������0��λ�õ�����
 */
static struct ObjVBO*
getvbo(struct Sprite* p)
{
	if(p->vbo)
	{
		return (struct ObjVBO*)LStatk_getAddressByIndex(p->vbo->ptrList,0);
	}
	return 0;
}
/*
	 ������
*/
#define  VBO_VERTEX_SPLINE 5	//u v x y z
static void updateSpriteMat4x4(struct Sprite* b,float x,float y,float z,float scaleX,float scaleY,float scaleZ,float _rx,float _ry,float _rz);
//��ӡuv GL_T2F_N3F_V3F
static void 
printfVertex(GLfloat* vert,int vertLength)
{
	int i;

	int n = 0;
	for(i = 0;i < vertLength* VBO_VERTEX_SPLINE;i++)
	{
		if(n>=VBO_VERTEX_SPLINE){
			printf("\n");
			n=0;
		}
		
		printf("%f,",vert[i]);
		n++;
	}
}

/*
 *	float * v Ϊ sizeof(float) * 12 = 48 bytes
 */
static void
SetUV_6Vertex(float *v,const int gap,struct Vec2 a1,struct Vec2 a2,struct Vec2 a3,struct Vec2 a4)
{
	
	int a = 1;
	int b = 0;
	int i = 0;
	
	//if(filp)
	//{
	//	a = 0;
	//	b = 1;
	//}
	
	v[i+a] = a1.x;
	v[i+b] = a1.y;		
	i+=gap;
	//2
	v[i+a] = a2.x;
	v[i+b] = a2.y;	
	i+=gap;
	//3
	v[i+a] = a3.x ;
	v[i+b] = a3.y;	
	i+=gap;
	//4 = 3
	v[i+a] = a3.x;
	v[i+b] = a3.y;
	i+=gap;
	//5 = 2
	v[i+a] = a2.x;
	v[i+b] = a2.y;	
	i+=gap;
	//6
	v[i+a] = a4.x;
	v[i+b] = a4.y;
}


static void 
SetUV_4Vertex(float *v,const int gap,struct Vec2 a1,struct Vec2 a2,struct Vec2 a3,struct Vec2 a4)
{
	int a = 1;
	int b = 0;
	int i = 0;

	v[i+a] = a1.x;
	v[i+b] = a1.y;		
	i+=gap;
	//2
	v[i+a] = a2.x;
	v[i+b] = a2.y;	
	i+=gap;
	//3
	v[i+a] = a3.x ;
	v[i+b] = a3.y;	
	i+=gap;
	//4
	v[i+a] = a4.x;
	v[i+b] = a4.y;
}


/*
	����sprite��UV,����VBO����glBufferSubData����VBO����
*/
void sprite_setUV(struct Sprite* sprite,float x,float y,float w,float h)
{
	//int i=0;
	//����a,b����תUV
	/*int a = 1;
	int b = 0;*/


	struct Vec2 a1,a2,a3,a4;
	float* v;

	a1.x = x;
	a1.y = y;

	a2.x = x + w;
	a2.y = y;

	a3.x = x ;
	a3.y = y + h;

	a4.x = x + w;
	a4.y = y + h;

	v = sprite->vertexs;
	if(v){
		SetUV_6Vertex(v,VBO_VERTEX_SPLINE,a1,a2,a3,a4);
	}
	if(sprite->vbo)
	{	
		//VBOģʽ
		struct ObjVBO* vbo=getvbo(sprite);
		if(vbo->uvID==0)
		{
			printf("vbo->uvID = 0\n");
			assert(0);
		}
		else
		{
			float* __ptr;

			//Specifies the size in bytes of the data store region being replaced.(�������ڴ���ֽڳߴ�)
			int _bytesSize;

			if(sprite->parseType == Type_6Vertex)
			{
				float ptr[UV_COUNT_6*UV_GAP];
				
				//6��UV�������� * 2(һ��������Ҫ��uv����) * һ��float��ռ�õ��ֽڴ�С
				_bytesSize = UV_COUNT_6*UV_GAP*sizeof(float);
				
				SetUV_6Vertex(ptr,UV_GAP,a1,a2,a3,a4);
				__ptr = ptr;
			}
			else if(sprite->parseType == Type_4Vertex)
			{
				float ptr[UV_COUNT_4*UV_GAP];

				//4��UV�������� * 2(һ��������Ҫ��uv����) * һ��float��ռ�õ��ֽڴ�С
				_bytesSize = UV_COUNT_4*UV_GAP*sizeof(float);
				SetUV_4Vertex(ptr,UV_GAP,a1,a2,a3,a4);
				__ptr = ptr;
			}
			
			//����UV��VBO
			glBindBuffer(GL_ARRAY_BUFFER,vbo->uvID);
			glBufferSubData(GL_ARRAY_BUFFER,0,_bytesSize,__ptr);
		}
	}
}

//����һ���ı�����Ƭ,����һ�黺��������,�洢��������
static GLfloat* 
LoadQuadObj(int* ptrVertexLength)
{
/*
	struct EX* ex=ex_getInstance();
	int length = ex->spriteVert.vertLen;
	GLfloat* vertsPtr ;

	*ptrVertexLength = length / VBO_VERTEX_SPLINE;
	
	//copy��������
	vertsPtr = (GLfloat*)tl_malloc(sizeof(GLfloat)*length);
	memcpy(vertsPtr,ex->spriteVert.vertex,(size_t) sizeof(GLfloat) * length);
	
	return vertsPtr;
*/
	char* _objStr;
	int _dataCount;
	float* verts;
	_objStr=tl_loadfile("\\resource\\obj\\quad.obj",0);
	obj_parse(_objStr,&_dataCount,(int*)&verts,OBJ_UV_VERTEX);
	*ptrVertexLength = _dataCount;
	tl_free(_objStr);
	return verts;
}

static struct Obj_vbo_model* 
load_vbo_6Vertex()
{	
    //uv vertex

	GLfloat _lverts[SPRITE_VERTEX_COUNT_30]={
		0.000000,0.000000,		-0.500000,-0.500000,0.000000,
		1.000000,0.000000,		-0.500000,0.500000,0.000000,
		0.000000,1.000000,		0.500000,-0.500000,0.000000,
		0.000000,1.000000,		0.500000,-0.500000,0.000000,
		1.000000,0.000000,		-0.500000,0.500000,0.000000,
		1.000000,1.000000,		0.500000,0.500000,0.000000
	};
	const int dataType = OBJ_UV_VERTEX;

	struct Obj_vbo_model* vbo;
	char buffer[G_BUFFER_64_SIZE];
	tl_newName(buffer,G_BUFFER_64_SIZE);

	vbo = objVBO_create(buffer,dataType);

	objVBO_pushNode(vbo ,(GLfloat*)_lverts,sizeof(_lverts) / sizeof(GLfloat));
	return vbo;
}
static struct Obj_vbo_model* 
f_objVBO_load4Vertex(){
	int verts,_bufferSize;
	char* _objStr;
	const int dataType = OBJ_UV_VERTEX;
	struct Obj_vbo_model* vbo;
	char buffer[G_BUFFER_64_SIZE];
	_objStr=tl_loadfile("\\resource\\obj\\quad.obj",0);

	tl_newName(buffer,G_BUFFER_64_SIZE);
	vbo = objVBO_create(buffer,dataType);

	obj_parse((char*)_objStr,&_bufferSize,&verts,dataType);
	objVBO_pushExportObj(vbo,_objStr);
	tl_free(_objStr);
	return vbo;
}
/*
 *����VBO
 * uv normal verter��uv vertex��֧��
 */
static struct Obj_vbo_model* 
load_vbo(int ParseType)
{
	if(ParseType == Type_6Vertex)
	{
		return load_vbo_6Vertex();
	}
	else if(ParseType == Type_4Vertex)
	{
		return f_objVBO_load4Vertex();
	}
	return NULL;
}

/*
	����2�����������
*/
static void
setHitTriangle(struct Sprite* spr)
{
	int i = 0;
	struct Vec2 a1,a2,a3,a4;
	if(!spr->hitTriangle){
		//û�����ÿ���ײ����
		return;
	}

	a1.x =  spr->screenX;
	a1.y =	spr->screenY;
	
	a2.x = spr->screenX + spr->mWidth;
	a2.y = spr->screenY;

	a3.x = spr->screenX+ spr->mWidth;
	a3.y = spr->screenY+ spr->mHeight;

	a4.x = spr->screenX;
	a4.y =  spr->screenY+spr->mHeight;

	//a1
	spr->hitTriangle[0] = a1.x ;
	spr->hitTriangle[1] = a1.y ;
	spr->hitTriangle[2] = 0;

	//a2
	spr->hitTriangle[3] = a2.x;
	spr->hitTriangle[4] = a2.y;
	spr->hitTriangle[5] = 0;

	//a3
	spr->hitTriangle[6] = a3.x;
	spr->hitTriangle[7] = a3.y;
	spr->hitTriangle[8] = 0;

	////===================================================

	//a1
	spr->hitTriangle[9] =  a1.x;
	spr->hitTriangle[10] = a1.y;
	spr->hitTriangle[11] = 0;

	//a4
	spr->hitTriangle[12] = a4.x;
	spr->hitTriangle[13] = a4.y;
	spr->hitTriangle[14] = 0;

	//a3
	spr->hitTriangle[15] =  a3.x;
	spr->hitTriangle[16] =  a3.y;
	spr->hitTriangle[17] = 0;
}

/*
	���ò㼶
*/
static void
setLayer(struct Sprite* sprite,int layer)
{
	sprite->pos_z = layer;
}

/*
	����sprite����Ļ����,(��Ļ�����Ͻ�Ϊ0,0��ʼ��)
*/
static void
setSpriteScreenPos(struct EX* ex,struct Sprite* sprite,float x,float y)
{
	sprite->screenX = x;
	sprite->screenY = y;

	sprite->pos_x = x;
	sprite->pos_y = ex->_screenHeight-y-sprite->mHeight;	
}
/*
	����sprite�ĳߴ�
*/
void sprite_resize(struct Sprite* spr,int w,int h)
{
	spr->mWidth = w;
	spr->mHeight = h;

	//spr->screenY = spr->screenY-h;
	//setHitTriangle(spr);//���µ������
	sprite_setpos(spr,spr->screenX,spr->screenY);
}

void sprite_setpos(struct Sprite* spr,int x,int y)
{
	setSpriteScreenPos(ex_getInstance(),spr,x,y);//������Ļ����
	setHitTriangle(spr);//���µ������
}

static void 
InitType(struct Sprite* pSpr)
{
	pSpr->useVBO = 1;
	if(pSpr->useVBO){
		pSpr->parseType = Type_4Vertex;//Type_4Vertex,Type_6Vertex;
		pSpr->vbo=load_vbo(pSpr->parseType);//ʹ��VBO
	}else
		pSpr->vertexs = LoadQuadObj(&pSpr->vertLen);//���ض�������(��VBOʵ��)
}

struct Sprite* 
sprite_create(char* _spriteName,
			  int x,int y,int width,int height,
			  void (*clickCallBack)(struct Sprite* ,int ,int ))
{
	//&ex->myButtonPtr;//
	struct HeadInfo* base = NULL;
	//��ʼ����ť
	struct Sprite* pSpr =	(struct Sprite*)tl_malloc(sizeof(struct Sprite));//new Button;
	memset(pSpr,0,sizeof(struct Sprite));

	pSpr->m_bPressed = 0;

	pSpr->mWidth  = width;
	pSpr->mHeight = height;

	pSpr->hitTriangle = tl_malloc(sizeof(float)*SPRITE_TRIANGLE_COUNT);

	//���ö�����֯��ʽ
	InitType(pSpr);

	pSpr->base = base_create(TYPE_SPRITE_FLIE,_spriteName,0,0,0);
	sprite_setpos(pSpr,x,y);

	base = pSpr->base;
	base->parent = pSpr;
	if(base){
		setv(&base->flags,FLAGS_VISIBLE);//FLAGS_RAY
		pSpr->clickCallBack = clickCallBack;
		ex_add(pSpr);
	}

	pSpr->pos_z = ex_zBuffer();
	
	return pSpr;
}

void 
sprite_setDragScope(struct Sprite* pSpr,int x,int y,int w,int h)
{
	struct HeadInfo* base = base_get((void*)pSpr);

	if(pSpr->mWidth>w ||pSpr->mHeight>h)
	{
		log_color(0xff0000,"������������̫Сsprite����Ļ���ص�λ(mWidth = %d,��ק����w=%d)(mHeight = %d,��ק����h=%d)\n",(int)pSpr->mWidth,w,(int)pSpr->mHeight,h);
		//assert(0);
		return;
	}

	setv(&base->flags,FLAGS_DRAG);
	pSpr->oldx = pSpr->screenX;
	pSpr->oldy = pSpr->screenY;
	pSpr->dragX = x;
	pSpr->dragY = y;
	pSpr->dragWidth = w;
	pSpr->dragHeight = h;
}

/*
	�Ƿ������Ⱦ
*/
int 
sprite_isEnable(int data)
{
	int objType;
	struct HeadInfo* base = base_get((void*)data);
	
	objType = base->curType;

	if(objType == TYPE_SPRITE_FLIE && getv(&(base->flags),FLAGS_VISIBLE))
	{
		return 1;
	}
	return 0;
}

/*
	����sprite����
*/
static void
updateSpriteMat4x4(struct Sprite* p,
				   float x,float y,float z,
				   float sx,float sy,float sz,
				   float rx,float ry,float rz)
{

	Matrix44f xyz,m_scale,m_rx,m_ry,m_rz, result,tmp2,tmp3;
	mat4x4_zero(p->mat4x4);
	mat4x4_zero(result);
	mat4x4_zero(tmp2);
	mat4x4_zero(tmp3);

	//x,y,z
	mat4x4_identity(xyz);
	mat4x4_translate(xyz,x,y,z);

	//scale
	mat4x4_identity(m_scale);
	mat4x4_scale(m_scale,sx,sy,sz);

	//rx
	mat4x4_identity(m_rx);
	mat4x4_rotateX(m_rx,rx);

	//ry
	mat4x4_identity(m_ry);
	mat4x4_rotateY(m_ry,ry);

	//rz
	mat4x4_identity(m_rz);
	mat4x4_rotateZ(m_rz,rz);//PI/2

	multi2(tmp2,m_ry,m_rx);		//ry,rx���ϱ任
	multi2(tmp3,tmp2,m_rz);	//rz  *(ry * rx)���ϱ任
	multi2(result,tmp3,m_scale);//���ž���
	
	multi2(p->mat4x4,xyz,result);//λ�ƾ���
}

//��ȡSprite���õĲ�������,
//���Դ�ͼ���л�ȡ,Ҳ�����Զ����������
static void* 
getMaterial(struct Sprite* p){
	if(p->atals == NULL)
	{
		//printf("p->atals == NULL");
		//assert(0);
		
		if(p->material){
			return p->material;
		}

		//ʹ�������е�һ��Ĭ�ϵĲ���
		return ex_get_defaultMaterial();
	}
	return p->atals->material;
}

//��Ⱦһ���ı���
static void 
renderSprite(struct Sprite* p)
{
	void* material = getMaterial(p);//p->atals->material;

	if(material){
		//void* material = getMaterial(p);//p->atals->material;
		struct HeadInfo* base = base_get(p);
		char* shaderName = 0;
		if(!material){
			printf("(%s)Spriteû�и������\n",((struct HeadInfo*)p->base)->name);
			assert(0);
		}
		

		{
			GMaterial* __mat = (GMaterial*)material;
			//char* type = __mat->glslType;
			shaderName = __mat->glslType;
		}
		if(p->useVBO){
			//��Ⱦ���߲���vboģʽ
			objVBO_renderNode(getvbo(p),material,
				//"spritevbo",
				shaderName,
				p->mat4x4,
				base_get_ploygonLineMode(base),base,NULL);
		}else
			//���ù̶����߷�ʽs
			tmat_renderSprite(material,
							//"sprite",
							shaderName,
							p->mat4x4,p->vertexs,p->vertLen,
							GL_T2F_V3F,
							base_get_ploygonLineMode(base));//��vboģʽ
	}
}

/*
 *���¾���
 */
static void 
updateMatrix(struct Sprite* p)
{
	struct HeadInfo* b = p->base;
	//p->zScale = 1.5;
	updateSpriteMat4x4(p,b->x,b->y,b->z,p->mWidth*p->zScale,p->mHeight*p->zScale,p->zScale,b->rx,b->ry,b->rz);
}

/*
	����һ��sprite 
*/
void 
sprite_drawRender(int data)
{
	struct EX* e = ex_getInstance();

	struct Sprite* spr = (struct Sprite*)data;
	float x = spr->pos_x+spr->mWidth/2;
	float y = spr->pos_y+spr->mHeight/2;
	float z = spr->pos_z;//-2.0ֻ��Ϊ�˰�ť�ɼ�,��������������ֵ
	
	//////////////////////////////////////////////////////////////////////////
	//isChange == 1��ʱ��Ż���¾���
	int isChange = 0;

	struct HeadInfo* base = spr->base;

	const int _Time_Delay_ = 10;//������ӳ�ʱ��(����)		10����ָ�״̬
	
	int btneffect = getv(&base->flags,FLAGS_BUTTON_EFFECT);

	if(!sprite_isEnable((int)spr))
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//λ�Ʒ����仯,���¾���
	if(base->x!=x ||base->y!=y||base->z!=z)
	{
		isChange = 1;
	}

	base->x = x;
	base->y = y;
	base->z = z;
	
	if(btneffect && spr->m_bPressed )
	{
		float targetScale = 0.95;

		if(spr->zScale!=targetScale)
		{
			//��������һ��

			spr->zScale = targetScale;//���ʱ,��ť��߱ȱ仯
			//printf("���һֱ���Ű�ťSprite %f,%f\n",pBtn->m_fWidth,pBtn->m_fHeight);
			base->time=get_longTime()+_Time_Delay_;
			isChange = 1;
		}
	}
	else
	{
		spr->zScale = 1.0;

		if(btneffect &&	base->time!=0 && base->time - get_longTime()<=0)
		{
			//_Time_Delay_����֮��ᴦ��
			isChange = 1;
			base->time = 0;
			//printf("playRun!!!!");
		}
		
	}

	if(spr->m_bPressed)
	{
		//�����갴�µ�ʱ��,��һֱִ�������߼�
		

		/*if(spr->callLuaMouseDown!=NULL)
		{
			ex_callParmLuaFun((const char*)spr->callLuaMouseDown,base->name);
		}*/
		ex_lua_evt_dispatch(spr,EVENT_ENGINE_SPRITE_CLICK_DOWN,base->name);
	}
	else
	{
	}

	/*if(spr->screenX==0 && spr->screenY == 20)
	{
		ex_showLogFloat(spr->zScale);
	}*/

	if(isChange)
	{
		updateMatrix(spr);
	}

	renderSprite(spr);
}

/*
	�Ƿ���������ק��Χ����
*/
static int
haveDragScope(struct Sprite* ptr){
	//if(ptr->m_fPosX+ptr->dragWidth - ptr->m_fWidth)
	if(ptr->dragWidth || ptr->dragHeight)
	{
		return 1;
	}
	return 0;
}
//static void 
//fLuaDragMove(const char* luaFunName,const char* name,float progress)
//{
//	lua_State* L =ex_getInstance()->mylua;
//	lua_getglobal(L,luaFunName);
//	lua_pushstring(L,name);
//	lua_pushnumber(L,progress);
//	if(lua_pcall(L,2,0,0)!=0){
//		printf("error %s\n", lua_tostring(L,-1));
//	}
//}
/*
	�Ƿ�������
*/
static int 
IsDragDirection(struct Sprite* sprite)
{
	return sprite->dragDirection;
}
//========================================================
//#define _ZERO_


//����ק�ؼ��Ŀ�
static float 
GetWidth(struct Sprite* p)
{
#ifdef _ZERO_
	return 0.0f;
#endif
	return p->mWidth;
}

//����ק�ؼ��ĸ�
static float 
GetHeight(struct Sprite* p)
{
#ifdef _ZERO_
	return 0.0f;
#endif
	return p->mHeight;
}
static void 
changeDragXY(struct Sprite* p,int* px,int* py){
//���ScrollBar ScrollBarBG_Click�ӿ�


	if(haveDragScope(p))
	{
		int cx0,cx1,cy0,cy1,oldx,oldy;
		float progress;
	
		oldx = p->oldx;
		oldy = p->oldy;

		cx0 = p->dragX+oldx;
		cx1 = oldx+p->dragWidth - GetWidth(p);

		cy0 = p->dragY+oldy; 
		cy1 = oldy+p->dragHeight- GetHeight(p);

		if(*px <= cx0)
			*px = cx0;
		else if(*px>=cx1) 
			*px = cx1;

		if(*py <= cy0) 
			*py = cy0;
		else if(*py>=cy1)
			*py = cy1;
		
		if(IsDragDirection(p))
			progress = (p->screenY-p->oldy)/(p->dragHeight - GetWidth(p));//�������
		else
			progress = (p->screenX-p->oldx)/(p->dragWidth - GetHeight(p));//�������
		
		//if(p->callLuaDragMove)
		//	fLuaDragMove(p->callLuaDragMove,progress);//���͸����¼�
		//if(p->callLuaDragMove)
		//{
		//	struct HeadInfo* b = base_get(p);
		//	fLuaDragMove(p->callLuaDragMove,b->name,progress);
		//}

		{
			//�����¼���lua
			char _str[G_BUFFER_64_SIZE];
			struct HeadInfo* b = base_get(p);
			sprintf_s(_str, G_BUFFER_64_SIZE,"%s,%d,%d,%.3f",b->name,*px,*py,progress);	
			//printf("c=[%s]\n",_str);
			ex_lua_evt_dispatch(p,EVENT_ENGINE_SPRITE_CLICK_MOVE,_str);
		}
	}
}
//========================================================
void 
sprite_mouseMove(int data)
{
	struct EX* e = ex_getInstance();	
	if(sprite_isEnable(data))
	{
		struct HeadInfo* base = base_get((void*)data);
		if(getv(&base->flags,FLAGS_DRAG))
		{
			struct Sprite* ptr = (struct Sprite*)data;
			if(ptr->m_bPressed){
				//��갴��,ֻ����������귢���仯��ʱ��				
				int x = e->mouseState.moveX - ptr->mouseDownX;
				int y = e->mouseState.moveY - ptr->mouseDownY;
				changeDragXY(ptr,&x,&y);
				sprite_setpos(ptr,x,y);
			}
		}
	}
}
/*
	��갴��
*/
void sprite_action(const int data)
{
	if(sprite_isEnable(data))
	{
		struct EX* ex = ex_getInstance();

		struct Sprite* btn = (struct Sprite*)data;
		struct HeadInfo* base;
		if(!btn)
		{
			return;
		}

		base = btn->base;

		if(ex->mouseState.button==GLUT_LEFT_BUTTON){
			switch(ex->mouseState.action)
			{
				case GLUT_DOWN:
					
					

					//������£�
					//if( OnMouseDown(btn, ex->mouseState.xMouse, ex->mouseState.yMouse) ){
						//printf("to do something!\n");
					//}
					break;

				case GLUT_UP:
					//OnMouseUp(btn);
					btn->m_bPressed = 0;
					break;
			}
		}
	}
}

//��Ⱦһ����Ͷsprite
void sprite_updatePos(int data)
{
	if(sprite_isEnable(data))
	{
		struct Sprite* sprite = (struct Sprite*)data;
		sprite_setpos(sprite,sprite->screenX,sprite->screenY);
	}	
}

/*
	����sprite
*/
void sprite_dipose(struct Sprite* spr)
{
	//spr->ptr_luaCallBack = 0;

	LStack_delNode(ex_getInstance()->renderList,(int)spr);//����Ⱦ�ڵ��б����Ƴ�

	if(spr->hitTriangle)
		tl_free(spr->hitTriangle);

	if(spr->vertexs)
		tl_free(spr->vertexs);
	
	//ɾ�����ʾ��
	if(spr->material){
		tmat_dispose(spr->material);
		spr->material = NULL;
	}
	/*if(spr->material)
		tmat_dispose(spr->material);*/
	spr->atals = NULL;
	
	if(spr->base){
		base_dispose(spr->base);
		spr->base = NULL;
	}
	
	//if(spr->callLuaDragMove)
	//{
	//	tl_free(spr->callLuaDragMove);
	//}

	/*if(spr->callLuaMouseDown)
	{
		tl_free(spr->callLuaMouseDown);
	}*/

	/*if(spr->luaTablePtr)
	{
		tl_free(spr->luaTablePtr);
	}*/
	
	if(spr->vbo)
	{
		objVBO_dispose(spr->vbo);
	}

	tl_free(spr);
}

/*
	Ϊsprite����ͼ��
*/
void 
sprite_texName(struct Sprite* ptr,
					const char* texName,
					struct AtlasNode* ptrNode)
{
	float width,height;
	struct AtlasNode n;

	if(ptr->atals){
	//��ȡͼ������,��䵽ptrNode������
		atals_tex(ptr->atals,texName,ptrNode);

		width = ptr->atals->width;
		height = ptr->atals->height;
	}
	else
	{
		log_code(ERROR_NOT_EXIST_Atals);
		printf("��spriteû��ͼ��Atals��ʱ��,�޷�����sprite_texName�ӿ�!\n");
		assert(0);
		
		ptrNode->x = 0;
		ptrNode->y = 0;
		ptrNode->width = ptr->mWidth;
		ptrNode->height = ptr->mHeight;
		width=ptr->mWidth;
		height=ptr->mHeight;
	}

	n = *ptrNode;

	//����ͼ��UV.
	sprite_setUV(ptr,		
		1.0f-(height-n.y)/height+1,
		1.0f-(width-n.x)/width+1,
		n.height/height,
		n.width/width
		); 
}

void 
sprite_rotateZ(struct Sprite* ptr,float rz)
{
	struct HeadInfo* b = (struct HeadInfo*)ptr->base;
	b->rz = rz;
	updateMatrix(ptr);
}