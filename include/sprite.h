#ifndef _SPRITE_H_
#define _SPRITE_H_

//#define USE_VBO_SPRITE	//�Ƿ�ʹ��VBOģʽ��Sprite

//2d��������
struct Sprite
{
	void* base;
	float pos_x;		//��ʾ������ͶӰ����ϵ(���½�Ϊ����ԭ��)�����꣬
	float pos_y;
	float pos_z;
	
	//z�������
	float zScale;

	int mouseLocalX,mouseLocalY;

	//float scaleWH;

	//��Ļ����,�����Ͻǿ�ʼ����0,0
	float screenX;
	float screenY;

	float mWidth;		//sprite����Ļ���ص�λ
	float mHeight;
	int m_bPressed;

	//int isLock;

	int mouseDownX,mouseDownY;//����������µ�����

	GLfloat* vertexs;//���㻺������,ʹ����֮��Ҫɾ��tl_free
	int	vertLen;

	//void* material;//��ť����

	struct Atals* atals;//ͼ��
	
	/************************************************************************/
	/* ��������                                                    */
	/************************************************************************/
	void* material;
	
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
		lua�лص��ĺ�����
	*/
	//int callLuaFunName[G_BUFFER_32_SIZE];

	/*
		lua�е�sprite�ƶ��¼�����
	*/
	//char* callLuaDragMove;
	
	/*
	 *��곣���¼�
	 */
	//char* callLuaMouseDown;

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


};
/*
	��ʼ����ť
*/
struct Sprite* sprite_create(
	char* _spriteName,
	int x,int y,
	int width,int height,
	void (*clickCallBack)(struct Sprite* ,int ,int ));

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

/*
 *��תrz�� PI��n����,ֻ�任��Ⱦ����
 */
void sprite_rotateZ(struct Sprite* ptr,float rz);
/*
	����sprite�ĳߴ�
*/
void sprite_resize(struct Sprite* spr,int w,int h);

#endif