#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"winmm.lib") 

#define WINDOW_WIDTH 500.0
#define WINDOW_HEIGHT 500.0

/*
int StartX = 18, StartY = 70, Width = 16, Height = 8, Bcol, Brow;
GLfloat BlackX, BlackY;

// ���� ����ǥ
float Color[25][3] = 
{ 
    {0,0,1},{1,0,0},{0.5,0,1},{0.5,0.5,0.5},{0.5,0.5,1}, 
    {0.5,0.5,0},{1,0.9,0.3},{1,0.5,1},{0.7,0.2,1},{0.2,0.5,0.1},
    {0.8,1,1},{0,1,0.5},{0,0,0},{0,1,0},{0.7,0.5,0.3},
    {0.2,0.2,0.6},{0.1,0.8,1},{0.2,0.4,1},{1,0.4,0.4},{0.8,0,0.4}, 
    {1,1,0},{1,1,0.7},{1,0.2,0.8},{0.7,0.7,0.7},{0.2,1,1}
};

void Dis_rect(float CX, float CY, float Width, float Height, float *Colorz) // �簢�� �׸��� �Լ�
{
    glColor3f(Colorz[0], Colorz[1], Colorz[2]);
    glBegin(GL_POLYGON);
    glVertex3f(CX - Width / 2, CY - Height / 2, 0.0);
    glVertex3f(CX + Width / 2, CY - Height / 2, 0.0);
    glVertex3f(CX + Width / 2, CY + Height / 2, 0.0);
    glVertex3f(CX - Width / 2, CY + Height / 2, 0.0);
    glEnd();
}

void Mydisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    int X, Y = StartY;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            X = StartX + j * Width;
            Dis_rect(X, Y, Width, Height, Color[i * 5 + j]);
            if (Color[i * 5 + j][0] == 0 && Color[i * 5 + j][1] == 0 && Color[i * 5 + j][2] == 0)
            {
                BlackX = X; BlackY = Y; Brow = i; Bcol = j;
            }
        }
        Y = StartY - (1 + i) * Height;
    }
    glFlush();
}

short is_Collison(GLfloat X, GLfloat Y, GLfloat* Ary) // �浹 ����
{
    if (Ary[0] - 8 < X && X < Ary[0] + 8 && Ary[1] - 4 < Y && Y < Ary[1] + 4)
        return 1;
    return 0;
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) // ���콺�� ������ ���� ��ǥ�� ����, ���� (0, 0)�̴�.
{
    GLfloat Up[2], Down[2], Left[2], Right[2], m_X, m_Y;
    Up[0] = BlackX; Up[1] = BlackY + 8;
    Down[0] = BlackX; Down[1] = BlackY - 8;
    Left[0] = BlackX - 16; Left[1] = BlackY;
    Right[0] = BlackX + 16; Right[1] = BlackY;
    m_X = X * (100.0 / WINDOW_WIDTH); m_Y = (WINDOW_HEIGHT - Y) * (100.0 / WINDOW_HEIGHT);
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN 
        && 12 < m_X && m_X < 92 && 36 < m_Y && m_Y < 76)
    {
        if (is_Collison(m_X, m_Y, Up))
        {
            Color[Brow * 5 + Bcol][0] = Color[(Brow - 1) * 5 + Bcol][0];
            Color[Brow * 5 + Bcol][1] = Color[(Brow - 1) * 5 + Bcol][1];
            Color[Brow * 5 + Bcol][2] = Color[(Brow - 1) * 5 + Bcol][2];
            Color[(Brow - 1) * 5 + Bcol][0] = Color[(Brow - 1) * 5 + Bcol][1] = Color[(Brow - 1) * 5 + Bcol][2] = 0;       
        }
        else if (is_Collison(m_X, m_Y, Down))
        {
            Color[Brow * 5 + Bcol][0] = Color[(Brow + 1) * 5 + Bcol][0];
            Color[Brow * 5 + Bcol][1] = Color[(Brow + 1) * 5 + Bcol][1];
            Color[Brow * 5 + Bcol][2] = Color[(Brow + 1) * 5 + Bcol][2];
            Color[(Brow + 1) * 5 + Bcol][0] = Color[(Brow + 1) * 5 + Bcol][1] = Color[(Brow + 1) * 5 + Bcol][2] = 0;
        }
        else if (is_Collison(m_X, m_Y, Left))
        {
            Color[Brow * 5 + Bcol][0] = Color[Brow * 5 + Bcol - 1][0];
            Color[Brow * 5 + Bcol][1] = Color[Brow * 5 + Bcol - 1][1];
            Color[Brow * 5 + Bcol][2] = Color[Brow * 5 + Bcol - 1][2];
            Color[Brow * 5 + Bcol - 1][0] = Color[Brow * 5 + Bcol - 1][1] = Color[Brow * 5 + Bcol - 1][2] = 0;
        }
        else if (is_Collison(m_X, m_Y, Right))
        {
            Color[Brow * 5 + Bcol][0] = Color[Brow * 5 + Bcol + 1][0];
            Color[Brow * 5 + Bcol][1] = Color[Brow * 5 + Bcol + 1][1];
            Color[Brow * 5 + Bcol][2] = Color[Brow * 5 + Bcol + 1][2];
            Color[Brow * 5 + Bcol + 1][0] = Color[Brow * 5 + Bcol + 1][1] = Color[Brow * 5 + Bcol + 1][2] = 0;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    srand((unsigned int)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("201663021 �̰���");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);

    glutDisplayFunc(Mydisplay);
    glutMouseFunc(MyMouseClick);

    glutMainLoop();
    return 0;
}
*/

