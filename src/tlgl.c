/*
OpenGL 工具箱
opengl相关算法接口等等
*/
//#define _OPEN_GL_TOOL_
//#ifdef _OPEN_GL_TOOL_
//#pragma warning(disable:4996)

#include <gl/glew.h>
#include <gl/glut.h>

#include <stdio.h>

#include <string.h>
#include <assert.h>

#include <math.h>

#include "tools.h"
#include "tl_malloc.h"
#include "gettime.h"
#include "str.h"
#include "jgl.h"
#include "tlgl.h"
#include "tmat.h"
#include "obj_vbo.h"
#include "node.h"
#include "evt.h"
#include "str.h"



#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

static int g_vbo_size;
//struct GProgramObject* gProgramObject=NULL;


/*
	打印顶点数组
*/
static void 
f_printfVers(float* vert,int count){
	int i,m = 0;
	printf("0		1		2		3		4		5		6			7\n");
	printf("u		v		nx		ny		nz		x		y			z\n");
	for( i =0;i<count;i++){

		if(m<7){
			m++;
			if(m == 1){
				printf("\n");
			}
		}else{
			m = 0;
		}
		printf("(%d)%f	",i,vert[i]);
	}
}
/************************************************************************/
/* 跨度为8个float的结构                                                 */
/************************************************************************/
static void 
sortList(float* vertexs,int count,
					 float* pMin,float* pMax,int offsetValue)
{
	int i;
	int _curCount = count*sizeof(float);
	float* xList = (float*)tl_malloc(_curCount);
	//float xList[12000];
	for(i  = 0;i< count;i++){
		float v = vertexs[i*8+5+offsetValue];
		//printf("%f\n",v);
		xList[i] = v;
	}
	tl_quickSort(xList,count);
	*pMin = xList[0];
	*pMax = xList[count-1];
	tl_free(xList);
}
static float 
tl_max(float v0,float v1){
	return v0 > v1 ? v0 : v1;
}
/*取绝对值最大的做碰撞盒(8个float跨度)*/
void 
tlgl_absMax(float* vertexs,int count,float* out){
	float minX,maxX,minY,maxY,minZ,maxZ;
	float v0,v1,v2;
	sortList(vertexs,count,&minX,&maxX,0);
	sortList(vertexs,count,&minY,&maxY,1);
	sortList(vertexs,count,&minZ,&maxZ,2);
	if(maxX < 0 || maxY < 0 || maxZ <0 || minX > 0 || minY > 0 || minZ > 0){
		assert(0);
	}
	//abs();
	//max(abs(minX),
	v0 = tl_max((float)abs(minX),maxX);
	v1 = tl_max((float)abs(minY),maxY);
	v2 = tl_max((float)abs(minZ),maxZ);
	out[0] = -v0;
	out[1] = v0;
	out[2] = -v1;
	out[3] = v1;
	out[4] = -v2;
	out[5] = v2;
}
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
static void 
f_minMax(float* vertexs,int count,float* out){
	float minX,maxX,minY,maxY,minZ,maxZ;
	sortList(vertexs,count,&minX,&maxX,0);
	sortList(vertexs,count,&minY,&maxY,1);
	sortList(vertexs,count,&minZ,&maxZ,2);
	out[0] = minX;
	out[1] = maxX;
	out[2] = minY;
	out[3] = maxY;
	out[4] = minZ;
	out[5] = maxZ;
}

