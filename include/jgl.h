#ifndef _JGL_H_
#define _JGL_H_
#include <gl/glew.h>

struct RGB { 
	GLubyte blue;
	GLubyte green;
	GLubyte red;
	//GLubyte alpha;
};
typedef struct RGBA { 
	GLubyte blue;
	GLubyte green;
	GLubyte red;
	//GLubyte alpha;
}RGBA;
/*
	����·������һ����ͼ
	����·����.tga,.bmp������ͬ���͵���ͼ

	����mTextureID
*/
GLuint 
jgl_loadTex(const char *szFileName);

/*
	����һ��colorType���͵��Զ���λͼ

	����mTextureID
*/
GLuint
jgl_createRGB_Texture(int width,int height,GLuint colorType);


GLuint
jgl_create_opengl_RGBA_Tex(int width,int height,GLenum rgbaTypes);
////////////////////////////////////////////////////////////////////
// ��ȡtga�ļ�
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas..... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *jgl_readTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

/*
	���²�����ͼ����
	const char* imgUrl		:����·��
*/
void 
jgl_subImage(GLuint texName,const char* imgUrl,int offsetX,int offsetY,int subImageWidth,int subImageHeight);

/*
 *��texName�����ֽ������ݸ���texture
 **/
void 
jsl_sub(GLuint texName,		//��ͼ���
		GLubyte* pBytes,GLenum eFormat,GLenum type,
		int offsetX,int offsetY,
		int subImageWidth,int subImageHeight);

/*
	����һ��λͼ�ڴ�,�������Ϊ��ɫ,������ʵ��������һ��CPU�еĻ�����buffer
*/
GLbyte*
jgl_createRGB_Bitmap(int imageWidth,int imageHeight);

/*
	����һ��rgbaλͼ�ڴ�,�������Ϊ͸��ɫ
*/
GLbyte*
jgl_createRGBA(int imageWidth,int imageHeight);
/************************************************************************/
/* ָ���ڴ���ڵ�����ֵ                                                                     */
/************************************************************************/
void
jgl_drawPontPixelRGB(GLbyte* bytes,int x,int y,GLint w, GLint h,struct RGB* rgb);
#endif