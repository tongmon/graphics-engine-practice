#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

void Display() // ȭ�� ��� �Լ�
{
	glClear(GL_COLOR_BUFFER_BIT); // ȭ�� ����

	// ��
	glColor3f(0.9, 0.5, 0.0); // ���� ����
	glBegin(GL_POLYGON); // ���� ���� �ٰ����� �׸��ڴ� (POLYGON), ���� GL_LINES, ���� GL_POINT
	glVertex3f(-0.75, -0.6, 0.0);
	glVertex3f(-0.05, -0.6, 0.0);
	glVertex3f(-0.05, 0.1, 0.0);
	glVertex3f(-0.75, 0.1, 0.0);
	glEnd();
	glColor3f(1.0, 0.3, 0.4);
	glBegin(GL_POLYGON);
	glVertex3f(-0.75, 0.3, 0.0);
	glVertex3f(-0.85, 0.1, 0.0);
	glVertex3f(0.05, 0.1, 0.0);
	glVertex3f(-0.05, 0.3, 0.0);
	glEnd();
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.7, -0.3, 0.0);
	glVertex3f(-0.5, -0.3, 0.0);
	glVertex3f(-0.5, -0.1, 0.0);
	glVertex3f(-0.7, -0.1, 0.0);
	glEnd();
	glColor3f(0.5, 0.2, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.35, -0.6, 0.0);
	glVertex3f(-0.15, -0.6, 0.0);
	glVertex3f(-0.15, -0.2, 0.0);
	glVertex3f(-0.35, -0.2, 0.0);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex3f(-0.7, -0.2, 0.0);
	glVertex3f(-0.5, -0.2, 0.0);
	glVertex3f(-0.6, -0.3, 0.0);
	glVertex3f(-0.6, -0.1, 0.0);
	glEnd();
	glLineWidth(7.0);
	glBegin(GL_LINES);
	glVertex3f(-0.3, -0.4, 0.0);
	glVertex3f(-0.28, -0.4, 0.0);
	glEnd();

	// ����
	glColor3f(0.5, 0.2, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.4, -0.6, 0.0);
	glVertex3f(0.6, -0.6, 0.0);
	glVertex3f(0.6, -0.1, 0.0);
	glVertex3f(0.4, -0.1, 0.0);
	glEnd();
	glColor3f(0.0, 0.8, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.75, -0.1, 0.0);
	glVertex3f(0.6, 0.0, 0.0);
	glVertex3f(0.4, 0.0, 0.0);
	glVertex3f(0.25, -0.1, 0.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(0.7, 0.0, 0.0);
	glVertex3f(0.5, 0.2, 0.0);
	glVertex3f(0.3, 0.0, 0.0);
	glEnd();

	glFlush(); // �޸𸮿� ���ǵ� ������ ����Ϳ��� ��Ƴ��� ���
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("201663021 �̰���");

	glClearColor(1.0, 1.0, 1.0, 0.0); // ���� ���� �� �ʱ�ȭ ���� ����

	// ������ ���� ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// x, y, z ��ǥ ���� ����
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glutDisplayFunc(Display); // ȭ�� ��½� ȣ���ؾ��ϴ� �Լ�

	glutMainLoop(); // �޽��� ����
	return 0;
}