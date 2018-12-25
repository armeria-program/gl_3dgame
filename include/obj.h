#ifndef _OBJ_H_
#define _OBJ_H_

#define _USE_SIGNED_SHORT_

#ifdef _USE_SIGNED_SHORT_
	typedef  unsigned short _UNSIGNED_SHORT;//��0~65535 //2�ֽ�
#else
	typedef  unsigned int _UNSIGNED_SHORT;	//0~65535	//4�ֽ�
#endif

//typedef  int _UNSIGNED_SHORT;
struct ExportOBJ_Data
{
	float *ptrVertex;
	float *ptrUV;
	float* ptrNormal;
	int vertexCount;		/*������������*/

	//����
	_UNSIGNED_SHORT *ptrIndex;
	int indexLength;

	int renderVertCount;//��Ҫ��Ⱦ�Ķ������

	
};
/*
	obj�ļ����ز���ʼ���� ,len:����ĳ���

	int* handle:struct ObjParseVOType*=====>��ΪObjParseVOType��������
	
	ע:	int* outArray ��һ��tl_malloc�������ڴ��,������֮��ǵ��ͷ�
	������������:��ȷ��ʹ�÷�ʽ
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//����һ���ı�����Ƭ
	static void loadquad()
	{
		int vertexCount = 0;//�������

		int _len=0;
		int verts = 0,obj = 0;
		char* _objStr;

		printf("�ڴ��:%d\n",tl_memByteSize());

		_objStr=tl_loadfile("..\\resource\\obj\\quad.obj",NULL);
		
		obj_parse(_objStr,&_len,&verts,&obj);
		
		vertexCount = _len / 8;
		
		//�ͷ��ַ�����
		tl_free(_objStr);

		//�ͷŶ�������
		tl_free(verts);

		printf("�ڴ��:%d\n",tl_memByteSize());
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	char* ������ַ���
	int*  outlen:	���ݸ�������
	int*  outArray	��������
	int exportType	����������
*/
void obj_parse(const char* data,int* outlen,int* ptrOutArray,int exportType);
/*
	��data�ַ���������һ��struct OBJ_Data�ṹ��
*/
struct ExportOBJ_Data* obj_getData(const char* data);
/*
	����
*/
void obj_delData(struct ExportOBJ_Data* ptr);

#endif