static int 
f_boxArray(float* in,float* out){
	#define X_min 0
	#define X_max 1

	#define Y_min 2
	#define Y_max 3

	#define Z_min 4
	#define Z_max 5

	#define _LENGTH_ BOX_SIZE

	float triData[_LENGTH_] = 
	{
		in[X_min], in[Y_min], in[Z_max],		in[X_max], in[Y_min], in[Z_max],		in[X_min],  in[Y_max], in[Z_max],

		in[X_max], in[Y_min], in[Z_max],in[X_max], in[Y_max], in[Z_max],in[X_min], in[Y_max], in[Z_max],

		in[X_min], in[Y_max], in[Z_max],in[X_max], in[Y_max], in[Z_max],in[X_min], in[Y_max], in[Z_min],

		in[X_max], in[Y_max], in[Z_max],in[X_max], in[Y_max], in[Z_min],in[X_min], in[Y_max], in[Z_min],

		in[X_min], in[Y_max], in[Z_min],in[X_max], in[Y_max], in[Z_min],in[X_min], in[Y_min], in[Z_min],

		in[X_max], in[Y_max], in[Z_min],in[X_max], in[Y_min], in[Z_min],in[X_min], in[Y_min], in[Z_min],

		in[X_min], in[Y_min], in[Z_min],in[X_max], in[Y_min], in[Z_min],in[X_min], in[Y_min], in[Z_max],

		in[X_max], in[Y_min], in[Z_min],in[X_max], in[Y_min], in[Z_max],in[X_min], in[Y_min], in[Z_max],

		in[X_max], in[Y_min], in[Z_max],in[X_max], in[Y_min], in[Z_min],in[X_max], in[Y_max], in[Z_max],

		in[X_max], in[Y_min], in[Z_min],in[X_max], in[Y_max], in[Z_min],in[X_max], in[Y_max], in[Z_max],

		in[X_min], in[Y_min], in[Z_min],in[X_min], in[Y_min], in[Z_max],in[X_min], in[Y_max], in[Z_min],

		in[X_min], in[Y_min], in[Z_max],in[X_min], in[Y_max], in[Z_max],in[X_min], in[Y_max], in[Z_min]

	};
	memcpy(out,triData,(int)sizeof(float)*_LENGTH_);
	return _LENGTH_;
}
//float* tl_createBoxArray(float* in,int* pLength){
//	float* out = tl_malloc(sizeof(float)*_LENGTH_);
//	float triData[_LENGTH_] = 
//	{
//		in[X_min], in[Y_min], in[Z_max],		in[X_max], in[Y_min], in[Z_max],		in[X_min],  in[Y_max], in[Z_max],
//
//		in[X_max], in[Y_min], in[Z_max],in[X_max], in[Y_max], in[Z_max],in[X_min], in[Y_max], in[Z_max],
//
//		in[X_min], in[Y_max], in[Z_max],in[X_max], in[Y_max], in[Z_max],in[X_min], in[Y_max], in[Z_min],
//
//		in[X_max], in[Y_max], in[Z_max],in[X_max], in[Y_max], in[Z_min],in[X_min], in[Y_max], in[Z_min],
//
//		in[X_min], in[Y_max], in[Z_min],in[X_max], in[Y_max], in[Z_min],in[X_min], in[Y_min], in[Z_min],
//
//		in[X_max], in[Y_max], in[Z_min],in[X_max], in[Y_min], in[Z_min],in[X_min], in[Y_min], in[Z_min],
//
//		in[X_min], in[Y_min], in[Z_min],in[X_max], in[Y_min], in[Z_min],in[X_min], in[Y_min], in[Z_max],
//
//		in[X_max], in[Y_min], in[Z_min],in[X_max], in[Y_min], in[Z_max],in[X_min], in[Y_min], in[Z_max],
//
//		in[X_max], in[Y_min], in[Z_max],in[X_max], in[Y_min], in[Z_min],in[X_max], in[Y_max], in[Z_max],
//
//		in[X_max], in[Y_min], in[Z_min],in[X_max], in[Y_max], in[Z_min],in[X_max], in[Y_max], in[Z_max],
//
//		in[X_min], in[Y_min], in[Z_min],in[X_min], in[Y_min], in[Z_max],in[X_min], in[Y_max], in[Z_min],
//
//		in[X_min], in[Y_min], in[Z_max],in[X_min], in[Y_max], in[Z_max],in[X_min], in[Y_max], in[Z_min]
//
//	};
//	memcpy(out,triData,(int)sizeof(float)*_LENGTH_); 
//	*pLength = _LENGTH_;
//	return out;
//}

//
//#undef X_min
//#undef X_max
//#undef Y_min
//#undef Y_max
//#undef Z_min
//#undef Z_max
//#undef _LENGTH_


//设置固定长宽高的包围盒
void tlgl_setAbsBoundBox(float x,float y,float z,float* triData){
	float v[6]={-x/2,x/2,-y/2,y/2,-z/2,z/2};
	f_boxArray(v,triData);
}

//测试用灯
//struct Vec3 gLightPos;
//float gFloatValue = 1.5f;


//static float s_VBO_Size;	/*VBO大小kb*/

//float tlgl_vbo_use(){
//	return s_VBO_Size;
//}


/*
OpenGL Version	GLSL Version
			2.0	110
			2.1	120
			3.0	130
			3.1	140
			3.2	150
			3.3	330
			4.0	400
			4.1	410
			4.2	420
			4.3	430
*/
void tlgl_getVersion(){
	int major,minor;
	const char* verstr = (const char*)glGetString( GL_VERSION );
	if( (verstr == NULL) || (sscanf_s( verstr, "%d.%d", &major, &minor ) != 2) ){
		major = minor = 0;
		printf("Invalid GL_VERSION format!!!\n" );
	}else{
		printf("opengl 版本: %s \n",verstr);
	}
}
void tlgl_setVboSize(int size,int isAdd){
	isAdd == 1 ? (g_vbo_size+=size) : (g_vbo_size-=size);
}

