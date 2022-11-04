#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdlib.h>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

GLint Figure = 1;
GLfloat Red = 0.5, Green = 0.5, Blue = 0.5, Scale = 1.0;

void MyDisplay() 
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(Red, Green, Blue);
    if (Figure == 1) glutWireSphere(0.2 * Scale, 30, 30);
    else if (Figure == 2) glutWireTeapot(0.2 * Scale);
    else glutWireTorus(0.1 * Scale, 0.2 * Scale, 30, 30);
    glFlush();
}

void MyMainMenu(int entryID) // 메인메뉴
{   
   glutPostRedisplay();
}

void MyColorMenu(int entryID) // 컬러 메뉴
{
    if (entryID == 1) { Red = 1.0; Blue = Green = 0.0; }
    else if (entryID == 2) { Green = 1.0; Blue = Red = 0.0; }
    else { Blue = 1.0; Red = Green = 0.0; }
    glutPostRedisplay();
}

void MyScaleMenu(int entryID) // 크기 메뉴
{
    Scale = entryID;
    glutPostRedisplay();
}

void MyObjectMenu(int entryID) // 모양 메뉴
{
    Figure = entryID;
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("201663021 이경준");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    GLint MyColorMenuID = glutCreateMenu(MyColorMenu); // 색상 서브 메뉴
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);

    GLint MyScaleMenuID = glutCreateMenu(MyScaleMenu); // 크기 서브 메뉴
    glutAddMenuEntry("Small", 1);
    glutAddMenuEntry("Mid", 2);
    glutAddMenuEntry("Big", 3);

    GLint MyObjectMenuID = glutCreateMenu(MyObjectMenu); // 모양 서브 메뉴
    glutAddMenuEntry("Sphere", 1);
    glutAddMenuEntry("Teapot", 2);
    glutAddMenuEntry("Torus", 3);

    GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
    glutAddSubMenu("Color", MyColorMenuID); // 색상 서브 메뉴 추가
    glutAddSubMenu("Size", MyScaleMenuID); // 크기 서브 메뉴 추가
    glutAddSubMenu("Object", MyObjectMenuID); // 모양 서브 메뉴 추가

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(MyDisplay);

    glutMainLoop();
    return 0;
}