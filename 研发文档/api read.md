### glutSwapBuffers,glFlush
�򵥵�˵glFlush����ǿ��ˢ�°ɣ�Ҫ֪��OPENGL��ʹ��һ����Ⱦ�������Դ�������ģ�һ������£������ύ��OPENGL��ָ����������͵�����������ִ�еģ����Ƿŵ�һ�����������棬�����������������һ�ι���������������ִ�У��ܶ�ʱ��ֻ�м���ָ������䲻���Ǹ��������ģ������˵��Щָ�����û�б����͵��������������Ҫ����glFlush��ǿ�ư���Щָ���͵���������д���
glutSwapBuffers��ʵ���ǽ���������������������ĳ���ʹ�õ���˫���弼���ɣ��������ּ����Ļ��������еĻ��ƶ��ǻ��Ƶ�һ����̨�Ļ���������ģ�������������������Ϳ��������ƵĶ����ˣ�����ô����~
```
glutPostRedisplay
//��ʾ
glutDisplayFunc(void (*func)(void));����

//�ı䴰�ڴ�Сʱ����
glutReshapeFunc(void (*func)(void));

//�����¼���keyָ���µļ���x��y��ʾ����key��ʱ����λ��
glutKeyboardFunc(void (*func)(unsigned char key,int x,int y));����

//������¼�,button��ʾ����ĳ����,stateָ��ǰ����״̬(���»��ͷ�),x��y��ʾ���λ��
glutMouseFunc(void (*func)(int button,int state,int x,int y));����

//��������ƶ�ʱ����
glutMotionFunc(void (*func)(int x,int y));��

//����ƶ�ʱ���á�
glutPassiveMotionFunc(void (*func)(int x,int y));����

//����ʱ����
glutIdleFunc(void (*func)(void));����
```
### FBO,RBO
FBO:Frame Buffer Object
RBO:Render Buffer Object
һ��FBO���԰󶨶��RBO,��Ⱦ������������Ӧ�ó�������2Dͼ�񻺳�����
��Ⱦ�������������洢��ɫ����Ȼ�ģ��ֵ������Ϊ��ɫ����Ȼ�ģ�������һ��֡����������
��Ⱦ�������Ͳ���ϵͳ�ṩ�������Ļ��ƴ���pbuffer �кܶ����Ƶĵط�����Ⱦ����������ֱ�ӱ�GL ��Ϊ����ʹ�á�
FBO��һ����ɫ����Ⱥ�ģ���������ȾĿ��ļ��ϡ�����2D ͼ���ܱ�������֡�������������ɫ���ϡ�
