//warning C4005: '' : macro redefinition	重复定义的宏
//warning C4311: 'type cast'	类型转化

//#pragma comment(lib,"my.lib")

//用默认的MALLOC会引起相关的bug,谨慎用之

#include <stdio.h>
#include <assert.h>
#include <string.h>

#pragma comment (lib,"lua5.1.lib")
#pragma comment (lib,"common.lib")
#pragma comment (lib,"ftfont.lib")
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

//=============================================
#include "ftfont.h"
#include "common.h"
#include "tween.h"

#include "tools.h"
#include "gettime.h"

#include "ex.h"
#include "tmat.h"

#include "md5.h"
#include "jgl.h"
#include "atlas.h"

#include "sprite.h"

#include "obj_vbo.h"

#include "node.h"
#include "animtor.h"
#include "tl_malloc.h"
#include "xml.h"
#include "psv.h"
#include "text.h"
#include "array.h"
#include "evt.h"
#include "str.h"
#include "base.h"
#include "frame.h"

#include "ftext.h"

//=============================================

/*
 *主目录路径 处理成动态自适应，如果找不到第一个路径就找第二个路径
 */
#define _MAIN_PATH_ "c:\\gl_3dgame"

//#define _TEST_GL_TEX_

#ifdef _TEST_GL_TEX_
static void
f_gl_tex(){
	//printf("_TEST_GL_TEX_\n");


	int width = 128;
	int height = 32;
	struct Sprite* spr = sprite_create("spr1",10,10,width,height,0);
	spr->material = tmat_create_rgba("font1",width,height,GL_BGRA);
	tmat_set_discardAlpha(spr->material,1);//设置alpha不透明

	printf("w = %d h = %d\n",width,height);
}
#endif





/*
	lua脚本入口
*/
static char g_initLua[G_BUFFER_64_SIZE];

//引擎引用
//EngineX* engine;
struct EX* ex=NULL;

void scene_specialKeys(int key, int x, int y){
	float _cameMoveSpeed = -0.5f;
	struct EX* ex = ex_getInstance(); //p->enginePtr;
	struct ECamera cam = ex->cam;
	switch(key) 
	{ 
	case GLUT_KEY_F1 : 
		//ex_info();
		break;
	case GLUT_KEY_F2:
		setv(&(ex->flags),EX_FLAGS_RAY);
		break;

	case GLUT_KEY_F3:
		resetv(&(ex->flags),EX_FLAGS_RAY);
		break;

	case GLUT_KEY_F11:
		{
			//f_switchBoxLine();
			break;
		}
	case GLUT_KEY_F12:
		//
		break;

	case GLUT_KEY_UP:
		cam.z-=_cameMoveSpeed;
		break;
	case GLUT_KEY_DOWN:
		cam.z+=_cameMoveSpeed;
		break;
	case GLUT_KEY_LEFT:
		cam.x-=_cameMoveSpeed;
		break;
	case GLUT_KEY_RIGHT:
		cam.x+=_cameMoveSpeed;
		break;
	case GLUT_KEY_PAGE_UP:
		cam.y+=_cameMoveSpeed;
		break;
	case GLUT_KEY_PAGE_DOWN:
		cam.y-=_cameMoveSpeed;
		break;
	}
	ex_cam_set_pos(cam.x,cam.y,cam.z);
}

/*
解析成两个char*
*/
static void
f_pa1rse_key_value(const char*str,char*key,char*v){
	int n = str_pos(str,",",1);
	int l = strlen(str);
	memset(key,0,G_BUFFER_64_SIZE);
	memset(v,0,G_BUFFER_64_SIZE);
	tl_strsub(str,key,0,n - 1);
	tl_strsub(str,v,n + 1,l - 1);
}
void onSpecialKeys(int key, int x, int y) {
	scene_specialKeys(key,x,y);
}
static void 
playCallBack(void* data){
	struct HeadInfo* _pObj = (struct HeadInfo*)data;
	frame_set(_pObj->frameAnim,"stand");
	//base_playOnce(_pObj->frameAnim,"stand");
	//printf("处理完成 %s 设置动作 %ld\n",_pObj->name,get_longTime());
}

/*
*	鼠标2D转3D
*/
void pickScreenTo3D(GLint button, GLint state, GLint x, GLint y){
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLint realy;
	GLdouble wx, wy, wz;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) 
		{
			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
			realy = viewport[3] - y - 1;
			printf("Coordinate at curosr are (%4d, %4d, %4d)\n", x, y,viewport[3]);
			gluUnProject(x, realy, 0, mvmatrix, projmatrix, viewport, &wx,
				&wy, &wz);
			printf("World coords at z=0 are (%f, %f, %f)\n", wx, wy, wz);
			gluUnProject(x, realy, 1, mvmatrix, projmatrix, viewport, &wx,
				&wy, &wz);
			printf("World coords at z=1 are (%f, %f, %f)\n", wx, wy, wz);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			//exit(0); 
			break;
		}
	default: break;
	}
}

/*
	射线拾取多边形
*/
static void 
f_rayPickCallBack(int evtId,void* data){
	

	struct EX* ex = ex_getInstance();
	struct HitResultObject* last = (struct HitResultObject*)data;
	struct HeadInfo* _node = ex_find_headinfo(ex,last->name);
	
	if(_node){		
		if(_node->luaPickCallBack){
			int n = (int)ex_find_ptr(ex,(const char*)_node->name);
			ex_callIntLuaFun(_node->luaPickCallBack,n);//lua拾取回调	
		}
				
		//printf("(%s)射线拾取到的3D坐标:\t%.3f\t%.3f\t%.3f\t%s\tcurType=%d\n",last->name,last->x,last->y,last->z,_node->suffix,_node->curType);

	}
}

/*
	lua_pcall(lua_State *L, int nargs, int nresults, int errfunc)
	nargs:参数个数，nresults：返回值个数，errFunc：错误处理函数，0表示无，表示错误处理函数在栈中的索引
*/
static void
f_onSpriteClickCallBack(struct Sprite* sprite,int localX,int localY){
	//printf("testBtnClickCallBack	spriteName:%s,%d,%d\n",((struct HeadInfo*)sprite->base)->name,localX,localY);	
	//sprite_dipose(spr);

	{
		////C调用lua脚本	btnCallBack
		//if((size_t)strlen((const char*)sprite->callLuaFunName))
		//{
		//	lua_State* L =mylua;
		//	lua_getglobal(L,(const char*)sprite->callLuaFunName);
		//	lua_pushinteger(L,(int)sprite);
		//	//lua_pushinteger(L,sprite->ptr_luaCallBack);
		//	//lua_pushlstring();
		//	if(lua_pcall(L,1,0,0)!=0){
		//		printf("error %s\n", lua_tostring(L,-1));
		//	}
		//}


		//鼠标点击了Sprite

		struct HeadInfo* b = base_get(sprite);
		sprite->mouseLocalX = localX;
		sprite->mouseLocalY = localY;
		ex_lua_evt_dispatch(sprite,EVENT_ENGINE_SPRITE_CLICK,b->name);

		//if((int)strlen((const char*)sprite->callLuaFunName)>0)
		//{
		//	sprite->mouseLocalX = localX;
		//	sprite->mouseLocalY = localY;
		//	ex_callParmLuaFun((const char*)sprite->callLuaFunName,b->name);
		//	printf("name->%s\n",b->name);
		//}
	}
}

