#ifndef _INPUT_H_
#define _INPUT_H_
/*
 *input����ǻ���ftext���������չ,��Ⱦ�ڵ��һ��Sprite������Ⱦ,����Ƚ���.
 *���Ҫ���ñ���,���Ի���input���������չ,input��������������ֱ�������Ⱦ
 *input����ֻ��Ⱦ�ı��ʹ�����̽���
 */

/*
 *��ʼ���������,����һ�����Ϊwidth���������
 */
void* input_create(int width);
/*
 *����input���
 */
void input_dispose(void* p);
/*
 *��������
 */
void intput_set_pos(void *p,int x,int y);

/*������ͼ��,δ���õ�ʱ��,������һ��Ĭ�ϵ���ͼ��Ϊ�����ı���*/
//void input_set_bg(void* p,const char* url);

/*��ȡ������еĵ�ǰ�ı�����,��ǰ���ַ��������洢�ڶ��ڴ���*/
char* input_get_heap_str(void* p);

//��ȡ����������
void* input_get_container(void* p);
//��ȡftext���
void* input_get_ftext(void* p);
//�������
void input_clear(void* p);
#endif