#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <stdlib.h>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

GLfloat Red = 0.5, Green = 0.5, Blue = 0.5; // ����, ó���� ȸ��
GLfloat Dy = 0.0, Dx = 0.0; // X, Y ������

void MyDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(Red, Green, Blue);

    glBegin(GL_POLYGON);
    glVertex3f(-0.5 + Dx, -0.5 + Dy, 0.0);
    glVertex3f(0.5 + Dx, -0.5 + Dy, 0.0);
    glVertex3f(0.5 + Dx, 0.5 + Dy, 0.0);
    glVertex3f(-0.5 + Dx, 0.5 + Dy, 0.0);
    glEnd();

    glFlush();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y)
{
    switch (KeyPressed)
    {
    case 'q':
        exit(0); break;
    case 27: //'esc' Ű�� �ƽ�Ű �ڵ� ��
        exit(0); break;
    case 'r': // ������
        Red = 1.0, Green = 0.0, Blue = 0.0; break;
    case 'g': // �ʷϻ�
        Red = 0.0, Green = 1.0, Blue = 0.0; break;
    case 'b': // �Ķ���
        Red = 0.0, Green = 0.0, Blue = 1.0; break;
    case 'i': // ��
        Dy = Dy + 0.01; break;
    case 'm': // �Ʒ�
        Dy = Dy - 0.01; break;
    case 'j': // ��
        Dx = Dx - 0.01; break;
    case 'k': // ��
        Dx = Dx + 0.01; break;
    case 's': // ���߾�
        Dy = 0.0, Dx = 0.0; break;
    }
    glutPostRedisplay(); // �ٽ� ���÷����ش޶� ��� ����� �̺�Ʈ ������ ����
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("OpenGL Drawing Example");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);

    glutMainLoop();
    return 0;
}