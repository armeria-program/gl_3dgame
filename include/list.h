#ifndef _LIST_H_
#define _LIST_H_
/**
*	�б�ṹ��
*/
struct List
{
	float* list;
	int length;
};
//****************************************************************************//
/*	����
int main(int argc, char* argv[])
{
	struct List* a = (struct List*)tl_malloc(sizeof(struct List));
	list_create(a);
	
	list_add(a,1.2);
	list_add(a,0.5);
	
	printf("�б��� %d\n",a->length);
	
	for(int i = 0;i < a->length;i++)
	{
		printf("%f\n",a->list[i]);
	}
	list_dispose(a);
	tl_free(a);
	return 0;
}
*/

/*
*	��ʼ��һ���б�,��ʼ����Ϊ0
*/
void list_create(struct List* _l);
/*
*	Ϊ�б����һ��float����,������0λ��ʼ����
*/
void list_add(struct List* _l,float value);
/*
*	�����б�	
*/
void list_dispose(struct List* _l);
#endif
//=======================================================================