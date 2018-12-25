//ʹ��EBO���ƾ���(����������)
#include <gl/glew.h>
#include <gl/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

#include "tools.h"

void userInit();  //�Զ����ʼ��
void reshape(int w, int h);   //�ػ�
void display(void);
void keyboardAction(unsigned char key, int x, int y);   //�����˳��¼�

GLuint eboId;//element buffer object���    
GLuint vboId;//vertext buffer object���    
GLuint vaoId;//vertext array object���    


//������������    
const GLfloat vertices[] = {
	-0.5f,-0.5f,0.0f,1.0f,
	0.5f,-0.5f,0.0f,1.0f,
	0.5f,0.5f,0.0f,1.0f,
	-0.5f,0.5f,0.0f,1.0f,
};
// ��������
GLshort indices[] = {
	0, 1, 3,  // ��һ��������
	1, 2, 3   // �ڶ���������
};

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Rectangle demo");

	//ʹ��glew����Ҫִ��glewInit����Ȼ���й��̻ᱨ��
	//glewInitҪ����glut����˻����ĳ�ʼ��֮��ִ��
	glewInit();

	//�Զ����ʼ��������VAO��VBO,EBO
	userInit();

	//�ػ溯��
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	//ע����̰����˳��¼�
	glutKeyboardFunc(keyboardAction);
	glutMainLoop();
	return 0;
}

//�Զ����ʼ������    
void userInit()
{
	//printf("sizeof(indices) = %d,%d\n",sizeof(indices),sizeof(GLshort));

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//����VAO����
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	//����VBO���󣬰Ѷ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//����EBO����	
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	//����EBO����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���Ͷ������ݷ�ʽ
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//���VAO
	glBindVertexArray(0);
	//���EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//�������ڴ�С�ص�����    
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

//���ƻص�����    
void display(void)
{
	int	indices_num;
	
	indices_num = sizeof(indices)/sizeof(GLshort);//��������

	glClear(GL_COLOR_BUFFER_BIT);
	//��VAO
	glBindVertexArray(vaoId);
	//����ģ��
	glDrawElements(GL_TRIANGLES, indices_num, GL_UNSIGNED_SHORT, NULL);
	glutSwapBuffers();
}

//���̰����ص�����    
void keyboardAction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033:  // Escape key    
		exit(0);
		break;
	}
}