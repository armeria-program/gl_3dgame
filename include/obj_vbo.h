#ifndef _OBJ_VBO_
#define _OBJ_VBO_
#include <gl/glew.h>
#include <gl/glut.h>

#include "tools.h"
#include "anim.h"

/*
 *VBO�ṹ��
 */
struct ObjVBO
{
	GLuint vertexID;
	GLuint uvID;
	GLuint normalID;
	GLuint indexID;
	int renderVertCount;//��Ҫ��Ⱦ�Ķ������
#ifdef CALCULATE_VBO_SIZE
	//VBO��ռ�õĳߴ�
	int byteSize;
#endif
};
/*
	ʹ��VBOģʽ
*/
struct Obj_vbo_model
{
	//vbo�����б�.��Ϊһ���ڵ��ʱ���Ǿ�̬ģ��,����Ƕ���ڵ��ʱ�����MeshAnimation
	struct LStackNode* ptrList;

	//VBO������֯�ķ�ʽ
	int dataType;
};
/*
	����
*/
void objVBO_dispose(struct Obj_vbo_model* p);

/*
	VAOģʽ����ģ������,���һ��VBO�ڵ�

	GLfloat* verts	uv,normal vertex���ݻ�����ָ��
	ע��:verts���ݵ���֯��ʽ��_pvboModel->dataTypeҪ����һ��
	verts�ṹ:vertex_x,vertex_y,vertex_z,u,v,normalX,normalY,normalZ

	int	_bufferSize float�ĸ���
*/
void objVBO_pushNode(struct Obj_vbo_model* _pvboModel,GLfloat* verts,int _bufferSize);

//��ȡ��һ��struct ObjVBO*����
void* 
objVBO_createNode(int vertsType,GLfloat* verts,int size);
/*
	����������ʽ��������
*/
void objVBO_pushExportObj(struct Obj_vbo_model* _pvboModel,const char* str);

/*
	����
*/
struct Obj_vbo_model* objVBO_create(const char* name,int type);
/*
	��Ⱦ
*/
void objVBO_render(int data,int parms);

/*
 * ��Ⱦһ��vbo�ڵ�
 * renderCallBack��Ⱦ�ص�
 */
void objVBO_renderNode(
						struct ObjVBO* vbo,
						struct GMaterial* tmat,
						Matrix44f m,//��ǰģ�͵ľ���
						int flag,
						void (*renderCallBack)(int,struct ObjVBO*));

#endif