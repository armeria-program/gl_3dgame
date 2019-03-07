#ifndef _DEF_H_
#define _DEF_H_
//#define DEBUG//����debugģʽ

#ifdef BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

//�����겿��

//*****************************************************************************
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

#define  INF    1e9//���������
#define  EP     1e-9//��������С

#define  TRUE 1
#define FALSE 0
//*****************************************************************************



#endif