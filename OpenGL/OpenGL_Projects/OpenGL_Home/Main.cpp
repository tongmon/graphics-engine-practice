#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

void Display() // 화면 출력 함수
{
	glClear(GL_COLOR_BUFFER_BIT); // 화면 비우기

	// 집
	glColor3f(0.9, 0.5, 0.0); // 색상 지정
	glBegin(GL_POLYGON); // 지금 부터 다각형을 그리겠다 (POLYGON), 선은 GL_LINES, 점은 GL_POINT
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

	// 나무
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

	glFlush(); // 메모리에 정의된 정점을 모니터에다 쏟아내는 기능
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("201663021 이경준");

	glClearColor(1.0, 1.0, 1.0, 0.0); // 버퍼 지울 때 초기화 색상 설정

	// 투영법 적용 안함
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// x, y, z 좌표 범위 설정
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glutDisplayFunc(Display); // 화면 출력시 호출해야하는 함수

	glutMainLoop(); // 메시지 루프
	return 0;
}