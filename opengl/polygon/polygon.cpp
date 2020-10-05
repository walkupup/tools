#include<GL/glut.h>


void myDisplay(void)
{
	glEnable(GL_DEPTH_TEST);//深度检测
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);//矩阵为投影变换模式
	glLoadIdentity();//获得单位矩阵
	gluPerspective(60, 1, 1, 4000);//透视变换角度75度，长宽比1：1，最近可视距离1，最远4000

	glMatrixMode(GL_MODELVIEW);//矩阵为视图模式
	glLoadIdentity();
	gluLookAt(1, 0, 1, 1, 1.732, 0, 0, 0, 1);//整体布局，视角位置（这里是45度倾角），物体位置，z轴正向

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
	glutCreateWindow("正方形");
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);
	glutMainLoop();
	return 0;
}