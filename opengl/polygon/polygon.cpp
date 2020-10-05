#include<GL/glut.h>


void myDisplay(void)
{
	glEnable(GL_DEPTH_TEST);//��ȼ��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);//����ΪͶӰ�任ģʽ
	glLoadIdentity();//��õ�λ����
	gluPerspective(60, 1, 1, 4000);//͸�ӱ任�Ƕ�75�ȣ������1��1��������Ӿ���1����Զ4000

	glMatrixMode(GL_MODELVIEW);//����Ϊ��ͼģʽ
	glLoadIdentity();
	gluLookAt(1, 0, 1, 1, 1.732, 0, 0, 0, 1);//���岼�֣��ӽ�λ�ã�������45����ǣ�������λ�ã�z������

	GLfloat pointA[2] = { 1, 1 };
	GLfloat pointB[2] = { 1.707, 1.707 };
	GLfloat pointC[2] = { 1, 2.414 };
	GLfloat pointD[2] = { 1 - 0.707, 1 + 0.707 };

	glClear(GL_COLOR_BUFFER_BIT); 
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON); 
	glVertex2fv(pointA);
	glVertex2fv(pointB);
	glVertex2fv(pointC);
	glVertex2fv(pointD);
	glEnd(); 
	glFlush();
	glutSwapBuffers();
}
void myIdle()
{
	myDisplay();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("������");
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);
	glutMainLoop();
	return 0;
}