int tlgl_getVboSize()
{
	return g_vbo_size;
}
void tlgl_showGLVersion(){
	int err;
	//返回负责当前OpenGL实现厂商的名字
	const GLubyte* name = glGetString(GL_VENDOR);			

	//返回一个渲染器标识符，通常是个硬件平台
	const GLubyte* biaoshifu = glGetString(GL_RENDERER);	

	//返回当前OpenGL实现的版本号
	const GLubyte* OpenGLVersion =glGetString(GL_VERSION);	

	//返回当前GLU工具库版本
	const GLubyte* gluVersion= gluGetString(GLU_VERSION);	
//#ifdef DEBUG
	printf("==================================================================\n");
	printf("OpenGL实现厂商的名字：%s\n渲染器标识符：%s\nOpenGL实现的版本号：%s\nGLU工具库版本：%s\n",name,biaoshifu,OpenGLVersion,gluVersion);
	printf("==================================================================\n");
//#endif
	err = strcmp((const char*)OpenGLVersion,"1.1.0");
	if(!err){
		printf("该版本的OPENGL不支持VBO,VAO!!!\n");
		assert(0);
	}
	
}

GLuint tlgl_bindTex(GLuint program3D,GLsizei w,GLsizei h,GLubyte* pImage,const char* tex,int texIndex){
	GLuint mTextureID=0;
	
	GLuint localTex = glGetUniformLocation(program3D,tex);//"texture"

	/*选择当前活跃的纹理单元*/
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D,mTextureID);

	//GLuint* outTexID,
	//*outTexID = mTextureID;
	
	/*绑定采样贴图标示到着色器,0对应纹理第一层*/
	glUniform1i(localTex,texIndex);//0

	/*根据指定的参数,上载位图数据,生成一个2D纹理Texture*/
	glTexImage2D(GL_TEXTURE_2D,0,3,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,pImage);

	/*释放pImage数据的缓冲区*/
	//tl_free((void*)pImage);

	/*此为纹理过滤参数设置*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );
	return mTextureID;
}


void tlgl_bindVar(GLuint program3D,float* lightDir)
{
	GLuint loc2 = glGetUniformLocation(program3D,"lightDir");    
	glUniform3fv(loc2,3,lightDir);    
}
struct IShaderName
{
	int i;
	char vs[G_BUFFER_32_SIZE];
	char ps[G_BUFFER_32_SIZE];
};
//顶点着色器
//"uniform mat4 mat1;"
//"void main(){									"
//"	gl_TexCoord[0] = gl_MultiTexCoord0;			"
//"	gl_Position =  gl_ModelViewProjectionMatrix *  gl_Vertex;		"
//"}",




static void fSplitHandler(int* parms,char* str){
	struct IShaderName* p = (struct IShaderName*)parms;
	//resload_push(res,str);
	if(p->i == 0){
		memcpy(p->vs,str,strlen(str));
	}else{
		memcpy(p->ps,str,strlen(str));
	}
	p->i++;
}
void tlgl_glslProgrom(GLuint* progrom3D,const char* name){

	char *vs=NULL,*ps=NULL;
	struct IShaderName sn;
	char __vs[G_BUFFER_128_SIZE];
	char __ps[G_BUFFER_128_SIZE];
	memset(&sn,0,sizeof(struct IShaderName));

#ifdef DEBUG
	printf("创建着色器... (%s)\n",name);
#endif
	str_split(name,';',fSplitHandler,(int*)&sn);
	memset(__vs,0,G_BUFFER_128_SIZE);
	sprintf_s(__vs,G_BUFFER_128_SIZE,"\\resource\\shader\\%s.vs",sn.vs);

	memset(__ps,0,G_BUFFER_128_SIZE);
	sprintf_s(__ps,G_BUFFER_128_SIZE,"\\resource\\shader\\%s.ps",sn.i <= 1 ? sn.vs : sn.ps);

	vs = tl_loadfile(__vs,NULL);
	ps = tl_loadfile(__ps,NULL);

	tlgl_createShader(vs,ps,progrom3D);

	tl_free((void*)vs);
	tl_free((void*)ps);
}
void tlgl_drawPloyLine(){

	GLfloat polyfactor = 1.0;
	GLfloat polyunits = 1.0;

	//剔除多边形背面
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glDisable(GL_POLYGON_OFFSET_FILL);

	//禁用多面显示
	glDisable(GL_CULL_FACE);
	
	//设置线框的偏移,否则会看到线框若隐若现
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(polyfactor, polyunits);
	
	//设置线框尺寸
	glLineWidth(1.0f);
	glUseProgram(0);
	glColor3f (1.0, 1.0, 0.0);
	//设置多边形渲染方式(实体还是线框)
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	
}
enum{
	GRAPHICS_ATTRIB_VERTEX = 0,
	GRAPHICS_ATTRIB_NORMAL = 1,
	GRAPHICS_ATTRIB_TEXTURE = 2,
};
/*
	将数据绑定一些变量到着色器程序中
*/
static void
f_bind_GLSL_var(GLuint program)
{
	// 将通用顶点属性索引与命名属性变量相关联
	glBindAttribLocation(program, GRAPHICS_ATTRIB_VERTEX, "_Position");	//顶点xyz
	glBindAttribLocation(program, GRAPHICS_ATTRIB_NORMAL, "_TexCoord");	//uv
	glBindAttribLocation(program, GRAPHICS_ATTRIB_TEXTURE, "_Normal");	//normal
}

/*
 *显示片段着色器程序错误日志
 */
