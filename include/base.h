#ifndef _BASE_H_
#define _BASE_H_

/*
	�����ṹ��,����ent3d��md2�̳�
*/
typedef struct HeadInfo{
	/*�¼�����*/
	void* evtList;
	/*����*/
	int curType;
	
	/*����,�����ڲ�ѯ,��ΪΨһʵ��*/
	char name[G_BUFFER_32_SIZE];
	/*�ļ���׺*/
	char suffix[G_BUFFER_16_SIZE];

	/*����*/
	double x,y,z;

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
	//void (*renderCallBack)(void* ptr);
	
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
	/*�Ƿ���һ��Node*�ṹ*/
	int isNode;
	
	/*
		�����ĸ�����
	*/
	void* parent;
	void* _move_tp;
	void* _ry_tp;
}HeadInfo;

/*
	��Ⱦһ�������ʵ�ģ��
*/
void base_renderByMaterial(struct HeadInfo* base);



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

struct HeadInfo* base_get(void* p);

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
//int base_isCanAnimation(struct HeadInfo* ptr);
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
 *ms����y����ת��ry
 **/
void 
base_rotate_to(HeadInfo* bp,float ms,float ry);

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
#endif