#ifndef _TOOLS_H_
#define _TOOLS_H_

#define _DEBUG_MODE_

#include "global_define.h"
/*
	是否使用开发模式,用户调试
	0关闭
	1开启
*/
static const int _IS_DEVELOPMENT_ = 1;

/*
	函数命名约定说明
	=======================================================================================
	f_IRenderNode	: f_			局部函数,模块内部函数(私有)
					  IRenderNode	I作为接口Interface大写第一个字符,
									表示这是一个接口实现,传递CallBack类型的函数回调指针即可
	---------------------------------------------------------------------------------------
	Load		: md5_			模块命md5,(公开)
					  load			模块公共方法

	=======================================================================================
	私有结构体放在 .c文件中
	公用结构体放在 .h文件中

	.h	文件放好函数声明，写好注释和描述信息
	.c	函数实现
*/

/*
函数指针的使用
int fmax(int a,int b){
	if(a>b){
		return a;
	}
	return b;
}
int testF(int (*pmax)(int, int),int a,int b){
	return pmax(a,b);
}
printf("%d\n",testF(fmax,10,20));
*/







/*
	//为回调函数命名，类型命名为 CallBackFun，参数为char *p
	int Afun(char *p){
    	printf("show string:%s!\n", p);   
    	return 0;
	}
	
	//执行回调函数，方式一：通过命名方式
	int call(CallBackFun pCallBack, char *p){   
	    printf("call 直接打印出字符%s!\n", p);   
	    pCallBack(p);   
	    return 0;
	}
	char *p = "hello";
	call(Afun, p);   
*/


//四元数数学库
/* Quaternion (x, y, z, w) */
typedef float Quat4_t[4];

/* Vectors */
typedef float Vec2_t[2];
typedef float Vec3_t[3];
typedef float Vec8_t[8];

enum {X, Y, Z, W};

/*
*	函数回调定义
*/
typedef int (*CallBackFun)(char *p);


/*
*	切割字符串结构体
*/
struct StrCutSplit
{
	/*
	*	被切割出来的字符节点
	*/
	char* str;

	/*
	*	传递的参数
	*/
	int* parms;
};

//字符分割结构体
struct StringSplit{
	char* dest;/*源字符串*/
	char* sign;/*目标搜索字符串*/
	char* pOutstr;/*字符输出缓存区*/
	int pos;/*当前指针移动的坐标*/
	int isEnd;/*isEnd == -1	搜索结束,isEnd == 0搜索还未到字符串末尾*/
};



/*
*双端链栈
*/
struct LStackNode{
	int data;
	struct LStackNode *pre;//前驱节点
	struct LStackNode *next;//下一个节点
};
/*
*	opengl数据结构
*/
struct VerNorUvStruct
{
	float* pvs;			
	float* pnormals;	
	float* puvs;		
};

struct Vec2{
	float x,y;
};
/*
*	3D向量
*/
struct Vec3{
	float x,y,z;
};

struct Vec4
{
	float x,y,z,w;
};

/*
*
*	射线与三角形的交点数据结构
*
*/
struct HitResultObject{
	//拾取的交点坐标
	float x,y,z;

	//是否已经拾取到,拾取到:TL_TRUE,未拾取到:TL_FALSE
	int isHit;

	//拾取的对象名字
	char name[G_BUFFER_32_SIZE];

	//射线与三角形的交点 距离 射线起点的距离
	float distance;

	//三角形的三个顶点
	//float pickTriangle[9];
};

/**
*	列表结构体
*/
struct List
{
	float* list;
	int length;
};


/*
这是一个单节点结构体

xml			--xml指针
parent		--根节点坐标
start		--起始坐标
end			--结束坐标
row			--行描述数据
*/
struct XmlNode
{
	//父结点引用
	struct XMLSList* parentXml;
	int parent;
	int start;
	int end;
};

/*
*	Type:XMLSList
*	-------------
*	
*	XML 节点列表
	
	outlen		--输出的长度
	out			--临时缓存去 用于处理数据

	pNodeKey	--搜索的根节点
	pKey		--用来搜索的关键字
	pRes		--用来保存搜索到的值
	pNode		--用来保存XmlNode指针

*/
struct XMLSList
{
	struct LStackNode* list;
	char* data;
	char* buffer;
};

#define  ARRAY_HEAP 1	//堆内存需要tl_free释放
#define  ARRAY_STACK 0 //栈内存,自动释放

/*
	可边长数据
*/
typedef struct Array{
	int* ptr;
	int length;	//数组长度
	int cur;	//当前游标
	int mode;	//数组模式
}Array;
struct PCsvVO{
	/*
	数据行list
	*/
	struct LStackNode* list;

	/*
	关键字索引
	*/
	int keyIndex;

