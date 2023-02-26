#include <Windows.h>
#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <ctime>
#include <cstdlib>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
GLfloat mat_shininess[] = { 15.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat light_position[] = { 0.9, 0.9, 0.9, 0.0 };

void InitLight()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	// glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

float Dist(float x_1, float z_1, float x_2, float z_2)
{
	return (x_1 - x_2) * (x_1 - x_2) + (z_1 - z_2) * (z_1 - z_2);
}

void MyDisplay() 
{
	short Sign[2] = { -1, 1 };
	float X[3], Z[3], CordX[3], CordZ[3];
	short X_sign[3], Z_sign[3];
	X[0] = (rand() % 60) / 100.0; Z[0] = (rand() % 80) / 100.0; 
	X_sign[0] = rand() % 2; Z_sign[0] = rand() % 2;
	CordX[0] = X[0] * Sign[X_sign[0]]; CordZ[0] = Z[0] * Sign[Z_sign[0]];
	while (true)
	{
		X[1] = (rand() % 60) / 100.0; Z[1] = (rand() % 80) / 100.0;
		X_sign[1] = rand() % 2; Z_sign[1] = rand() % 2;
		CordX[1] = X[1] * Sign[X_sign[1]]; CordZ[1] = Z[1] * Sign[Z_sign[1]];
		if (Dist(CordX[0], CordZ[0], CordX[1], CordZ[1]) >= 0.20)
		{
			if (CordX[1] <= 0.6 && -0.6 <= CordX[1] && CordZ[1] <= 0.6 && -0.6 <= CordZ[1])
			{
				break;
			}
		}
	}
	while (true)
	{
		X[2] = (rand() % 60) / 100.0; Z[2] = (rand() % 80) / 100.0;
		X_sign[2] = rand() % 2; Z_sign[2] = rand() % 2;
		CordX[2] = X[2] * Sign[X_sign[2]]; CordZ[2] = Z[2] * Sign[Z_sign[2]];
		if (Dist(CordX[0], CordZ[0], CordX[2], CordZ[2]) >= 0.21 && Dist(CordX[1], CordZ[1], CordX[2], CordZ[2]) >= 0.21)
		{
			if (CordX[2] <= 0.6 && -0.6 <= CordX[2] && CordZ[2] <= 0.6 && -0.6 <= CordZ[2])
			{
				break;
			}
		}
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.8, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	mat_diffuse[0] = 1.0; mat_diffuse[1] = 0.0; mat_diffuse[2] = 0.0; mat_diffuse[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glTranslatef(CordX[0], 0.0f, CordZ[0]);
	glTranslatef(0.7f, 0.0f, 0.0f);
	glutSolidSphere(0.1, 10, 10);

	mat_diffuse[0] = 0.0; mat_diffuse[1] = 1.0; mat_diffuse[2] = 0.0; mat_diffuse[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glTranslatef(-CordX[0] + CordX[1], 0.0f, -CordZ[0] + CordZ[1]);
	glutSolidCube(0.2);

	mat_diffuse[0] = 0.0; mat_diffuse[1] = 0.0; mat_diffuse[2] = 1.0; mat_diffuse[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glTranslatef(-CordX[1] + CordX[2], 0.0f, -CordZ[1] + CordZ[2]);
	glutSolidTeapot(0.2);

	glFlush();
}

void MyReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);	
}

int main(int argc, char** argv) 
{
	srand((unsigned int)time(NULL)); // 난수 생성을 위한 초기화
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("OpenGL Sample Drawing");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	InitLight( );

	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);

	glutMainLoop();
	return 0;

}