#include<GL/glut.h>

// 太阳、地球和月亮
// 假设每个月都是30天
// 一年12个月，共是360天
static int day = 150; // day的变化：从0到359
void myDisplay(void)
{
	glDepthFunc(GL_LEQUAL);//设置深度<=通过,与物体之间的前后有关
	glEnable(GL_DEPTH_TEST);//深度检测
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);//矩阵为投影变换模式
	glLoadIdentity();//获得单位矩阵
	gluPerspective(75, 1, 1, 400000000);//透视变换角度75度，长宽比1：1，最近可视距离1，最远400000000两倍地球公转半径

	glMatrixMode(GL_MODELVIEW);//矩阵为视图模式
	glLoadIdentity();
	gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);//整体布局，视角位置（这里是45度倾角），物体位置，z轴正向

														  //绘制红色太阳
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(69600000, 20, 20);
	//绘制蓝色的地球
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(day / 360.0 * 360, 0.0f, 0.0f, -1.0f);
	glTranslatef(150000000, 0.0f, 0.0f);
	glutSolidSphere(15945000, 20, 20);
	//绘制黄色的“月亮”
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(day / 30.0 * 360 - day / 360.0 * 360, 0.0f, 0.0f, -1);
	glTranslatef(38000000, 0.0f, 0.0f);
	glutSolidSphere(4345000, 20, 20);
	glFlush();
	glutSwapBuffers();
}
void myIdle()
{
	++day;
	if (day >= 360)
		day = 0;
	myDisplay();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("天体");
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);
	glutMainLoop();
	return 0;
}