#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <stdlib.h>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

GLfloat Red = 0.5, Green = 0.5, Blue = 0.5; // 색상, 처음에 회색
GLfloat Dy = 0.0, Dx = 0.0; // X, Y 증가량

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
    case 27: //'esc' 키의 아스키 코드 값
        exit(0); break;
    case 'r': // 빨간색
        Red = 1.0, Green = 0.0, Blue = 0.0; break;
    case 'g': // 초록색
        Red = 0.0, Green = 1.0, Blue = 0.0; break;
    case 'b': // 파란색
        Red = 0.0, Green = 0.0, Blue = 1.0; break;
    case 'i': // 위
        Dy = Dy + 0.01; break;
    case 'm': // 아래
        Dy = Dy - 0.01; break;
    case 'j': // 좌
        Dx = Dx - 0.01; break;
    case 'k': // 우
        Dx = Dx + 0.01; break;
    case 's': // 정중앙
        Dy = 0.0, Dx = 0.0; break;
    }
    glutPostRedisplay(); // 다시 디스플레이해달라 라는 명령을 이벤트 루프에 넣음
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