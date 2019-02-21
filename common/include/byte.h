/*�ֽ�������*/
#ifndef _BYTE_H_
#define _BYTE_H_
#include "def.h"

#define IS_BIG_ENDIAN 	1	//���
#define IS_SMALL_ENDIAN 0	//С��

//�ֽ�����дģ��===============================================================================================

/*
	�ֽ����������� 
*/
struct BytesVO{
	char* d;										//�ֽ��������ݶ���,���ݳ�
	int   mPos;										//д���� �ֽ��α��λ�� 0Ϊ��ʼ�ĵ�һ������
	int   length;									//�ֽڵĳ���	
	int	  bigEndian;								//�Ƿ��Ǵ��ģʽ1 0
};
/*
Example:

void bs_example()
{
	struct	BytesVO bs,_b;
	int v;
	int len = 128;
	char str[32];
	memset(str,0,32);
	bs_write_start(&bs,len);
	bs_writeByte(&bs,2);
	bs_writeInt(&bs,2339);
	bs_writeShort(&bs,65535);
	bs_writeUTF(&bs,"helloworld!",11);

	bs_read_start(&_b,bs.d,len);
	bs_readByte(&_b,&v);			printf("%d\n",v);
	bs_readInt(&_b,&v);				printf("%d\n",v);
	bs_readShort(&_b,(short*)&v);	printf("%d\n",v);
	bs_readUTF(&_b,str);			printf("%s\n",str);

	bs_dispose(&bs);
	bs_dispose(&_b);
}
*/
//void bs_setbig(struct BytesVO* v,int state);/*���ô�˺�С��*/
DLLIMPORT void bs_read_start(struct BytesVO* v,char* s,int len,int isBig);/*��ʼ����ʱ�򴥷�,����һ�����ȵ��ֽ�,�����������*/
DLLIMPORT void bs_writeByte(struct BytesVO*t,char v);/*���ֽڶ�����д��һ��byte*/
DLLIMPORT void bs_readByte(struct BytesVO*t,int* dest);/*���ֽڶ����һ��byte*/
DLLIMPORT void bs_readInt(struct BytesVO*t,int* out);/*��ȡһ��int*/
DLLIMPORT void bs_readFloat(struct BytesVO*t,float* out);/*��ȡһ��float*/
DLLIMPORT void bs_readUnsignedLong(struct BytesVO* t,unsigned long* out);/*��ȡһ���޷��ŵ�long������*/
DLLIMPORT void bs_readShort(struct BytesVO*t,short* out);/*��ȡһ��short*/
DLLIMPORT void bs_readUnsignedShort(struct BytesVO* t,unsigned short *p);/*��ȡһ���޷��ŵ�short*/
DLLIMPORT void bs_readUTF(struct BytesVO*,char* dest);/*��ȡһ���ַ���*/
DLLIMPORT void bs_writeInt(struct BytesVO*t,int v);/*дһ��int*/
DLLIMPORT void bs_writeShort(struct BytesVO* t,short v);/*д��һ��short*/
DLLIMPORT void bs_writeUTF(struct BytesVO*t,char* s,int len);/*	д��һ������Ϊlen������,дһ���ַ���,ǰ4���ֽ�(int)�洢�ַ����ĳ���,�ַ����ĳ�����Ϣ�����ֽ�����ͷ��λ*/
DLLIMPORT void bs_write_start(struct BytesVO* t,int bufferSize);/*��ʼ���ֽ�������*/
DLLIMPORT void bs_writeByteByPos(struct BytesVO* t,int byte,int pos);/*��ָ��λ��дһ��Byte*/
DLLIMPORT void bs_writeIntByPos(struct BytesVO*t,int v,int pos);/*��ָ��λ��дһ��Int*/
DLLIMPORT void bs_dispose(struct BytesVO*t);/*����ֽ���������ݣ����� length �� position ��������Ϊ 0*/
#endif