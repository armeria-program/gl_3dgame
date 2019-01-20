#ifndef _ARR_
#define _ARR_

/*
	���鴴��
*/
struct Array* array_create(int length,int mode);

/*
	�ͷ�����
*/
void array_free(struct Array* ptr);
/*
	�洢���ݾ�
*/
void array_put(struct Array* ptr,int d);
/*
	���鳤��
*/
int array_len(struct Array* ptr);
/*
	��ȡ�����ҵ����ݷ���1,û�����ݾ�0
*/
int array_get(struct Array* ptr,int index,int* outData);
/*
	��������
*/
int array_set(struct Array* p,int index,int data);
/*
	��ӡ��������
*/
void array_log(struct Array* p);

/*
	����key�и�ɶ���ַ��ļ�,������������,�ַ����и���ַ�������
*/
struct Array* array_split(const char* str,char key);

#endif