	/*
	引用数组,无需释放
	*/
	struct Array* keylist;
};
typedef struct Str{
	char* s;
}Str;



//循环vo
struct BaseLoopVO
{
	//开始的时间
	long start;

	//需要使用的时间
	long usems;

	//目标坐标
	float tx,ty,tz;

	//移动中的坐标
	float *px,*py,*pz;

	//回调
	void (*callBack)(struct BaseLoopVO*);

	//回调次数
	int count;

	void (*delayCallBack)(void*);
	void* delayParms;

	struct Vec3 o;
};

//typedef int (*CallBack_Param)(void* p,void* in);
/*
	开辟数据块
*/
void* MALLOC(int size);
/*
	释放内存块
*/
void FREE(void*p);


//===============================================================================================
/*
 *	判断是否存在文件file,存在返回1 不存在返回0
 *	19:35 2017/7/8
 */
int tl_isFileExist(const char* file);

/*
*	设置局部路径,如果设置了局部路径将影响 tl_loadfile
*/
void tl_setLocalPath(const char* path);

/*
 *	读写一个文件,获取文件中的文本加载glsl的时候使用,或者加载obj等文件时使用,
 *	int* pLength	文件长度(字节) 外部传入指针 用保存字符串的长度
 *	注:加载成功之后记得用 tl_free 释放字符引用!
 */
char* tl_loadfile(const char* path,int* pLength);
/*
	将数据buffer 增量写入filePath中
	-------------------------------------------------
	wb 只写打开或新建一个二进制文件；只允许写数据。 
	wb+ 读写打开或建立一个二进制文件，允许读和写。 
	wt+ 读写打开或着建立一个文本文件；允许读写。 
	at+ 读写打开一个文本文件，允许读或在文本末追加数据。 
	ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。 
*/
void  tl_writeAppandFile(const char* filePath,char* buffer);
/*
	dest切割字符串sign

	dest		--源字符串
	sign		--搜索字符
	pOutstr		--输出字符串

	==============================================
	char buffer[G_BUFFER_MAX_SIZE];
	char* _str = "jiangjqUI\n1234UI\nAbde";
	int pos = 0;
	int index = 0;
	while(index!=-1)
	{
		index = tl_splitSeries(_str,"\n",&pos,buffer);
		printf("%d(%s)",pos,buffer);
	}
	==============================================
*/
//int tl_splitSeries(const char* dest,const char* sign,int *pos,char* pOutstr);





/*获取文件列表*/
void tl_get_files(const char* path,struct LStackNode* stack);

/*获取exe执行路径*/
void tl_get_exe_path(char *argv[],char* out);

/******************************************************************************
tl_http_ 为前缀的方法名为http功能相关的接口方法
******************************************************************************/
/*获取query_string的访问字符*/
void tl_http_get_query_string(char* result);

/*编码	DataByte  [in]输入的数据长度,以字节为单位*/
char* tl_base64_encode(const char* data, int data_len);

/*解码DataByte[in]输入的数据长度,以字节为单位,OutByte [out]输出的数据长度,以字节为单位,请不要通过返回值计算,输出数据的长度*/
char* tl_base64_decode(const char *data, int data_len) ;

/*URL编码*/
void tl_urlEncode(const char* URL,char* result);

/*URL解码*/
void tl_urlDecode(const char* URL,char* result);

/*快速排序*/
void tl_quickSort(float* pData,int Count);

/*冒泡排序	double a[5]={3.1,2.5,1.0,5.0,4.3}; tl_bubbleSort(a,5);*/
void tl_bubbleSort(float* _array,int _len);

void tl_bubbleSortInt(int* _array,int _length);

/*
*	生成一个包围盒顶点数组	
/	返回float*的个数
*/
int tl_boxArray(float* v,float* out);

float tl_max(float v0,float v1);

/*
*	in 6个顶点min max输入
*	返回一个tl_malloc的float列表,需要tl_free释放
*/
//float* tl_createBoxArray(float* in,int* pLength);
/*
	求出x,y,z的min,max,即4个顶角的坐标
	out输出		0		1		2		3		4		5
				xmin	xmax	ymin	ymax	zmin	zmax

	count	--顶点个数
*/
void tl_minMax(float* vertexs,int count,float* out);

//取绝对值最大的做碰撞盒
void tl_absMax(float* vertexs,int count,float* out);

/*
 *	设置固定长宽高的包围盒
 *	x,y,z 为长宽高
 */
void tl_setAbsBoundBox(float x,float y,float z,float* outTriData);

/*	根据顶点创建AABB数据
	
	v[0] = minX;
	v[1] = maxX;
	v[2] = minY;
	v[3] = maxY;
	v[4] = minZ;
	v[5] = maxZ;

	trianglesArray :	三角形顶点数组
	vertexCount:		顶点个数
	*outTriData:		输出的数据

	返回float的个数	
*/
int tl_createAABBbyVert(float* trianglesArray,int vertexCount,float* outTriData);

