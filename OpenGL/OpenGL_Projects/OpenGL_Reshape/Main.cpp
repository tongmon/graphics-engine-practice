#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// 라이브러리 추가
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

#define W_Width 300
#define W_Height 300

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);	//컬러버퍼에 초기화 색을 가함
    glColor3f(0.5, 0.5, 0.5); 		//회색

    glBegin(GL_POLYGON);        	//사각형
    glVertex3f(-0.5, -0.5, 0.0);//좌하단 좌표
    glVertex3f(0.5, -0.5, 0.0);  	//좌하단 좌표
    glVertex3f(0.5, 0.5, 0.0);  	//우상단 좌표
    glVertex3f(-0.5, 0.5, 0.0);    //좌상단 좌표	  
    glEnd();

    glFlush();
}

void MyReshape(int NewWidth, int NewHeight) {
    //glViewport(0, 0, 300, 300);	
    glViewport(0, 0, NewWidth, NewHeight);	// Ori

    GLfloat WidthFactor = (GLfloat)NewWidth / (GLfloat)W_Width;
    GLfloat HeightFactor = (GLfloat)NewHeight / (GLfloat)W_Height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-1.0, 1.0 , -1.0, 1.0 , -1.0, 1.0 ); //default
    glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor, -1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 1.0); // 좌표 재설정

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(W_Width, W_Height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Example Drawing");

    glClearColor(1.0, 1.0, 1.0, 1.0); //초기화 색은 백색

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape); // 윈도우를 다시 그릴 때 적용, 크기가 변경되던지.... 다른 윈도우가 가렸다 다시 보이던지...

    glutMainLoop();
    return 0;
}