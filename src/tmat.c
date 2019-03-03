#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include "tools.h"
#include "tl_malloc.h"
#include "tmat.h"
#include "tlgl.h"
#include "jgl.h"
#include "ex.h"
#include "gettime.h"
//#define _DEBUG_



//=======================================================================================
void 
tmat_setFlatColor(struct GMaterial* ptr,float r,float g,float b)
{
	//ptr->diffuseOutLine = 1;
	ptr->_diffuseOutLineColor.x = r;
	ptr->_diffuseOutLineColor.y = g;
	ptr->_diffuseOutLineColor.z = b;
	ptr->_diffuseOutLineColor.w = 1.0f;
}
void tmat_getShader(void* pvoid,char* _out,int _outBufferSize)
{
	struct GMaterial* _mat = pvoid;
	memset(_out,0,_outBufferSize);
	memcpy(_out,_mat->glslType,strlen(_mat->glslType));
}

//���ŵ�ʱ���
static float playTime = 0;

//����Ԥ����ͼ����
enum{_TEXTURE_NUM_=2};
/*
	��ɫ��������ͼ����
	���������������
*/
static void 
f_updateTexture(GLuint program3D,struct GMaterial* mat)
{
	
	const char *_textureKeyArray[_TEXTURE_NUM_] = {"texture1","texture2"};
	const int _textureIndexArray[_TEXTURE_NUM_] = {GL_TEXTURE0,GL_TEXTURE1};
	int _texIndex;
	for(_texIndex = 0;_texIndex < _TEXTURE_NUM_;_texIndex++)
	{
		int _texture1 = glGetUniformLocation(program3D,_textureKeyArray[_texIndex]);

		if(_texture1!=-1){
			glUniform1i(_texture1,_texIndex);
			
			glActiveTexture(_textureIndexArray[_texIndex]);

			glBindTexture(GL_TEXTURE_2D,mat->texs[_texIndex]);
		}
	}
}

//static float testValue = 1.0;
/*
	�任
	������,��ת
	����==> ƽ�Ƶ�(0,0,0) ==>���� ==>�ָ���ԭ������
	��ת==> ƽ�Ƶ�(0,0,0) ==>��ת ==>�ָ���ԭ������

*/
static void f_uploadMat4x4(GLint location_mat4x4,Matrix44f _out_mat4x4)
{
	glUniformMatrix4fv(location_mat4x4,1,GL_TRUE,_out_mat4x4);
}

/*****************************************************************
������ɫ��,��������
������ɫ��ÿ�μ���һ��,GPU���м�Ⱥ����
******************************************************************/

