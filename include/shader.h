#ifndef _SHADER_H_
#define _SHADER_H_
#define SHADER_VBO_SIMPLE 1 

////font1�ı���ɫ���ϴ����ݵ�GPU�Ļص�
//void 
//font1_updateVarCallback(void* material,Matrix44f M,void* param,void* cam);

//9������ɫ���ص�
void
grid9CallBack(void* material,Matrix44f M,void* param,void* cam);

//SpriteVBO��ɫ��
void
vboSimpleCallBack(void* material,Matrix44f M,void* param,void* cams);

//�߿���Ⱦ��ɫ��
void 
vboLineCallBack(void* material,Matrix44f M,void* param,void* cam);
#endif