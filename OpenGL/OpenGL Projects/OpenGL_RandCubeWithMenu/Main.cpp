#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <ctime>
#include <cstdlib>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

enum { Stop, Continue, Exit }; // �������� �������� ��������
GLfloat MyColors[8][3] = { {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5},
	{0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5} }; // ���� ����Ʈ
GLubyte CubeList[24] = { 0,3,2,1, 2,3,7,6, 0,4,7,3, 1,2,6,5, 4,5,6,7, 0,1,5,4 };
GLfloat Red, Green, Blue, MyVertices[8][3]; // ���� ����Ʈ
GLint Which_Color = 0, Sign[2] = { 1, -1 }, Stance = Continue, is_Started = 0;

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) // ���콺 �Լ�
{
	if (is_Started == 0 || Stance == Stop) return;
	Y = 900 - Y; GLfloat bY = (Y / 900.0) * 8, bX = (X / 900.0) * 8;
	bY = bY < 4 ? -4 + bY : bY - 4; bX = bX < 4 ? -4 + bX : bX - 4; // ���콺 ��ǥ�� ��ǥ�迡 �°� ����
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN
		&& MyVertices[0][0] <= bX && bX <= MyVertices[2][0] && MyVertices[0][1] <= bY && bY <= MyVertices[2][1])
	{
		// Ŭ���ǰ� �δ��� ���� �ȿ� ������ ���� ��ü
		Which_Color = (Which_Color + 1) % 3; // ���� ����, 0�� ���� 1�� �ʷ� 2�� �Ķ�
		if (Which_Color == 0) { Red = 1.0; Green = 0.0; Blue = 0.0; }
		else if (Which_Color == 1) { Red = 0.0; Green = 1.0; Blue = 0.0; }
		else { Red = 0.0; Green = 0.0; Blue = 1.0; }
		for (int i = 0; i < 8; i++)
		{
			MyColors[i][0] = Red; MyColors[i][1] = Green; MyColors[i][2] = Blue;
		}
		glutPostRedisplay();
	}
}

void MyDisplay() // ȭ�� ��� �Լ�
{
	glClear(GL_COLOR_BUFFER_BIT); // ȭ�� ����

	glFrontFace(GL_CCW); // �� ���� �ð�� �� �� �ݽð�� �� �� ����
	glEnable(GL_CULL_FACE); // ������ ���Ÿ� �� �Ÿ� �� �Լ� �̿�
	glCullFace(GL_BACK); // �޸� ����

	glEnableClientState(GL_COLOR_ARRAY); // ���� �迭 ����
	glEnableClientState(GL_VERTEX_ARRAY); // ���� �迭 ����

	glColorPointer(3, GL_FLOAT, 0, MyColors); // �迭 ������ �Ǽ�(float)�� 3��
	glVertexPointer(3, GL_FLOAT, 0, MyVertices);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i = 0; i < 6; i++)
	{
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &CubeList[4 * i]);
	}
	glutSwapBuffers();
}

void MyMenu(int entryID) // �޴� �Լ�
{
	if (entryID == 1) { is_Started = 1; }
	else if (entryID == 2) { if (is_Started == 1) Stance = Stop; }
	else if (entryID == 3) { Stance = Continue; }
	else { exit(1); }
}

void MyTimer(int Value)
{
	if (Stance == Continue && is_Started == 1)
	{
		MyVertices[0][0] = Sign[rand() % 2] * ((rand() % 35) / 10.0); MyVertices[0][1] = Sign[rand() % 2] * ((rand() % 35) / 10.0);
		MyVertices[1][0] = MyVertices[0][0]; MyVertices[1][1] = MyVertices[0][1] + 0.5;
		MyVertices[2][0] = MyVertices[0][0] + 0.5; MyVertices[2][1] = MyVertices[0][1] + 0.5;
		MyVertices[3][0] = MyVertices[0][0] + 0.5; MyVertices[3][1] = MyVertices[0][1];
		MyVertices[4][0] = MyVertices[0][0]; MyVertices[4][1] = MyVertices[0][1]; MyVertices[5][0] = MyVertices[1][0]; MyVertices[5][1] = MyVertices[1][1];
		MyVertices[6][0] = MyVertices[2][0]; MyVertices[6][1] = MyVertices[2][1]; MyVertices[7][0] = MyVertices[3][0]; MyVertices[7][1] = MyVertices[3][1];
		for (int i = 0; i < 8; i = i + 4)
		{
			MyVertices[i][2] = i < 4 ? 0.25 : -0.25;
		}
		glutPostRedisplay();
	}
	glutTimerFunc(1000, MyTimer, 1); // 0.1�ʸ��� Ÿ�̸� ����
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // ���� ���۸� ��带 ����
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("201663021 �̰���");

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);

	GLint MyMenuID = glutCreateMenu(MyMenu);
	glutAddMenuEntry("���� ����", 1); // ���� ���� �޴� �߰�
	glutAddMenuEntry("����", 2); // ���� �޴� �߰�
	glutAddMenuEntry("���", 3); // ��� �޴� �߰�
	glutAddMenuEntry("����", 4); // ���� �޴� �߰�

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(MyDisplay);
	glutTimerFunc(1000, MyTimer, 1);
	glutMouseFunc(MyMouseClick);

	glutMainLoop();
	return 0;
}