static void 
f_updateShaderVar(GLuint program3D,struct GMaterial* _material, Matrix44f M)
{

	//��������ɫ
	int _outlineColor = glGetUniformLocation(program3D,"_outlineColor");

	//Alpha	-->vboDiffuse.ps
	int _Alpha = glGetUniformLocation(program3D,"_Alpha");

	//��ȡ��1����������
	GLint mat1 = glGetUniformLocation(program3D,"_mat1");

	//͸�Ӿ���
	GLint _perspectivePtr = glGetUniformLocation(program3D,"_PerspectiveMatrix4x4");

	//��ͼ����
	GLint _modelViewPtr = glGetUniformLocation(program3D,"_ModelViewMatrix4x4");

	GLint ui_perspectivePtr = glGetUniformLocation(program3D,"ui_PerspectiveMatrix4x4");
	GLint ui_modelViewPtr = glGetUniformLocation(program3D,"ui_ModelViewMatrix4x4");

	//��ȡ��2����������
	int mat2 = glGetUniformLocation(program3D,"_mat2");

	//�߹⿪��
	int bSpecular = glGetUniformLocation(program3D,"bSpecular");

	//ʱ����
	int iGlobalTime = glGetUniformLocation(program3D,"_iGlobalTime");

	//��ɫ
	int _lineColor = glGetUniformLocation(program3D,"_lineColor");
	
	//ʹ��flat��Ⱦ
	int _useFlat = glGetUniformLocation(program3D,"_useFlat");
	
	//�߿��(line.vs)
	int _LineWidth = glGetUniformLocation(program3D,"_LineWidth");

	//uv����ֵ
	int _uvScale =glGetUniformLocation(program3D,"_uvScale");

	//����λͼ
	f_updateTexture(program3D,_material);

	if(iGlobalTime!=-1)
	{
		playTime+= 1.0f / (g_fps*10);
		glUniform1f(iGlobalTime,playTime);
	}

	//if(pos!=-1){
	//	glUniform3f(pos,0,0,0);//Ĭ�ϴ�������0,0,0��ʱ��,��ʹ���ⲿ����ļ�ֵ
	//}else{
	//	//printf("��ɫ����δ�ҵ��������pos\n");
	//}

	if(_outlineColor!=-1)
	{
		struct Vec4 c = _material->_outlineColor;
		glUniform4f(_outlineColor,c.x,c.y,c.z,0.0);
	}
	
	if(_useFlat!=-1)
	{
		int _stat = getv(&_material->flags,SHADER_FLAG_FLAT_COLOR);
		glUniform1i(_useFlat,_stat);
		if(_stat)
		{
			int _diffuseFlatColor = glGetUniformLocation(program3D,"_diffuseFlatColor");
			struct Vec4 c = _material->_diffuseOutLineColor;
			glUniform4f(_diffuseFlatColor,c.x,c.y,c.z,0.0);
		}
	}

	if(_lineColor!=-1)
	{
		struct Vec4 c = _material->_lineColor;
		glUniform4f(_lineColor,c.x,c.y,c.z,0.0);
	}
	if(_LineWidth!=-1)
	{
		if(_material->lineWidth <= 0)
		{
			log_code(ERROR_BAD_VALUE);
			assert(0);
		}
		glUniform1f(_LineWidth,_material->lineWidth);
	}
	else
	{
/*
#ifdef _DEBUG_
		printf("didn`t exist _LineWidth");
#endif
*/
	}

	if(_uvScale!=-1){
		if(_material->uvScale != 0){
			glUniform1f(_uvScale,_material->uvScale);
		}else{
			glUniform1f(_uvScale,1.0f);
		}
	}

	if(mat1!=-1){//ģ�ͱ任����

		f_uploadMat4x4(mat1,M);

	}else{
		//printf("��ɫ����δ�ҵ�����������mat1\n");
	}

	//͸�ӱ任����
	if(_perspectivePtr!=-1)
	{
		f_uploadMat4x4(_perspectivePtr,ex_getInstance()->perspectiveMatrix);
	}

	if(_modelViewPtr!=-1)
	{
		//mat4x4_transpose(ex_getInstance()->modelViewMatrix);//ת�þ���
		f_uploadMat4x4(_modelViewPtr,ex_getInstance()->modelViewMatrix);
	}

	if(ui_perspectivePtr!=-1)
	{
		f_uploadMat4x4(ui_perspectivePtr,ex_getInstance()->ui_perspectiveMatrix);
	}
	if(ui_modelViewPtr!=-1)
	{
		f_uploadMat4x4(ui_modelViewPtr,ex_getInstance()->ui_modelViewMatrix);
	}

	if(mat2!=-1){
		Matrix44f _scaleMat;
		float _s = 1.0;
		mat4x4_zero(_scaleMat);
		mat4x4_identity(_scaleMat);
		mat4x4_scale(_scaleMat,_s,_s,_s);//����
		//mat4x4_rotateX(_scaleMat,90);//��ת
		glUniformMatrix4fv(mat2,1,GL_TRUE,_scaleMat);
	}

	//�Ƿ����ø߹�1.0����,0.0�ر�
	if(bSpecular!=-1){
		GLfloat _openSpecular = 1.0f;
		glUniform1f(bSpecular,_openSpecular);
	}
	
	if(_Alpha!=-1)
	{
		glUniform1f(_Alpha,_material->_Alpha);
	}
}
//struct GMaterial *tmat_create()
//{
//	struct GMaterial* mat = (struct GMaterial*)tl_malloc(sizeof(struct GMaterial));
//	memset(mat,0,sizeof(struct GMaterial));
//
//	return mat;
//}
/*
	��������ѹ��һ����ͼ·��,curTexIndex����+1(curTexIndex��0��ʼ)
*/
static void 
f_tmat_pushTex(struct GMaterial* p,const char* path){
	if(path!=0){
		int len = strlen(path);
		char* _s;
		p->texArray[p->curTexIndex]=(int)tl_malloc(len+1);
		_s = (char*)p->texArray[p->curTexIndex];
		memset(_s,0,len+1);
		memcpy(_s,path,len);
		
		//printf("%s\n",p->texarray[p->curTexIndex]);

		p->curTexIndex++;
	}else
	{
		//assert(0);
	}
}

