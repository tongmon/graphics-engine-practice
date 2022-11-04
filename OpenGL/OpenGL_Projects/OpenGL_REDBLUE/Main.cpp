#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"winmm.lib")
#pragma warning(disable:4996)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int State = 1, is_start = 0, is_Pause = 0;

void Mydisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (State == 1) glColor3f(1.0, 0.0, 0.0);
    else glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-1, -1, 0.0);
    glVertex3f(0, -1, 0.0);
    glVertex3f(0, 1, 0.0);
    glVertex3f(-1, 1, 0.0);
    glEnd();
    if (State == 1) glColor3f(0.0, 0.0, 1.0);
    else glColor3f(1, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(0, -1, 0.0);
    glVertex3f(1, -1, 0.0);
    glVertex3f(1, 1, 0.0);
    glVertex3f(0, 1, 0.0);
    glEnd();
    glFlush();
}

void MyMainMenu(int id)
{
    if (id == 1) is_start = 1;
    if (id == 2) is_Pause = 1;
    if (id == 3) is_Pause = 0;
    if (id == 4) exit(0);
}

void MyTimer(int value)
{
    State = State == 1 ? 0 : 1;
    if (is_start == 1 && is_Pause == 0) glutPostRedisplay();
    glutTimerFunc(500, MyTimer, 1);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("201663021 이경준");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
    glutAddMenuEntry("Start", 1); // 색상 서브 메뉴 추가
    glutAddMenuEntry("Stop", 2); // 크기 서브 메뉴 추가
    glutAddMenuEntry("Continue", 3); // 모양 서브 메뉴 추가
    glutAddMenuEntry("Exit", 4); // 모양 서브 메뉴 추가

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(Mydisplay);
    glutTimerFunc(500, MyTimer, 1);

    glutMainLoop();
    return 0;
}