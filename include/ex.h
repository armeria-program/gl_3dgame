#ifndef _EX_H_
#define _EX_H_
#include "tlgl.h"
#include "lua.h"
//#include "lualib.h"
//#include "lauxlib.h"

#define  SPRITE_TRIANGLE_COUNT	18	//һ��sprite 2d��Ƭ��Ҫ��������(2 * 3 * 3)���ݳ���

/************************************************************************/
/*  EX->flags(32λ,�洢32��״̬)										*/
/************************************************************************/
/*
	EX_FLAGS_RAY:
	����ʰȡ����ö��,����һ���ܿ���
	���þ�̬��ײ����,���ó�TRUE�Ϳ������߼��,���������ʰȡ,����һ�����忪��,����ģ�Ϳ��Ե�������
*/
#define EX_FLAGS_RAY 0x01
#define EX_FLAGS_DRAW_DEBUG_TEXT 0x02	/*�Ƿ���ʾ������Ϣ���ı��ڱ��⴦*/
#define EX_FLAGS_RAY_TO_UI 0x04			/*�Ƿ������˽���Ԫ��*/
#define EX_FLAGS_LEFT_DOWN 0x08			/*����Ƿ����������*/

//##############################################################
#define	E_RenderModeNormal	1			//			�̶�������Ⱦģʽ
#define	E_RenderModeVBO		2			//			VBO��Ⱦģʽ

/************************************************************************/
/* �����¼�����                                                         */
/************************************************************************/
#define EVENT_ENGINE_RENDER_3D 100				//������Ⱦ�ص�
#define EVENT_ENGINE_KEYBOARD  101				//ȫ�ּ����¼�

//�����¼�״̬
struct E_KeyBoard
{
	unsigned char key;
	int x,y;
};

//���״̬
struct MouseState
{
	GLint button;
	GLint action;
	
	GLint xMouse,yMouse;//����µ����״̬
	
	int moveX,moveY;//��ǰ�ƶ���λ��
};
struct ECamera{
	/*
	*	��Ӱ������
	*/
	float x,y,z;
	float rx,ry,rz;
	//void* ptrFollow;//��������ö���
	//float followDistance;//����������ľ���
	Vec3 followOffset;//����ڸ����ɫ��ƫ��ֵ
};
//=============================================================================================================
struct EX
{	 
	//public
	/*
		�¼��б�
	*/
	void* evtList;
	/*
	*	��Ӱ������
	*/
	//float camx,camy,camz;
	//float camRotateX,camRotateY,camRotateZ;
	struct ECamera cam;//camera
	/*
		����ö�ٶ���,������λ�洢
	*/
	int flags;

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
		��Ļ�ߴ�
	*/
	float _screenWidth,_screenHeight;

	/*
		��Ⱦѭ���ص�
	*/
	void (*loopCallBack)(void);

	/*
		��ǰ���״̬
	*/
	struct MouseState mouseState;

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

	

	//struct Atals* myAtals;//����

	/*
		Ĭ�ϲ���
	*/
	void* defaultMaterial;
	
	
	
};

//�ӿڶ���
//=====================================================
/*
*	���������豸������
*/
struct EX* ex_create();

/*
*	��Ⱦѭ��
*/
void ex_render(void);

/*
*	��ʼ��
*/
void ex_init(struct EX* p,GLdouble zfar);

/*
*	��������
*/
void ex_dispose(struct EX* p);

/*
	���ض�������,����
*/
int ex_load_anim_config(void* ptr,char* animConf,long fps);

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
void ex_cam_set_pos(float x,float y,float z);

//��camera���������
//void ex_cam_bind(void* ptr);
/*
������Ⱦ�ı�����ɫ
*/
void setBgColor(float r,float g,float b);

/*
	��ӡ���浱ǰ��Ϣ
*/
void ex_info();

/*
	���³����еĽ�������
*/
void ex_update_uiPos();

/*
	���һ����Ⱦ�ڵ�
*/
void ex_add(void* ptr);
/*
	��ȡ������
*/
struct EX* ex_getInstance();
/*
	����һ��ģ��
	E_RenderMode mode:	����ʹ�õ���Ⱦģʽ
*/
void* 
ex_load_model(char* name,const char* url,int mode);

/*
	���ö���
*/
void ex_set_anim(void* ptr,const char* animKey);
/*
 *	���ö���
 *	playend:��ǰ�����ؼ�֡�������֮��ص��ú�������
 */
int 
ex_animtor_ptr_setcur(void* ptr,const char* animKey,void (*playend)(void*));
/*
	����Ļ���Ͻ���ʾ��־�ı�
*/
void ex_showLog(const char* buffer);
///*
// *	��ʾһ��������־
// */
//void ex_showLogFloat(float v);

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


//void updatePerspectiveMatrix( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar);
void ex_updatePerspctiveModelView();

/*
	Ѱ�ҽڵ�
*/
void* 
ex_find_ptr(struct EX* ptr,const char* name);
/*
*	����name��ȡHeadInfo;
*/
struct HeadInfo* 
ex_find_headinfo(struct EX* p,const char* name);

/************************************************************************/
/* �����ڵ�                                                                     */
/************************************************************************/
void* 
ex_find(const char* name);
/*
	ɾ�������е�һ������
*/
void ex_ptrRemove(void* ptr);

/*
 *���ö���ĳ���Ŀ��
 */
void setLookTarget(void* ptr,float x,float y,float z);
/*
	��ȡ�����һ��Ĭ�ϲ���,����ʹ��,�ڴ���û�в��ʵ�ʱ������
*/
void* ex_get_defaultMaterial();


/************************************************************************/
/* ������                                                               */
/************************************************************************/
int ex_rename(void* p,const char* name);

/*
	���ò���
*/
void 
ex_set_material(void* ptr,void* material);

//��ȡ�������ı�ʶλ
void ex_setv(void* p,int flag);
void ex_resetv(void* ptr,int flag);
int ex_getv(void* ptr,int flag);
/************************************************************************
	��ǰ��FPS                                                                  
	frame per second ÿһ�ؼ�֡��Ҫ�ĺ���
************************************************************************/
extern int g_fps;

/************************************************************************
	ÿһ�ؼ�֡���ӳٺ�����                                                                    
************************************************************************/
extern long g_delayTime;


/*
 *��ȡƫ��ֵ
 */
int ex_get_gap(int type);

#endif