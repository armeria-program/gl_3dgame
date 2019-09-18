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

//#define EVENT_ENGINE_SPRITE_FOCUS_OUT   108	//2d�����뿪��ʱ�򴥷�
//#define EVENT_ENGINE_SPRITE_FOCUS_IN    109	//2d��������ʱ�򴥷�


/*
*�����ʼ�����(stage2d,�豸,lua����ע�����,����ִ���ϲ�ҵ��)
*/
#define EVENT_ENGING_INIT 200	
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
//struct ECamera{
//	/*
//	*	��Ӱ������
//	*/
//	float x,y,z;
//	double rx,ry,rz;
//	//void* ptrFollow;//��������ö���
//	//float followDistance;//����������ľ���
//	//Vec3 followOffset;//����ڸ����ɫ��ƫ��ֵ
//};
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

	/*
		��־�ı�
	*/
	//struct TextField* logTf;

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

	/*
		��������
	*/
	//int allVertexTotal;

	/*
		͸�Ӿ���
	*/
	//Matrix44f perspectiveMatrix;
	/*
		ģ����ͼ����
	*/
	//Matrix44f modelViewMatrix;

	//Matrix44f ui_perspectiveMatrix;
	//Matrix44f ui_modelViewMatrix;
	//struct Atals* myAtals;//����
	
	//int _isinit;//�����Ƿ��Ѿ���ʼ�� 
	void* atals;
	
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
	
	//fbo����
	void* fbo;
	
	//װ��fbotex��sprite
	//void* fboTexSprite;

	//fbo��ͼ����
	//void* fboTexture;
};

//�ӿڶ���
//=====================================================
/*
*	���������豸������
*/
//struct EX* ex_create();

/*
*	��Ⱦѭ��
*/
void ex_render(void);

void 
ex_renderlistCall(void _callBack(int));

/*
*	��ʼ��
*/
void ex_init(struct EX* p,GLdouble zfar,float sw,float sh);

void 
ex_reshape(int w,int h);

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
void ex_get_info();

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
//��ȡһ��Ĭ�ϵ�uiͼ��
void* ex_get_ui_atals();
/*
	��ȡzBuffer
*/
float ex_newPosZ();


//void updatePerspectiveMatrix( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar);
//void ex_refresh3dModelView();



/*
	Ѱ��md2,md5,obj,sprite�ڵ�
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
//void ex_setv(void* p,int flag);
//void ex_resetv(void* ptr,int flag);
//int ex_getv(void* ptr,int flag);
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
extern int g_sprite_line;

/*
 *��ȡƫ��ֵ
 */
int ex_get_gap(int type);
/*
	����һ������
*/
void ex_alert(const char* format,...);
/*
	��lua����һ���¼�
*/
void 
ex_lua_evt_dispatch(void* obj,int evtid,const char* data);

/*
	��lua����һ��ȫ���¼�
*/
void 
ex_lua_global_evt_dispatch(int evtid);
//����2d��̨�ߴ�
void 
ex_resize_stage2d();

/*
 *������������,ֻ��Ҫ����Ⱦ���ڷ����ߴ�仯�����¼���һ�μ���
 */
//void ex_calculat_ortho();

//�л�����ǰcamera,�����п��ܻ��ж��camera,���ǿ�������������������л�����ǰ��cam
//�����Ĭ�ϵ�cam�л���fbo��cam,��ʵcam����һ�����͸�Ӿ����һ��ģ�;���
void ex_switch3dCam(void* cam);

//��Ⱦһ��3D�ڵ�
void 
ex_render3dNode(int data);

//����VBOģʽ��ģ��
void* 
ex_loadVBO(char* name,const char* url);

//�л���2d camera
void ex_switch2dCam(void* cam);

#endif