static void 
showShaderErrorLog(GLuint _shaderObject)
{
	GLint logLen;
	//得到编译日志长度
	glGetShaderiv(_shaderObject,GL_INFO_LOG_LENGTH,&logLen);

	if (logLen > 0)
	{
		char *log = (char *)tl_malloc(logLen);
		GLsizei written;
		//得到日志信息并输出
		glGetShaderInfoLog(_shaderObject,logLen,&written,log);
		//cerr << "vertex shader compile log : " << endl;
		//cerr << log << endl;

		printf("shader compile log : %s\n",log);

		tl_free(log);//释放空间
	}

}

int 
tlgl_createShader(GLchar* vertex,GLchar* fragment,GLuint* outProgram){

	//顶点着色器对象
	GLuint vertexShaderObject = 0;

	//片段着色器对象
	GLuint fragmentShaderObject = 0;
	GLint vertCompiled, fragCompiled;
	GLint linked;
	GLuint programObject;
	// 创建一个顶点着色器对象和一个片元着色器对象
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	
	// 加载glsl着色器文本对象到着色器对象中
	glShaderSource(vertexShaderObject, 1,(const GLchar**) &vertex, NULL); 
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragment, NULL); 
	
	// 编译顶点着色器
	glCompileShader(vertexShaderObject); 
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &vertCompiled); 
	
	// 编译片元着色器
	glCompileShader(fragmentShaderObject); 
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &fragCompiled); 
	if (!vertCompiled) {
		printf("======================fail vertCompiled!\n");
		printf("\n***********************************************************\n%s\n***********************************************************\n",vertex);
		
		showShaderErrorLog(vertexShaderObject);

		assert(0);
		return FALSE;
	} 
	if(!fragCompiled){
		printf("=========================fail fragCompiled!\n");
		printf("\n***********************************************************\n%s\n***********************************************************\n",fragment);

		showShaderErrorLog(fragmentShaderObject);
		assert(0);
		return FALSE;
	}

	// 创建一个程序对象并赋予到两个着色器对象中
	programObject = glCreateProgram();
	glAttachShader(programObject, vertexShaderObject);
	glAttachShader(programObject, fragmentShaderObject);

	f_bind_GLSL_var(programObject);

	// 连接程序
	glLinkProgram(programObject);
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if (!linked){
		printf("shader link fail!");
		assert(0);
		return FALSE;
	}
	*outProgram =  programObject;
	return TRUE;
}

void tlgl_AABB(float* trianglesArray,int vertexCount,float* triData,
			   int (*callBack)(char*),
			   int parms,float r,float g,float b,int isDraw){

	/*
	v[0] = minX;
	v[1] = maxX;
	v[2] = minY;
	v[3] = maxY;
	v[4] = minZ;
	v[5] = maxZ;
	*/
	float v[6]={0,0,0,0,0,0};

	f_minMax(trianglesArray,vertexCount,v);

	//构建包围盒并存储在triData数组中
	f_boxArray(v,triData);
	
	if(isDraw == TRUE){
		//是否进行绘制

		if(callBack!=NULL)
			callBack((char*)parms);

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D,0);
		
		glColor3f (r, g, b);
		
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		glInterleavedArrays(GL_V3F,0,triData);
		glDrawArrays(GL_TRIANGLES,0,BOX_SIZE/3);
	}
}
//#undef SIZE

void tlgl_drawTrianglesLine(float* _array,int _arrayCount,float r,float g,float b){
	glDisable(GL_CULL_FACE);
	glUseProgram(0);
	glColor3f (r, g, b);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	glInterleavedArrays(GL_V3F,0,_array);
	glDrawArrays(GL_TRIANGLES,0,_arrayCount/3);
}

//void tlgl_screenSize(float* w,float* h){
//	*w = s_ScreenWidth;
//	*h = s_ScreenHeight;
//}
void tlgl_drawText(const char* _txtStr,float x,float y,GLfloat r,GLfloat g,GLfloat b){

	unsigned int i;
	//要显示的字符
	//char *str = "current fps = ";
	unsigned int n = (unsigned int)strlen(_txtStr);

	glUseProgram(0);
	glColor3f(r, g, b);

	//glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glRasterPos2i(x,y);
	//glBindTexture(GL_TEXTURE_2D,(GLuint)NULL);



	//逐个显示字符串中的每个字符
	for ( i = 0; i < n; i++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *(_txtStr+i));
	}
	glPopAttrib ();
}

void tlgl_drawSign(float* pos,float size){
	if(size > 0){
		int i,k;
		glBindTexture(GL_TEXTURE_2D,(GLuint)NULL);
		glPointSize(1.0f);
		for(i = 0;i < 3;i++){
			float tmp[3] = {0,0,0};
			for(k = 0;k < 3;k++){
				if(i == k){
					tmp[k]=size;
					break;
				}
			}
			glBegin(GL_LINES); 
			for(k=-1;k<=1;k+=2){	
				glVertex3f(pos[0]+k*tmp[0],pos[1]+k*tmp[1],pos[2]+k*tmp[2]);
			}
			glEnd();
		}
	}
}
void tlgl_drawLine(float* a,float* b)
{
	glPointSize(1.0f);
	glBegin(GL_LINE_STRIP); 
	glVertex3f(a[0],a[1],a[2]);
	glVertex3f(b[0],b[1],b[2]);
	glEnd();
}
//int tlgl_getCurTime()
//{
//	return glutGet(GLUT_ELAPSED_TIME);
//}


