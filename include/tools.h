#ifndef _TOOLS_H_
#define _TOOLS_H_

#define _DEBUG_MODE_

#include "global_define.h"
#include "vmath.h"
/*
	是否使用开发模式,用户调试
	0关闭
	1开启
*/
//static const int _IS_DEVELOPMENT_ = 1;

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

/*
*双端链栈
*/
typedef struct LStackNode{
	int data;
	struct LStackNode *pre;//前驱节点
	struct LStackNode *next;//下一个节点
};

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
*	设置局部路径,如果设置了局部路径将影响 tl_loadfile
*/
void tl_set_local_path(const char* path);

/*
 *	读写一个文件,获取文件中的文本加载glsl的时候使用,或者加载obj等文件时使用,
 *	int* pLength	文件长度(字节) 外部传入指针 用保存字符串的长度
 *	注:加载成功之后记得用 tl_free 释放字符引用!
 */
char* tl_loadfile(const char* path,int* pLength);

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
//void tl_http_get_query_string(char* result);//废弃

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

//=============================================================================

/*
*	顶点解析
*/
struct VerNorUvStruct* tl_parseVerts2(float* verts,int len);
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

	
	void (*pCallBack)(char*,void*),
	char* 分割的子字符串
	void* 传递的参数引用
*/
void tl_split_cut(
				  const char* dest,const char sign,
				  void (*pCallBack)(char*,void*),
				  void* parms);
 //int (*pCallBack)(struct StrCutSplit*)
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


//void tl_strSplitLoop(struct StringSplit* sp);
/*
	char* str ===>char ch
	"A" ==>'A'
*/
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
//void tl_strType(const char* str,int type,char* out,int wordSize);
/*
	根据文本获取类型
	----------------
*/
int tl_typeStr(const char* str,const char* target);

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

#ifndef _MEM_
#define _MEM_
/*
	内存池模块
	支持当内存块用完的时候,动态申请
*/
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

/*	

	设置状态位
	v:一个引用地址
*/
void setv(int* v,int flag);
/*
*	清空flag位置的状态为0
*/
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


#endif