/*
*	�洢��GPU�ڴ���һ����ͼ
*	int texIndex		:		��������
*	GLuint tex			:		����������
*/
static void 
f_tmat_createTexFromGPU(struct GMaterial* mat,int texIndex,GLuint tex){
	//������������
	if(texIndex>=mat->curTexIndex)
	{
		printf("���������������!\n");
		assert(0);
		return;
	}

	if(mat->texs[texIndex])
	{
		//��Ҫɾ�����������ݺ��ٴ���
		printf("�����쳣,���������Ѿ���������\n");
		assert(0);
		return;
	}

	mat->texs[texIndex] = tex;
	//mat->texs[texIndex]=jgl_loadTex("//resource//texture//2x2.tga");//����ͼ��
}

GLuint 
tmat_getTextureByIndex(struct GMaterial* pmat,int index)
{
	return (GLuint)pmat->texs[index];
}

/*
	struct GMaterial* mat:	��������
	float x,float y,float z	����
*/
void tmat_render(void* pvoid,const char* shader,Matrix44f M)
	 //float x,float y,float z,float scale,
{
	GLuint program3D;
	struct GMaterial* material = pvoid;

	if(shader==NULL)
	{
		printf("���ݵ�Shader�ַ���Ϊnull\n");
		assert(0);
	}

	if(material==NULL)
	{
		printf("ģ�ͣ�%s��û�����ò���\n");
		assert(0);
		return;
	}

	if(!material->glslType){
		printf("����ɫ������ָ��,��Ҫ������ɫ��\n");
		assert(0);
		return;
	}
	
	program3D = tlgl_getShader(shader);//mat->glslType
	
	//�л�����ǰ����ɫ������
	glUseProgram(program3D);
	
	//����ɫ���ϴ��������
	f_updateShaderVar(program3D,material,M);
}

/*
	��������·��,��ʹ�����֮����ͷ�
*/
static void 
f_clearTexArray(struct GMaterial *mat)
{
	int i = 0;
	
	for(i = 0;i< MATERIAL_TEXTURE_COUNT;i++){
		int _p = mat->texArray[i];
		if(_p){
			tl_free((void*)_p);
			mat->texArray[i] = 0;
		}
	}
}
/*
	ɾ��GPU��ͼ
*/
static void f_deleteGPU_texture(struct GMaterial* mat)
{
	int i;
	for(i = 0;i< MATERIAL_TEXTURE_COUNT;i++)
	{
		if(mat->texs[i]!=0)
		{
			//���Ҫɾ��������������	������ʹ�����淽ʽ;
			glDeleteTextures(1,&(mat->texs[i]));
			mat->texs[i] = 0;
		}
	}
}
void tmat_disposeTextureByIndex(struct GMaterial* mat,int index)
{
	int i;
	for(i = 0;i< MATERIAL_TEXTURE_COUNT;i++)
	{
		if(mat->texs[i]!=0 && i == index)
		{
			//���Ҫɾ��������������	������ʹ�����淽ʽ;
			glDeleteTextures(1,&(mat->texs[i]));
			mat->texs[i] = 0;
		}
	}
}
//����һ������
void tmat_dispose(void* pvoid)
{
	struct GMaterial* mat = pvoid;

	f_deleteGPU_texture(mat);	
	
	tl_free((void*)mat);
}
/*
	Ϊ���ʴ�����ͼ
*/
static void f_createMaterialTexture(struct GMaterial *p)
{
	int i;
	for(i = 0;i< p->curTexIndex;i++)
	{
		GLuint tex = jgl_loadTex((const char*)p->texArray[i]);
		f_tmat_createTexFromGPU(p,i,tex);
	}
}
/*
	����ɫ��
*/
static void 
f_assignShader(struct GMaterial* tmat,const char* glslType)
{
	memset(tmat->glslType,0,G_BUFFER_32_SIZE);
	memcpy(tmat->glslType,glslType,strlen(glslType));
}
static void
f_initMaterial(struct GMaterial* tmat){
	//������alpha��ֵ = 1.0
	tmat->_Alpha = 1.0f;

	//�����߿���ɫ
	tmat->_outlineColor.x = 0;
	tmat->_outlineColor.y = 1;
	tmat->_outlineColor.z = 1;

	//����flat��ɫ
	tmat_setFlatColor(tmat,0.43,0.51,0.96);
}