/*
解析输出
u,v	normalX,normalY,normalZ posX,posY,posZ
*/
static void 
f_parseVerts(float* verts,int* pVerts,int* pUVs,int* pNormals,int len){
	int i,k = 0,u = 0,n = 0,ver_span = 3,uv_spane=2,normal_span = 3;
	int all_span = ver_span + normal_span + uv_spane;
	float* vs =			(float*)tl_malloc(len*ver_span);
	float* normals =	(float*)tl_malloc(len*normal_span);
	float* uvs =		(float*)tl_malloc(len*uv_spane);

	memset(vs,0,len*ver_span);
	memset(normals,0,len*normal_span);
	memset(uvs,0,len*uv_spane);

	for(i = 0;i<len*all_span;i+=all_span){
		uvs[u] = verts[i];		
		uvs[u+1] = verts[i+1];

		normals[n]= verts[i+2];	
		normals[n+1]= verts[i+3];	
		normals[n+2]= verts[i+4];

		vs[k] = verts[i+5];		
		vs[k+1] = verts[i+6];		
		vs[k+2] = verts[i+7];

		u+=uv_spane;k+=ver_span;n+=normal_span;
	}
	*pVerts = (int)vs;
	*pNormals = (int)normals;
	*pUVs = (int)uvs;
}
void tlgl_vao(GLuint* vao,int* pLen,GLfloat* verts){
	GLuint vbo[2];
	GLuint posBuffer,uvBuffer;
	int vs,uvs,normals;
	f_parseVerts(verts,&vs,&uvs,&normals,*pLen);

	glGenBuffers(2, vbo);
	posBuffer = vbo[0];
	uvBuffer  = vbo[1];
	glBindBuffer(GL_ARRAY_BUFFER,posBuffer);
	glBufferData(GL_ARRAY_BUFFER,(*pLen) * 3 * sizeof(float),(GLfloat*)vs,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
	glBufferData(GL_ARRAY_BUFFER,(*pLen) * 2 * sizeof(float),(GLfloat*)uvs,GL_STATIC_DRAW);

	glGenVertexArrays(1,vao);
	glBindVertexArray(*vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
	
	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//	printf("vert:%d,uv:%d\n",posBuffer,uvBuffer);
	tl_free((void*)vs);
	tl_free((void*)uvs);
	tl_free((void*)normals);
}

void tlgl_deleteVBO(GLuint* m_nVBOVertices,GLuint* m_nVBOTexCoords){
	glDeleteBuffers(1,(const GLuint*)&m_nVBOTexCoords);
	glDeleteBuffers(1,(const GLuint*)&m_nVBOVertices);
	printf("删除VBO %d %d\n",(int)m_nVBOVertices,(int)m_nVBOTexCoords);
}
/*
	glGenBuffers()创建缓存对象并且返回缓存对象的标示符。它需要2个参数：第一个为需要创建的缓存数量，第二个为用于存储单一ID或多个ID的GLuint变量或数组的地址。
	void glGenBuffers(GLsizei n, GLuint* buffers) 

	//判断是否是缓冲区对象
	GLboolean glIsBuffer(GLuint buffer);

	//指定当前活动缓冲区的对象
	void glBindBuffer(GLenum target, GLuint buffer);

	//target:可以是GL_ARRAY_BUFFER()（顶点数据）或GL_ELEMENT_ARRAY_BUFFER(索引数据)
	//size:存储相关数据所需的内存容量
	//data:用于初始化缓冲区对象，可以是一个指向客户区内存的指针，也可以是NULL
	//usage:数据在分配之后如何进行读写,如GL_STREAM_READ，GL_STREAM_DRAW，GL_STREAM_COPY
	void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	

	GLuint* m_nVBOVertices,
					GLuint* m_nVBOTexCoords,
					GLuint* m_nVBONormals,
*/

void tlgl_render_triangles(GLfloat* vertex,int vertexCount)
{
	if(!vertexCount){
		printf("vertexCount is 0\n");
		assert(0);
	}

	if(!vertex){
		printf("vertex is NULL\n");
		assert(0);
	}
	
	glInterleavedArrays(GL_T2F_N3F_V3F,0,vertex);
	glDrawArrays(GL_TRIANGLES,0,(GLsizei)vertexCount);
}

/*
	Vertex Buffer Object(VBO)

glEnableClientState(GLenum array );

	Value	Meaning
	GL_COLOR_ARRAY	If enabled, use color arrays with calls to glArrayElement, glDrawElements, or glDrawArrays.
	See also glColorPointer.
	GL_EDGE_FLAG_ARRAY	If enabled, use edge flag arrays with calls to glArrayElement, glDrawElements, or glDrawArrays.
	See also glEdgeFlagPointer.
	GL_INDEX_ARRAY	If enabled, use index arrays with calls to glArrayElement, glDrawElements, or glDrawArrays.
	See also glIndexPointer.
	GL_NORMAL_ARRAY	If enabled, use normal arrays with calls to glArrayElement, glDrawElements, or glDrawArrays.
	See also glNormalPointer.
	GL_TEXTURE_COORD_ARRAY	If enabled, use texture coordinate arrays with calls to glArrayElement, glDrawElements, or glDrawArrays.
	See also glTexCoordPointer.
	GL_VERTEX_ARRAY	If enabled, use vertex arrays with calls to glArrayElement, glDrawElements, or glDrawArrays.
	See also glVertexPointer.


===============================================================================================================
void glVertexPointer(GLint size,GLenum type,GLsizei stride,const GLvoid * pointer)
参数：
	size：指定了每个顶点对应的坐标个数，只能是2,3,4中的一个，默认值是4
	type：指定了数组中每个顶点坐标的数据类型，可取常量:GL_BYTE, GL_SHORT,GL_FIXED,GL_FLOAT;
	stride:指定了连续顶点间的字节排列方式，如果为0，数组中的顶点就会被认为是按照紧凑方式排列的，默认值为0
	pointer:制订了数组中第一个顶点的首地址，默认值为0，对于我们的android，大家可以不用去管什么地址的，一般给一个IntBuffer就可以了。
	描述：
	当开始render的时候，glVertexPointer 用一个数组指定了每个顶点的坐标，
	size指定了每个顶点的坐标个数
	type指定了每个坐标的数据类型，(和尚注:注意，这里不同的数据类型含义不同，如果选择GL_FIXED，那么0x10000表示单位长度，如果选择GL_FLOAT那么10f表示单位度。)
	stride指定了一个顶点和下一个顶点间数据的排列方式，是"单一数组"还是"分散数组"，单一数组的存储方式通常是更具效率的。当一个顶点数组被指定以后，size,type,stride,pointer被存储成client-side
	当glDrawArrays或者glDrawElements被调用的时候，如果顶点数组处于可用状态，就会被使用；可以使用glEnableClientState或者glDisableClientState来激活或者禁用一个顶点数组，顶点数组默认是不可用并且是不可被glDrawArrays、glDrawElements两个函数使用的。
	使用glDrawArrays函数可以根据顶点数组构建一个原始构图序列
	使用glDrawElements可以根据序列化的顶点集合来创建相同的序列
	Notes
	glVertexPointer is typically implemented on the client side
	Errors
	GL_INVALID_VALUE is generated if size is not 2, 3, or 4
	GL_INVALID_ENUM is generated if type is is not an accepted value
	GL_INVALID_VALUE is generated if stride is negative
	错误：
	GL_INVALID_VALUE size 不是 2, 3, 4中的一个的时候
	GL_INVALID_ENUM type 不是一个允许的值
	GL_INVALID_VALUE stride是错误的
*/
void tlgl_vbo_render(GLuint* m_nVBOVertices,GLuint* m_nVBOTexCoords,GLuint* m_nVBONormals,int len){
/*
	glEnableClientState( GL_VERTEX_ARRAY );			// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );	// Enable Texture Coord Arrays

	glBindBuffer( GL_ARRAY_BUFFER, *m_nVBOVertices );
	glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );

	glBindBuffer( GL_ARRAY_BUFFER, *m_nVBOTexCoords );
	glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );

	glDrawArrays( GL_TRIANGLES, 0, len );			// Draw All Of The Triangles At Once
	// Disable Pointers
	glDisableClientState( GL_VERTEX_ARRAY );		// Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY ); // Disable Texture Coord Arrays//

	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
*/




	//有bug
	glEnableClientState( GL_NORMAL_ARRAY ); 

	glEnableClientState( GL_VERTEX_ARRAY );			// Enable Vertex Arrays

	glEnableClientState( GL_TEXTURE_COORD_ARRAY );	// Enable Texture Coord Arrays

	//glBindBuffer( GL_ARRAY_BUFFER, *m_nVBONormals );
	//glNormalPointer( 3, GL_FLOAT, 0, 0);

	glBindBuffer( GL_ARRAY_BUFFER, *m_nVBOVertices );
	glVertexPointer( 3, GL_FLOAT, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, *m_nVBOTexCoords );
	glTexCoordPointer( 2, GL_FLOAT, 0, 0 );

	glDrawArrays( GL_TRIANGLES, 0, len );			// Draw All Of The Triangles At Once

	// Disable Pointers
	glDisableClientState( GL_NORMAL_ARRAY ); // Disable Texture Coord Arrays
	glDisableClientState( GL_VERTEX_ARRAY );		// Disable Vertex Arrays
	glDisableClientState( GL_TEXTURE_COORD_ARRAY ); // Disable Texture Coord Arrays//
	
	glBindVertexArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}






//void base_boxStaticVert_init(struct HeadInfo* base)
//{
//	if(!base->boxVertLength){
//		//printf("boxVertLength未初始化!\n");
//		//assert(0);
//		//return;
//		base->boxVertLength = BOX_SIZE;
//	}
//
//	if(!base->staticBoxVert){
//		base->staticBoxVert = (float*)tl_malloc(sizeof(float)*base->boxVertLength);
//	}
//	
//	memset(base->staticBoxVert,0,base->boxVertLength);
//}


void 
tlgl_triangles(float* _array,int _arrayCount,GLenum mode){
	glDisable(GL_CULL_FACE);
	glPolygonMode (GL_FRONT_AND_BACK, mode);
	glInterleavedArrays(GL_V3F,0,_array);
	glDrawArrays(GL_TRIANGLES,0,_arrayCount/3);
}



void static
fprintVertex(struct VertexData* vertex){
	int i;
	int n = 0;
	for(i = 0;i < vertex->vertLen;i++){
		printf("i:%f",i,vertex->vertex[i]);
		n++;
		if(n>=3){
			n = 0;
			printf("\n");
		}
	}
}

///*
//*	拾取反馈log
//*/
//void f_pickLog(struct HitResultObject* pHitVec){
//	printf("{拾取的物体名%s 射线检测状态%d 交点坐标:%.3f,%.3f,%.3f 距离射线起点: %.3f}\n",pHitVec->name,pHitVec->isHit,pHitVec->x,pHitVec->y,pHitVec->z,pHitVec->distance);
//}


//static void oldTrans(struct HeadInfo* base)
//{
//	if(base->render!=NULL){
//		base->render(base);
//	}
//}

//static void f_Translate2(struct HeadInfo* base)
//{
//	oldTrans(base);
//}



//==============================================================================
//hit接口实现
static void f_gluMultMatrixVecd(const GLfloat matrix[16], const GLfloat in[4],
								GLfloat out[4])
{
	int i;

	for (i=0; i<4; i++) {
		out[i] = 
			in[0] * matrix[0*4+i] +
			in[1] * matrix[1*4+i] +
			in[2] * matrix[2*4+i] +
			in[3] * matrix[3*4+i];
	}
}
static void f_tlgl__gluMultMatricesd(const GLfloat a[16], const GLfloat b[16],
							   GLfloat r[16])
{
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			r[i*4+j] = 
				a[i*4+0]*b[0*4+j] +
				a[i*4+1]*b[1*4+j] +
				a[i*4+2]*b[2*4+j] +
				a[i*4+3]*b[3*4+j];
		}
	}
}

