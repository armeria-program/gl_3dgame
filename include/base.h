#ifndef _BASE_H_
#define _BASE_H_

#define BOX_R 1.0 //��ײ�е�rֵ

/*
	�����ṹ��,����ent3d��md2�̳�
*/
typedef struct HeadInfo{
	/*�¼�����*/
	void* evtList;
	/*����*/
	int curType;
	
	/*����,�����ڲ�ѯ,��ΪΨһʵ��*/
	char* name;

	/*����*/
	double x,y,z;

	float rx,ry,rz;
	
	//��ת��ͽǶ�
	float ax,ay,az,angle;

	/*����ֵ*/
	float scale;
	
	/*
		��������	===>struct GMaterial* tmat;
	*/
	struct GMaterial* tmat;

	/*
		��ʶλ,��ʶ����״̬
	*/
	int flags;
	
	/*
		����ı任����
	*/
	Matrix44f* m;

	//�������仯��ʱ������Ϊ1
	GLbyte changed;
	/*
		��ǰ��2dcam��ʱ�����2dcam(sprite)
		�ýӿ�sprite_set2dCam������
		��ǰ��2d camera����,��������ȷ����sprite���ĸ�2d camera����ռ�,
		 
		��ǰ��3d�����ʱ�����3dCam(md2,obj...)
	*/
	void* cam;
}HeadInfo;

/*
	��Ⱦһ�������ʵ�ģ��
*/
void base_renderByMaterial(struct HeadInfo* base,GLfloat* vertex,int vertLen);



/*
	Base ����ģ�Ͷ���Ļ�������,λ��,��ת,����
	�����ģ�������Ƿ���md2,md5,obj�޹�,���������ǵĸ���
*/


///*
//	��������
//*/
//struct VertexData
//{
//	float* vert;	//��������
//	int count;		//float ���ݸ���
//};

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
/************************************************************************/
/* ���ö��������ֵ                                                                  
/************************************************************************/
void 
base_set_scale(void* p,float scale);
/*
	���¾���
*/
void base_updateMat4x4(struct HeadInfo* base);
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
	��ȡ��Ⱦģʽ�Ƿ����߿���ʵ��ģʽ
*/
int base_get_ploygonLineMode(int flag);

/*
*	����Ⱦ�б�����ʰȡ
*	struct Vec3* offset	����һ��ƫ������,������Ϊ��ͼ�����ƫ��ֵ
*/
void base_seachPick(struct LStackNode* list,struct Vec3* nearPoint,struct Vec3* farPoint,struct HitResultObject* last);

/*
*	����
*/
void base_dispose(struct HeadInfo* base);

struct HeadInfo* base_get(void* p);

/*
	��� HeadInfo �ṹ��,����head��Ϣ��ȡ�����Ϣ,��Ҫβ������һ��HeadInfo*����
*/
//void base_get(void* point,struct HeadInfo* base);

//ʵ����Ⱦ
void base_renderFill(struct HeadInfo* node,GLfloat* vertex,int vertLen);

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
//int base_isCanAnimation(struct HeadInfo* ptr);
/*
	����һ�����ʰȡ��lua�����ص�
*/
//void base_setLuaPick(struct HeadInfo* base,const char* luaFunctionName);

/************************************************************************/
/* ���ú�׺                                                    */
/************************************************************************/
//void base_set_suffix(struct HeadInfo* base,const char* str);
/*
	˫�����
*/
GLboolean base_cullface(int flag);
//==========================================================================

/*
 *ms����y����ת��ry
 **/
//void base_rotate_to(HeadInfo* bp,float ms,float ry);

/*
	��ʼ���
	GLint xMouse, GLint yMouse:���ʰȡ��2d��Ļ����
	mRayPickCallBack �ص�
*/
void base_hit_mouse(GLint xMouse, GLint yMouse,float screenWidth,float screenHeight,
			   struct LStackNode *renderList,Matrix44f perspectiveMatrix,Matrix44f modelViewMatrix,
			   void (*mRayPickCallBack)(struct HitResultObject*));

/*
	�ý�ɫ��������x y z
*/
void base_look_at(HeadInfo* p,float x,float y,float z);
/*
	λ�Ƶ�x y z����
*/
void 
base_move(HeadInfo* ptr,int ms,
		  float x,float y,float z,
		  void (*endCallBack)(void*),
		  void (*updateCallBack)(void*)
		  );


void base_setv(void* ptr,int flag);
void base_resetv(void* ptr,int flag);
int base_getv(void* ptr,int flag);

//��������������Ⱦ�ڵ�
void* base_findByName(void* list,const char* name);


//�����ĸ��¾���Ľӿ�,�Ż�����,��֤ÿһ��frameֻ����һ�ξ���
void
base_realUpdateMat4x4(void* p);

//����cam���
void
base_set_cam(void* p,void*cam);

//��ȡcam���
void*
base_get_cam(void* p);
/*
	v:1 ��ʾ 0 ����
*/
void base_set_visible(struct HeadInfo* base,int v);

//���Զ�������ת
void base_rotate_vec(struct HeadInfo* base,float ax,float ay,float az,float angle);

#endif