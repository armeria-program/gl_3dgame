#ifndef __TLGL_H__
#define __TLGL_H__
#include "tools.h"

#include <gl/glew.h>
#include <gl/glut.h>

#include "tmat.h"



/*shader����*/
//#define SHADER_DIFFUSE 0x01

//#define SHADER_BUMP 0x03
//
//#define SHADER_NORMAL_MAP	0x04	//���߲���
//#define SHADER_MULT_TEXTURE 0x05	//��������
//#define SHADER_SHADOW_MAP	0x06	//��Ӱ
/*

EX->flags(32λ,�洢32��״̬)

EX_FLAGS_RAY:
����ʰȡ����ö��,����һ���ܿ���
���þ�̬��ײ����,���ó�TRUE�Ϳ������߼��,���������ʰȡ,����һ�����忪��,����ģ�Ϳ��Ե�������
*/
#define EX_FLAGS_RAY 0x01

/******************************************************************************************/
/*
	������ʾ�����	FLAGS
	2��ָ����
*/
#define FLAGS_RENDER_BOUND_BOX		0x01	//��Ⱦ��ʾ��ײ����,�Ƿ���Ⱦ������3d����İ�Χ��,��Χ��������������,��ײ�ȵ�
#define FLAGS_DRAW_RAY_COLLISION	0x02	//�������ߺ���	(�Ƿ�������߾�̬��Χ��,���ڲ��� �鿴���߰�Χ��),��ʹû������FLAGS_RAY,��ôҲ�ǿ��Ի������ߺ��ӵ�,�������������鿴
#define FLAGS_LINE_RENDER			0x04	//�Ƿ���ʾ��Ⱦ�߿�
#define FLAGS_OUTLINE				0x08	//�Ƿ���ʾ��Ե����(�̶�����ʵ��)
#define FLAGS_RAY					0x10	//16�Ƿ��������ʰȡ����(��ֻ����3dʰȡ,2dʰȡ����������)
#define FLAGS_VISIBLE				0x20	//32��ʾ��������ģ��,(�Ƿ������Ⱦ������)
#define FLAGS_RENDER_DRAWSKELETON	64		//��Ⱦ�����ڵ�(md5ģ�ͲŻ���ʾ)
#define FLAGS_ANIM_ADAPTIVE			128		//�Ƿ���ݹؼ�֡��֡������ؼ�֡�Ķ���(�Ƿ�����fps 1,����  0,������)
#define FLAGS_GLSL_OUTLINE			256		//��GLSLʵ�ֵ�������
#define FLAGS_DRAW_PLOYGON_LINE		512		//�ڵ�һ���������������߿���Ⱦ(�̶�����ģʽ����GL_LINE),��ȡ����˫����ʾ
#define FLAGS_BUTTON_EFFECT			1024	//���ð�ť��Ч(���е���¼���ǰ���²Ż��и���Ч)
#define FLAGS_DRAW_NORMAL			2048	//���Ʒ���
#define FLAGS_DRAG					4096	//Sprite�Ƿ����ק
#define FLAGS_DRAW_PLOYGON_POINT	8192	//��ģʽ
#define FLAGS_DISABLE_CULL_FACE		16384	//������ʾ˫��




#define SHADER_FLAG_FLAT_COLOR			0x01	//����ƽ����ɫ��


/******************************************************************************************/
#define OBJ_SPAN_COUNT				8		//objģ���ļ� ���ݽṹ�����

struct TLGLinfo{
	/*��ɫ��*/
	GLdouble zfar;
	int* pScreenWidth;
	int* pScreenHeight;
};

/*
*	��������ṹ��
*	�����л��ؼ�֡,���Ŷ���
*/
typedef struct FrameAnim
{
	/*
	 *	��ǰ��������ʼ�ؼ�֡�ͽ����ؼ�֡������(��0��ʼ)
	 */
	int frameStart;
	/*
	 *	����֡����,��������0��ʼ
	 */
	int frameEnd;
	
	/*
	 * ÿһ֡�ļ��ʱ��
	 * fps = 60;
	 * fpsInterval = 1000 / fps = 16.6666;
	 */
	long fpsInterval;

	/*
	 * ��ǰ�Ĳ�ֵ
	 */
	long _subTick;

	/*
	*	��ǰ�ؼ�֡������(��0��ʼ)
	*/
	int curFrame;

	
	/*
	*��ǰ����
	*/
	char curAnim[G_BUFFER_16_SIZE];
	/*
	*ǰһ�ζ���
	*/
	char oldAnim[G_BUFFER_16_SIZE];
	
	/*
	*	����Ƭ������
	*	"stand,0,39|run,40,45|"
	*/
	char animConfig[G_BUFFER_128_SIZE];

	/*
	*	����һ�ζ����ص�
	*/
	//CallBackFun playOnceCallBack;
	void (*playOnce)(struct FrameAnim*);
	

}FrameAnim;

