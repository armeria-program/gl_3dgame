//MD5�ӿ�
#ifndef _MD5_H_
#define _MD5_H_
#include "tools.h"

//==========================================================================

struct MD5{
	void* base;
	//========================================
	/*��������Ĺؼ�֡֡��*/
	int fpsInterval;
	int frameTicket;				/*�ؼ�֡������*/
	void* model;					/*md5�ṹ������*/
	Vec3_t *vertexArray;			/*��������*/
	Vec8_t *vertexOut;
	Vec8_t *trianglesArray;
	GLuint *vertexIndices;			/*������������*/
	
	void* curAnim;
	struct LStackNode* pAnimList;	//�����б�
	void* animInfo;					//anim_info_t
	void *skeleton;					//md5_joint_t
};
/*
	��������ģ��
*/
void md5_loadMesh(struct MD5* p,const char* path);

/*
	���ض����ļ�
 */
void md5_loadAnim(struct MD5* p, const char* animFile,const char* animName);
/*
	���ö���
	#define ANIM_STR "stand,walk,att,skill01,skill02"
	
	anim=0 stand
	anim=1 walk
*/
void md5_setAnim(void* _md5,const char* anim);

/*
	��Ⱦ
*/
void md5_render(struct MD5* p,int fps);

/*
	����md5
*/
void md5_dispose(struct MD5* out);
/*
	��ȡmd5�����Ĺؼ�֡��
*/
int md5_getFrameNum(void* _md5,char* animType);

/*
	��ȡ��ǰ�����������ܹؼ�֡��
*/
int md5_getTotalFrame(void* p);

/*
	��ȡ��ǰ��md5�ؼ�֡����
*/
int md5_getFrameIndex(void* _md5);
/*
	���ƹ����ڵ�
*/
void md5_drawSkeleton(void* ptr);

#endif