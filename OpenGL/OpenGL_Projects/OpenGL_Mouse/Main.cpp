#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <stdlib.h>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

#define W_Width 900.0
#define W_Height 900.0

typedef struct _rect
{
    GLfloat TLX, TLY, BRX, BRY;
}Rect; // �簢�� ����ü

Rect* Ary; int Size = 10, index = 0; // �簢�� ��� �迭
GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY; // �簢�� ��ǥ

void MyDisplay() 
{
    glViewport(0, 0, W_Width, W_Height);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5, 0.5, 0.5);

    for (int i = 0; i < index; i++) // �� �簢�� �������� �ʰ� �ٽ� ���
    {
        glBegin(GL_POLYGON);
        glVertex3f(Ary[i].TLX / W_Width, (W_Height - Ary[i].TLY) / W_Height, 0.0);
        glVertex3f(Ary[i].TLX / W_Width, (W_Height - Ary[i].BRY) / W_Height, 0.0);
        glVertex3f(Ary[i].BRX / W_Width, (W_Height - Ary[i].BRY) / W_Height, 0.0);
        glVertex3f(Ary[i].BRX / W_Width, (W_Height - Ary[i].TLY) / W_Height, 0.0);
        glEnd();
    }

    glBegin(GL_POLYGON);
    glVertex3f(TopLeftX / W_Width, (W_Height - TopLeftY) / W_Height, 0.0);
    glVertex3f(TopLeftX / W_Width, (W_Height - BottomRightY) / W_Height, 0.0);
    glVertex3f(BottomRightX / W_Width, (W_Height - BottomRightY) / W_Height, 0.0);
    glVertex3f(BottomRightX / W_Width, (W_Height - TopLeftY) / W_Height, 0.0);
    glEnd();

    glFlush();
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) // ���콺�� ������ ���� ��ǥ�� ����, ���� (0, 0)�̴�.
{
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) 
    {
        TopLeftX = X;
        TopLeftY = Y;
    }
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_UP)
    {
        if (index >= Size)
        {
            Size = Size * 2;
            Ary = (Rect*)realloc(Ary, sizeof(Rect) * Size); // ���� �Ҵ� �뷮�� �� á���� 2��� �������� �ٽ� �Ҵ�
        }
        if (Ary != NULL)
        {
            Ary[index].BRX = BottomRightX; Ary[index].BRY = BottomRightY;
            Ary[index].TLX = TopLeftX; Ary[index].TLY = TopLeftY;
            index++;
        }
        glutPostRedisplay();
    }
}

void MyMouseMove(GLint X, GLint Y)
{
    BottomRightX = X;
    BottomRightY = Y;
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    Ary = (Rect*)malloc(sizeof(Rect) * Size); // �簢�� �迭 �ʱ� ���� �Ҵ�
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(W_Width, W_Height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("201663021 �̰���");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0); // ��ǥ�� ����

    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);

    glutMainLoop();

    free(Ary); // �Ҵ� ����
    return 0;
}