/*
** Invert 4x4 matrix.
** Contributed by David Moore (See Mesa bug #6748)
*/
static int f_tlgl__gluInvertMatrixd(const GLfloat m[16], GLfloat invOut[16])
{
	GLfloat inv[16], det;
	int i;

	inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
	+ m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
	inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
	- m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
	inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
	+ m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
	inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
	- m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
	inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
	- m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
	inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
	+ m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
	inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
	- m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
	inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
	+ m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
	inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
	+ m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
	inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
	- m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
	inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
	+ m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
	inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
	- m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
	inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
	- m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
	inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
	+ m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
	inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
	- m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
	inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
	+ m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

	det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
	if (det == 0)
		return GL_FALSE;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return GL_TRUE;
}

GLint
tlgl_UnProject(GLfloat winx, GLfloat winy, GLfloat winz,
			 const GLfloat modelMatrix[16], 
			 const GLfloat projMatrix[16],
			 const GLint viewport[4],
			 GLfloat *objx, GLfloat *objy, GLfloat *objz)
{
	GLfloat finalMatrix[16];
	GLfloat in[4];
	GLfloat out[4];

	f_tlgl__gluMultMatricesd(modelMatrix, projMatrix, finalMatrix);
	if (!f_tlgl__gluInvertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);

	in[0]=winx;
	in[1]=winy;
	in[2]=winz;
	in[3]=1.0;

	/* Map x and y from window coordinates */
	in[0] = (in[0] - viewport[0]) / viewport[2];
	in[1] = (in[1] - viewport[1]) / viewport[3];

	/* Map to range -1 to 1 */
	in[0] = in[0] * 2 - 1;
	in[1] = in[1] * 2 - 1;
	in[2] = in[2] * 2 - 1;

	f_gluMultMatrixVecd(finalMatrix, in, out);
	if (out[3] == 0.0) return(GL_FALSE);
	out[0] /= out[3];
	out[1] /= out[3];
	out[2] /= out[3];
	*objx = out[0];
	*objy = out[1];
	*objz = out[2];
	return(GL_TRUE);
}
/*
glVertexAttribPointer(
	0, 
	3, 
	GL_FLOAT, GL_FALSE, 
	3 * sizeof(GLfloat), 
	(GLvoid*)0);
glVertexAttribPointer函数的参数非常多，所以我会逐一介绍它们：

第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
下个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个GLfloat之后，我们把步长设置为3 * sizeof(GLfloat)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子(译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节)。
最后一个参数的类型是GLvoid*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。
*/


