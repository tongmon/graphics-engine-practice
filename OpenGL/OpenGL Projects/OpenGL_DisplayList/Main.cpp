#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

int MyListID, MyListID2;
float delta = 0.1;

void MyCreateList() // 보류 모드 예시
{
    MyListID = glGenLists(2);
    glNewList(MyListID, GL_COMPILE); // 그림을 미리 컴파일시 그려놓아라
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(-0.5, 0.5, 0.0);
    glEnd();
    glEndList();

    MyListID2 = MyListID + 1;
    glNewList(MyListID2, GL_COMPILE);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.5);
    glVertex3f(-0.2, -0.5, 0.0);
    glVertex3f(0.8, -0.5, 0.0);
    glVertex3f(0.8, 0.5, 0.0);
    glVertex3f(-0.2, 0.5, 0.0);
    glEnd();
    glEndList();
}

void MyDisplay() 
{
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 300, 300);
    glCallList(MyListID); // 미리 정점 그려놓는 함수 등록
    //glCallList(MyListID2);	// after Model Transform, can be reused
    glFlush();
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("201663021 이경준");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);

    glutDisplayFunc(MyDisplay);
    MyCreateList();

    glutMainLoop();
    return 0;
}