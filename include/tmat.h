#ifndef _TMAT_H_
#define _TMAT_H_
#include "tools.h"
#include "tlgl.h"
#include "vmath.h"

/*
	Ԥ������ͼ����,��������8����ͼ
*/
#define MATERIAL_TEXTURE_COUNT 8

/*
	���ʽṹ��
	����ɫ�����,��ϸ��ֲ���,����GLSL
*/
typedef struct GMaterial{

	/*
	*	Ĭ�ϵ���ɫ������
	*/
	char glslType[G_BUFFER_32_SIZE];

	/*
	*	��ǰ����ɫ������
	*/
	char curGlslType[G_BUFFER_32_SIZE];

	/*
	*	��������,Ԥ������̬�ռ�,����洢��GPU��������������,ʹ����ɺ�Ҫ�ͷ�
	*/
	unsigned int texs[MATERIAL_TEXTURE_COUNT];

	//��ǰ��ͼ·������,��0��ʼ
	int curTexIndex;
	//��ͼ����
	int texArray[MATERIAL_TEXTURE_COUNT];
	
	struct Vec4 _lineColor;
	float lineWidth;
	float uvScale;//uv����ֵ
	struct Vec4 _outlineColor;

	//int diffuseOutLine;
	struct Vec4 _diffuseOutLineColor;
	
	//����alpha
	float _Alpha;

	int flags;//���ʱ�ʾ��
};

/*
	����һ������,���ط�������
	texCount:��ͼ����
*/
void*  
tmat_create(const char* glslName,int texCount,...);

/*
	����һ������ͼ�Ĳ���
*/
void* 
tmat_createTex(const char* glslType,GLint width,GLint height);
/*
	���ٲ���
*/
void tmat_dispose(void* mat);


//float x,float y,float z,float scale
/*
	�л�Ϊ��ǰ�Ĳ���
*/
void tmat_render(void* mat,const char* shader,Matrix44f M);  

/*
	��ȡ��ǰ������ɫ����(����һ������Ĭ�ϵ�һ��shader)
*/
void tmat_getShader(void* mat,char* _out,int _outBufferSize);

/*
	 ����һ��Sprite(����������ͼ�Ķ����),��VBOģʽ
*/
void tmat_renderSprite(struct GMaterial *_material,const char* shader,Matrix44f mat4x4,float* vertexs,int vertLen,int format,int mode);
/*
 *���ô�ɫ��Ⱦ��ʹ�õ���ɫ,��ʹ�õ�ʱ��Ĭ����ɫΪ0,0,0
 */
void tmat_setFlatColor(struct GMaterial* ptr,float r,float g,float b);

/*
 *	ɾ��ָ�������ϵ���ͼ
 */
void tmat_disposeTextureByIndex(struct GMaterial* ptr,int index);

/*
	����������ȡ��������
*/
GLuint tmat_getTextureByIndex(struct GMaterial* pmat,int index);
#endif