//=====================================================================




void tlgl_getGPU_memery(GLint* total_mem_kb,GLint* cur_avail_mem_kb){
	#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
	#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
	 
	//只对英伟达的显卡有用，对于AMD的显卡没有测试
	//GLint total_mem_kb = 0:
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, 
	total_mem_kb);
	 
	//GLint cur_avail_mem_kb = 0:
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, 
	cur_avail_mem_kb);
}


/*
####################################################################

根据顶点创建AABB数据
	
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
####################################################################
*/
int 
tlgl_aabb(float* trianglesArray,int vertexCount,float* outData){
	float vMinMax[6]={0,0,0,0,0,0};
	int length;
	f_minMax(trianglesArray,vertexCount,vMinMax);
	
	length = f_boxArray(vMinMax,outData);
	/*if(scale!=1){
		int i;
		for( i = 0;i< BOX_SIZE;i++){
			outData[i] = outData[i]*scale;
		}
	}*/
	return length;
}
/*
	绘制多边形法线(固定管线)
	float normalSize  法线的长度
*/
void 
tlgl_drawNormal(GLfloat* vertex,int vertLen,float normalSize)
{
	if(!vertex)
	{
		printf("rData->vertex数据为空引用!!!\n");
		assert(0);
	}else{
		int n;
		int floatNum = OBJ_SPAN_COUNT * vertLen;//数据个数

		//glColor3f(0.5, 0.5, 0.5);
		glLineWidth(1.0f);
		
		
		glBindTexture(GL_TEXTURE_2D,0);//解绑定纹理缓存
		//printf("====================\n");
		 //u v		vx    vy    vz       x    y    z
		for(n = 0;n < vertLen;n++)
		{
			float x,y,z,nx,ny,nz;
			struct Vec3 vNormal;

			nx = vertex[n*OBJ_SPAN_COUNT+2];
			ny = vertex[n*OBJ_SPAN_COUNT+3];
			nz = vertex[n*OBJ_SPAN_COUNT+4];

			x = vertex[n*OBJ_SPAN_COUNT+5];
			y = vertex[n*OBJ_SPAN_COUNT+6];
			z = vertex[n*OBJ_SPAN_COUNT+7];

			vNormal.x = nx;
			vNormal.y = ny;
			vNormal.z = nz;

			//printf("法线数据%f,%f,%f\n",nx,ny,nz);


			vec3Normalize(&vNormal);//法线归一
			vec3Mult(&vNormal,normalSize);

			
			glBegin(GL_LINES);
			glVertex3f(x, y, z);
			
			glVertex3f(x + vNormal.x , y + vNormal.y ,z + vNormal.z);
			
			//glVertex3f(x - vNormal.x , y - vNormal.y ,z - vNormal.z);//反转渲染法线
			
			glEnd();
		}
	}
	
}
void 
tlgl_createRay(struct VertexData* rayVertexData,
					 float* inputVertex,int inputVertexCount)
{
	rayVertexData->vertLen = BOX_SIZE;
	rayVertexData->vertex = (float*)tl_malloc(sizeof(float)*rayVertexData->vertLen);
	tlgl_aabb(inputVertex,inputVertexCount,rayVertexData->vertex);
}
void 
tlgl_drawColorLine(	Matrix44f matrix44f,
					struct GMaterial* m,
					GLfloat* vertex,int vertLen,
					float r,float g,float b,void* cam)
{
	//struct GMaterial* m = (struct GMaterial*)base->tmat;
	m->_lineColor.x = r;
	m->_lineColor.y = g;
	m->_lineColor.z = b;
	tmat_render(m,"simp;diffuseStateBox",matrix44f,cam);
	tlgl_triangles(vertex,vertLen,GL_LINE);
}

void
tlgl_get_matrix(Matrix44f m,float x,float y,float z,
					float rx,float ry,float rz){
	//这里需要优化成矩阵计算方式
	Matrix44f n;
	Matrix44f xyz;
	//**********************************************
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	
	glRotatef(-180/PI*rx,1,0,0);
	glRotatef(-180/PI*ry,0,1,0);
	glRotatef(-180/PI*rz,0,0,1);
	
	//glTranslatef(x,y,z);
	
	//mat4x4_zero(n);
	glGetFloatv(GL_MODELVIEW_MATRIX,n);
	//**********************************************
	
	mat4x4_identity(xyz);
	mat4x4_translate(xyz,x,y,z);
	
	mat4x4_transpose(xyz);
	//mat4x4_printf("xyz",xyz);
	
	mat4x4_identity(m);
	//mat4x4_mult(2,m,n,xyz);//先旋转再移动
	mat4x4_mult(2,m,xyz,n);//先移动旋转

	mat4x4_transpose(m);	
}

