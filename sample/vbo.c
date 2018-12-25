//ʹ��VBO��������������
#include <GL/glew.h> 
#include <gl/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

void userInit();  //�Զ����ʼ��
void reshape(int w, int h);   //�ػ�
void display(void);
void keyboardAction(unsigned char key, int x, int y);   //�����˳��¼�

GLuint vboId=0;//vertex buffer object���    
//GLuint vaoId;//vertext array object���    
GLuint programId;//shader program ���    

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(100, 100);
	glutCreateWindow("Rectangle demo");

	//ʹ��glew����Ҫִ��glewInit����Ȼ���й��̻ᱨ��
	//glewInitҪ����glut����˻����ĳ�ʼ��֮��ִ��
	glewInit();

	//�Զ����ʼ��������VAO��VBO����
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
	//������������    
	const GLfloat vertices[] = {
		/*-0.5f,-0.5f,0.0f,1.0f,
		0.5f,-0.5f,0.0f,1.0f,
		0.5f,0.5f,0.0f,1.0f,
		-0.5f,0.5f,0.0f,1.0f,*/

		
	
		/*����������*/
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		
		0.0f,1.0f,0.0f,
		0.75f,0.75f,0.0f,
		1.0f,0.0f,0.0f,
	};
	printf("������aɾ��VBO����\n");

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//����VAO����
	/*glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);*/

	printf("δ����֮ǰvboId= %d\n",vboId);
	//����VBO����	
	glGenBuffers(1, &vboId);
	printf("������֮��vboId= %d\n",vboId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//����VBO����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���VBO��
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
	glClear(GL_COLOR_BUFFER_BIT);

	//glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

	//��VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);//���Ͷ������ݷ�ʽ
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);//����ģ��
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);//���Ͷ������ݷ�ʽ
	glDrawArrays(GL_TRIANGLES, 0, 6);//����6������

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
	glutPostRedisplay ();
}

//���̰����ص�����    
void keyboardAction(unsigned char key, int x, int y)
{
	int kv = (int)key;
	
	switch (key)
	{
		case 033:  // Escape key    
			exit(0);
			break;
	}
	if(kv==97){
		//��a�ͷ�VBO,ɾ��ģ������
		//printf("glDeleteBuffers %d\n",vboId);
		glDeleteBuffers(1,&vboId);
		//printf("glDeleteBuffers %d\n",vboId);
	}
}