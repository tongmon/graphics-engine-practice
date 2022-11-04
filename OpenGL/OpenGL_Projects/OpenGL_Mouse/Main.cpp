#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <stdlib.h>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

#define W_Width 900.0
#define W_Height 900.0

typedef struct _rect
{
    GLfloat TLX, TLY, BRX, BRY;
}Rect; // 사각형 구조체

Rect* Ary; int Size = 10, index = 0; // 사각형 출력 배열
GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY; // 사각형 좌표

void MyDisplay() 
{
    glViewport(0, 0, W_Width, W_Height);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5, 0.5, 0.5);

    for (int i = 0; i < index; i++) // 전 사각형 지워지지 않게 다시 출력
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

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) // 마우스는 윈도우 기준 좌표라 왼쪽, 위가 (0, 0)이다.
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
            Ary = (Rect*)realloc(Ary, sizeof(Rect) * Size); // 동적 할당 용량이 꽉 찼으면 2배로 증가시켜 다시 할당
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
    Ary = (Rect*)malloc(sizeof(Rect) * Size); // 사각형 배열 초기 동적 할당
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(W_Width, W_Height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("201663021 이경준");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0); // 좌표계 설정

    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);

    glutMainLoop();

    free(Ary); // 할당 해제
    return 0;
}