/*加载图集*/
static int
REG_atals_load(lua_State *L){
	int n = lua_gettop(L);
	const char* path = lua_tostring(L,1);
	const char* name = lua_tostring(L,2);
	struct Atals* ptr = atals_load(path,name);
	lua_pushinteger(L,(int)ptr);
	return 1;
}
/*
注册创建Sprite
spr=sprite_create("btnName",0,100,72,24)
*/
static int 
REG_sprite_create(lua_State *L)
{
	int n = lua_gettop(L);//获取脚本中的参数个数

	//name
	const char* _name = lua_tostring(L,1);
	
	//x
	int x = lua_tointeger(L,2);

	//y
	int y = lua_tointeger(L,3);

	//width
	int w = lua_tointeger(L,4);
	
	//height
	int h = lua_tointeger(L,5);
	
	////点击的回调函数文本
	//const char* _clickFuncString = lua_tostring(L,6);
	//
	////移动函数文本
	//const char* _move = lua_tostring(L,7);
	//
	////鼠标按下去的回调
	//const char* _mouseDown = lua_tostring(L,8);
	
	//拖动的方向
	int direction = lua_tointeger(L,6);//9
	//////////////////////////////////////////////////////////////////////////
	struct Sprite* ptr;

	ptr = sprite_create((char*)_name,x,y,w,h,f_onSpriteClickCallBack);
	
	//copyMoveFuncStr(ptr,_move);
	//copyMouseDownFuncStr(ptr,_mouseDown);
	
	ptr->dragDirection = direction;//拖拽的方向

	//这里去引擎中的图集myAtals
	//ptr->atals = ex_getInstance()->myAtals;
	{
		//指定图集
		//int atals = lua_tointeger(L,10);//10
		//ptr->atals = (struct Atals*)atals;
	}
	lua_pushinteger(L,(int)ptr);
	return 1;
}
//为sprite设置图集
static int 
REG_sprite_bindAtals(lua_State *L)
{
	int n = lua_gettop(L);//获取脚本中的参数个数
	struct Sprite* sprite = (struct Sprite*)lua_tointeger(L,1);//1
	int atals = lua_tointeger(L,2);//2

	//设置图集
	sprite->atals = (struct Atals*)atals;
	return 0;
}

static int
REG_sprite_bindMaterial(lua_State *L){
	struct Sprite* sprite = (struct Sprite*)lua_tointeger(L,1);//1
	int material = lua_tointeger(L,2);//2
	sprite->material = (void*)material;
	return 0;
}

static int
REG_sprite_setDragScope(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	int w = lua_tointeger(L,4);
	int h = lua_tointeger(L,5);
	sprite_setDragScope((struct Sprite*)ptr,x,y,w,h);
	return 0;
}

//void sprite_setDragScope(struct Sprite* pSpr,int x,int y,int w,int h);


//	void sprite_texName(struct Sprite* spr,const char* texName)
static int 
REG_sprite_texName(lua_State *L)
{
	struct AtlasNode node;

	int ptr=lua_tointeger(L,1);
	const char* tex=lua_tostring(L,2);
	
	sprite_texName((struct Sprite*)ptr,tex,&node);

	lua_pushnumber(L,node.x);
	lua_pushnumber(L,node.y);
	lua_pushnumber(L,node.width);
	lua_pushnumber(L,node.height);
	return 4;
}

static int 
REG_tf_create(lua_State *L)
{
	int n = lua_gettop(L);
	int bufferSize = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	int r,g,b;
	struct TextField* tf;
	if(n==3){
		r = g = b = 1.0f;//无参数,就默认的现实白色的文本
	}else{
		r = lua_tonumber(L,4);
		g = lua_tonumber(L,5);
		b = lua_tonumber(L,6);
	}
	tf=tf_create(bufferSize,x,y,r,g,b);
	lua_pushinteger(L,(int)tf);
	lua_pushinteger(L,tf->height);
	//printf("REG_tf_create参数个数%d\n",n);
	return 2;
}
static int 
REG_tf_setPos(lua_State *L)
{
	int tf = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	tf_setPos((struct TextField*)tf,x,y);
	return 0;
}
static int 
REG_tf_setText(lua_State *L)
{
	int tf = lua_tointeger(L,1);
	const char* str = lua_tostring(L,2);
	tf_setText((struct TextField*)tf,str);
	return 0;
}

static int 
REG_sprite_setpos(lua_State *L)
{
	int sprite = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	sprite_setpos((struct Sprite*)sprite,x,y);
	return 0;
}
static int 
REG_tf_dispose(lua_State *L)
{
	int tf = lua_tointeger(L,1);
	tf_dispose((struct TextField*)tf);
	return 0;
}


/*
	注册setUV,设置2D精灵的UV
*/
static int
REG_setUV(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	float x = lua_tonumber(L,2);
	float y = lua_tonumber(L,3);
	float w = lua_tonumber(L,4);
	float h = lua_tonumber(L,5);
	sprite_setUV((struct Sprite*)ptr,x,y,w,h);
	return 0;
}

/*
	注册setv
*/
static int
REG_setv(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	int flags = lua_tointeger(L,2);
	ex_setv((void*)ptr,flags);


	////printf("flags=%d\n",flags);
	//if(!flags){
	//	printf("设置了一个(%d)flags\n",flags);
	//	assert(0);
	//}
	//
	//if(ptr!=0){
	//	struct HeadInfo* base=base_get((void*)ptr);
	//	setv(&base->flags,flags);
	//}
	//else
	//{
	//	printf("ptr=%d 取ptr错误\n",ptr);
	//	assert(0);
	//}
	return 0;
}
/*
	注册resetv
*/
static int
REG_resetv(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	int flags = lua_tointeger(L,2);
	//struct HeadInfo* base=base_get((void*)ptr);
	//resetv(&base->flags,flags);
	ex_resetv((void*)ptr,flags);
	return 0;
}
/*
	注册getv
*/
static int
REG_getv(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	int flags = lua_tointeger(L,2);
	//struct HeadInfo* base=base_get((void*)ptr);
	//lua_pushinteger(L,getv(&base->flags,flags));
	lua_pushinteger(L,ex_getv((void*)ptr,flags));
	return 1;
}

/*
	注册tmat_createMaterial
*/
static int
REG_tmat_createMaterial(lua_State *L){
	const char* glslName = lua_tostring(L,1);
	//const char* tex0 = lua_tostring(L,2);//第1张贴图
	//const char* tex1 = lua_tostring(L,3);//第2张贴图
	void* material;

	#ifdef DEBUG 
		int _t = get_longTime();
		log_color(0xff00ff,"######## 开始构建材质,使用shader(%s)\n",glslName);
	#endif

	material = tmat_create(glslName,8,
		(char*)lua_tostring(L,2),
		(char*)lua_tostring(L,3),
		(char*)lua_tostring(L,4),
		(char*)lua_tostring(L,5),
		(char*)lua_tostring(L,6),
		(char*)lua_tostring(L,7),
		(char*)lua_tostring(L,8),
		(char*)lua_tostring(L,9)
		);
	//printf("[%s]\n",lua_tostring(L,4));
	#ifdef DEBUG
		log_color(0xff00ff,"######## 构建材质及其相关纹理 0x%0x 共消耗 %ld 毫秒\n",
			(int)material,(get_longTime()-_t));
	#endif

	lua_pushinteger(L,(int)material);

	return 1;
}
/************************************************************************/
/* 创建一个指定宽高的位图                                                                     */
/************************************************************************/
static int
REG_tmat_createMaterialTex(lua_State *L){
	const char* glslName = lua_tostring(L,1);
	int w = lua_tointeger(L,2);
	int h = lua_tointeger(L,3);
	void* material = tmat_createTex(glslName,w,h);
	lua_pushinteger(L,(int)material);
	return 1;
}

/*
	注册设置材质
	void 
	setMaterial(void* ptr,void* material)
*/
static int
REG_setMaterial(lua_State* L){
	int ptr = lua_tointeger(L,1);
	int material = lua_tointeger(L,2);
	//setMaterial((void*)ptr,(void*)material);
	//((struct HeadInfo*)base_get((void*)ptr))->tmat = (struct GMaterial*)material;
	ex_set_material((void*)ptr,(void*)material);
	return 0;
}
/*
	注册	int load_model(const char* url,char* name,float x,float y,float z,float scale);
*/
static int
REG_load_model(lua_State *L)
{
	const char* name=lua_tostring(L,1);
	const char* url = lua_tostring(L,2);
	float x = lua_tonumber(L,3);
	float y = lua_tonumber(L,4);
	float z = lua_tonumber(L,5);
	float scale = lua_tonumber(L,6);
	
	void* ptr = ex_load_model((char*)name,(char*)url,E_RenderModeNormal);
	if(scale == 0)	scale = 1.0;
	base_set_scale((void*)base_get(ptr),scale);

	lua_pushinteger(L,(int)ptr);
	return 1;
}

