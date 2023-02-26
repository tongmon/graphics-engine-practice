#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdlib.h>
#include <math.h>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

#define W_Width 700.0 
#define W_Height 700.0
#define PI 3.14159265358979323846

// 정육면체 정점 8개 배열 선언
GLfloat MyVertices[6][3] = { {-0.25,0,0.25}, {0.25,0,0.25}, {0.25,0,-0.25}, {-0.25,0,-0.25},
                            {0,0.5,0}, {0,-0.5,0} };

// 색상은 빛을 비출 것이기에 모든 면을 회색으로 설정했다.
GLfloat MyColors[6][3] = { {1.0, 0.0,0.0}, {0.0, 1.0,0.0}, {0.0,0.0,1.0}, {1.0,0.0,1.0},
                            {0.2,0.2,0.2}, {0.2,0.2,0.2} };

// 빛을 비추기 위해 필요한 각 면의 법선 벡터 설정
//GLfloat MyNormal[6][3] = { {0,0,1}, {0,-1,0}, {1,0,0}, {0,1,0}, {0,0,-1}, {-1,0,0} };

// 정점 리스트 설정
GLubyte MyVertexList[24] = { 0,1,4, 2,4,1, 3,4,2, 0,4,3, 5,1,0, 5,2,1, 3,2,5, 5,0,3 };

GLint AngleY = 0, AngleX = 0, AngleZ = 0;
double OldX, OldY, CurX, CurY;

GLdouble RadConv(GLint Ang) // 각도를 라디안으로 변환
{
    return Ang * (PI / 180.0);
}

void MyDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnableClientState(GL_COLOR_ARRAY); // 색상 배열 선언
    glColorPointer(3, GL_FLOAT, 0, MyColors); // 배열 단위는 실수(float)형 3개
    glEnableClientState(GL_VERTEX_ARRAY); // 정점 배열 선언
    glVertexPointer(3, GL_FLOAT, 0, MyVertices); // 배열 단위는 실수(float)형 3개

    glPushMatrix();
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);

    glPushMatrix();
    glRotatef(AngleY, 0, 1, 0);
    glRotatef(AngleX, 1, 0, 0);
    for (int i = 0; i < 8; i++) // 정육면체 각 면 그리기
    {
        glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_BYTE, &MyVertexList[3 * i]); // 정점 리스트에서 정의된 면 그리기
    }
    glPopMatrix();

    glPopMatrix();   
    glutSwapBuffers();
}

void MyMouse(GLint Button, GLint State, GLint X, GLint Y)
{
    // 마우스 내용
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
    glutCreateWindow("201663021 이경준");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(90, W_Width / W_Height, 0.01, 1000); // 원근 투상 무대 설정
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0); // 평면 투상 무대 설정

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMouseMove);
    glutMainLoop();
    return 0;
}