void* 
tmat_create(const char* glslType,int texCnt,...){
	
	struct GMaterial* tmat = (struct GMaterial*)tl_malloc(sizeof(struct GMaterial));
	memset(tmat,0,sizeof(struct GMaterial));
	
	if(texCnt > MATERIAL_TEXTURE_COUNT){
		log_code(ERROR_BAD_VALUE);//����Ԥ������ͼ����
		assert(0);
	}
	else{
		int i;
		va_list ap;
		va_start(ap, texCnt);
		for(i = 0; i < texCnt; i++){
			int p = va_arg(ap, int);			
			if(p && strlen((char*)p) > 0) {
				f_tmat_pushTex(tmat,(char*)p);
			}
		}
		va_end(ap);
	}

	f_assignShader(tmat,glslType);

	//��ʼ������ͼ
	f_createMaterialTexture(tmat);

	//��������·���б���Ҫ�Ķ��ڴ�����
	f_clearTexArray(tmat);

	f_initMaterial(tmat);

	return tmat;
}

//����һ������ͼ�Ĳ���
void* 
tmat_createTex(const char* glslType,GLint width,GLint height){
	struct GMaterial* tmat = (struct GMaterial*)tl_malloc(sizeof(struct GMaterial));
	memset(tmat,0,sizeof(struct GMaterial));
	
	{
		//����һ��ָ���Ŀ��λͼ����
		GLuint tex = jgl_createRGB_Texture(width,height,GL_RGB);
		
		//ָ��0������λ�õ�λͼ
		tmat->curTexIndex++;
		f_tmat_createTexFromGPU(tmat,0,tex);
	}
	f_assignShader(tmat,glslType);
	f_initMaterial(tmat);
	
	return tmat;
}

void* 
tmat_create_rgba(const char* glslType,GLint width,GLint height,GLenum rgbaType){
	struct GMaterial* tmat = (struct GMaterial*)tl_malloc(sizeof(struct GMaterial));
	GLuint tex;
	memset(tmat,0,sizeof(struct GMaterial));
	
	tex=jgl_create_opengl_RGBA_Tex(width,height,rgbaType);
	tmat->curTexIndex++;
	f_tmat_createTexFromGPU(tmat,0,tex);
	f_assignShader(tmat,glslType);
	f_initMaterial(tmat);
	return tmat;
}


void tmat_renderSprite(struct GMaterial *_material,const char* shader,Matrix44f mat4x4,GLfloat* vertexs,int vertLen,int format,int mode)
{
	tmat_render(_material,shader,mat4x4);	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);//�޳�ǰ��
	glPolygonMode (GL_BACK,mode);
	glInterleavedArrays(format,0,vertexs);
	glDrawArrays(GL_TRIANGLES,0,(GLsizei)vertLen);
	glDisable(GL_CULL_FACE);
}