int StartX = 10, StartY = 90, Width = 20, Height = 20, BlackNum; // ó�� �׷����� ��� �߽��� X, Y, ��� �� ����, ������ ��� �ε���
GLfloat BlackX, BlackY; // ������ ����� X, Y ��ǥ

// ���� ����ǥ
float Color[25][3] =
{
    {0,0,1},{1,0,0},{0.5,0,1},{0.5,0.5,0.5},{0.5,0.5,1},
    {0.5,0.5,0},{1,0.9,0.3},{1,0.5,1},{0.7,0.2,1},{0.2,0.5,0.1},
    {0.8,1,1},{0,1,0.5},{0,0,0},{0,1,0},{0.7,0.5,0.3},
    {0.2,0.2,0.6},{0.1,0.8,1},{0.2,0.4,1},{1,0.4,0.4},{0.8,0,0.4},
    {1,1,0},{1,1,0.7},{1,0.2,0.8},{0.7,0.7,0.7},{0.2,1,1}
};

void Dis_rect(float CX, float CY, float Width, float Height, float* Colorz) // �簢�� �׸��� �Լ�
{
    glColor3f(Colorz[0], Colorz[1], Colorz[2]);
    glBegin(GL_POLYGON);
    glVertex3f(CX - Width / 2, CY - Height / 2, 0.0);
    glVertex3f(CX + Width / 2, CY - Height / 2, 0.0);
    glVertex3f(CX + Width / 2, CY + Height / 2, 0.0);
    glVertex3f(CX - Width / 2, CY + Height / 2, 0.0);
    glEnd();
}

void Dis_Line(float Start, short is_X) // ���׸��� �Լ�
{
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(3);
    glBegin(GL_LINES);
    if (is_X == 1) glVertex3f(0.0, Start, 0.0);
    else glVertex3f(Start, 0.0, 0.0);
    if (is_X == 1) glVertex3f(100.0, Start, 0.0);
    else glVertex3f(Start, 100.0, 0.0);
    glEnd();
}

void Mydisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    int X, Y = StartY;
    for (int i = 0; i < 5; i++) // ��ϵ� �׸���
    {
        for (int j = 0; j < 5; j++)
        {
            X = StartX + j * Width;
            Dis_rect(X, Y, Width, Height, Color[i * 5 + j]);
            if (Color[i * 5 + j][0] == 0 && Color[i * 5 + j][1] == 0 && Color[i * 5 + j][2] == 0)
            {
                BlackX = X; BlackY = Y; BlackNum = i * 5 + j;
            }
        }
        Y = StartY - (1 + i) * Height;
    }
    X = 0, Y = 0;
    Dis_Line(0.1, 1); Dis_Line(0.1, 0);
    Dis_Line(99.9, 1); Dis_Line(99.9, 0);
    for (int i = 0; i < 5; i++) // ���� �׸���
    {
        Dis_Line(X, 1); Dis_Line(Y, 0);
        X = X + 20; Y = Y + 20;
    }
    glFlush();
}

short is_Collison(GLfloat X, GLfloat Y, GLfloat* Ary) // �浹 ����
{
    if (Ary[0] - Width / 2 < X && X < Ary[0] + Width / 2 && Ary[1] - Height / 2 < Y && Y < Ary[1] + Height / 2)
        return 1;
    return 0;
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) // ���콺�� ������ ���� ��ǥ�� ����, ���� (0, 0)�̴�.
{
    GLfloat Up[2], Down[2], Left[2], Right[2], m_X, m_Y; // ��, �Ʒ�, ��, �� ����� �߽���ǥ, ���콺 ��ǥ
    int Change_Brick, is_Changed = 0; // �ٲ� ����� BlackNum ����, ��� ���� �ٲپ�� �ϴ��� ���� ����
    Up[0] = BlackX; Up[1] = BlackY + Height; // ������ �簢�� ���� ����� �߽� ��ǥ ����
    Down[0] = BlackX; Down[1] = BlackY - Height; // ������ �簢�� �Ʒ��� ����� �߽� ��ǥ ����
    Left[0] = BlackX - Width; Left[1] = BlackY; // ������ �簢�� ���� ����� �߽� ��ǥ ����
    Right[0] = BlackX + Width; Right[1] = BlackY; // ������ �簢�� ������ ����� �߽� ��ǥ ����
    m_X = X * (100.0 / WINDOW_WIDTH); m_Y = (WINDOW_HEIGHT - Y) * (100.0 / WINDOW_HEIGHT); // ���콺 ��ǥ ����
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
    {
        if (is_Collison(m_X, m_Y, Up)) // �� �浹
        {
            Change_Brick = BlackNum - 5; is_Changed = 1;
        }
        else if (is_Collison(m_X, m_Y, Down)) // �Ʒ� �浹
        {
            Change_Brick = BlackNum + 5; is_Changed = 1;
        }
        else if (is_Collison(m_X, m_Y, Left)) // ���� �浹
        {
            Change_Brick = BlackNum - 1; is_Changed = 1;
        }
        else if (is_Collison(m_X, m_Y, Right)) // ������ �浹
        {
            Change_Brick = BlackNum + 1; is_Changed = 1;
        }
        if (is_Changed == 1)
        {
            Color[BlackNum][0] = Color[Change_Brick][0];
            Color[BlackNum][1] = Color[Change_Brick][1];
            Color[BlackNum][2] = Color[Change_Brick][2];
            Color[Change_Brick][0] = Color[Change_Brick][1] = Color[Change_Brick][2] = 0;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    srand((unsigned int)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // 500 * 500
    glutInitWindowPosition(500, 100);
    glutCreateWindow("201663021 �̰���");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0); // ��ǥ�� ����

    glutDisplayFunc(Mydisplay);
    glutMouseFunc(MyMouseClick);

    glutMainLoop();
    return 0;
}