/*��Ļ�ߴ�*/
//void tlgl_screenSize(float* w,float* h);

void tlgl_getVersion();/*�������OpenGL�汾����ҪGLSL 2.0֧��*/

/*�ػ�,���ڿ�߷����仯*/
//void tlgl_reshape(int w,int h);

/*��ʾ�汾*/
void tlgl_showGLVersion();
/*Ϊ��ɫ�����������*/
//void tlgl_bindTex(GLuint pro,GLsizei imageWidth,GLsizei imageHeight,GLubyte* pImage,GLuint* pTextureID);



/*
*	������ɫ������
*/
int  tlgl_createShader(GLchar* vertex,GLchar* fragment,GLuint* outProgram);
/*
*	����AABB��Χ��
*	float r,float g,float b �߿����ɫ
*/
void tlgl_AABB(float* trianglesArray,int vertexCount,float* triData,CallBackFun callBack,int parms,float r,float g,float b,int isDraw);


/*
 *	�������������
 *	float* _array			��	��������
 *	int _arrayCount			:	��������Ԫ�ظ���
 *	float r,float g,float b :	RGB��ɫ(0.0f~1.0f)
 */
void tlgl_drawTrianglesLine(float* _array,int _arrayCount,float r,float g,float b);

//void tlgl_renderBox(float* triData,CallBackFun callBack,/*char* parms*/int parms,float scale);

/*
 * ����Ļ����x,y����������
 */
void tlgl_drawText(const char* s,float x,float y,GLfloat r,GLfloat g,GLfloat b);
void tlgl_drawSign(float* pos,float size);/*����һ�������*/
void tlgl_drawLine(float* a,float* b);/*����a��b��������*/
//int  tlgl_getCurTime();/*��ȡ��ǰ���е�ʱ��*/
/*����һ����ɫ������*/
void tlgl_glslProgrom(GLuint* outProgra,const char* glslType);
void tlgl_bindVar(GLuint program3D,float* lightDir);
void tlgl_vao(GLuint* vao,int* pLen,GLfloat* verts);

/*
	����vbo
	GLuint* m_nVBOVertices,GLuint* m_nVBOTexCoords,GLuint* m_nVBONormals,
*/
//void tlgl_createVBO(GLfloat* verts,int len);

/*ɾ��vbo*/
void tlgl_deleteVBO(GLuint* m_nVBOVertices,GLuint* m_nVBOTexCoords);

void tlgl_vbo_render(GLuint* m_nVBOVertices,GLuint* m_nVBOTexCoords,GLuint* m_nVBONormals,int len);/*��Ⱦvbo*/
//float tlgl_vbo_use();/*vboʹ�õ��ڴ��С*/
/*
	
*/
GLuint tlgl_bindTex(GLuint program3D,GLsizei w,GLsizei h,GLubyte* pImage,const char* tex,int texIndex);
/*
*	�߿�ģʽ��Ⱦ
*/
void tlgl_drawPloyLine();


///*
//	ͨ����Ⱦ�ӿ�,��vbo���߷�vboģʽ��Ⱦ
//	vbo==NULL��ʱ��ʹ�÷�VBOģʽ
//*/
//void tlgl_renderMode(struct VBO_Obj* vbo,GLfloat* vertex,int vertexCount);
//

/*
	��Ⱦ������
*/
void tlgl_render_triangles(GLfloat* vertex,int vertexCount);

/*
	��Ⱦһ�������ʵ�ģ��
*/
void base_renderByMaterial(struct HeadInfo* base);

//=========================================================================================================
struct MaterialList
{
	struct LStackNode* texList;		/*��ͼָ��*/

