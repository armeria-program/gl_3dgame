//
//  main.cpp
//  OpenGL_05_VertexArray
//
//  Created by apple on 14/12/30.
//  Copyright (c) 2014�� cc. All rights reserved.
//

#include <GL/glew.h> 
#include <gl/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

#define POINTER 1           //ʹ�÷ֱ�ָ���������ɫ�������鷽ʽ
#define INTERLEAVED 2       //ʹ�û��ָ���������ɫ�������鷽ʽ


#define DRAWARRAY 1         //ʹ��DrawArray��ʽ
#define ARRAYELEMENT  2     //ʹ��ArrayElement��ʽ
#define DRAWELEMENTS 3      //ʹ��DrawElement��ʽ

//��ǰ���õ�ģʽ
int setupMethod = POINTER;
int derefMethod = DRAWARRAY;

/**
 *  ���ö�����������Ͷ�����ɫ����(�ֱ�ָ��)
 */
void setupPointers() {
    
    //������������
    static GLint vertices[] = {25, 25,
        100, 325,
        175, 25,
        175, 325,
        250, 25,
        325, 325
    };
    
    //������ɫRBG����
    static GLfloat colors[] = {1.0, 0.2, 0.2,
        0.2, 0.2, 1.0,
        0.8, 1.0, 0.2,
        0.75, 0.75, 0.75,
        0.35, 0.35, 0.35,
        0.5, 0.5, 0.5
    };
    
    //���ö�����������
    glEnableClientState(GL_VERTEX_ARRAY);
    //���ö�����ɫ����
    glEnableClientState(GL_COLOR_ARRAY);
    
    //ָ��������������
    glVertexPointer(2, GL_INT, 0, vertices);
    //ָ��������ɫ����
    glColorPointer(3, GL_FLOAT, 0, colors);
    
}

/**
 *  ���ö�������Ͷ�����ɫ����(�������)
 */
void setupInterleave() {
    
    /**
     *  intertwined[0~2]* n Ϊ��������
     *  intertwined[3~5]* n Ϊ������ɫ
     */
    static GLfloat intertwined[] = {
        1.0, 0.2, 1.0, 100.0, 100.0, 0.0,
        1.0, 0.2, 0.2, 0.0, 200.0, 0.0,
        1.0, 1.0, 0.2, 100.0, 300.0, 0.0,
        0.2, 1.0, 0.2, 200.0, 300.0, 0.0,
        0.2, 1.0, 1.0, 300.0, 200.0, 0.0,
        0.2, 0.2, 1.0, 200.0, 100.0, 0.0
    };
    
    //ָ����϶�������
    glInterleavedArrays (GL_C3F_V3F, 0, intertwined);
}

/**
 *  ��ʼ������
 */
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //������ɫģʽ
    //GL_SMOOTH �ƶ���������ɫ���в�ֵ������֮���������
    //����������ɫ��ͬ��ʹ����������Ч����ͬ
    //���������ɫ��ͬ������ֹ���Ч��
    glShadeModel(GL_SMOOTH);
    //���ö������飬�ֱ�ָ����ɫ������
    setupPointers();
}

/**
 *  չʾ����Ч��
 */
void display(void) {
    
    //������ɫ������
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (derefMethod == DRAWARRAY) {
        //�Ӷ��������еĵ�һ�����㿪ʼ����ȡ6�����㣬����������
        glDrawArrays(GL_TRIANGLES, 0, 6);
    } else if (derefMethod == ARRAYELEMENT) {
        //ȡ�������������е�����3������л���������
        glBegin(GL_TRIANGLES);
        glArrayElement(2);
        glArrayElement(3);
        glArrayElement(5);
        glEnd();
    } else if (derefMethod == DRAWELEMENTS) {
        //ָ����Ҫ���ƵĶ�������������ƶ����
        GLuint indices[4] = {0, 1, 3, 4};
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, indices);
    }
    
    glFlush ();
    
}

/**
 *  �������ڳߴ�
 *
 *  @param width  ���
 *  @param height �߶�
 */
void reshape(int width, int height) {
    //�����ӿھ���������Ĭ������£��ӿڱ�����Ϊռ�ݴ򿪴��ڵ��������ؾ���
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    //��ͶӰ����Ӧ�����ľ������
    glMatrixMode(GL_PROJECTION);
    //�����ǽ�֮ǰ����任���±仯����ջ���������¹�λ����Ϊ��λ���󣡵�����֮ǰ�ľ���任������Ӱ�쵽��Ϊֹ�ˣ�
    glLoadIdentity();
    //ָ��2D�ü�����ϵ��naer��farʹ��Ĭ��ֵ-1��1
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
}

/**
 *  �����¼��ص�
 *
 *  @param key ��λ
 *  @param x   x����
 *  @param y   y����
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
            //ESC
        case 27:
            exit(0);
            break;
    }
}

/**
 *  ����¼��ص�
 *
 *  @param button ��ť����
 *  @param state  ���״̬
 *  @param x      x����
 *  @param y      y����
 */
void mouse (int button, int state, int x, int y) {
    switch (button) {
        //���
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                //��������(ʹ����ɫ�������������鷽ʽ) �� (ʹ�û������ķ�ʽ) �л�
                if (setupMethod == POINTER) {
                    setupMethod = INTERLEAVED;
                    setupInterleave();
                } else if (setupMethod == INTERLEAVED) {
                    setupMethod = POINTER;
                    setupPointers();
                }
                //�ػ�
                glutPostRedisplay();
            }
            break;
        //�Ҽ�
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                if (derefMethod == DRAWARRAY) {
                    derefMethod = ARRAYELEMENT;
                } else if (derefMethod == ARRAYELEMENT) {
                    derefMethod = DRAWELEMENTS;
                } else if (derefMethod == DRAWELEMENTS) {
                    derefMethod = DRAWARRAY;
                }
                //�ػ�
                glutPostRedisplay();
            }
            break;
        default:
            break;
    }
}

int main(int argc, const char * argv[]) {
    
    //��ʼ��GLUT��
    glutInit(&argc, (char**)argv);
    //���õ����壬RGB���ظ�ʽ�Ĵ���
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //���ô��ڴ�С
    glutInitWindowSize(350, 350);
    //���ô�������
    glutInitWindowPosition (100, 100);
    //��������
    glutCreateWindow("VertexArray");
    
    //��ʼ������
    init();
    
    //����չʾ�Ļص�����
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    //�����߳̿�ʼѭ��
    glutMainLoop();
    
    return 0;
}