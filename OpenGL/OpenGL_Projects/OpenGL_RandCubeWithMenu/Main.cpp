#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <ctime>
#include <cstdlib>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

enum { Stop, Continue, Exit }; // 정지인지 진행인지 종료인지
GLfloat MyColors[8][3] = { {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5},
	{0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,0.5} }; // 색상 리스트
GLubyte CubeList[24] = { 0,3,2,1, 2,3,7,6, 0,4,7,3, 1,2,6,5, 4,5,6,7, 0,1,5,4 };
GLfloat Red, Green, Blue, MyVertices[8][3]; // 정점 리스트
GLint Which_Color = 0, Sign[2] = { 1, -1 }, Stance = Continue, is_Started = 0;

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) // 마우스 함수
{
	if (is_Started == 0 || Stance == Stop) return;
	Y = 900 - Y; GLfloat bY = (Y / 900.0) * 8, bX = (X / 900.0) * 8;
	bY = bY < 4 ? -4 + bY : bY - 4; bX = bX < 4 ? -4 + bX : bX - 4; // 마우스 좌표를 좌표계에 맞게 수정
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN
		&& MyVertices[0][0] <= bX && bX <= MyVertices[2][0] && MyVertices[0][1] <= bY && bY <= MyVertices[2][1])
	{
		// 클릭되고 두더지 범위 안에 있으면 색상 교체
		Which_Color = (Which_Color + 1) % 3; // 색상 설정, 0은 빨강 1은 초록 2는 파랑
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

void MyDisplay() // 화면 출력 함수
{
	glClear(GL_COLOR_BUFFER_BIT); // 화면 비우기

	glFrontFace(GL_CCW); // 앞 면을 시계로 할 지 반시계로 할 지 결정
	glEnable(GL_CULL_FACE); // 가린면 제거를 할 거면 이 함수 이용
	glCullFace(GL_BACK); // 뒷면 제거

	glEnableClientState(GL_COLOR_ARRAY); // 색상 배열 선언
	glEnableClientState(GL_VERTEX_ARRAY); // 정점 배열 선언

	glColorPointer(3, GL_FLOAT, 0, MyColors); // 배열 단위는 실수(float)형 3개
	glVertexPointer(3, GL_FLOAT, 0, MyVertices);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i = 0; i < 6; i++)
	{
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &CubeList[4 * i]);
	}
	glutSwapBuffers();
}

void MyMenu(int entryID) // 메뉴 함수
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
	glutTimerFunc(1000, MyTimer, 1); // 0.1초마다 타이머 가동
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // 더블 버퍼링 모드를 설정
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("201663021 이경준");

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);

	GLint MyMenuID = glutCreateMenu(MyMenu);
	glutAddMenuEntry("게임 시작", 1); // 게임 시작 메뉴 추가
	glutAddMenuEntry("중지", 2); // 중지 메뉴 추가
	glutAddMenuEntry("계속", 3); // 계속 메뉴 추가
	glutAddMenuEntry("종료", 4); // 종료 메뉴 추가

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(MyDisplay);
	glutTimerFunc(1000, MyTimer, 1);
	glutMouseFunc(MyMouseClick);

	glutMainLoop();
	return 0;
}