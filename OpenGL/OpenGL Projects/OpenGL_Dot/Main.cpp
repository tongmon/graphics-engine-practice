#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <ctime>
#include <cstdlib>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

void Display() // ȭ�� ��� �Լ�
{
	short Sign_Ary[2] = { -1, 1 };
	glClear(GL_COLOR_BUFFER_BIT); // ȭ�� ����
	glPointSize(5.0); // �� ũ�� Ű���
	for (int i = 0; i < 50; i++)
	{
		short Color = rand() % 3;
		short X_signed = rand() % 2, Y_signed = rand() % 2;
		float X = (rand() % 51) / 100.0;
		float Y = (rand() % 51) / 100.0;
		if (Color == 0) // ��
		{
			glColor3f(1.0, 0.0, 0.0);
		}
		else if (Color == 1) // ��
		{
			glColor3f(0.0, 1.0, 0.0);
		}
		else // ��
		{
			glColor3f(0.0, 0.0, 1.0);
		}
		glBegin(GL_POINTS);
		glVertex3f(X * Sign_Ary[X_signed], Y * Sign_Ary[Y_signed], 0.0);
		glEnd();
	}
	glFlush(); // �޸𸮿� ���ǵ� ������ ����Ϳ��� ��Ƴ��� ���
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL)); // ���� ������ ���� �ʱ�ȭ

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