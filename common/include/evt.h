#ifndef _EVT_H_
#define _EVT_H_
#include "def.h"

/*
	ע��:�۲���ģʽ��ȱ�����ڹ۲����ڶ��ʱ��,����¼��ɷ���һ����Ⱦѭ������,��ô�����ǵ��µ�.����ڴ�������Ƶ�ʲ���,Ϊ�˽�����϶������ַ�ʽ����ѡ.
*/

//���¼�
DLLIMPORT void
evt_on(void* ptr,int id,void (*evtCallBack)(int,void*));

//����¼�
DLLIMPORT void
evt_off(void* ptr,int id,void (*evtCallBack)(int,void*));

//�ɷ��¼�
DLLIMPORT void
evt_dispatch(void* ptr,int evtID,void* sendData);

//�����¼����
DLLIMPORT void 
evt_dispose(void* ptr);
#endif