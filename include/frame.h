#ifndef _FRAME_H_
#define _FRAME_H_
/*
*	��������ṹ��
*	�����л��ؼ�֡,���Ŷ���
*/
typedef struct FrameAnim
{
	/*
	 *	��ǰ��������ʼ�ؼ�֡�ͽ����ؼ�֡������(��0��ʼ)
	 */
	int frameStart;
	/*
	 *	����֡����,��������0��ʼ
	 */
	int frameEnd;
	
	/*
	 * ÿһ֡�ļ��ʱ��
	 * fps = 60;
	 * fpsInterval = 1000 / fps = 16.6666;
	 */
	long fpsInterval;

	/*
	 * ��ǰ�Ĳ�ֵ
	 */
	long _subTick;

	/*
	*	��ǰ�ؼ�֡������(��0��ʼ)
	*/
	int curFrame;

	
	/*
	*��ǰ����
	*/
	char curAnim[G_BUFFER_16_SIZE];
	/*
	*ǰһ�ζ���
	*/
	char oldAnim[G_BUFFER_16_SIZE];
	
	/*
	*	����Ƭ������
	*	"stand,0,39|run,40,45|"
	*/
	char animConfig[G_BUFFER_128_SIZE];

	/*
	*	����һ�ζ����ص�
	*/
	//CallBackFun playOnceCallBack;
	void (*playOnce)(struct FrameAnim*);
	

}FrameAnim;
/*
*	����ؼ�֡
*/
void frame_run(struct FrameAnim* frameAnim);
/*
*	�ýӿ�ֻ����һ�ζ���,������һ��ѭ����,�ָ���һ������
*/
void frame_play(struct FrameAnim* frameAnim,const char* anim);
/*
*	���ö���
*/
void frame_set(struct FrameAnim* frameAnim,const char* anim);

#endif