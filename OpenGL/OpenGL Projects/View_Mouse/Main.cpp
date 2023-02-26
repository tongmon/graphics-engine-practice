#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdlib.h>
#include <math.h>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

#define W_Width 700.0 
#define W_Height 700.0
#define PI 3.14159265358979323846

// ������ü ���� 8�� �迭 ����
GLfloat MyVertices[6][3] = { {-0.25,0,0.25}, {0.25,0,0.25}, {0.25,0,-0.25}, {-0.25,0,-0.25},
                            {0,0.5,0}, {0,-0.5,0} };

// ������ ���� ���� ���̱⿡ ��� ���� ȸ������ �����ߴ�.
GLfloat MyColors[6][3] = { {1.0, 0.0,0.0}, {0.0, 1.0,0.0}, {0.0,0.0,1.0}, {1.0,0.0,1.0},
                            {0.2,0.2,0.2}, {0.2,0.2,0.2} };

// ���� ���߱� ���� �ʿ��� �� ���� ���� ���� ����
//GLfloat MyNormal[6][3] = { {0,0,1}, {0,-1,0}, {1,0,0}, {0,1,0}, {0,0,-1}, {-1,0,0} };

// ���� ����Ʈ ����
GLubyte MyVertexList[24] = { 0,1,4, 2,4,1, 3,4,2, 0,4,3, 5,1,0, 5,2,1, 3,2,5, 5,0,3 };

GLint AngleY = 0, AngleX = 0, AngleZ = 0;
double OldX, OldY, CurX, CurY;

GLdouble RadConv(GLint Ang) // ������ �������� ��ȯ
{
    return Ang * (PI / 180.0);
}

void MyDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnableClientState(GL_COLOR_ARRAY); // ���� �迭 ����
    glColorPointer(3, GL_FLOAT, 0, MyColors); // �迭 ������ �Ǽ�(float)�� 3��
    glEnableClientState(GL_VERTEX_ARRAY); // ���� �迭 ����
    glVertexPointer(3, GL_FLOAT, 0, MyVertices); // �迭 ������ �Ǽ�(float)�� 3��

    glPushMatrix();
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

    glPushMatrix();
    glRotatef(AngleY, 0, 1, 0);
    glRotatef(AngleX, 1, 0, 0);
    for (int i = 0; i < 8; i++) // ������ü �� �� �׸���
    {
        glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_BYTE, &MyVertexList[3 * i]); // ���� ����Ʈ���� ���ǵ� �� �׸���
    }
    glPopMatrix();

    glPopMatrix();   
    glutSwapBuffers();
}

void MyMouse(GLint Button, GLint State, GLint X, GLint Y)
{
    // ���콺 ����
    GLdouble mX = X * (4.0 / W_Width), mY = (W_Height - Y) * (4.0 / W_Height);
    mX -= 2.0; mY -= 2.0;
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        CurX = OldX = mX; CurY = OldY = mY;
        glutPostRedisplay();
    }
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_UP) {
        glutPostRedisplay();
    }
}

void MyMouseMove(GLint X, GLint Y)
{
    GLdouble mX = X * (4.0 / W_Width), mY = (W_Height - Y) * (4.0 / W_Height);
    mX -= 2.0; mY -= 2.0;
    CurX = mX; CurY = mY;
    AngleY = (AngleY - (int)((OldX - CurX) * 200)) % 360;
    AngleX = (AngleX - (int)((OldY - CurY) * 200)) % 360;
    OldX = CurX; OldY = CurY;
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(W_Width, W_Height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("201663021 �̰���");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(90, W_Width / W_Height, 0.01, 1000); // ���� ���� ���� ����
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0); // ��� ���� ���� ����

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMouseMove);
    glutMainLoop();
    return 0;
}