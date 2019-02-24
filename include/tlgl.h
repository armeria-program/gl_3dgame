#ifndef __TLGL_H__
#define __TLGL_H__
#include "tools.h"

#include <gl/glew.h>
#include <gl/glut.h>

#include "tmat.h"
#include "vmath.h"

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
#define FLAGS_REVERSE_FACE			32768	//��ת��Ⱦ��

#define SHADER_FLAG_FLAT_COLOR			0x01	//����ƽ����ɫ��


/************************************************************************/
/* headInfo�¼�����                                                     */
/************************************************************************/
#define EVENT_RAY_PICK 1				//ʰȡ����ص�

/******************************************************************************************/
#define OBJ_SPAN_COUNT				8		//objģ���ļ� ���ݽṹ�����
#define BOX_SIZE 108		//6����(12��������)��Χ�гߴ�(float����)

//�������ݼ����
enum
{
	UV_GAP		=	2,		//uv�������
	NORMAL_GAP	=	3,		//normal�������
	VERTEX_GAP	=	3		//�����������
};

//=================================================================================================
//����ֲ���ʽ
//#define OBJ_UV_VERTEX			0
//#define OBJ_UV_VERTEX_NORMAL	1
enum {
	//OBJ_NULL = -1,
	/*
	 * uv vertex
	 */
	OBJ_UV_VERTEX=0,
	/*
	 * uv vertex normal
	 */
	OBJ_UV_VERTEX_NORMAL=1,
};
struct VertexData
{
	//GLfloat���ݵĳ���
	int vertLen;

	//������������,����һ������
	GLfloat* vertex;

	//VBO����
	//struct VBO_Obj* vbo;
};

struct TLGLinfo{
	/*��ɫ��*/
	GLdouble zfar;
	int* pScreenWidth;
	int* pScreenHeight;
};

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
void tlgl_AABB(float* trianglesArray,int vertexCount,float* triData,int (*CallBackFun)(char*),int parms,float r,float g,float b,int isDraw);


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
	����shader���ͻ�ȡһ��
*/
GLuint tlgl_getShader(const char* glslType);





/*
	��ȡ�Դ�ʹ�ú����Դ��С(kb)
*/
void tlgl_getGPU_memery(GLint* total_mem_kb,GLint* cur_avail_mem_kb);


/*
	����vbo�ߴ�
*/
void tlgl_setVboSize(int size,int isAdd);

/*
	��ȡvbo�ߴ�
*/
int tlgl_getVboSize();

/*
 *	���ù̶�����ߵİ�Χ��
 *	x,y,z Ϊ�����
 */
void tlgl_setAbsBoundBox(float x,float y,float z,float* outTriData);
/*
*	vertexs:	�����׵�ַ
*	count:		����ĸ���
*	ȡ����ֵ��������ײ��(8��float���)
*/
void tlgl_absMax(float* vertexs,int count,float* out);

int 
tlgl_aabb(float* trianglesArray,int vertexCount,float* outData);

void 
tlgl_triangles(float* _array,int _arrayCount,GLenum mode);

GLint
tlgl_UnProject(GLfloat winx, GLfloat winy, GLfloat winz,
			   const GLfloat modelMatrix[16], 
			   const GLfloat projMatrix[16],
			   const GLint viewport[4],
			   GLfloat *objx, GLfloat *objy, GLfloat *objz);

/*
	���ƶ���η���(�̶�����)
	float normalSize  ���ߵĳ���
*/
void 
tlgl_drawNormal(struct VertexData* rData,float normalSize);

/*
	��������ʰȡ����
	float* inputVertex	�����ζ�������
	inoutVertexCount	float����
*/
void tlgl_createRay(struct VertexData* rayVertexData,float* inputVertex,int inputVertexCount);

/*
 *	����һ�������߿��mesh
 */
void tlgl_drawColorLine(
				   Matrix44f matrix44f,struct GMaterial* m,
				   GLfloat* vertex,int vertLen,
				   float r,float g,float b
				   );
#endif