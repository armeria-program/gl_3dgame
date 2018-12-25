//http://blog.csdn.net/dcrmg/article/details/53556664
void userInit();  //�Զ����ʼ��  
void reshape(int w, int h);   //�ػ�  
void display(void);  
void keyboardAction(unsigned char key, int x, int y);   //�����˳��¼�  

GLuint vboId;//vertex buffer object���      
//GLuint vaoId;//vertext array object���      
GLuint programId;//shader program ���      

int t1(int argc, char **argv)  
{  
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);  
	glutInitWindowPosition(100, 100);  
	glutInitWindowSize(64, 64);  
	glutCreateWindow("Rectangle demo");  

	//ʹ��glew����Ҫִ��glewInit����Ȼ���й��̻ᱨ��  
	//glewInitҪ����glut����˻����ĳ�ʼ��֮��ִ��  
	glewInit();  
	//glutInit(&argc,argv);


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
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	//������������      
	const GLfloat vertices[] = {  
		-0.5f,-0.5f,0.0f,1.0f,  
		0.5f,-0.5f,0.0f,1.0f,  
		0.5f,0.5f,0.0f,1.0f,  
		-0.5f,0.5f,0.0f,1.0f,  
	};  

	////����VAO����  
	//glGenVertexArrays(1, &vaoId);  
	//glBindVertexArray(vaoId);  

	//����VBO����     
	glGenBuffers(1, &vboId);  
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

	//��VBO  
	glBindBuffer(GL_ARRAY_BUFFER, vboId);  
	glEnableVertexAttribArray(0);  

	//���Ͷ������ݷ�ʽ  
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);  

	//����ģ��  
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);  

	glBindBuffer(GL_ARRAY_BUFFER, 0);  
	glDisableVertexAttribArray(0);  

	glutSwapBuffers();  
}  

//���̰����ص�����      
void keyboardAction(unsigned char key, int x, int y)  
{  
	switch (key)  
	{  
	case 033:  // Escape key      
		// exit(0);  
		printf("exit!\n");
		break;
	case 97://a
		//ɾ��VBO����
		glDeleteBuffers(1,&vboId);
		break;
	}  
}  