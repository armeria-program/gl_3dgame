#ifndef _GLOBAL_DEFINE_H_
#define _GLOBAL_DEFINE_H_

#define DEFINE_BASE_64_STRING  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

#define PI 3.1415926f    //�����(Բ����180��)
#define EPSILON 0.000001
//===================buffer�ߴ綨��===================
#define G_CHAR_SIZE_ 10			/*���е�char�ߴ� 10*/
#define G_BUFFER_64K_SIZE 65536/*64kb*/
#define G_BUFFER_8192_SIZE 8192	/*4096�ֽڻ�����*/
#define G_BUFFER_4096_SIZE 4096	/*4096�ֽڻ�����*/
#define G_BUFFER_2048_SIZE 2048	/*2048�ֽڻ�����*/
#define G_BUFFER_MAX_SIZE 1024	/*1024�ֽڻ�����*/
#define G_BUFFER_1024_SIZE 1024	/*1024�ֽڻ�����*/
#define G_BUFFER_512_SIZE 512	/*512�ֽڵ�����*/
#define G_BUFFER_256_SIZE 256	/*256�ֽڵ�����*/
#define G_BUFFER_128_SIZE 128	/*128�ֽڳߴ�Ļ���*/
#define G_BUFFER_64_SIZE 64		/*64�ֽڳߴ�Ļ���*/
#define G_BUFFER_32_SIZE 32		/*32�ֽڳߴ�Ļ���*/
#define G_BUFFER_16_SIZE 16		/*16�ֽڳߴ�Ļ���*/
#define G_BUFFER_8_SIZE 8		/*8�ֽڳߴ�Ļ���*/
#define G_SIGN_POINT '.'		/*����.*/
#define G_SIGN_SWAY '\\'		/*б��/ */
#define G_SIGN_XML_LEFT '<'
#define G_SIGN_XML_RIGHT '>'

#define G_SIGN_SWAY_REVERSE '/'/*	\\/		*/
#define G_SIGN_RIGHT_BRACKETS "}"
#define G_SIGN_LEFT_BRACKETS "{"
#define G_SIGN_QUOTATION "\""	/*˫����*/
#define G_SIGN_COMMA ","		/*����*/
#define G_SIGN_HALFWAY_LINE "|"	/*����*/
#define G_SIGN_NULL ""			/*���ַ���*/
#define G_SIGN_SPACE " "		/*�ո�*/
#define G_SIGN_EQUAL "="		/*�Ⱥ�*/
#define G_SIGN_BR "\n"			/*����*/
#define G_SIGN_AITA '@'
#define SORT_ASCEND 1	//��������
#define SORT_DROP   2	//��������

#define EX_SIGN_SIZE 5.0f		//��ǵĳߴ�

#define BOX_SIZE 108		//6����(12��������)��Χ�гߴ�(float����)

#define IS_BIG_ENDIAN 	1	//���
#define IS_SMALL_ENDIAN 0	//С��

//#define ANIM_STR "stand,walk,att,skill01,skill02"

#define  INF    1e9//���������
#define  EP     1e-9//��������С

#define  TRUE 1
#define FALSE 0

//===================״̬�붨��=====================

//===================�����붨��=====================

/* λͼλ������*/
//#define ERR_TEXTURE_BIT 0x03
//#define ERROR_ENGINE_NOT_INITIALIZATION 0	//����δ��ʼ��

//=================================================================================================
//����ֲ���ʽ
//#define OBJ_UV_VERTEX			0
//#define OBJ_UV_VERTEX_NORMAL	1
enum {
	//OBJ_NULL = -1,
	/*
	 * uv vertex
	 */
	OBJ_UV_VERTEX=0,
	/*
	 * uv vertex normal
	 */
	OBJ_UV_VERTEX_NORMAL=1,
};

//�������ݼ����
enum
{
	UV_GAP		=	2,		//uv�������
	NORMAL_GAP	=	3,		//normal�������
	VERTEX_GAP	=	3		//�����������
};

//=================================================================================================

//��ֵö��
#define KEY_ESC 27
#define KEY_1 49
#define KEY_2 50
#define KEY_A 97
#define KEY_D 100

//�ļ����Ͷ���
#define TYPE_OBJ_FILE 0	//obj����
#define TYPE_MD5_FILE 1 //md5����
#define TYPE_MD2_FILE 2 //md2����
#define TYPE_SPRITE_FLIE 3//UI���������е�sprite
#define TYPE_TEXT_FILE	4	//�ı�����
#define TYPE_OBJ_VBO_FILE	5//VBO�ļ�����

#define  SUFFIX_OBJ "obj"
#define  SUFFIX_MD5MESH "md5mesh"
#define  SUFFIX_MD2 "md2"


/* �ļ�����*/
#define TYPE_UNKNOW_FILE -1
#define FILE_FORMAT_STR "obj,md5mesh"
#define TYPE_ERROR_FILE -1//�޷�ʶ�������

#endif