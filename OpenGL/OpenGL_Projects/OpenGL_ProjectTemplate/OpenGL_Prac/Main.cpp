#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

void Display() // ȭ�� ��� �Լ�
{
	glClear(GL_COLOR_BUFFER_BIT); // ȭ�� ����
	glBegin(GL_POLYGON); // ���� ���� �ٰ����� �׸��ڴ� (POLYGON), ���� GL_LINE, ���� GL_POINT
	// ���� ������ �ݽð��������
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glEnd(); // �ٰ����� ������.
	glFlush(); // �޸𸮿� ���ǵ� ������ ����Ϳ��� ��Ƴ��� ���
}

int main()
{
	glutCreateWindow("OpenGL Hello World!");
	glutDisplayFunc(Display); // ȭ�� ��½� ȣ���ؾ��ϴ� �Լ�

	glutMainLoop(); // �޽��� ����
	return 0;
}