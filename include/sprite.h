#ifndef _SPRITE_H_
#define _SPRITE_H_
#include "vmath.h"
//#define USE_VBO_SPRITE	//�Ƿ�ʹ��VBOģʽ��Sprite
/************************************************************************/
/* �Ź���ṹ��                                                                     */
/************************************************************************/
struct Grid9Node{
	float left,right,top,bottom,sx,sy,width,height;
};
//2d��������
struct Sprite{

	void* base;
	float pos_x;		//��ʾ������ͶӰ����ϵ(���½�Ϊ����ԭ��)�����꣬
	float pos_y;
	float pos_z;		//�ô�ֵ����ʶsprite�Ƿ���ǰ��һ��s
	
	//z�������
	float zScale;
	//�����������ĵ���ֲ�����
	int mouseLocalX,mouseLocalY;

	//��Ļ����,�����Ͻǿ�ʼ����0,0
	float screenX;
	float screenY;

	float mWidth;		//sprite����Ļ���ص�λ
	float mHeight;
	int m_bPressed;

	int mouseDownX,mouseDownY;//����������µ�����

	int hitX,hitY,hitWidth,hitHeight;//���������Χ����

	void* vertexs;//GLfloat*���㻺������,ʹ����֮��Ҫɾ��tl_free
	int	vertLen;

	void* material;//����

	struct Atals* atals;//ͼ��
	
	/************************************************************************/
	/* ��������                                                    */
	/************************************************************************/
	//void* material;
	
	Matrix44f mat4x4;
	
	/*
		������ʰȡ��ʱ��ʹ��
	*/
	float *hitTriangle;
	//float *triangle2;
	
	/*
		Sprite����ص�
	*/
	void (*clickCallBack)(struct Sprite* ptrSprite,int localX,int localY);

	/*
		��Sprire��Ե�0,0�������õ���ק��Χ,������õ���0,û�з�Χ����
	*/
	int	dragX,dragY,dragWidth,dragHeight,oldx,oldy;

	/*
		--0:�����϶�����,1�����϶�����
	*/
	int	dragDirection;

	//char* luaTablePtr;
	int parseType;
	int useVBO;//�Ƿ�ʹ��VBOģʽ
	struct Obj_vbo_model* vbo;


	//�Ӷ����б�
	struct LStackNode *childs;
	//������
	void* parent;
	//�ֲ�����,����ڸ�����,�������stage�Ͼ�Ϊ0,0
	int localx,localy;
	//�Ź�������
	struct Grid9Node* grid9;
};

//����sprite��z���ֵ
void
sprite_set_z(void* p,float v);
/*,
 *Ϊspr���һ���Ӷ���
 **/
void sprite_addChild(void* spr,void* child);
/*�Ƴ��Ӷ���*/
void sprite_removeChild(void* spr,void* child);

//�������꣬����ھֲ�����
//void sprite_setLocalPos(void* ptr,int x,int y);

/*
 *��ȡȫ������
 */
//void sprite_getGlobalPos(void* spr,int* px,int *py);

//���õ���ص�
void
sprite_set_clickHandle(void* p,void (*clickCallBack)(void* ,int ,int ));

/*
 *��addChild֮��������������
 *�����sprite��������sprite���Ӷ����ʱ��,ʹ�øýӿ�����������ڸ����������
 */
void sprite_set_self_pos(void* p,int x,int y);

/* 
	��ʼ����ť

	void (*clickCallBack)(struct Sprite* ,int ,int )	����ص�����,���δ���ûص������ǲ������õ��ʰȡ�¼���
*/
struct Sprite* sprite_create(
	char* _spriteName,
	int x,int y,
	int width,int height,
	void (*clickCallBack)(struct Sprite* ,int ,int ),void* _2dCam);

/*
	����sprite��UV
*/
void sprite_setUV(struct Sprite* sprite,float x,float y,float w,float h);

/*
	����һ��sprite 
*/
void sprite_drawRender(int data);

/*
	��ť�¼���Ч
*/
void sprite_action(const int data);

/*
	��Ⱦ����ui�ڵ�
*/
void sprite_updatePos(int data);
/*
	ɾ��sprite�ڵ�
*/
void sprite_dipose(struct Sprite* spr);

/*
	����ͼ���е�key����λͼ,atlas��ʵ��һ������
*/
void sprite_texName(struct Sprite* spr,const char* texName,struct AtlasNode* ptr);
/*
	��������
*/
void sprite_setpos(struct Sprite* spr,int x,int y);

/*
	�������������קֻ�øýӿڸ���sprite����
*/
void sprite_mouseMove(int data);

/*
	�Ƿ������Ⱦ
*/
int sprite_isEnable(int data);

/*
	����sprite���϶���Χ
	x,y:����ק������
	w:����ק�Ŀ��
	h:����ק�ĸ߶�
*/
void sprite_setDragScope(struct Sprite* pSpr,int x,int y,int w,int h);

void sprite_rotateX(struct Sprite* ptr,float rx);
void sprite_rotateY(struct Sprite* ptr,float ry);
/*
 *��תrz�� PI��n����,ֻ�任��Ⱦ����
 */
void sprite_rotateZ(struct Sprite* ptr,float rz);

/*
	����sprite�ĳߴ�
*/
void sprite_resize(struct Sprite* spr,int w,int h);
/*����Z����  */
void sprite_set_scale_z(struct Sprite* spr,float v);
//��������˽����֮����Դ�͸�����
//void
//sprite_open_through(void* spr);
/*
	�Ƿ�������¼�
*/
int 
sprite_isCanClick(void* p);
//ʹ����Ա���Ⱦ,��ʾ����
//void 
//sprite_set_render(void* p,int v);

//ΪSprite����һ��Ĭ�ϵ�ͼ��
void
sprite_set_default_tex(void* p);

/*
 *����һ���������ķ�Χ
 */
void
sprite_set_hit_rect(void*p,int x,int y,int width,int height);
//��ȡsprite�Ĳ��ʾ��
void*
sprite_get_material(void* sprite);

//����һ��û�и���ͼ��2dSprite
void*
sprite_createEmptyTex(int texW,int texH,void* _2dCam);

//���õ�ǰ��sprite��2dcam,��2dcam��ʱ����
void
sprite_set2dCam(void* spr,void* _2dcam);
//����9����
//void
//sprite_set9Grid(void* spr,int top,int bottom,int left,int right);

//���þŹ�������
void
sprite_set_grid9(void* mat,float left,float right,float top,float bottom,float w,float h);

//����ͼ��
void
sprite_bindAtals(void* p,void* atals);
//void sprite_refresh_local_pos(void* p);
//�Ƴ�����
//void sprite_removeSelf(void* p);
#endif