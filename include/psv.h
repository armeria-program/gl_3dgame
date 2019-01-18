
//=============================================================================
#ifndef _PCSV_H_
#define _PCSV_H_
struct PCsvVO{
	/*
	������list
	*/
	struct LStackNode* list;

	/*
	�ؼ�������
	*/
	int keyIndex;

	/*
	��������,�����ͷ�
	*/
	struct Array* keylist;
};

//��������
/************************************

struct PCsvVO * pcsv;
int out;
Array* arr;
tl_memState(TRUE);
printf("===========>%d bytes\n",tl_memByteSize());
pcsv = pcsv_load("D:\\source\\resource\\CaiLiaoHeCheng.csv","itemid",1);

arr=pcsv_getValue(pcsv,"qianghua10");
array_get(arr,1,&out);
if(out){
	printf("%s\n",(char*)out);
}
pcsv_dispose(pcsv);
printf("===========>%d bytes\n",tl_memByteSize());

***************************************/

/*
*	��������
*	const char* path	:���ص�·��
*	const char* key		:����
*	int checkIndex		:�õڼ�����Ϊkey���б�,��0��ʼ(csv���� ֻ��һ����Ϊ��������.����ǵ�1����Ϊ����,checkIndex=0)
*/
struct PCsvVO* pcsv_load(const char* path,const char* mainkey,int checkIndex);
/*
����PCsvVO����
*/
void pcsv_dispose(struct PCsvVO* p);

/*
����keyValueƥ��������ֵ��ȡ��������
ע��:�����Array����Ҫ��array_free�ͷ�,��Ϊ��pcsv_dispose���ͷ�
*/
struct Array* pcsv_getValue(struct PCsvVO* p,const char* keyValue);

//========================================================================
#endif