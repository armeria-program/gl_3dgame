#ifndef _EX_H_
#define _EX_H_
#include "tlgl.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


#define  SPRITE_TRIANGLE_COUNT	18	//һ��sprite 2d��Ƭ��Ҫ��������(2 * 3 * 3)���ݳ���

struct TextField
{
	void* base;

	//�ı�����
	char* textPtr;

	//�ı���Ļ����
	int x,y;

	//�ı���ɫ
	float r,g,b;

	//�ı��ĸ߶�
	int height;
	
	//��������С
	int bufferSize;
	
	//�ı��ܳ���
	int textWidth;
};

struct TextField* tf_create(int bufferSize,int x,int y,float r,float g,float b);
void tf_dispose(struct TextField* tf);
void tf_setPos(struct TextField* tf,int x,int y);
void tf_setText(struct TextField* tf,const char* str);
/*
 *	��ȡ��ǰ�ı��Ŀ��
 */
int tf_getTextWidth(struct TextField* tf);
void tf_copy(struct TextField *tf_source,struct TextField *tf_dest);
/*
	��ȡ�ı�
*/
//void tf_getStr(struct TextField* tf,char* const buffer,const int bufferSize);

typedef struct Ent3D{
	struct HeadInfo* base;

	//struct VBO_Obj* vbo;

	/*�������*/
	int vertexCount;

	/*��������*/
	GLfloat* vertex;

}Ent3D;


struct MD2_Object
{
	/*
	*	ͷ��������Ϣ,�ɼ̳�
	*/
	struct HeadInfo* base;

	/*
	*	���������,�ö���ֻ�Ǵ洢md2ģ�͵����ݽṹ
	*/
	//struct MD2_ParseObj* parseHandle;
	void* parseHandle;
};

//���״̬
struct MouseState
{
	GLint button;
	GLint action;
	
	GLint xMouse,yMouse;//����µ����״̬
	
	int moveX,moveY;//��ǰ�ƶ���λ��
};

//=============================================================================================================
struct EX
{
	//public
	/*
	*	��Ӱ������
	*/
	float camx,camy,camz;

	/*
		����ö�ٶ���,������λ�洢
	*/
	int flags;
	
	/*
	frame per second ÿһ�ؼ�֡��Ҫ�ĺ���
	*/
	int fps;

	/*
	��Ⱦ�ڵ�����б�
	*/
	struct LStackNode *renderList;

	/*
	*	�������ɫ
	*/
	float bg_r,bg_g,bg_b;

	//public
	/*
	*	����
	*/
	GLdouble zFar;

	/*
	*	���ʰȡģ��
	*/
	struct Ent3D* pick3DObject;

	/*
		��Ļ�ߴ�
	*/
	float _screenWidth,_screenHeight;

	/*
		��Ⱦѭ���ص�
	*/
	void (*loopCallBack)(void);

	void (*drawLine_callBack)(void);

	/*
		��ǰ���״̬
	*/
	struct MouseState mouseState;

	/*
		����ص���������
	*/
	void (*mRayPickCallBack)(struct HitResultObject* hit);


	///*
	//	sprite��������Դ,�����ͷź�Ҫ�������Դ
	//*/
	//struct VertexData spriteVert;

	/*
		�Ƿ��Ѿ�������˽���
	*/
	int isHitRaySprite;

	/*
		��־�ı�
	*/
	struct TextField* logTf;

	lua_State* mylua;

	/*
		UI �ӽǵ�z���
	*/
	int zBuffer;

	/*
		�ܹ���Z���
	*/
	int allzBuffer;

	/*
		��������
	*/
	int allVertexTotal;

	/*
		͸�Ӿ���
	*/
	Matrix44f perspectiveMatrix;
	/*
		ģ����ͼ����
	*/
	Matrix44f modelViewMatrix;

	Matrix44f ui_perspectiveMatrix;
	Matrix44f ui_modelViewMatrix;

	float camRotateX,camRotateY,camRotateZ;

	//struct Atals* myAtals;//����

	/*
		Ĭ�ϲ���
	*/
	void* defaultMaterial;
};

//�ӿڶ���
/*

	1.ת��Ϊ�༭����
	2.���нű�md2Export.ms
	3.��ѡSave Anumation (Frame Step���ó�1),Generate Normal,Active Time Segment
	4.Export

*	���ز�����md2ģ�Ͷ���
*	�ֽ�������ʹ�ô�˷�ʽ
*/
void md2_load(struct MD2_Object* _md2,const char* str,int len,const char* name);
/*
*	��Ⱦ�ص�
*/
void md2_render(struct MD2_Object* _md2);
/*
*	����
*/
void md2_dispose(struct MD2_Object* _md2);


void ent_dispose(struct Ent3D* p);
//=====================================================
/*
���������豸������
*/
struct EX* ex_create();

/*
��Ⱦѭ��
*/
void render(void);

/*
����name��ȡHeadInfo;
*/
struct HeadInfo* ex_find(struct EX* p,const char* name);
void* ex_find_node(const char* name);
/*
��ʼ��
*/
void ex_init(struct EX* p,GLdouble zfar);

/*
��������
*/
void ex_dispose(struct EX* p);

/*
����md5ģ��
*/
void* createMD5(struct EX* engine,struct XMLSList* xml,const char* id,const char* targetName,float x,float y,float z,float _scale);


