#ifndef _EX_H_
#define _EX_H_
#include "tlgl.h"
#include "lua.h"
//#include "lualib.h"
//#include "lauxlib.h"

#define  SPRITE_TRIANGLE_COUNT	18	//һ��sprite 2d��Ƭ��Ҫ��������(2 * 3 * 3)���ݳ���

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

	///*
	//*	���ʰȡģ��
	//*/
	//struct Ent3D* pick3DObject;

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
	///************************************************************************/
	///* ��ʱ����															
	///************************************************************************/
	//int _temp_fpsOut;
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
void setCamPos(float x,float y,float z);

/*
������Ⱦ�ı�����ɫ
*/
void setBgColor(float r,float g,float b);

/*
	��ӡ���浱ǰ��Ϣ
*/
void ex_info();

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
void ex_add(void* ptr);
/*
	��ȡ������
*/
struct EX* ex_getInstance();
/*
	����һ��obj,����md2��
	float rayRadius	:����ʰȡ�뾶
*/
int ex_load_model(char* name,const char* url,float x,float y,float z,float scale);

/*
	���ö���
*/
void ex_set_anim(void* ptr,const char* animKey);
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

/*
	����VBOģʽ��ģ��
*/
int ex_load_vbo(char* name,const char* url);

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

/*
	��ȡfps
*/
int ex_fps();

#endif