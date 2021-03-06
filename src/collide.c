#include <string.h>
#include <assert.h>

#include "tools.h"
#include "tl_malloc.h"
#include "collide.h"
#include "obj.h"
#include "gettime.h"
#include "md2.h"
#include "ex.h"
#include "base.h"
static void 
pushCollionBox(struct CollideBox* ptr,int verts,int vertsCount)
{
	struct VertexData* pvd = tl_malloc(sizeof(struct VertexData));
	tlgl_createRay(pvd,(float*)verts,vertsCount);
	LStack_push(ptr->list,pvd);
}

static void
loadObj(struct CollideBox* ptr,const char* url)
{
	char* _objStr;
	int verts,_bufferSize;
	_objStr=tl_loadfile(url,0);
	obj_parse((char*)_objStr,&_bufferSize,&verts,OBJ_UV_VERTEX_NORMAL);

	pushCollionBox(ptr,verts,_bufferSize/(UV_GAP+VERTEX_GAP+NORMAL_GAP));

	tl_free((void*)verts);
	tl_free(_objStr);
}
/*
 *构造md2的静态盒子列表
 */
static void
push_md2Frame(struct CollideBox* ptr,void* _md2Parse,int gap,int frameCount)
{
	int i;
	struct MD2_Frame* frame;
	
	int _numFrames = md2parse_totalFrames(_md2Parse);// _md2Parse->numFrames;
	if(frameCount > 0){
		_numFrames = frameCount;
	}

	for(i = 0;i < _numFrames;i++){
		frame = md2parse_getFrame(_md2Parse,i);//&(_md2Parse->pframe[i]);
		pushCollionBox(ptr,(int)frame->vertices,frame->vertCount);
	}
}
static void 
loadmd2(struct CollideBox* ptr,const char* url,int frameCount)
{
	char* _objStr;
	int fileSize;
	void * _parse;
	int dataType = OBJ_UV_VERTEX_NORMAL;
	_objStr=tl_loadfile((char*)url,&fileSize);
	
	//_parse=(struct MD2_ParseObj*)tl_malloc(sizeof(struct MD2_ParseObj));
	_parse = md2parse_load(_objStr,fileSize);
	push_md2Frame(ptr,_parse,ex_get_gap(dataType),frameCount);
	
	md2parse_dispose(_parse);
	tl_free(_objStr);
}

/*
 *加载一个碰撞盒子
 */
struct CollideBox* 
collide_load(const char* url,int frameCount)
{
	char suffix[G_BUFFER_16_SIZE];
	struct CollideBox* ptr = tl_malloc(sizeof(struct CollideBox));
	memset(ptr,0,sizeof(struct CollideBox));
	
	ptr->list = LStack_create();

	tl_getSuffixByPath((char*)url,suffix,G_BUFFER_16_SIZE);
	
	if(frameCount < 0){
		log_code(ERROR_BAD_VALUE);
		return ptr;
	}

	if(!strcmp(suffix,"obj"))
	{
		loadObj(ptr,url);
	}
	else if(!strcmp(suffix,"md2"))
	{
		loadmd2(ptr,url,frameCount);
	}else{
		log_color(0xff0000,"未实现类型%s\n",url);
		assert(0);
	}
	return ptr;
}

/*
 *删除一个静态盒子节点
 */
static void
delCollideNode(int data,int parms)
{
	struct VertexData* vd = (struct VertexData*)data;
	tl_free(vd->vertex);
	vd->vertex=0;
	tl_free(vd);
}

void 
collide_dispose(struct CollideBox* ptr)
{	 
	LStack_ergodic_t(ptr->list,delCollideNode,0);
	LStack_delete(ptr->list);
	tl_free(ptr);
}
struct VertexData* 
collide_getIndex(struct CollideBox* ptr,int index)
{
	return (struct VertexData*)LStatk_getAddressByIndex(ptr->list,index);
}

struct VertexData*
collide_cur(struct CollideBox* ptr)
{
	return collide_getIndex(ptr,ptr->index);
}