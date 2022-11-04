#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <time.h>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"winmm.lib")
#pragma warning(disable:4996)

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

void draw_arm(float angle, float length)
{
    glPushMatrix();
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 10 * length, 1.0f); // �ð�ħ�� ���̴� ��Ȯ���� ����
    // glScalef(10 * length, 1.0f, 1.0f);
    if (length == 1.0f) // ��ħ
        glColor3f(1.0, 0.0, 0.0);
    if (length == 1.2f) // ��ħ
        glColor3f(0.0, 1.0, 0.0);
    if (length == 1.4f) // ��ħ
        glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.1, 0.0);
    glVertex3f(-0.02, 0.0, 0.0);
    glVertex3f(0.02, 0.0, 0.0);
    /*
    glVertex3f(-0.1, 0.0, 0.0);
    glVertex3f(0.0, 0.02, 0.0);
    glVertex3f(0.0, -0.02, 0.0);
    */
    glEnd();
    glPopMatrix();
}

void tick_mark()
{
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.1, 0.0, 0.0);
    glVertex3f(0.0, 0.02, 0.0);
    glVertex3f(0.0, -0.02, 0.0);
    glEnd();
}

void draw_plate()
{
    for (int i = 0; i < 60; i++)
    {
        glPushMatrix();

        glRotatef(i * 6.0f, 0.0f, 0.0f, -1.0f);
        glTranslatef(1.55f, 0.0f, 0.0f);

        if (i % 5 == 0)
            glScalef(2.0f, 1.0f, 1.0f);

        tick_mark();

        glPopMatrix();
    }
}

void MyReshape_2(int NewWidth, int NewHeight) // ȭ�� ũ�⿡ ���� �ð� ũ�Ⱑ ����
{
    glViewport(0, 0, NewWidth, NewHeight);

    GLfloat WidthFactor = (GLfloat)NewWidth / (GLfloat)NewHeight;
    GLfloat HeightFactor = (GLfloat)NewHeight / (GLfloat)NewWidth;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (NewWidth <= NewHeight)
        glOrtho(-2.0, 2.0, -2.0 * HeightFactor, 2.0 * HeightFactor, -2.0, 2.0);
    else
        glOrtho(-2.0 * WidthFactor, 2.0 * WidthFactor, -2.0, 2.0, -2.0, 2.0);
}

void MyReshape(int NewWidth, int NewHeight) // ȭ�� ũ�Ⱑ ���ص� �� �ð� ũ�⸦ ����
{
    glViewport(0, 0, NewWidth, NewHeight);

    GLfloat WidthFactor = (GLfloat)NewWidth / (GLfloat)WINDOW_WIDTH;
    GLfloat HeightFactor = (GLfloat)NewHeight / (GLfloat)WINDOW_HEIGHT;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-2.0 * WidthFactor, 2.0 * WidthFactor, -2.0 * HeightFactor, 2.0 * HeightFactor, -2.0, 2.0); // ��ǥ �缳��
}

void Mydisplay()
{
    struct tm* nowtime;
    time_t t;
    float hour, minute, second;

    time(&t); //���� �ð��� �˾ƿ�

    nowtime = localtime(&t); //�ð� ����ü�� ��ȯ
    hour = (nowtime->tm_hour % 12) * 30.0;
    minute = nowtime->tm_min * 6.0;
    second = nowtime->tm_sec * 6.0;
    /*
    hour = (nowtime->tm_hour % 12) * 30.0 + 90;
    minute = nowtime->tm_min * 6.0 + 90;
    second = nowtime->tm_sec * 6.0 + 90;
    */

    glClear(GL_COLOR_BUFFER_BIT);

    draw_plate();
    draw_arm(hour, 1.0f);
    draw_arm(minute, 1.2f);
    draw_arm(second, 1.4f);

    glFlush();
}

void MyTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(100, MyTimer, 1);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Clock");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(Mydisplay);
    glutReshapeFunc(MyReshape_2);
    glutTimerFunc(100, MyTimer, 1);

    glutMainLoop();
    return 0;
}