/************************************************************************/
/* 单元测试                                                             */
/************************************************************************/
int REG_test_unit_01(lua_State *L);
int REG_test_unit_01_init(lua_State *L);
static int
REG_test_unit_02(lua_State *L){
	const char* name=lua_tostring(L,1);
	float value = lua_tonumber(L,2);
	int n = (int)ex_find_ptr(ex,name);
	struct HeadInfo* h = ex_find_headinfo(ex,name);
	

	Quat4_t s;
	Quat4_t e;
	Quat4_t o;

	s[W] = 0;
	s[X] = 0;
	s[Y] = 1;
	s[Z] = 0;

	e[W] = 1;
	e[X] = 1;
	e[Y] = 0;
	e[Z] = 0;
	
	Quat_slerp(s,e,value,o);
	
	Quat_to_matrrix(o,h->quat_m);
	base_updateMat4x4(h);
	return 0;
}

/*
	日志字符缓冲区512字节,打印当前内存池的内存
*/
static void 
f_showCurMemery512(const char* str){
	//if(tl_memGetStat()){

	int totleByte;

	char buffer[G_BUFFER_512_SIZE];

	memory_get_info(&totleByte,0);

	memset(buffer,0,G_BUFFER_512_SIZE);
	sprintf_s(buffer,G_BUFFER_512_SIZE,"%s -->%d bytes",str,totleByte);
	printf("%s\n",buffer);
	//}
}
/*
	输出当前已经使用了的字节大小
*/
static int
REG_tl_showCurMemery512(lua_State *L)
{
	const char* str = lua_tostring(L,1);
	f_showCurMemery512(str);
	return 0;
}

/*
	加载VBO模型
*/
static int
REG_load_VBO_model(lua_State *L){
	const char* name = lua_tostring(L,1);
	const char* url = lua_tostring(L,2);
	void* ptr = ex_load_model((char*)name,(char*)url,E_RenderModeVBO);
	lua_pushinteger(L,(int)ptr);
	return 1;
}

static int
REG_ftext(lua_State *L){
	void* ptr = (void*)lua_tointeger(L,1);
	
	const char* funcName = lua_tostring(L,2);

	if(!strcmp(funcName,"create")){
		const char* name = lua_tostring(L,3);
		const char* size = lua_tostring(L,4);
		float w,h,fw,fh;
		sscanf_s(size,"%f,%f,%f,%f",&w,&h,&fw,&fh);
		lua_pushinteger(L,(int)ftext_create((char*)name,w,h,fw,fh));
		return 1;
	}
	if(ptr == 0){
		printf("未找到ftext句柄引用\n");
		assert(0);
	}
	if(!strcmp(funcName,"setpos")){
		const char* pos = lua_tostring(L,3);
		float x,y;
		sscanf_s(pos,"%f,%f",&x,&y);
		//lua_pushinteger(L,(int)ftext_create((char*)name));
		ftext_setpos(ptr,x,y);
		return 0;
	}
	if(!strcmp(funcName,"parse")){
		int w,h;
		ftext_parse(ptr,lua_tostring(L,3),&w,&h);
		lua_pushinteger(L,w);
		lua_pushinteger(L,h);
		return 2;
	}
	if(!strcmp(funcName,"dispose")){
		ftext_dispose(ptr);
		return 0;
	}
	if(!strcmp(funcName,"clear")){
		ftext_clear(ptr);
		return 0;
	}

	if(!strcmp(funcName,"setchar")){
		const char* chars = lua_tostring(L,3);
		const char* pos = lua_tostring(L,4);
		float x,y;
		int w,h;
		sscanf_s(pos,"%f,%f",&x,&y);
		ftext_set(ptr,(char*)chars,x,y,&w,&h);
		lua_pushinteger(L,w);
		lua_pushinteger(L,h);
		return 2;
	}

	return 0;
}
static int
REG_core_xml(lua_State *L){
	const char* funcName = lua_tostring(L,1);

	//加载xml
	if(!strcmp(funcName,"load")){
		const char* url = lua_tostring(L,2);
		char* str = tl_loadfile(url,0);//lua_tostring(L,2);
		void* xml = xml_parse(str);
		lua_pushinteger(L,(int)xml);
		return 1;
	}

	//文本加载xml
	if(!strcmp(funcName,"loadstr")){
		const char* str = lua_tostring(L,2);
		//char* str = tl_loadfile(url,0);//lua_tostring(L,2);
		void* xml = xml_parse(str);
		lua_pushinteger(L,(int)xml);
		return 1;
	}
	
	//删除xml
	if(!strcmp(funcName,"del")){
		int ptr = lua_tointeger(L,2);
		xml_del((void*)ptr);
		//xml_del((void*)lua_tonumber(L,2));
		return 0;
	}

	if(!strcmp(funcName,"getNodeByIndex")){
		int xml = lua_tointeger(L,2);
		int index = lua_tointeger(L,3);
		int n = (int)xml_getNodeByIndex((void*)xml,index);
		lua_pushinteger(L,n);
		return 1;
	}

	if (!strcmp(funcName,"getrow"))
	{
		int xml = lua_tointeger(L,2);
		const char* parms = lua_tostring(L,3);
		const char* parmsValue = lua_tostring(L,4);
		lua_pushinteger(L,(int)xml_getrow((void*)xml,parms,parmsValue));
		return 1;
	}

	if(!strcmp(funcName,"getfloat"))
	{
		int _node = lua_tointeger(L,2);
		const char* key = lua_tostring(L,3);
		lua_pushnumber(L,xml_getfloat((void*)_node,key));
		return 1;
	}

	if (!strcmp(funcName,"getstr"))
	{
		int _node = lua_tointeger(L,2);
		const char* key = lua_tostring(L,3);
		int bufferSize =  lua_tointeger(L,4);
		char* buffer = tl_malloc(bufferSize);
		xml_getstr((void*)_node,key,buffer,bufferSize);
		lua_pushstring(L,buffer);
		tl_free(buffer);
		return 1;
	}

	//const char* str = lua_tostring(L,3);
	
	return 0;
}


static float
toFloat(const char* str)
{
	float f;
	sscanf_s(str,"%f",&f);
	return f;
}
static int
toInt(const char* str)
{
	int f;
	sscanf_s(str,"%d",&f);
	return f;
}

/*
 *获取对象的属性
 */
