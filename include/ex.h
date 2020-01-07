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
//#define EX_FLAGS_RAY 0x01

//#define EX_FLAGS_DRAW_DEBUG_TEXT 0x02	/*�Ƿ���ʾ������Ϣ���ı��ڱ��⴦*/
#define EX_FLAGS_RAY_TO_UI 0x04			/*�Ƿ������˽���Ԫ��*/
#define EX_FLAGS_LEFT_DOWN 0x08			/*����Ƿ����������*/

//##############################################################
#define	E_RenderModeNormal	1			//			�̶�������Ⱦģʽ
#define	E_RenderModeVBO		2			//			VBO��Ⱦģʽ

//�ļ����Ͷ���
#define TYPE_OBJ_FILE 0	//obj����
#define TYPE_MD5_FILE 1 //md5����
#define TYPE_MD2_FILE 2 //md2����
#define TYPE_SPRITE_FLIE 3//UI���������е�sprite
#define TYPE_TEXT_FILE	4	//�ı�����
#define TYPE_OBJ_VBO_FILE	5//VBO�ļ�����
#define TYPE_FONT_TXT  6	//freetype�ı�
#define TYPE_LINENODE 7 //��

//��ֵö��
#define KEY_ESC 27
#define KEY_1 49
#define KEY_2 50
#define KEY_A 97
#define KEY_B 98
#define KEY_C 99
#define KEY_D 100
#define KEY_I 105

/************************************************************************/
/* �����¼�����                                                         */
/************************************************************************/
#define EVENT_ENGINE_RENDER_3D 100				//������Ⱦ�ص�
#define EVENT_ENGINE_KEYBOARD  101				//ȫ�ּ����¼�
#define EVENT_ENGINE_RESIZE	   102				//resize�¼�

#define EVENT_ENGINE_BASE_UPDATE	   102				//base�����¼�
#define EVENT_ENGINE_BASE_END		   103				//base�����¼�

#define EVENT_ENGINE_SPRITE_CLICK		   104//click
#define EVENT_ENGINE_SPRITE_CLICK_DOWN	   105//click down
#define EVENT_ENGINE_SPRITE_CLICK_MOVE	   106//click move
#define EVENT_ENGINE_SPRITE_FOCUS_CHANGE   107//���㷢���仯
#define EVENT_ENGINE_TEX_LOAD_COMPLETE	   108//��ͼ���������
#define EVENT_ENGINE_COMPLETE			   109//����¼�,���¼��ǿ��Թ����


//#define EVENT_ENGINE_SPRITE_FOCUS_OUT   108	//2d�����뿪��ʱ�򴥷�
//#define EVENT_ENGINE_SPRITE_FOCUS_IN    109	//2d��������ʱ�򴥷�

#define CUST_LUA_EVENT_SPRITE_FOCUS_CHANGE 110 //��lua�㷢�ͽ���仯
#define CUST_LUA_EVENT_INPUT_CHANGE		   111 //��lua�㷢��input����仯
/*
*�����ʼ�����(stage2d,�豸,lua����ע�����,����ִ���ϲ�ҵ��)
*/
#define EVENT_ENGING_INIT 200	

/**
 * ��ʱ���¼�
 */
#define EVENT_TIMER 201

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
	// �м����� -1:����,1:���� 0:��ʱ�� ˵��û�й���
	int midDirection;
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
	//struct ECamera cam;//camera
	/*
		����ö�ٶ���,������λ�洢
	*/
	int flags;
	
	/************************************************************************/
	/* �����豸���                                                                     */
	/************************************************************************/
	void* ft;

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
		��ǰ���״̬
	*/
	struct MouseState mouseState;
	struct HitResultObject* hit;
	lua_State* mylua;

	/*
		UI �ӽǵ�z���
	*/
	//int zBuffer;

	//��ǰ�Ľ�����ui z�������
	float ui_pos_z;

	/*
		�ܹ���Z���
	*/
	int allzBuffer;

	//2d��̨
	void* stage2d;
	
	//��ʱ���sprite��ȡ����Ϣ
	void* clickInfo;

	//��ǰ���Ľ���(struct Sprite*)
	void* curFocus;
	
	//3d Camera����
	void* _3dcam;

	//2d Camera����
	void* _2dcam;

	//��ǰ��camera
	void* _3dCurCam;
	
	//��ǰ��2dCam
	void* _2dCurCam;
	
	//fbo���������
	struct LStackNode *fboList;

	void* pro3d;//��ɫ�����������

	//��ֵ������
	void* mapptr;

	struct LStackNode* timelaterList;

};