/*
	功能描述:射线与三角形相交
	-----------------------------
	orig	:射线起始坐标
	dir		:方向向量			(射线向量-目标目标向量=方向向量)
	vert0~2	:三角形3个顶点坐标

	pHit	: 0不相交,1相交
	out		:x,y,z 交点坐标
	-----------------------------
	使用方法:
	void testHit(){
	double orig[3] = {0,-0.5,0.5};//射线坐标
	double target[3] = {0,-1,0};//目标点
	double dir[3] = {orig[0]-target[0],orig[1]-target[1],orig[2]-target[2]};//射线向量-目标目标向量=方向向量
	double vert0[3] = {0,0,0};
	double vert1[3] = {-1,-1,0};
	double vert2[3] = {1,-1,0};
	double out[3];
	int hit;
	tl_hitTriangle(orig,dir,vert0,vert1,vert2,out,&hit);
	printf("是否有交点 %d,交点坐标 x:%f y:%f z:%f\n",hit,out[0],out[1],out[2]);
}
*/
void tl_hitTriangle(float orig[3],float dir[3],float vert0[3],float vert1[3],float vert2[3],float* out,int* pHit);
/*
	计算fps
*/
void tl_calculate_fps();
/*
	获取fps
*/
int tl_get_fps();

void tl_strSplitInit(struct StringSplit* p,const char* dest,const char* sign,char* pOutstr);
/*
	打印顶点数组
*/
void tl_printfVers(float* vert,int count);
//=============================================================================


/*vec设置*/
void tl_set_vec(Vec3_t vec,float x,float y,float z);
/*
	c = a + b
*/
void tl_add_vec(Vec3_t a,Vec3_t b,Vec3_t c);

/*
	自偏移
*/
void tl_offset_vec(Vec3_t pos,float x,float y,float z);

/*vec缩放*/
void tl_scale_vec(Vec3_t vec,Vec3_t res,float scale);


void Quat_computeW (Quat4_t q);
void Quat_normalize (Quat4_t q);
void Quat_multQuat (const Quat4_t qa, const Quat4_t qb, Quat4_t out);
void Quat_multVec (const Quat4_t q, const Vec3_t v, Quat4_t out);
void Quat_rotatePoint (const Quat4_t q, const Vec3_t in, Vec3_t out);
/*点乘*/
float Quat_dotProduct (const Quat4_t qa, const Quat4_t qb);

void  Quat_slerp (const Quat4_t qa, const Quat4_t qb, float t, Quat4_t out);

/*将verts解析输出,u,v	normalX,normalY,normalZ posX,posY,posZ*/
void tl_parseVerts(float* verts,int* pVerts,int* pUVs,int* pNormals,int len);
/*
*	顶点解析
*/
struct VerNorUvStruct* tl_parseVerts2(float* verts,int len);
//****************************************************************************//



//****************************************************************************//
/*链栈*/
/*
	First in Last out
	先进后出

	--------------------------------------------------
	Remarks:
		使用<1>会造成栈释放内存泄露,栈游标指针被移动到最后一位了
		<1>	Error
		while(p!=NULL&&p->next!=NULL){
		    if(p!=NULL){
		    	p=p->next;
				Mem_free(p->data);
			} 
		}
		
		<2> OK
		while(LStack_length(p)!=0){
			int c;
			LStack_pop(p,&c);
			Mem_free(c);
		}
		LStack_clear(p);

		200 
			∑ 	= 5+6+7+8+9+......+200　
			i = 5 

example
	--------------------------------------------------
void LStackTEST(){

	void* top,*p;

	int data;
	char buffer[128];
	int j=0;
	void* s = (void*)LStack_create();
	int i;
		
	LStack_push(s,10);
	LStack_push(s,25);
	LStack_push(s,36);
	//删除一个节点
	//int t=LStack_delNode(s,10);
	//反转链栈
	LStack_rev(s);
	//遍历链栈
	top = s;
	p=top;
	memset(buffer,0,128);
	while((int)LStack_next(p)){
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		j+=sprintf_s(buffer+j,128,"%d,",data);
	}
	printf("%s\n",buffer);

	LStack_delete(s);

}
*/
/*
	Function:	LStack_sort(使用方法)
	--------------------------
	对链栈进行排序给定的结构体第一个字段为*.f
	内部结构体：SortStruct

	1.	对p_list进行赋值,p_list存储链栈的地址
	
	2.	交换数组中的链栈地址的值
		冒泡排序升序排列
		可以采用其他的排序算法实现
	
	
	//测试代码
	
	#define SIZE 3

	typedef struct{
		int f;
	}TStruct;

	LStackNode* s = LStack_create();
	int a[SIZE]={67,78,72};

	int i;
	for(i=SIZE-1;i>=0;i--){
		TStruct* st=(TStruct*)MALLOC(sizeof(TStruct));
		
		st->f = a[i];
		LStack_push(s,(int)st);
	}	    

	printf("length:	%d\n",LStack_length(s));

	LStack_sort(s,LSTACKNODE_SORT_ASCEND);
	
	while(LStack_length(s)!=0){
		int va;
		LStack_shift(s,&va);
		TStruct* ts = (TStruct*)va;
		printf("%d\n",ts->f);
	}
	#undef SIZE
	
	getchar();

*/