	struct LStackNode* vboList;		/*vbo�б�*/
};

/*
	��ͼ�ṹ��
*/
struct Tex
{
	int w,h;
	char texName[G_BUFFER_64_SIZE];			/*��ͼ��*/
	//GLubyte* image;							/*��ͼ����*/
	int use;							/*ʹ�õĸ���*/
	int mTextureID;
};
//
///*
//	���ʽṹ��
//*/
//struct Material{
//	GLuint programObject;					/*��ɫ������*/
//	GLuint mTextureID;						/*Textureָ��*/
//};

struct VertexData
{
	//GLfloat���ݵĳ���
	int vertLen;
	
	//������������,����һ������
	GLfloat* vertex;

	//VBO����
	//struct VBO_Obj* vbo;
};

/*
	Base ����ģ�Ͷ���Ļ�������,λ��,��ת,����
	�����ģ�������Ƿ���md2,md5,obj�޹�,���������ǵĸ���
*/

typedef struct Md2Info
{
	//��ʼ֡
	int s;
	
	//�����ؼ�֡
	int e;

	//����Ƭ����
	char* animName;

	//��¼�Ƿ��ҵ�
	int findState;
}Md2Info;

///*
//	��������
//*/
//struct VertexData
//{
//	float* vert;	//��������
//	int count;		//float ���ݸ���
//};

/*
	�����ṹ��,����ent3d��md2�̳�
*/
typedef struct HeadInfo{

	/*����*/
	int curType;
	
	/*����,�����ڲ�ѯ,��ΪΨһʵ��*/
	char name[G_BUFFER_32_SIZE];
	/*�ļ���׺*/
	char suffix[G_BUFFER_16_SIZE];

	/*����*/
	float x,y,z;

	float rx,ry,rz;

	/*����ֵ*/
	float scale;
	
	/*
		��������	===>struct GMaterial* tmat;
	*/
	struct GMaterial* tmat;

	//char curShader[G_BUFFER_32_SIZE];//��ǰʹ�õ�shader

	///*
	//	vbo����
	//*/
	//void* vboPtr;

	/*
	*	��̬��Χ�ж�����������
	*/
	float* boxVertPtr;

	///*
	//*	��̬��Χ�ж�������,����������ʰȡ��
	//*/
	//float* staticBoxVert;

	/*
		��̬��������
	*/
	struct VertexData rayVertexData;

	///*
	//*	��̬��Χ�еĳ����
	//*/
	//float boundWidth,boundHeight,boundLength;

	/*
		��ʶλ,��ʶ����״̬
	*/
	int flags;
	
	/*
	*	����������
	*/
	struct FrameAnim* frameAnim;

	///*
	//*	��Ⱦ�ص�
	//*/
	//void (*render)(struct HeadInfo*);

	/*
	*	����Ⱦ����(��������)
	*/
	struct VertexData rData;

	/*
		��Ⱦ�ڵ�ص��ӿ�,�˽ӿ���������չ��
	*/
	void (*renderCallBack)(void* ptr);

	/*
		����ı任����
	*/
	Matrix44f m;
	
	/************************************************************************/
	/* ��Ԫ������															*/
	/************************************************************************/
	Matrix44f quat_m;

	struct Vec3 target;
	int lookat;
	
	//����ʱ���洢�ı���
	long time;
	
	/*��������Ĺؼ�֡֡��*/
	int fpsInterval;
	

	/*
	�Զ������õ�ǰ�Ĺؼ�֡����,0��ʼ										
	-1��ʾû��ʹ�õ�״̬,0����ʹ��ָ���Ĺؼ�֡,��ʼ����ʱ�����ó�-1
	*/
	int custframe;
	
	/*
		luaʰȡ�ص�
	*/
	char* luaPickCallBack;
	/*
		ʰȡ����ɫ
	*/
	float boxR,boxG,boxB;

}HeadInfo;

//�ӿڶ��岿��

/*
*	curType:			ģ������
*	name:				ģ����,��������
*	x,y,z:				��ʼ������
*	const char* tex:	��ͼ·��
*/
struct HeadInfo* base_create(int curType,const char* name,float x,float y,float z);

