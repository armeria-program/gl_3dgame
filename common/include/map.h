#ifndef _MAP_H_
#define _MAP_H_

struct MapNode{
	char* key;
	void* value;
};
//������ֵ����
void*
map_create();

//���ݼ�ֵ��ȡ����
void*
map_get(void* map,const char* key);
//ɾ��һ����ֵ
void
map_del_key(void*map,const char* key);
//����һ����ֵ����ֵ
void*
map_set(void* map,const char* key,void* value);
//����mapֵ����
void
map_dispose(void* map);

//���ݼ�ֵ�����е�ֵ��ȡ��ֵ����
void*
map_getNodeByValue(void* mapptr,void* value);

//��ӡ���map�Ľڵ��б���Ϣ
void
map_info(void* mapptr);

#endif