/*
*	初始化一个链栈,第一个节点为头节点引用
*	返回头节点的引用
*/
struct LStackNode*	LStack_create();

/*判断链栈头结点的指针域是否为空*/
int 	LStack_isEmpty(struct LStackNode* top);

/*进栈操作就是要在链表的第一个结点前插入一个新结点，进栈成功返回1*/
int 	LStack_push(struct LStackNode* topk, int e);

/*
	遍历接口
	int (*callBack)(int node,int input)
	int node: 遍历的node引用
*/
void LStack_ergodic(struct LStackNode* list,int (*callBack)(int,int),int parms);
/*
	遍历,回调函数不处理返回逻辑
*/
void LStack_ergodic_t(struct LStackNode* list,void (*callBack)(int,int),int parm);
/*
*	根据索引获取数据,先入栈的索引值小
example:
void listTest()
{
	void* list = (void*)LStack_create();
	int i;
	int out;
	int findIndex = 9;
	for(i = 0;i < 10;i++)
	{
		LStack_push(list,i+1);
	}
	printf("list len:%d\n",LStack_length(list));
	
	if(LStatk_getNodeByIndex(list,findIndex,&out))
	{
		printf("索引上的值%d\n",out);
	}
	else
	{
		printf("找不到索引%d\n",findIndex);
	}
}
*/
int		LStatk_getNodeByIndex(struct LStackNode* top,int index,void* out);

/*
*	out 存储的是一个地址
*/
int		LStatk_getAddressByIndex(struct LStackNode* top,int index);

/*出栈*/
int 	LStack_pop(struct LStackNode* top,int *e);			
/*
*	求栈长度
*/
int 	LStack_length(struct LStackNode* top);		
/*销毁栈*/
void 	LStack_delete(struct LStackNode* top);

/*求栈顶元素*/
int 	LStack_top(struct LStackNode* top,int *e);

/*删除一个栈节点*/
int 	LStack_delNode(struct LStackNode *p,int node);

/*清空栈中的元素*/
void	LStack_clear(struct LStackNode* top);

/*反转链栈*/
void	LStack_rev(struct LStackNode* top);

/*算法时间复杂度O(1)删除 	LStack 中的第一个元素，并返回该元素。	其余的 	LStack 元素将从其原始位置 i 移至 i - 1。	e为取出的值*/
void	LStack_shift(struct LStackNode* top,int *e);

/*栈排序*/
void	LStack_sort(struct LStackNode* s,int sortType);

/*获取下一个节点的指针*/
void*	LStack_next(void* s);

/*获取上一个节点的指针*/
void*	LStack_pre(struct LStackNode* s);

/*获取当前节点的data数据*/
int		LStack_data(void* s);

/*
	栈遍历
	---------------------------
	param[0]		保存链栈node
	param[1~...]	储存in 或者 out指针
*/
void	LStack_IErgodic_S(void* _s,int (*callBack)(char*),int* param);

//void	LStack_IErgodic_N(void* _s,CallBackFun callBack,int* param);
//void	LStack_IErgodic(void* _s,void* in,int(__cdecl*callBack_Param)(void* p,void* in));
//****************************************************************************//
/*	案例
int main(int argc, char* argv[])
{
	struct List* a = (struct List*)tl_malloc(sizeof(struct List));
	list_create(a);
	
	list_add(a,1.2);
	list_add(a,0.5);
	
	printf("列表长度 %d\n",a->length);
	
	for(int i = 0;i < a->length;i++)
	{
		printf("%f\n",a->list[i]);
	}
	list_dispose(a);
	tl_free(a);
	return 0;
}
*/

/*
*	初始化一个列表,起始长度为0
*/
void list_create(struct List* _l);
/*
*	为列表添加一个float数据,索引从0位开始增加
*/
void list_add(struct List* _l,float value);
/*
*	销毁列表	
*/
void list_dispose(struct List* _l);
//=======================================================================
//****************************************************************************//
/*字符串处理*/
//#include "str.h"

