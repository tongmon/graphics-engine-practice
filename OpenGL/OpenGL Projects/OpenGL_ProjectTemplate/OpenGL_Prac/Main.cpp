#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

void Display() // 화면 출력 함수
{
	glClear(GL_COLOR_BUFFER_BIT); // 화면 비우기
	glBegin(GL_POLYGON); // 지금 부터 다각형을 그리겠다 (POLYGON), 선은 GL_LINE, 점은 GL_POINT
	// 정점 순서는 반시계방향으로
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	glEnd(); // 다각형이 끝났다.
	glFlush(); // 메모리에 정의된 정점을 모니터에다 쏟아내는 기능
}

int main()
{
	glutCreateWindow("OpenGL Hello World!");
	glutDisplayFunc(Display); // 화면 출력시 호출해야하는 함수

	glutMainLoop(); // 메시지 루프
	return 0;
}