/*
����ent ����OBJ��̬ģ�Ͷ��� ʹ��VBO
flags:	��ʶ
*/
struct Ent3D* load_obj(const char* targetName,const char* mesh_s,
	float x,float y,float z,
	float scale);

/*
	����md5ģ��
*/
void* load_md5(const char* name,const char* url,float x,float y,float z,float scale);

/*
���һ��md2ģ�͵�����

ʹ�÷���:
gMd2=ex_md2Add(p,"horse",
"E:\\source\\resource\\md2\\horse.md2",
"E:\\source/resource/md2/horse.bmp",
"stand,0,39|run,40,45|attack,46,53|pain,54,65|jump,66,71|filp,72,83|salute,84,94|taunt,95,111|dead,178,197|",1);

const char* model:	ģ��·��
const char* tex:	��ͼ·��
char* animConf:		��������
const char* defaultAnim:	Ĭ�ϵ�һ������

lineSize:		�߿�ĳߴ� >0�����߿�,������
isCanSelect:	�Ƿ���Ա�ѡ��

void (*render)(struct HeadInfo*)	:��Ⱦ�ص�

int flags	:��ʾ
*/
struct MD2_Object* load_md2(
	const char* name,
	const char* model,
	float x,float y,float z,
	float scale
	);
/*
	���ض�������,����
*/
int load_animConfig(void* ptr,char* animConf,long fps);

/*
gluUnProject -- map window coordinates to object coordinates 
����Ļ����ӳ�䵽��������

gluUnProject maps the specified window coordinates into object coordinates using modelMatrix, 
projMatrix, and viewport. The result is stored in objx, objy, and objz. 
A return value of GL_TRUE indicates success, and GL_FALSE indicates failure


��ά��ά����ת�������� gluUnProject()
�˺����ľ�����;�ǽ�һ��OpenGL�����ڵĶ�ά��ת��Ϊ�����Ӧ�ĳ����е���ά���ꡣ
ת����������ͼ��ʾ���ɵ�P�ڴ����е�XY����õ�������ά�ռ��е��������꣩��
*/
void mousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse);

/*
	����ƶ�
*/
void mouseMove(int x,int y);

/*
��ͨ��������a,1,2,3
*/
void onKeyboardCallBack(unsigned char key, int x, int y);

/*
������Ӱ��������
*/
void setCamPos(float x,float y,float z);

/*
������Ⱦ�ı�����ɫ
*/
void setBgColor(float r,float g,float b);

/*
	��������ĳ��״̬
*/
void set(int type);

/*
	��ʼ������ؽӿ�
*/
void ex_bind(int argc,char** argv);

/*
	�ر�ĳ��״̬(���ø�״̬λΪ0)

	0001
&	0000
=	0000

	0001
|	0000
=	0001

*/
void reset(int t);
/*
	��ȡ���浱ǰ��״̬
*/
//int getState(int type);

/*
	��ӡ���浱ǰ��Ϣ
*/
void ex_info();

/*
	�л���ʶ
*/
void ex_switch_baseFlags(struct HeadInfo* _node,int flag);

/*
	����Ƿ񳣰���
*/
int ex_mouseIsLeftDown();
/*
	���³����еĽ�������
*/
void ex_update_uiPos();

/*
	���һ����Ⱦ�ڵ�
*/
void ex_addNode(struct EX* p, void* _node);

/*
	��ȡ������
*/
struct EX* ex_getInstance();
/*
	����һ��obj,����md2��
	float rayRadius	:����ʰȡ�뾶
*/
int load_model(char* name,const char* url,float x,float y,float z,float scale);
/*
	Ϊptr���ò���material
*/
void setMaterial(void* ptr,void* material);
/*
	����ptr��״̬
*/
void setv_ptr(void* ptr,int flags);

/*
	���ö���
*/
void setanim(void* ptr,const char* animKey);
/*
	����Ļ���Ͻ���ʾ��־�ı�
*/
void ex_showLog(const char* buffer);
/*
 *	��ʾһ��������־
 */
void ex_showLogFloat(float v);

/*
	�ص�lua����

	����һ��������lua����
	const char* luaFunName	:lua�еĺ�����
	int parm				:��������


	lua_pcall(lua_State *L, int nargs, int nresults, int errfunc)
	nargs:����������nresults������ֵ������errFunc������������0��ʾ�ޣ���ʾ����������ջ�е�����


	lua:
	===================================
	function btnCallBackEvt(spriteName)
	end
*/
void ex_callParmLuaFun(const char* luaFunName,const char* parm);
/*
	����lua����luaFunName(lua����������luaȫ�ֺ���),����һ��int�͵Ĳ���
*/
void ex_callIntLuaFun(const char* luaFunName,int value);
/*
	��ȡzBuffer
*/
int ex_zBuffer();

/*
	����VBOģʽ��ģ��
*/
int load_VBO_model(char* name,const char* url);

//void updatePerspectiveMatrix( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar);
void ex_updatePerspctiveModelView();
/*
 *���ö���ĳ���Ŀ��
 */
void setLookTarget(void* ptr,float x,float y,float z);
/*
	Ѱ�ҽڵ�
*/
void* ex_findNodeByName(struct EX* ptr,const char* name);
/*
	��ȡ�����һ��Ĭ�ϲ���,����ʹ��,�ڴ���û�в��ʵ�ʱ������
*/
void* ex_get_defaultMaterial();
/*
	�����߶�
*/
void ex_drawline();
#endif