/*
*	str.h
*	字符串处理接口
*/
/*查找字符串首次出现的位置，没有找到返回 -1，两个字符串相等返回 0*/
int tl_strpos(const char *haystack,const char *needle,int ignorecase);
/*
	str		--整段文本
	target	--需要查询的文本

	-1未找到
*/
int tl_strpos_s(const char *str,const char *target);

/*
	字符串长度
*/
int tl_strlen(const char* str);
/*
	//该方法在替换\n的时候有bug
	 字符串替换函数.
	 ---------------------
	 能替换所有的要替换的字符串,被替换的字符串和替换的字符串不一定一样长.
	 pInput			-- 输入字符串.
	 pOutput		-- 输出字符串, 要保证足够的空间可以存储替换后的字符串.
	 pSrc			-- 要被替换的子字符串, 比如 <br>
	 pDst			-- 要替换成的字符串, 比如   /n
	 注意:以上的字符串均要以'\0'结尾.
*/	
void tl_replace(char *pInput, char *pOutput, char *pSrc, char *pDst);
/*
*	tl_getIndexByKey("run,stand,dead,","stand",',');		====>return 1;
*	在str中搜索name,返回索引,索引从0开始,未找到返回-1
*	可以应用于 动作关键帧搜索等
*/
int tl_getIndexByKey(const char* str,const char* name,const char d);

//========================================================
/*desc:	MD5Version 10 key:	MD5Version out:	10*/
void tl_setString(const char* dest,const char* key,char* out);
void tl_setInt(const char* dest,const char* key,int* out);
/*获取c在str中的index值,如果没有返回-1*/
int tl_charIndex(char* str,char c);
/*删除str中的ch字符*/
void tl_delchar(char *str,char ch);
/*是否是大端模式 0 不是 1 是*/
int tl_big_endian (void);
/*根据整个文件的名字获取文件路径*/
void tl_getPathByFile(char* path,char* dest);
/*分割字符后,获取最后一个字符串,path = "D://data/source/resource/md5/body.md5anim" ,sign = '\\/',dest =	body.md5anim*/
void tl_getLastString(const char* path,char* dest,const char sign);
/*
	根据文件路径获取文件类型
	----------------------------
	path		--	"D://data/source/resource/objFile/pTorus1.obj"
	out			--	"obj"
	_outBufferSize	--输出缓冲区大小
*/
void tl_getSuffixByPath(char* path,char* out,int _outBufferSize);
/*
*根据路径获取文件名(去后缀名)"D://data/source/resource/md5/body.md5anim" ===>	"body"
*/
void tl_getPathName(const char* dest,char* out);

char* tl_strsep(char **stringp, const char *delim);

/*
	将double 转化为str 去掉尾数0
*/
void tl_double2str(double d,char* out);

/*是否是数字 return 1:是0:否*/
int   tl_isNum(char* s);

/*
	字符串截取
	----------------------
	start	--起始坐标 0开始
	end		--结束坐标
*/
void  tl_strsub(const char * in,char* out,int start,int end);

/*======================================================================

切割字符串+回调

//void  tl_split(const char* dest,const char sign,CallBackFun pCallBack);

	多参数传递切割
	const char* input:		输入字符串
	const char sign:		切割标示
	CallBackFun pCallBack:	回调接口
======================================================================*/
void tl_split_s(const char* input,const char sign,CallBackFun pCallBack,
				int* parms,int parmsLen);
/*
	字符串切割,比tl_split_s性能更好,临时字符串使用的栈空间中的数据
	使用方法:


	int animsSplitExample(char* point)
	{
		struct StrCutSplit* p = (struct StrCutSplit*)point;
		char* str = p->str;
		if(!strcmp(str,(char*)p->parms))
		{
			//找到字符串
			printf("找到字符串{%s}\n",str);
		}
		return 1;
	}
	tl_split_cut("run,walk,dead,",',',animsSplitExample,(int*)"walk");//寻找切割字符串中是否有"walk"字段
*/
void tl_split_cut(const char* dest,const char sign,
				  int (*pCallBack)(struct StrCutSplit*),int* parms);


/*
	用例:

	
	//	int* inputParms	输入的参数引用,inputParms可以作为输入字符串进行业务逻辑处理
	//	char* splitStr	被切割的字符串片段
	
	static void f_obj_animsSplit2(int* inputParms,char* splitStr){
		parseOneLine(inputParms,splitStr);
	}

	T t;
	tl_splitByStr((const char*)data,OBJ_SIGN_CHAR_BR,f_obj_animsSplit2,&t);

*/
void tl_splitByStr(const char* dest,const char sign,void (*pCallBack)(int*,char*),int* parms);


void tl_strSplitLoop(struct StringSplit* sp);
/*char* str ===>char ch*/
char tl_str2ch(const char* str);

