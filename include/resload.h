#ifndef _RES_LOAD_H_
#define _RES_LOAD_H_
//����һ�����ض���
void* resload_create(void(*endCallBack)(void*,void*),void* alals,void* param);
//ѹ��һ����Դ�ڵ�
void resload_push(void* ptr,const char* tex);
//��ʼ�Ǽ�����Դ
void resload_start(void* ptr);
#endif