/*
	��������
*/
void base_setPos(struct HeadInfo* base,float x,float y,float z);
void base_set_position(struct HeadInfo* base,struct Vec3* pos);
/*
	���¾���
*/
void updateMat4x4(struct HeadInfo* base);
/*
*	���ư�Χ��
*	void (*translateCallBack)(struct HeadInfo*)	:������λص�
*/
//void base_drawBoundBox(struct HeadInfo* base,float* vertices,int vertCount,void (*translateCallBack)(struct HeadInfo*));

/*
 *	��̬��ײ���� ֻ�ǻ�ȡ��ײ�������ݲ����л���
 *	��ʼ����̬��ײ��
 */
void base_md2_staticBox(struct HeadInfo* base);

/*
	��������ʰȡ����
	float* inputVertex	�����ζ�������
	inoutVertexCount	float����
*/
void base_createRayVertex(struct VertexData* rayVertexData,float* inputVertex,int inputVertexCount);

/*
*	���ö���
*/
void base_curAnim(struct FrameAnim* frameAnim,const char* anim);

/*
*	�ýӿ�ֻ����һ�ζ���,������һ��ѭ����,�ָ���һ������
*/
void base_playOnce(struct FrameAnim* frameAnim,const char* anim);

/*
*	����ؼ�֡
*/
void base_calculateFrame(struct FrameAnim* frameAnim);

/*
	��ȡ��Ⱦģʽ�Ƿ����߿���ʵ��ģʽ
*/
int base_get_ploygonLineMode(struct HeadInfo* base);

/*
*	����Ⱦ�б�����ʰȡ
*	struct Vec3* offset	����һ��ƫ������,������Ϊ��ͼ�����ƫ��ֵ
*/
void base_seachPick(struct LStackNode* list,struct Vec3* nearPoint,struct Vec3* farPoint,struct HitResultObject* last);

/*
*	����
*/
void base_dispose(struct HeadInfo* base);

struct HeadInfo* base_get2(void* p);

/*
	��� HeadInfo �ṹ��,����head��Ϣ��ȡ�����Ϣ,��Ҫβ������һ��HeadInfo*����
*/
//void base_get(void* point,struct HeadInfo* base);

//ʵ����Ⱦ
void base_renderFill(struct HeadInfo* node);

/*
	ʹ��ms�����ƶ���x,y,z����
	�ڵ�λʱ�����ƶ���һ��Ŀ������,����һ������Ĺ���
*/
void base_trace(struct HeadInfo* base,float x,float y,float z,float ms);

///*
//	��ʼ����̬��Χ��
//*/
//void base_boxStaticVert_init(struct HeadInfo* base);

/*
	�Ƿ���Բ��Ŷ���
*/
int base_isCanAnimation(struct HeadInfo* ptr);
/*
	����һ�����ʰȡ��lua�����ص�
*/
void base_setLuaPick(struct HeadInfo* base,const char* luaFunctionName);

/************************************************************************/
/* ���ú�׺                                                    */
/************************************************************************/
void base_set_suffix(struct HeadInfo* base,const char* str);
/*
	˫�����
*/
GLboolean base_cullface(struct HeadInfo* base);
//==========================================================================



/*
	��ʼ���
	mRayPickCallBack �ص�
*/
void hit_mouse(GLint xMouse, GLint yMouse,float screenWidth,float screenHeight,
			   struct LStackNode *renderList,
			   void (*mRayPickCallBack)(struct HitResultObject*));


/*
	����shader���ͻ�ȡһ��
*/
GLuint tlgl_getShader(const char* glslType);


//struct VBO_Obj* mat_loadModel(const char* path,int arrayLength);
/*
	���ƾ�̬��Χ��,�ð�Χ�����Զ��峤��ߵ�
*/
void tlgl_staticBox(struct HeadInfo* base);


/*
	��ȡ�Դ�ʹ�ú����Դ��С(kb)
*/
void tlgl_getGPU_memery(GLint* total_mem_kb,GLint* cur_avail_mem_kb);

/*
 *	�����߿�
 */
void drawLineByColor(struct HeadInfo* base,GLfloat* vertex,int vertLen,float r,float g,float b);

/*
	����vbo�ߴ�
*/
void tlgl_setVboSize(int size,int isAdd);

/*
	��ȡvbo�ߴ�
*/
int tlgl_getVboSize();
#endif