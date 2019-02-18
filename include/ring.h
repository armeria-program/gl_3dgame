#ifndef _RING_H_
#define _RING_H_
//������
void* ring_create();
//ѹ������
void ring_push(void* ptr,void* node);
//��һ������
void* ring_next(void* ptr);
//ǰһ������
void* ring_pre(void* ptr);
//���ٻ�
void ring_dispose(void* ptr);
//��ȡ��ǰ�Ļ�ѡ�������
void* ring_cur(void* ptr);
#endif