//�ӿڶ���
//=====================================================
/*
*	��Ⱦѭ��
*/
void ex_render(void);

//��Ⱦrenderlist���е�3d�ڵ�
void ex_renderAll3dNode();
//void f_renderlistCall(void _callBack(int));

/*
*	��ʼ��
*/
void ex_init(struct EX* p,GLdouble zfar,float sw,float sh);

//�ػ�ص�
void ex_reshape(int w,int h);

/*
*	��������
*/
void ex_dispose(struct EX* p);

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
	���һ����Ⱦ�ڵ�
*/
void ex_add(void* ptr);
/*
	��ȡ������
*/
struct EX* ex_getIns();
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
	����Ļ���Ͻ���ʾ��־�ı�
*/
void ex_showLog(const char* buffer);

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
void ex_ptr_remove(void* ptr);

/************************************************************************/
/* ������                                                               */
/************************************************************************/
int ex_rename(void* p,const char* name);

/*
	���ò���
*/
void 
ex_set_material(void* ptr,void* material);

/************************************************************************
	��ǰ��FPS                                                                  
	frame per second ÿһ�ؼ�֡��Ҫ�ĺ���
************************************************************************/
extern int g_fps;

/************************************************************************
	ÿһ�ؼ�֡���ӳٺ�����                                                                    
************************************************************************/
extern long g_delayTime;
/************************************************************************
�Ƿ���ȫ�ֵ�Sprite��ʾ�߿�
#define _SPRITE_RENDER_LINE_//��Ⱦ�߿�,���ڵ���,�˷�ʽ�����ڵ����õ�����ִ��,��Ϊʹ�õ��ǹ�����С��ģʽ
************************************************************************/
//extern int g_sprite_line;

/*
 *��ȡƫ��ֵ
 */
int ex_get_gap(int type);


//�л�����ǰcamera,�����п��ܻ��ж��camera,���ǿ�������������������л�����ǰ��cam
//�����Ĭ�ϵ�cam�л���fbo��cam,��ʵcam����һ�����͸�Ӿ����һ��ģ�;���
void ex_switch3dCam(void* cam);

//����VBOģʽ��ģ��
void* ex_loadVBO(char* name,const char* url);

//�л���2d camera
void ex_switch2dCam(void* cam);

//���һ��fbo��������
void ex_add_fbo(void* fbo);

//�������б����Ƴ�fbo
void ex_remove_fbo(void* fbo);

//����shader���ͻ�ȡһ��
GLuint ex_getProgrom3D(const char* glslType);

//��ָ�����¼�����һ������������
void ex_lua_evt_dispatch_f(void* obj,int evtid,float data);

//��lua����һ���¼�
void ex_lua_evt_dispatch(void* obj,int evtid,const char* data);
//*******************************************************************
/*
	���ض�������,����
*/
int ex_load_anim_config(void* ptr,char* animConf,long fps);

/*
 *	���ö���
 *	playend:��ǰ�����ؼ�֡�������֮��ص��ú�������
 */
int 
ex_animtor_ptr_setcur(void* ptr,const char* animKey,void (*playend)(void*));

/*
������Ӱ��������
*/
void ex_cam_set_pos(float x,float y,float z);

/*
������Ⱦ�ı�����ɫ
*/
void setBgColor(float r,float g,float b);

/*
	��ӡ���浱ǰ��Ϣ
*/
void ex_get_info();
#endif