static int
REG_get_attr(lua_State *L)
{
	int ptr = lua_tointeger(L,1);			//对象指针如Sprite,Ent3d,md2,md5,text
	const char* attrKey = lua_tostring(L,2);//获取的属性关键字
	//////////////////////////////////////////////////////////////////////////
	struct HeadInfo* base =base_get((void*)ptr);
	/*if(!base)
	{
		printf("找不到对象0x%0x\n",ptr);
		return 0;
	}*/
	//获取文本的宽度
	if(!strcmp(attrKey,"textWidth"))
	{
		int textWidth=tf_getWidth((struct TextField*)ptr);
		lua_pushinteger(L,textWidth);
	}


	
	//获取对象的类型
	if(!strcmp(attrKey,"type")){
		lua_pushinteger(L,base->curType);
	}

	//当前的动作帧(0开始)
	if(!strcmp(attrKey,"getCurFrame"))
	{
		if(base->curType == TYPE_MD5_FILE){
			int frame =  md5_getFrameIndex((struct MD5*)ptr);
			lua_pushinteger(L,frame);
		}
		else
		{
			log_code(ERROR_GET_FRAME);
		}
		return 1;
	}
	
	//获取当前md5,md2的关键帧数量
	if(!strcmp(attrKey,"frameCount"))
	{
		if(base->curType == TYPE_MD5_FILE){
			lua_pushinteger(L,md5_getTotalFrame((struct MD5*)ptr));
		}
		else{
			log_code(ERROR_PTR_TYPE_FAIL);
			lua_pushinteger(L,1);
		}
		return 1;
	}


	//获取鼠标点击的坐标
	if(!strcmp(attrKey,"spriteLocalXY"))
	{
		struct Sprite* sprite = (struct Sprite*)ptr;
		lua_pushinteger(L,sprite->mouseLocalX);
		lua_pushinteger(L,sprite->mouseLocalY);
		return 2;
	}

	if(!strcmp(attrKey,"spriteSize"))
	{
		struct Sprite* sprite = (struct Sprite*)ptr;
		lua_pushnumber(L,sprite->mWidth);
		lua_pushnumber(L,sprite->mHeight);
		return 2;
	}

	if(!strcmp(attrKey,"spritePos"))
	{
		struct Sprite* sprite = (struct Sprite*)ptr;
		lua_pushnumber(L,sprite->screenX);
		lua_pushnumber(L,sprite->screenY);
		return 2;
	}
	
	if(!strcmp(attrKey,"dragDirection")){
		struct Sprite* sprite = (struct Sprite*)ptr;
		lua_pushnumber(L,sprite->dragDirection);
		return 1;
	}

	if(!strcmp(attrKey,"x"))
	{
		float x = base->x;
		lua_pushnumber(L,x);
	}
	if(!strcmp(attrKey,"y"))
	{
		float y = base->y;
		lua_pushnumber(L,y);
	}
	if(!strcmp(attrKey,"z"))
	{
		float z = base->z;
		lua_pushnumber(L,z);
	}
	if(!strcmp(attrKey,"scale"))
	{
		float scale = base->scale;
		lua_pushnumber(L,scale);
		return 1;
	}
	if(!strcmp(attrKey,"xyz"))
	{
		lua_pushnumber(L,base->x);
		lua_pushnumber(L,base->y);
		lua_pushnumber(L,base->z);
		return 3;
	}
	if(!strcmp(attrKey,"rotate"))
	{
		lua_pushnumber(L,base->rx);
		lua_pushnumber(L,base->ry);
		lua_pushnumber(L,base->rz);
		return 3;
	}

	if(!strcmp(attrKey,"suffix"))
	{
		//获取文件后缀
		lua_pushstring(L,base->suffix);
		return 1;
	}
	if(!strcmp(attrKey,"campos"))
	{
		//float x = base->x;
		struct ECamera cam = ex->cam;
		lua_pushnumber(L,cam.x);
		lua_pushnumber(L,cam.y);
		lua_pushnumber(L,cam.z);
		return 3;
	}

	if(!strcmp(attrKey,"screenSize")){
		struct EX* ex = ex_getInstance();
		lua_pushnumber(L,ex->_screenWidth);
		lua_pushnumber(L,ex->_screenHeight);
		return 2;
	}
	if(!strcmp(attrKey,"get_longTime")){
		lua_pushnumber(L,get_longTime());
		return 1;
	}
	if(!strcmp(attrKey,"ex_info")){
		ex_info();
		return 0;
	}
	
	
	//获取当前时间 精确到豪秒
	if(!strcmp(attrKey,"get_longTime"))
	{
		lua_pushnumber(L,get_longTime());
	}
	else if(!strcmp(attrKey,"get_name"))
	{
		lua_pushstring(L,base->name);
		return 1;
	}
	else if(!strcmp(attrKey,"fps"))
	{
		lua_pushnumber(L,g_fps);
		return 1;
	}
	else if(!strcmp(attrKey,"delayTime")){
		lua_pushnumber(L,g_delayTime);
		return 1;
	}
	return 1;
}

/*
	设置Camera坐标
*/
static int 
f_SetCameraPosition(lua_State *L)
{
	float x= lua_tonumber(L,2);
	float y= lua_tonumber(L,3);
	float z= lua_tonumber(L,4);
	
	ex_cam_set_pos(x,y,z);

	ex_updatePerspctiveModelView();
	
	return 0;
}

/*
	设置坐标
*/
static int 
f_SetNodePosition(lua_State* L)
{
	struct Node* ptrNode = (struct Node*)lua_tointeger(L,2);
	
	float x = lua_tonumber(L,3);
	float y = lua_tonumber(L,4);
	float z = lua_tonumber(L,5);
	if(ptrNode)
		base_setPos(ptrNode->base,x,y,z);
	else
		assert(!"ptrNode对象引用为NULL");

	return 0;
}
/*
	设置缩放
*/
static int 
SetScale(lua_State* L)
{
	struct Node* ptrNode = (struct Node*)lua_tointeger(L,2);
	float scaleValue = lua_tonumber(L,3);
	if(ptrNode!=NULL)
	{
		ptrNode->base->scale=scaleValue;
		base_updateMat4x4(ptrNode->base);
	}
	else
	{
		printf("ptrNode=NULL\n");
	}
	return 0;
}
/*
	寻找节点
*/
static int 
L_FindNode(lua_State* L)
{
	void* node = ex_find_ptr(ex,lua_tostring(L,2));
	lua_pushinteger(L,(int)node);
	return 1;
}
/*
	寻找节点
*/
static int 
L_GetName(lua_State* L)
{
	struct Node* ptr = (struct Node*)lua_tointeger(L,2);
	struct HeadInfo* base =base_get((void*)ptr);
	lua_pushstring(L,base->name);
	return 1;
}
//获取系统时间
static int
GetLongTime(lua_State* L)
{
	lua_pushnumber(L,get_longTime());
	return 1;
}
//输出日志
static int
LogPut(lua_State* L){
	const char* str = lua_tostring(L,2);
	int color = lua_tonumber(L,3);
	//log_put(str,color);
	log_color(color,(char*)str);
	return 0;
}

/*
	执行引擎函数
*/
static int
REG_dofunc(lua_State *L)
{
	const char* attrKey = lua_tostring(L,1);//改变的属性关键字
	if(!strcmp(attrKey,"tf_copy"))
	{
		struct TextField *tf_source = (struct TextField*)lua_tointeger(L,2);
		struct TextField *tf_dest= (struct TextField *)lua_tointeger(L,3);
		if(tf_dest)
		{
			tf_copy(tf_source,tf_dest);
		}
		return 0;
	}
	if(!strcmp(attrKey,"tf_get_str"))
	{
		struct TextField *tf_source = (struct TextField*)lua_tointeger(L,2);
		lua_pushstring(L,(const char*)tf_source->textPtr);
		return 1;
	}
/*
	//设置材质的参数
	if(!strcmp(attrKey,"materialSetVar"))
	{
		struct Node* ptr = (struct Node*)lua_tointeger(L,2);
		struct HeadInfo* base =base_get((void*)ptr);
		const char* var=lua_tostring(L,3);
		if(!strcmp(var,"_LineWidth"))
		{
			struct GMaterial* m = (struct GMaterial*)base->tmat;
			m->lineWidth = lua_tonumber(L,4);
		}

		//设置透明度
		if(!strcmp(var,"_Alpha"))
		{
			struct GMaterial* m = (struct GMaterial*)base->tmat;
			m->_Alpha = lua_tonumber(L,4);
		}
		

		//设置
		return 0;
	}
*/
	
	//reload贴图
	if(!strcmp(attrKey,"reloadTexture")){
		//testTex
		struct Node* ptr = (struct Node*)lua_tointeger(L,2);
		struct HeadInfo* base =base_get((void*)ptr);
		struct GMaterial* m = (struct GMaterial*)base->tmat;
		const char* texName = lua_tostring(L,3);
		GLuint tex = tmat_getTextureByIndex(m,0);
		jgl_subImage(tex,texName,0,0,256,256);
	}

	if(!strcmp(attrKey,"SetCameraPosition"))
		return f_SetCameraPosition(L);
	
	if(!strcmp(attrKey,"SetNodePosition"))
		return f_SetNodePosition(L);

	if(!strcmp(attrKey,"SetScale"))
		return SetScale(L);

	if(!strcmp(attrKey,"FindNode"))
		return L_FindNode(L);

	if(!strcmp(attrKey,"GetName"))
	{
		return L_GetName(L);
	}

	if(!strcmp(attrKey,"GetLongTime"))
	{
		return GetLongTime(L);
	}

	if(!strcmp(attrKey,"LogPut")){
		return LogPut(L);
	}

	return 0;
}

