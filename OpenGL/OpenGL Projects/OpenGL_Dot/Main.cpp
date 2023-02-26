#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <ctime>
#include <cstdlib>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

void Display() // 화면 출력 함수
{
	short Sign_Ary[2] = { -1, 1 };
	glClear(GL_COLOR_BUFFER_BIT); // 화면 비우기
	glPointSize(5.0); // 점 크기 키우기
	for (int i = 0; i < 50; i++)
	{
		short Color = rand() % 3;
		short X_signed = rand() % 2, Y_signed = rand() % 2;
		float X = (rand() % 51) / 100.0;
		float Y = (rand() % 51) / 100.0;
		if (Color == 0) // 빨
		{
			glColor3f(1.0, 0.0, 0.0);
		}
		else if (Color == 1) // 녹
		{
			glColor3f(0.0, 1.0, 0.0);
		}
		else // 파
		{
			glColor3f(0.0, 0.0, 1.0);
		}
		glBegin(GL_POINTS);
		glVertex3f(X * Sign_Ary[X_signed], Y * Sign_Ary[Y_signed], 0.0);
		glEnd();
	}
	glFlush(); // 메모리에 정의된 정점을 모니터에다 쏟아내는 기능
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL)); // 난수 생성을 위한 초기화

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