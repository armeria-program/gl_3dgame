#ifndef _GET_TIME_H_
#define _GET_TIME_H_

#define	ERROR_SUIFFIX  1//����ĺ�׺
#define	ERROR_BAD_COLOR_VALUE  2//�������ɫֵ
#define	ERROR_NOT_EXIST_FILE  3	//�޷��ҵ��ļ�
#define	ERROR_NOT_EXIST_Atals 4	//�޷��ҵ�ͼ��
#define	ERROR_PTR_REMOVE_FAIL 5	//ptr_removeɾ������ʧ��
#define	ERROR_PTR_TYPE_FAIL 6	//����ƥ�����,δʵ�ָ����͵��߼�
#define ERROR_BAD_VALUE 7	//����ֵ
#define	ERROR_FLAGS 8	//����ı�ʾ״̬λ
#define ERROR_GET_FRAME 9 //��ȡ����֡
#define ERROR_RENAME 10		//������
#define ERROR_SET_FRAME 11 //���ö���֡
/*
 *	��ȡ��ǰʱ�� ��ȷ������
 *	clock�����������ʱ��Ϊ��������ʱ�䣬��λΪ�δ���(ticks)��
 */
long get_longTime();

/*���ɵ�ǰ��ʱ��2015/1/2 0:20:1*/
void get_allTime(char* out,int bufferSize);
/*�����־ ,д�뱾�ػ����ϴ���web������*/
void log_write(const char* _str);
/*
	��ȡ��ǰ���е�ʱ��
*/
float gettime_cur();

/************************************************************************
* ���һ����ɫ�ʵ���־             
* unsigned short color ��ɫ
/************************************************************************/
void
log_put(const char* s,int color);
/************************************************************************/
/* ����������                                                                     */
/************************************************************************/
void 
log_code(int c);

/*
*	���ָ��color��ɫ����־
*/
int 
log_color(int color,char* format, ...);
#endif