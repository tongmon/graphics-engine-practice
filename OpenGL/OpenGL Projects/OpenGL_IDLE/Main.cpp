#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")
#define X 1.0
#define Y 1.0

GLfloat cordX = 0.0, cordY = 0.0, Speed = 0.005, Radius = 0.1; // ���� ��ǥ�� �ӵ�, ������ ����
GLint xDir = 1, yDir = 1; // ����

void MyDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5, 0.5, 0.5);
    glutWireSphere(Radius, 20, 20);
    glutSwapBuffers(); // ���� ���ۿ� ���� �־��ش�. �����Ÿ��� �����ִ� ���� ���۸� ����
}

void MyIdle()
{
    xDir = -X + Radius < cordX && cordX < X - Radius ? xDir : -xDir;
    cordX = cordX + 2 * Speed * xDir;
    yDir = -Y + Radius < cordY && cordY < Y - Radius ? yDir : -yDir;
    cordY = cordY + Speed * yDir;
    glTranslatef(2 * Speed * xDir, Speed * yDir, 0.0); // Sphere �̵�
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("201663021 �̰���");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-X, X, -Y, Y, -1.0, 1.0);

    glutDisplayFunc(MyDisplay);
    glutIdleFunc(MyIdle);

    glutMainLoop();
    return 0;
}