/*
	根据类型获取文本
	use:

	char anim[G_BUFFER_32_SIZE];
	tl_strType("stand,walk",animType,anim,G_BUFFER_32_SIZE);
	
	---------------
	anim		--文本总长度
	animType	--文本类型
	out			--输出文本
	wordSize	--单个文本长度
*/
void tl_strType(const char* str,int type,char* out,int wordSize);
/*
	根据文本获取类型
	----------------
*/
int tl_typeStr(const char* str,const char* target);
//=======================================================================
/*
*	填充一个向量
*/
void vec3Set(struct Vec3* v,float x,float y,float z);
/*
*	向量相加 out = a + b
*/
void vec3Add(struct Vec3* a,struct Vec3* b,struct Vec3* out);
/*
*	向量相减 out = a - b
*/
void vec3Sub(struct Vec3* a,struct Vec3* b,struct Vec3* out);

/*
	计算a,b,c三个顶点的法线向量(面法线向量),输出到out
*/
void vec3CalculateNormal(struct Vec3* a,struct Vec3* b,struct Vec3* c,struct Vec3* out);
/*
*	向量叉乘(叉积)
*/
void vec3Cross(struct Vec3* a,struct Vec3* b,struct Vec3* out);
/*
*	求向量的长度
*/
float vec3Length(struct Vec3* v);
/*
 *	是否是0向量
 */
int vec3IsZero(struct Vec3* v);

/*
*	向量乘以一个标量
*/
void vec3Mult(struct Vec3* a,float value);

/*
*向量点乘
*/
float vecDot(struct Vec3* a,struct Vec3* b);

/*
*	求两向量的距离
*/
float vec3Distance(struct Vec3* a,struct Vec3* b);
/*
	规格化，单位化（Normalize）
	先要说矢量的长度：
	矢量Vector（x，y，z）
	矢量长度Length（Vector）= |Vector|=sqr（x*x+y*y+z*z）
	Normalize后：
	（x/Length（Vector），y/Length（Vector），z/Length（Vector））
	方向不变，长度为1个单位
*/
void vec3Normalize(struct Vec3* v);

/*
	向量除以一个float
*/
void vec3Div(struct Vec3* o,float v);

/*
	求两向量的夹角0~180之间
*/
float vecToAngle(struct Vec3* a,struct Vec3* b);

/*
 *  射线拾取
 *  
 *  当拾取到多个交点的时候该方法只取离交点最近的交点
 *	tri			:三角形描述数据
 *	dataCount	:tri数组的长度 sizeof(float) * dataCount
 *	nearPoint	:最近点
 *	farPoint	:最远点
 *	fpos		:对象坐标
 */
void tl_pickTriangle(float* tri,int dataCount,struct Vec3* nearPoint,struct Vec3* farPoint,struct Vec3* fpos,struct HitResultObject* pResult);



//========================================================================
/*位图处理*/
/*
	解析位图 photoshop 文件格式:Windows 深度 24位
*/
unsigned char* tl_bmp24(const char* path,int* biWidth,int* biHeight);

/*
	复制一个字符串,返回一个堆上的引用
*/
char* tl_strCopy(const char* str);

/*
	获取被分割的个数
*/
int tl_split_getlen(const char* str,char key);

/*
	二的指数
	
	求指数函数,
	例如2的x次方等于8,返回3;
	t= 8的时候return 3;即2的3次方等于8,x=3;
*/
int tl_bintree(int t);

//=============================================================================================




#ifndef _XML_H_
#define _XML_H_
/*
Type:XmlNode
-----------------

/*
	功能:解析xml字符串
	-------------
	str				--	xml文本
	outMaxLen		--	parms输出的长度,此长度为最大长度

	--------------------------------------
	example:

	struct XMLSList* xml = NULL;
	char* xmlstr =NULL;
	char out[G_BUFFER_MAX_SIZE];
	struct XmlNode* node=NULL;
	tl_memInit();


	xmlstr=tl_loadfile("d:\\1.xml",NULL);

	xml = xml_parse(xmlstr,G_BUFFER_MAX_SIZE);

	node =	xml_getrow(xml,"id","10");
	if(node!=NULL)
	{
	int num;
	xml_getstr(node,"id",out);
	sscanf(out,"%d",&num);
	printf("(%s,%d)\n",out,num);
	xml_getstr(node,"path",out);
	printf("(%s)\n",out);
	xml_getstr(node,"language",out);
	printf("(%s)\n",out);
	node =	xml_getrow(xml,"id","2");
	p->nameMaxSize = xml_getint(node,"value");

	node =	xml_getrow(xml,"id","3");
	rate = xml_getfloat(node,"value");
	}
	tl_free(xmlstr);
	getchar();
	--------------------------------------
*/
struct XMLSList* xml_parse(const char* str);

