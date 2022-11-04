#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// ���̺귯�� �߰�
#pragma comment (lib,"glut32.lib")
#pragma comment (lib,"glu32.lib")
#pragma comment (lib,"opengl32.lib")

#define W_Width 300
#define W_Height 300

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);	//�÷����ۿ� �ʱ�ȭ ���� ����
    glColor3f(0.5, 0.5, 0.5); 		//ȸ��

    glBegin(GL_POLYGON);        	//�簢��
    glVertex3f(-0.5, -0.5, 0.0);//���ϴ� ��ǥ
    glVertex3f(0.5, -0.5, 0.0);  	//���ϴ� ��ǥ
    glVertex3f(0.5, 0.5, 0.0);  	//���� ��ǥ
    glVertex3f(-0.5, 0.5, 0.0);    //�»�� ��ǥ	  
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
    glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor, -1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 1.0); // ��ǥ �缳��

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(W_Width, W_Height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Example Drawing");

    glClearColor(1.0, 1.0, 1.0, 1.0); //�ʱ�ȭ ���� ���

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape); // �����츦 �ٽ� �׸� �� ����, ũ�Ⱑ ����Ǵ���.... �ٸ� �����찡 ���ȴ� �ٽ� ���̴���...

    glutMainLoop();
    return 0;
}