static void 
f_updateCallBack(void* p){
	HeadInfo* ptr = (HeadInfo*)p;
	base_updateMat4x4(ptr);
	//printf("c core: %.3f %.3f %.3f\n",ptr->x,ptr->y,ptr->z);
	//ex_lua_evt_dispatch(ptr->parent,EVENT_ENGINE_BASE_UPDATE,0);
}
static void
f_endCallBack(void* p){
	HeadInfo* ptr = (HeadInfo*)p;
	ex_lua_evt_dispatch(ptr->parent,EVENT_ENGINE_BASE_END,ptr->name);
	//printf("f_endCallBack\n");
}

static void* _camTween;

static void 
f_cam_upadte(void* ptr){
	struct ECamera* cam = &(ex_getInstance()->cam);
	ex_updatePerspctiveModelView();
	//printf("rx = %.3f\n",cam->rx);
}

static void
f_set_glsl(GMaterial* m,char* str){
	char key[G_BUFFER_64_SIZE];
	char v[G_BUFFER_64_SIZE];
	f_pa1rse_key_value(str,key,v);

	//#################################################
	//log_color(0xff0000,"%s\n",str);
	if(!strcmp(key,"lineWidth")){
		m->lineWidth = toFloat(v);	//设置线框宽度
	}
	else if(!strcmp(key,"alpha")){
		m->_Alpha = toFloat(v);		//设置透明度
	}
	else if(!strcmp(key,"uvScale")){
		m->uvScale = toFloat(v);//设置uv缩放值 
	}

	if(!strcmp(key,"_lineColor")){
		//	m->lineWidth = toFloat(v);	//设置线框宽度
		float r,g,b;
		sscanf_s(v,"%f,%f,%f",&r,&g,&b);
		m->_lineColor.x = r;
		m->_lineColor.y = g;
		m->_lineColor.z = b;
	}
}
static int 
REG_glsl_set(lua_State *L){
	int ptr = lua_tointeger(L,1);	
	//const char* attrKey = lua_tostring(L,2);//改变的属性关键字
	const char* str = lua_tostring(L,2);	//改变的数据1

	struct GMaterial* m = (struct GMaterial*)ptr;
	f_set_glsl(m,(char*)str);
	return 0;
}
/*
	更新3D对象的属性
*/
static int
REG_change_attr(lua_State *L)
{
	int ptr = lua_tointeger(L,1);			//对象指针如Sprite,Ent3d,md2,md5
	const char* attrKey = lua_tostring(L,2);//改变的属性关键字
	const char* str = lua_tostring(L,3);	//改变的数据1
	const char* str2 = lua_tostring(L,4);	//改变的数据2
	struct HeadInfo* base =base_get((void*)ptr);
	struct Node* node = (struct Node*)ptr;
	int change = 0;
	if(!base)
	{
		//printf("找不到对象0x%0x\n",ptr);
		//return 0;
		
		//设置camera rotateX
		if(!strcmp(attrKey,"camRX"))
		{
			//base->z=getFloat(str);
			//updateMat4x4(base);
			ex_getInstance()->cam.rx = toFloat(str);
			change = 1;
		}
		if(!strcmp(attrKey,"camRY"))
		{
			ex_getInstance()->cam.ry = toFloat(str);
			change = 1;
		}
		if(!strcmp(attrKey,"camRZ"))
		{
			ex_getInstance()->cam.rz = toFloat(str);
			change = 1;
		}
		if(!strcmp(attrKey,"cam_xyz")){
			float x,y,z;
			sscanf_s(str,"%f,%f,%f",&x,&y,&z);
			ex_getInstance()->cam.x = x;
			ex_getInstance()->cam.y = y;
			ex_getInstance()->cam.z = z;
			change = 1;
		}
		if(!strcmp(attrKey,"cam_rotate")){
			struct ECamera* cam = &(ex_getInstance()->cam);

			float time;
			sscanf_s(str,"%f",&time);
			if(_camTween && tween_is_play(_camTween)){
				tween_stop(_camTween);
			}
			_camTween = tween_to(cam,time,0,f_cam_upadte,
								2,&cam->rx,PI * 1.75);

			//printf("%s:%d\n",str,_camTween);
		}
		if(!strcmp(attrKey,"loadfile")){
			char* data= tl_loadfile(str,0);
			lua_pushstring(L,data);
			tl_free(data);
			return 1;
		}

		//=======================================
		//设置camera的坐标
		if(!strcmp(attrKey,"setCamPos"))
		{
			float x,y,z;
			sscanf_s(str,"%f,%f,%f",&x,&y,&z);
			ex_cam_set_pos(x,y,z);
			change = 1;
		}

		if(!strcmp(attrKey,"setBgColor")){
			float r,g,b;
			sscanf_s(str,"%f,%f,%f",&r,&g,&b);
			//ex_cam_set_pos(x,y,z);
			setBgColor(r,g,b);
		}

		//改变sprite渲染方式
		if(!strcmp(attrKey,"g_sprite_line")){
			g_sprite_line = toInt(str);
			lua_pushnumber(L,g_sprite_line);
			return 1;
		}

		if(!strcmp(attrKey,"gc"))
		{
			//内存池gc
			//memory_get_info();
			memory_gc();
			//memory_get_info();
		}

		if(!strcmp(attrKey,"exit")){
			exit(0);
		}
		
		/*if(!strcmp(attrKey,"ft_test")){
			ftext_create("aa");
			return 0;
		}*/
		
	}

	//########################################################
	//可以处理成兼容模式
	//设置Sprite属性
	if(!strcmp(attrKey,"sprite_position")){
		//(struct Sprite*)ptr
		//printf("%s:%s\n",attrKey,str);
		float x,y;
		sscanf_s(str,"%f,%f",&x,&y);
		sprite_setpos((struct Sprite*)ptr,x,y);
	}
	//设置TextField坐标
	if(!strcmp(attrKey,"txt_position")){
		float x,y;
		sscanf_s(str,"%f,%f",&x,&y);
		tf_setPos((struct TextField*)ptr,x,y);
	}
	

	//设置材质的标示flags
	if(!strcmp(attrKey,"setMaterialFlags"))
	{
		struct GMaterial* ptrMaterial = (struct GMaterial*)base->tmat;
		int flags=toInt(str);
		setv(&ptrMaterial->flags,flags);

		//printf("%d",getv(&ptrMaterial->flags,SHADER_FLAG_FLAT_COLOR));
	}
	if(!strcmp(attrKey,"glslParms"))
	{
		struct GMaterial* m = (struct GMaterial*)base->tmat;

		f_set_glsl(m,(char*)str);

		//#################################################
	}

	//设置sprite Z轴旋转
	if(!strcmp(attrKey,"sprite_rotateZ"))
	{
		sprite_rotateZ((struct Sprite*)ptr,toFloat(str));
		change = 1;
	}

	if(!strcmp(attrKey,"sprite_resize"))
	{
		float w,h;
		sscanf_s(str,"%f,%f",&w,&h);
		sprite_resize((struct Sprite*)ptr,w,h);
		change = 1;
	}

	//=====================================================
	//rx
	if(!strcmp(attrKey,"rx"))
	{	
		base->rx=toFloat(str);
		base_updateMat4x4(base);
		//updateRotateY(base);
		change = 1;
	}
	//ry
	if(!strcmp(attrKey,"ry"))
	{	
		base->ry=toFloat(str);
		base_updateMat4x4(base);
		//updateRotateY(base);
		change = 1;
	}
	//ry
	if(!strcmp(attrKey,"rz"))
	{	
		base->rz=toFloat(str);
		base_updateMat4x4(base);
		//updateRotateY(base);
		change = 1;
	}
	//scale修改其缩放
	if(!strcmp(attrKey,"scale"))
	{
		base->scale=toFloat(str);
		base_updateMat4x4(base);
		//更新缩放值
		change = 1;
	}
	if(!strcmp(attrKey,"lookat"))
	{
		//base->scale=toFloat(str);
		//base_updateMat4x4(base);
		//更新缩放值
		
		float x,y,z,time;
		sscanf_s(str,"%f,%f,%f,%f",&x,&y,&z,&time);

		if(time <= 0){
			base_look_at(base,x,y,z);
		}else{
			Vec3 pos;
			//printf("==\n");
			float ry;
			float x0 = x - base->x;
			float y0 = y - base->y;
			float z0 = z - base->z;

			vec3Set(&pos,x,y,z);

			vec3Normalize(&pos);
			ry = vec_rotateAngle(pos.x, pos.z, 1.0f, 0.0f);
			//ry = abs(ry);
			printf("%.3f to %.3f\n",base->ry,ry);
			//base_rotate_to(base,time,ry);

		}
		
		//change = 1;
	}
	if(!strcmp(attrKey,"set_position")){
		float x,y,z;
		sscanf_s(str,"%f,%f,%f",&x,&y,&z);
		//base_look_at(base,x,y,z);
		base->x = x;
		base->y = y;
		base->z = z;
		base_updateMat4x4(base);
		//change = 1;
	}

	if(!strcmp(attrKey,"move")){
		int ms;
		float x,y,z;
		sscanf_s(str,"%d,%f,%f,%f",&ms,&x,&y,&z);
		base_move(base,ms,x,y,z,f_endCallBack,f_updateCallBack);
	}
	
	if(!strcmp(attrKey,"base_updateMat4x4")){
		/*int ms;
		float x,y,z;
		sscanf_s(str,"%d,%f,%f,%f",ms,&x,&y,&z);
		base_move(base,ms,x,y,z,0,0);*/
		base_updateMat4x4(base);
	}



	//======================================
	//x
	if(!strcmp(attrKey,"x"))
	{
		base->x=toFloat(str);
		base_updateMat4x4(base);
		change = 1;
	}
	//y
	if(!strcmp(attrKey,"y"))
	{
		base->y=toFloat(str);
		base_updateMat4x4(base);
		change = 1;
	}
	//z
	if(!strcmp(attrKey,"z"))
	{
		base->z=toFloat(str);
		base_updateMat4x4(base);
		change = 1;
	}
	else if(!strcmp(attrKey,"rename"))
	{
		ex_rename((void*)ptr,str);
	}
	

	//设置md5的关键帧
	if(!strcmp(attrKey,"setframe")){
		int v = toInt(str);
		//printf("设置动作索引 %d\n",v);
		
		//v++;
		
		if(v<-1){
			log_code(ERROR_BAD_VALUE);
			assert(0);
		}

		if(base->curType == TYPE_MD5_FILE){
			base->custframe = v;
		}else{
			log_code(ERROR_PTR_TYPE_FAIL);
		}
		
	}
	//设置md5的碰撞半径
	if(!strcmp(attrKey,"set_rayRadius"))
	{
		float radius = toFloat(str);
		if(base->curType == TYPE_MD5_FILE){
			float v = radius * 2;
			tlgl_setAbsBoundBox(v,v,v,base->rayVertexData.vertex);
		}else{
			log_code(ERROR_PTR_TYPE_FAIL);
		}
	}
	/************************************************************************/
	/* 设置静态盒子颜色                                                                     */
	/************************************************************************/
	if(!strcmp(attrKey,"set_box_color")){
		float r,g,b;
		sscanf_s(str,"%f,%f,%f",&r,&g,&b);
		//sprite_setpos((struct Sprite*)ptr,x,y);
		base->boxR = r;
		base->boxG = g;
		base->boxB = b;
	}

	//绑定拾取回调
	if(!strcmp(attrKey,"set_pickBind"))
	{
		if(!getv(&(base->flags),FLAGS_RAY)){
			log_code(ERROR_FLAGS);
			assert(0);
		}

		base_setLuaPick(base,str);
	}
	//设置xyz
	if(!strcmp(attrKey,"setModelPos"))
	{
		float x,y,z;
		sscanf_s(str,"%f,%f,%f",&x,&y,&z);
		
		if(base){
			base->x = x;
			base->y = y;
			base->z = z;
			base_updateMat4x4(base);
		}
		else
		{
			printf("setModelPos找不到对象\n");
		}
		change = 1;
	}

	//设置look at target
	if(!strcmp(attrKey,"setLookTarget"))
	{
		float x,y,z;
		sscanf_s(str,"%f,%f,%f",&x,&y,&z);
		setLookTarget((void*)ptr,x,y,z);
		change = 1;
	}
	
	//显示日志
	if(!strcmp(attrKey,"ex_showLog"))
	{
		ex_showLog(str);
	}
	/*
	if(!strcmp(attrKey,"test"))
	{
		multChange(base);
	}
	*/
	
	//////////////////////////////////////////////////////////////////////////
	//设置fps
	if(!strcmp(attrKey,"fps"))
	{
		float v = toFloat(str);

		if(node){
			if(node->base->curType ==TYPE_MD5_FILE){
				node->base->fpsInterval = v;
			}
			else if(node->base->curType ==TYPE_OBJ_VBO_FILE && node->anim){
				anim_set_fps(node->anim,v);
			}

			//printf("%d\n",node->base->curType);//md5 1,md2 5
		}else{
			printf("fps`s node = NULL\n");
			assert(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//添加关键帧切割
	if(!strcmp(attrKey,"animtor_push"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		int s,e;

		//获取后缀名
		/*
		if(!strcmp(node->base->suffix,SUFFIX_MD2)){
			int length = node->anim->allLength;
			printf("length = %d\n",length);
		}
		else{
			log_code(ERROR_FUNCTION_WAIT_TO_DO);//没有实现该功能
			assert(0);
		}
		*/
		//int len = node->anim->allLength;
		int total = anim_total(node->anim);
		
		sscanf_s(str2,"%d,%d",&s,&e);
		if(total < e  + 1 || total < e + 1){
			log_color(0xff0000,"总关键帧数%d s=%d e=%d\n",total,s,e);
			s = 0;
			e = total - 1;
		}
		animtor_push(animtor,str,s,e);
	}
	//设置当前动作
	if(!strcmp(attrKey,"animtor_setcur"))
	{
		/*
		struct Animtor* animtor=anim_get_animtor(node->anim);
		
		if(!animtor_setcur(animtor,(char* const)str))
		{
			printf("为(%s)设置动作:%s失败\n",base->name,str);
		}
		*/
		if(base->curType == TYPE_OBJ_FILE){
			//obj类型的数据未能找到该接口实现!
			log_code(ERROR_PTR_TYPE_FAIL);
			return 0;
		}

		ex_animtor_ptr_setcur((void*)ptr,str,0);
	}

	//开始播放当前动作
	if(!strcmp(attrKey,"animtor_play"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		animtor_play(animtor);
	}
	
	//暂停播放当前动作
	if(!strcmp(attrKey,"animtor_pause"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		animtor_pause(animtor);
	}

	//指定区间播放动画
	if(!strcmp(attrKey,"animtor_play_start_end"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		int s,e;
		sscanf_s(str,"%d,%d",&s,&e);
		//printf("%d,%d\n",s,e);
		animtor_play_start_end(animtor,s,e);
	}

	//////////////////////////////////////////////////////////////////////////
	//绑定一个射线拾取盒,只有绑定了射线拾取盒才能被点击拾取
	if(!strcmp(attrKey,"collide_load"))
	{
		int frameCount = 0;
		if(str2)
			sscanf_s(str2,"%d",&frameCount);

		node->ptrCollide=collide_load(str,frameCount);//"\\resource\\obj\\"..objName..".obj"
	}
	if(change)
		ex_updatePerspctiveModelView();
	return 0;
}


/*
	注册	load_md2Anim
*/
static int
REG_load_animConf(lua_State *L)
{
	int md2 = lua_tointeger(L,1);
	const char* animConf = lua_tostring(L,2);
	//const char* defaultAnim=lua_tostring(L,3);
	float fps = lua_tonumber(L,3);
	int stat=ex_load_anim_config((void*)md2,(char*)animConf,(float)fps);
	lua_pushinteger(L,stat);
	return 1;
}
/*
	注册md5_loadAnim
*/
static int
REG_load_md5Anim(lua_State *L)
{
	int md5 = lua_tointeger(L,1);
	const char* md5Anim = lua_tostring(L,2);
	const char* animKey =  lua_tostring(L,3);
	md5_loadAnim((struct MD5*)md5,md5Anim,animKey);
	return 0;
}
/*
	注册setanim
*/
static int
REG_setanim(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	const char* animKey =  lua_tostring(L,2);
	ex_set_anim((void*)ptr,animKey);
	return 0;
}

/*
	注册销毁Sprite
*/
static int 
REG_ptr_remove(lua_State *L){
	int ptr = lua_tointeger(L,1);
	ex_ptrRemove((void*)ptr);
	return 0;
}

static int 
REG_get_suffix(lua_State *L){
	char buffer[G_BUFFER_32_SIZE];
	tl_getSuffixByPath((char*)lua_tostring(L,1),buffer,G_BUFFER_32_SIZE);
	lua_pushstring(L,(const char*)buffer);
	return 1;
}

//执行lua脚本
static lua_State* 
runhelloTest(const char* script){
	long time = get_longTime();
	lua_State* lua_state = luaL_newstate();
	luaL_openlibs(lua_state);

	//注册接口函数
	
	lua_register(lua_state,"ptr_remove",REG_ptr_remove);
	
	lua_register(lua_state,"setv",REG_setv);
	lua_register(lua_state,"resetv",REG_resetv);
	lua_register(lua_state,"getv",REG_getv);
	
	//创建材质
	lua_register(lua_state,"createMaterial",REG_tmat_createMaterial);
	lua_register(lua_state,"createMaterialTex",REG_tmat_createMaterialTex);
		
	lua_register(lua_state,"load_model",REG_load_model);
	lua_register(lua_state,"load_animConfig",REG_load_animConf);
	lua_register(lua_state,"setMaterial",REG_setMaterial);
	lua_register(lua_state,"md5_loadAnim",REG_load_md5Anim);
	lua_register(lua_state,"setanim",REG_setanim);
	
	//单元测试案例01
	lua_register(lua_state,"test_unit_01",REG_test_unit_01);
	lua_register(lua_state,"test_unit_01_init",REG_test_unit_01_init);
	//单元测试案例02
	lua_register(lua_state,"test_unit_02",REG_test_unit_02);

	//获取属性
	lua_register(lua_state,"get_attr",REG_get_attr);
	//设置属性
	lua_register(lua_state,"change_attr",REG_change_attr);

	lua_register(lua_state,"glsl_set",REG_glsl_set);
	//执行函数
	lua_register(lua_state,"dofunc",REG_dofunc);
	lua_register(lua_state,"get_suffix",REG_get_suffix);


	//Sprite
	lua_register(lua_state,"sprite_create", REG_sprite_create);
	lua_register(lua_state,"sprite_texName",REG_sprite_texName);
	lua_register(lua_state,"setUV",REG_setUV);
	lua_register(lua_state,"sprite_setpos",REG_sprite_setpos);
	lua_register(lua_state,"sprite_setDragScope",REG_sprite_setDragScope);
	lua_register(lua_state,"sprite_bindAtals",REG_sprite_bindAtals);
	lua_register(lua_state,"sprite_bindMaterial",REG_sprite_bindMaterial);//绑定材质

	//图集
	lua_register(lua_state,"atals_load",REG_atals_load);

	//文本
	lua_register(lua_state,"tf_create",REG_tf_create);
	lua_register(lua_state,"tf_dispose",REG_tf_dispose);
	lua_register(lua_state,"tf_setPos",REG_tf_setPos);
	lua_register(lua_state,"tf_setText",REG_tf_setText);
	lua_register(lua_state,"load_VBO_model",REG_load_VBO_model);
	lua_register(lua_state,"tl_showCurMemery512",REG_tl_showCurMemery512);
	
	//ftext
	lua_register(lua_state,"ftext",REG_ftext);
	
	//xml接口
	lua_register(lua_state,"core_xml",REG_core_xml);
	//void sprite_setpos(struct Sprite* spr,int x,int y)
	
	if(luaL_dofile(lua_state,script))
	{
		printf("--------------------(%s)脚本运行错误!\n",script);
	}
	//lua_close(lua_state);
#ifdef DEBUG
	printf("初始化,注册,解析lua脚本耗时%ld毫秒\n",(get_longTime()-time));
#endif
	
	return lua_state;
}
/*
	在引擎初始化之前,初始化着色器
*/
static void
initShader()
{
	//tlgl_getShader("vboDiffuse");
	//tlgl_getShader("sprite");
	//tlgl_getShader("font");
}
static void
f_init(int id,void* p){

#ifdef DEBUG
	log_color(0xff0000,"事件%d\t引擎初始化完成,开始加载lua脚本!\n",id);
#endif
	

	//初始化日志文本缓存区
	ex_showLog("");

	initShader();

	//初始化图集
	//ex_getInstance()->myAtals=atals_load("//resource//texture//","1");

	//加载执行lua
	{
		char outStr[G_BUFFER_256_SIZE];
		char __ss[G_BUFFER_256_SIZE];
		sprintf_s(__ss,G_BUFFER_256_SIZE,"//include//lua//%s",g_initLua);
		tl_convert_path(__ss,outStr,G_BUFFER_256_SIZE);
#ifdef DEBUG
		printf("执行Lua脚本%s\n",outStr);
#endif
		ex_getInstance()->mylua=runhelloTest(outStr);


#ifdef _TEST_GL_TEX_
		f_gl_tex();
#endif

	}
}

/* 
初始化场景                                                                     
*/
static void 
initScene(){
	struct EX* ex = ex_getInstance();//scene->enginePtr;

	//初始化字体设备句柄
	ex->ft = ft_create("C:\\Windows\\Fonts\\simsun.ttc");
	
	if(ex->ft == 0){
		printf("字体设备创建失败!\n");
		return;
	}

	


	evt_once(ex_getInstance(),EVENT_ENGING_INIT,f_init);

	//在这里可以初始化一些 引擎需要的配置等文件
	evt_dispatch(ex,EVENT_ENGING_INIT,0);
}
/*
检测多重采样缓冲区是否存在,以及对每个像素使用几个采样
*/
void checkBuffersSample(){
	GLint bufs,samples;
	glGetIntegerv(GL_SAMPLE_BUFFERS,&bufs);
	glGetIntegerv(GL_SAMPLES,&samples);

#ifdef DEBUG
	printf("MSAA buffers = %d samples = %d\n",bufs,samples);
#endif
	
}

void tlgl_reshape(int w,int h){

	if (h == 0){
		h = 1;
	}
	ex->_screenWidth = w;
	ex->_screenHeight = h;

	ex_update_uiPos();
	
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	ex_updatePerspctiveModelView();

	ex_lua_global_evt_dispatch(EVENT_ENGINE_RESIZE);
}
void psv(){
	struct PCsvVO * pcsv;
	int out;
	Array* arr;
	//tl_memState(TRUE);
	//printf("===========>%d bytes\n",tl_memByteSize());
	pcsv = pcsv_load("D:\\source\\resource\\CaiLiaoHeCheng.csv","itemid",1);

	arr=pcsv_getValue(pcsv,"qianghua10");
	array_get(arr,1,&out);
	if(out){
		printf("%s\n",(char*)out);
	}
	pcsv_dispose(pcsv);
	//printf("===========>%d bytes\n",tl_memByteSize());
}
void strTest(){
	
	
	//tl_memState(TRUE);
	//printf("===========>%d bytes\n",tl_memByteSize());

	//printf("%d\n",strcspn("Jack","ck"));
	void* pstr = str_create("HIhelloABChelloDEFGhelloTF");


	
	/*for(i = 0;i <3;i++){
		str_appand(pstr,"ABC_");
	}*/

	printf("%s\n",str_get(pstr));
	str_replace(pstr,"hello","abc");
	printf("%s\n",str_get(pstr));

	
	
	//str_dispose(pstr);
	pstr = 0;
	
	//printf("===========>%d bytes\n",tl_memByteSize());
	
	//psv();
	//getchar();
}

void testGet(char* const str)
{
	int a;


	//s=NULL;
	char _s[128];
	memset(_s,0,128);
	memcpy(_s,str,strlen(str));

	a = getchar();
	printf("%d\n",a);
	if(a==65){
		printf("is OK!\n");
	}
}

static void 
loadXml(int* pLogStat,int* pMemory,
		int *screenWidth,int *screenHeight,float* rate,int* winIsCenter){
	struct XMLSList* xml = NULL;
	struct XmlNode* _node=NULL;
	
	char monster[G_BUFFER_64_SIZE];
	char* _data =  tl_loadfile("\\resource\\config.xml",NULL);
	//HANDLE hOut =  GetHandle();

	xml = xml_parse(_data);

#ifdef DEBUG	
	log_color(0x00ff00,"****************************************\nconfig.xml=\n%s\n****************************************\n",_data);
#endif
	//=============================================
	//解析
	_node =	xml_getrow(xml,"id","0");

	if(_node)
	{
		//怪物xml路径
		xml_getstr(_node,"monster",monster,G_BUFFER_64_SIZE);
		//printf("========================>怪物路径为%s\n",monster);
		
		*screenWidth = xml_getint(_node,"w");
		*screenHeight = xml_getint(_node,"h");
		*rate = xml_getfloat(_node,"rate");
		*pLogStat = xml_getint(_node,"setLogColor");
		*pMemory  = xml_getint(_node,"memory");
		*winIsCenter =  xml_getint(_node,"winpos");
		xml_getstr(_node,"initLuaName",g_initLua,G_BUFFER_64_SIZE);
	}
	else
		printf("节点数据为NULL\n");

	//ParseMemoryCfg(xml,&cfg->isMemory);
	
	//ParseLuaScript(xml,&cfg->LuaScript);
	xml_del(xml);
}

static void leftRayPickCallBack(struct HitResultObject* hit)
{
	struct HeadInfo* _node = ex_find_headinfo(ex,hit->name);
	if(_node)
	{
		//按着拾取的物体旋转
		_node->ry	+=	0.01f;
		base_updateMat4x4(_node);
	}
}

//static void loopCallBack(void)
//{
//	if(ex_mouseIsLeftDown())
//	{
//		hit_mouse(ex->mouseState.xMouse,ex->mouseState.yMouse,ex->_screenWidth,ex->_screenHeight,ex->renderList,leftRayPickCallBack);
//	}
//}
//static void
//tStrstr()
//{
//	const char* s1 ="abcd";
//	const char* s2 = "abc";
//	printf("strstr ======%d\n",strstr(s1,s2)-s1);
//}

//设置局部路径,如果设置了局部路径将影响 tl_loadfile
static void 
f_set_local_path(const char* path){
	char* p = (char*)g_local_path;
	if(strlen(path)>G_BUFFER_256_SIZE){
		printf("设置的路径长度大于缓冲区长度!\n");
		assert(0);
	}	
	if(!path){
		memset(p,0,G_BUFFER_256_SIZE);
	}else{
		memset(p,0,G_BUFFER_256_SIZE);
		memcpy(p,path,strlen(path));
	}
}
/* 设置窗口的坐标 */
static void
f_set_winpos(int type,int w,int h,float rate){
	float bottom_height = 40.0f;//底部偏移
	switch(type){
		case 1:
			//居中显示,设置窗口坐标
			glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-(w * rate))/2,(glutGet(GLUT_SCREEN_HEIGHT)-(h*rate))/2);
			break;
		case 2:
			//左下角
			glutInitWindowPosition(0,glutGet(GLUT_SCREEN_HEIGHT)-(h*rate) - bottom_height);
			break;
		case 3:
			//左上角
			glutInitWindowPosition(0,0);
			break;
		case 4:
			//右上角窗口
			glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-w * rate),0.0f);
			break;
		case 5:
			//分屏左上角窗口
			glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)),0.0f);
			break;
		case 6:
			//分屏左下角窗口
			glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH)+128,glutGet(GLUT_SCREEN_HEIGHT)-h*2);
			break;
		case 7:
			//右下角窗口
			glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-w * rate),glutGet(GLUT_SCREEN_HEIGHT)-h*2);
			break;
		case 8:
			//分屏居中显示,设置窗口坐标
			glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)*2-(w * rate)),(glutGet(GLUT_SCREEN_HEIGHT)-(h*rate))/2);
			break;
	}
}
void main(int argc,char** argv){ 
	
	float rate;//比率
	//窗口宽,高
	int w,h;
	int _log_stat;//日志状态
	int isMemory;//是否使用内存池
	int screenWidth,screenHeight,winpos;

	//设置局部路径
	f_set_local_path(_MAIN_PATH_);
	//tl_memInit("32,512@64,100@512,200@8192,200@81920,200@262144,100@5242880,25");//"64,100@512,200@8192,200@81920,200@262144,100@5242880,200"
	
	loadXml(&_log_stat,&isMemory,&screenWidth,&screenHeight,&rate,&winpos);

	log_state(_log_stat);//是否显示颜色日志
	//tl_memState(isMemory);
	
	//gotoxy(0,0);

	w = screenWidth;
	h = screenHeight;

	//testGet("dasdsds");
	
//	printf("系统使用内存:%ld\n",getWin_MemUsage());
	
	//tStrstr();
	//strTest();
	
	ex = ex_create();

	//ft_load();
	
	//printf("---->%.3f\n",exp(8));
	//{
	//	int k;
	//	//Sleep(100);
	//	int cnt = 100;
	//	for(k = 0;k < cnt;k++)	{
	//		printf("file%d / %d\r", k,cnt);
	//		sleep(100);
	//	}
	//	printf("\n");
	//}

	//启用射线拾取碰撞检测
	//setRayCollision(ex,TRUE);
	setv(&(ex->flags),EX_FLAGS_RAY);

	//设置背景
	//setBgColor(1.0f,1.0f,1.0f);
	//setBgColor(0,0,1);
	//setBgColor(0,0,0);

	//==========================================================================
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE| GLUT_DEPTH );
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |  GL_MULTISAMPLE);//GL_MULTISAMPLE多重采样

	checkBuffersSample();//多重采样检测

	//GL_MULTISAMPLE
	//GLUT: Fatal Error in test.exe: pixel format with necessary capabilities not found.

	//==========================================================================
	
	//log_color(0x00ff00,"窗口比率rate = %.3f\n",rate);
	
	
	f_set_winpos(winpos,w,h,rate);




	//设置窗口的宽高
	glutInitWindowSize(w*rate,h*rate);
	glutCreateWindow("test"/*argv[0]*/);
	//glShf_MousePlotL_SMOOTH);
	glutDisplayFunc(ex_render);
	//glutReshapeFunc(tlgl_reshape);
	/*键盘事件*/
	glutSpecialFunc(onSpecialKeys);

	//普通键盘事件监听
	glutKeyboardFunc(onKeyboardCallBack);

	glutIdleFunc(ex_render);

	if(TRUE)
	{
		GLenum err=glewInit();
		if(GLEW_OK!=err){
			printf("glewInit (%s)\n",glewGetErrorString(err));
			return;
		}
	}
	//printf("===================>0x%0x\n",(0xabcdef12>>16)&0xff);
	//printf("%d\n",tl_isFileExist("d:\\3dgameLog2.txt"));
	//获取opengl版本
	tlgl_showGLVersion();

	//初始化引擎
	ex_init(ex,100000);
	//evt_on(ex,EVENT_ENGINE_RENDER_3D,f_drawCall);

	//显示信息文本
	setv(&ex->flags,EX_FLAGS_DRAW_DEBUG_TEXT);

	//log_write("日志测试!");

	//log_state(0)

	//ex->loopCallBack = loopCallBack;

	evt_on(ex,EVENT_RAY_PICK,f_rayPickCallBack);

	//加载角色模型配置	
	//scene_loadAvtCfg(scene,"//resource//monster.xml");

	
	glutReshapeFunc(tlgl_reshape);
	//core_load("//resource//mat1.mat");

	glutMouseFunc(mousePlot);
	//glutPassiveMotionFunc		//鼠标按下的时候会暂停
	//glutMotionFunc			//鼠标按下的时候继续获取屏幕坐标
	glutMotionFunc(mouseMove);
	
	//初始化场景
	initScene();

	glutMainLoop();

	ex_dispose(ex);
	ex = NULL;
	
	lua_close(ex_getInstance()->mylua);
}