/*
Function:	xml_getRow
---------------
获取表内的一行

*/
struct XmlNode* xml_getrow(struct XMLSList* xml,const char* parms,const char* parmsValue);

/*
删除xml
*/
void xml_del(struct XMLSList* xml);

/*
获取参数值

buffer		--缓冲区
bufferSize	--缓冲区大小

return	--1：获取成功, 2:获取失败

*/
int xml_getstr(const struct XmlNode* node,const char* key,char* buffer,int bufferSize);

/*
	获取int
*/
int xml_getint(const struct XmlNode* node,const char* key);

/*
	获取float
*/
float xml_getfloat(const struct XmlNode* node,const char* key);

/*
	返回一个tl_malloc的字符串引用,在堆上,用完要释放
*/
char* xml_getCopystr(const struct XmlNode* node,const char* key);
#endif


#ifndef _ARR_
#define _ARR_



/*
	数组创建
*/
struct Array* array_create(int length,int mode);

/*
	释放数组
*/
void array_free(struct Array* ptr);
/*
	存储数据局
*/
void array_put(struct Array* ptr,int d);
/*
	数组长度
*/
int array_len(struct Array* ptr);
/*
	获取数据找到数据返回1,没有数据就0
*/
int array_get(struct Array* ptr,int index,int* outData);
/*
	设置数据
*/
int array_set(struct Array* p,int index,int data);
/*
	打印数组数据
*/
void array_log(struct Array* p);

/*
	根据key切割成多个字符文件,保存在数组中,字符串切割成字符串数组
*/
struct Array* array_split(const char* str,char key);

#endif

#ifndef _STR_H_
#define _STR_H_

/*
创建一个字符串
*/
struct Str* str_create(const char* s);

/*
销毁
*/
void str_dispose(struct Str* _str);
/*
在字符串尾部添加新字符串
*/
void str_appand(struct Str* _str,const char * s);
/*
在字符串尾部添加新字符串,指定截取字符串的长度为len
*/
void str_appand_s(struct Str* _str,const char * s,int len);

/*
字符串长度
*/
int str_length(struct Str* str);
/*
字符串替换
pSrc			-- 要被替换的子字符串, 比如 <br>
pDst			-- 要替换成的字符串, 比如   /n
注意:以上的字符串均要以'\0'结尾.
*/
void str_replace(struct Str* str,char *pSrc, char *pDst);

/*
	将dest中的字符数据copy到target中
*/
void str_copy(struct Str* target,struct Str* dest);

#endif
//=============================================================================================

#ifndef _MEM_
#define _MEM_
/*
	内存池模块
	支持当内存块用完的时候,动态申请
*/

/*开辟一块内存*/
void* tl_malloc(int size);

/*释放一块内存*/
void tl_free(void* s);

/*
	正在使用的内存总大小

	注:该接口只返回tl_malloc出来的内存块总尺寸
*/
int tl_memByteSize();

/*
	日志字符缓冲区512字节,打印当前内存池的内存
*/
void tl_showCurMemery512(const char* str);
/*初始化*/
void tl_memInit(const char* pool);
/*
	设置开关状态,TRUE为开启内存池,FALSE为关闭内存池
*/
void tl_memState(int _stat);

/*
	获取内存池的开启状态
*/
int tl_memGetStat();
#endif

//=============================================================================================
#ifndef _RAMP_H_
#define _RAMP_H_
/************************************************************************
* ramp		渐变接口
* 实现		vec的渐变
/************************************************************************/


//接口定义

/************************************************************************/
/* 在ms毫秒内将px移动到tx                                             */
/************************************************************************/
struct BaseLoopVO* ramp_vec_trace(float* px,float* py,float* pz,float tx,float ty,float tz,float ms);


/************************************************************************/
/* 延迟ms调用callBack(parms)                                                                     */
/************************************************************************/
struct BaseLoopVO* ramp_delay(void (*callBack)(void*),void* parms,float ms);
/*
	渐变回调,此方法放在渲染循环里
*/
void ramp_callBack();

/*
	是否在播放
*/
int ramp_isPlaying(struct BaseLoopVO* node);
/*
	停止播放
*/
void ramp_stopPlaying(struct BaseLoopVO* node);
#endif
//=============================================================================


//=============================================================================
#ifndef _PCSV_H_
#define _PCSV_H_


//测试用例
/************************************

struct PCsvVO * pcsv;
int out;
Array* arr;
tl_memState(TRUE);
printf("===========>%d bytes\n",tl_memByteSize());
pcsv = pcsv_load("D:\\source\\resource\\CaiLiaoHeCheng.csv","itemid",1);

arr=pcsv_getValue(pcsv,"qianghua10");
array_get(arr,1,&out);
if(out){
	printf("%s\n",(char*)out);
}
pcsv_dispose(pcsv);
printf("===========>%d bytes\n",tl_memByteSize());

***************************************/

