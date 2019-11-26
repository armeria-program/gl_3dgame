#ifndef _FTEXT_H_
#define _FTEXT_H_

/*
 *����һ���ı�
 *	txtName:ftext�����е�sprite�����Ψһ��ʶ
 */
void*
ftext_create(char* txtName,int texw,int texh,int fw,int fh,void* cam);
/************************************************************************/
/* ��ָ����������һ���ַ�����(һ�����ֻ�����һ��Ӣ��)
 * outpw,outph����ַ������ؿ��
/************************************************************************/
void 
ftext_set(void* p,char* s,int x,int y,int* outpw,int* outph);

/*
 *	�����ı�����Ļ����
 */
void 
ftext_setpos(void* p,int x,int y);

/*
 *  ��������
 *	��str�ַ���ת������ͼ����չʾ����,�ýӿڻ��������Ӻ������ı���Ⱦ
 *	outw,outh��ʾ��ǰftext���ı���Ⱦ����Ŀ��
 */
void
ftext_parse(void* p,const char* str,int *outw,int *outh);
/************************************************************************/
/* ���������е��ı�����                                                                   */
/************************************************************************/
void
ftext_clear(void* p);
/************************************************************************/
/* ��ʾ����                                                                     */
/************************************************************************/
void
ftext_vis(void* p,int vis);
//��ȡ�ı�������ı�����
char* 
ftext_get_str(void* p);
//��ȡ�ı�����Ŀ��
void
ftext_get_size(void* p,int* w,int *h);
//��ȡ��ǰ�ı�������
void
ftext_get_wordpos(void* p,int* w,int *h);
/************************************************************************/
/* ����������                                                                     */
/************************************************************************/
void
ftext_dispose(void* p);
/************************************************************************/
/* �����ı���������뻺������С,���ı�������Ҫ����Ļ�������ʱ����Ե��øĽӿ��޸�                              
/************************************************************************/
void
ftext_set_buffer(void* p,int bufferSize);
/*
 *��ȡ�ı��������ʾ����(Sprite)
 */
void* 
ftext_get_container(void* p);
//�����ַ��������洢����
void
ftext_open_wordlist(void* p);
//����һ���ַ�,��ɾ��һ���ַ�
void
ftext_pop_word(void* p);
//�����ı��Ƿ���
void 
ftext_set_wordWrap(void* p,int v);

//�����ı��ĵ������ͻص�����
void
ftext_set_hit(void* p,
			  void (*clickCallBack)(void* ,int ,int ),
			  int x,int y,int w,int h);

#endif