/*
*	加载数据
*	const char* path	:加载的路径
*	const char* key		:主键
*	int checkIndex		:用第几行作为key名列表,从0开始(csv表中 只用一行作为主键键名.如果是第1行作为主键,checkIndex=0)
*/
struct PCsvVO* pcsv_load(const char* path,const char* mainkey,int checkIndex);
/*
销毁PCsvVO数据
*/
void pcsv_dispose(struct PCsvVO* p);

/*
根据keyValue匹配主键的值获取该行数据
注意:这里的Array不需要用array_free释放,因为在pcsv_dispose会释放
*/
struct Array* pcsv_getValue(struct PCsvVO* p,const char* keyValue);

//========================================================================
#endif
#ifndef _VMATH_H
#define  _VMATH_H
 
 #ifdef _cplusplus
extern "C" {
#endif
 /*
0(0,0)	1(0,1)		2(0,2)		3(0,3)
4(1,0)	5(1,1)		6(1,2)		7(1,3)
8(2,0)	9(2,1)		10(2,2)		11(2,3)
12(3,0)	13(3,1)		14(3,2)		15(3,3)
 */
typedef float Matrix44f[16];
/*
	打印矩阵
*/
void mat4x4_printf(const char* name,Matrix44f M);
void mat4x4_double_printf(const char* name,double m[16]);
/*
	矩阵归一化
*/
void mat4x4_identity(Matrix44f M);

/*
 *反转矩阵(矩阵的逆)
 */
void mat4x4_invert(Matrix44f out,Matrix44f a);

void mat4x4_transformMat4(float out[4],float a[4],Matrix44f m);
/*
	矩阵位移
*/
void mat4x4_translate(Matrix44f M, float x, float y, float z);
//sx,sy,sz	缩放
void mat4x4_scale(Matrix44f M, float x, float y, float z);
////初始化归一矩阵
//void mat4x4_identity(Matrix44f M);
/*
	二维数组矩阵赋值
*/
void mat4x4_2t1(Matrix44f M,int m,int n,float value);
/*
	将a矩阵的值copy到out中
*/
void mat4x4_copy(Matrix44f M,Matrix44f out);
/*
	XYZ旋转
*/
void mat4x4_rotateX(Matrix44f M, float radian);

void mat4x4_rotateY(Matrix44f M, float radian);

void mat4x4_rotateZ(Matrix44f M, float radian);

void mat4x4_rotateXYZ(Matrix44f M, float radian, float x, float y, float z); 
 
void perspectiveFrustum(Matrix44f M, float left, float right, float top, float bottom, float near, float far); //透视投影
void orthoPerspect(Matrix44f M, float left, float right, float top, float bottom, float near, float far);  //正交投影
 
//void matrix_m(float **a_matrix, const float **b_matrix, const float **c_matrix,
//			  int krow, int kline, int kmiddle, int ktrl);
/*
	1维数组矩阵相乘,4x4 注意矩阵乘法有先后顺序
*/
void multi2(Matrix44f out,Matrix44f a, Matrix44f b);
/*
	数组的每个选项都是0
*/
void mat4x4_zero(Matrix44f M);

/**
 * 矩阵行列转置
 * Transpose the values of a mat4
 *
 * @param {mat4} out the receiving matrix
 * @param {mat4} a the source matrix
 * @returns {mat4} out
 */
void mat4x4_transpose(Matrix44f a);
/*
 *让矩阵朝向某个目标
 */
void mat4x4_lookAt(Matrix44f out,struct Vec3* eye,struct Vec3* center,struct Vec3* up);

//矩阵连乘
void mat4x4_mult(int c,Matrix44f o,...);

#ifdef _cplusplus
}
#endif
#endif
//========================================================================

//========================================================================

//设置状态位
void setv(int* v,int flag);
void resetv(int* v,int flag);
/*
	获取标示的状态
*/
int getv(int* v,int t);

//路径转化
int tl_convert_path(char* in,char* out,int outBufferSize);

/*
	地址转const char*
*/
void tl_address2str(int v,char* buffer,int bufferSize);
/*
 *获取偏移值
 */
int tl_getGap(int type);

/*
	构造一个新名字
*/
void tl_newName(char* buffer,int bufferSize);
/*
	将"true","false"转化为1,0
*/
int tl_boolString2Int(const char* str);
/*
	0,1转化为"true","false"
*/
const char* tl_intConvertBool(int i);

/*
*	将四元数转化为矩阵
*/
void 
Quat_to_matrrix(const Quat4_t qa,Matrix44f out);

#endif