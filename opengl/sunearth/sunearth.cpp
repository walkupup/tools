#include<GL/glut.h>

// ̫�������������
// ����ÿ���¶���30��
// һ��12���£�����360��
static int day = 150; // day�ı仯����0��359
void myDisplay(void)
{
	glDepthFunc(GL_LEQUAL);//�������<=ͨ��,������֮���ǰ���й�
	glEnable(GL_DEPTH_TEST);//��ȼ��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);//����ΪͶӰ�任ģʽ
	glLoadIdentity();//��õ�λ����
	gluPerspective(75, 1, 1, 400000000);//͸�ӱ任�Ƕ�75�ȣ������1��1��������Ӿ���1����Զ400000000��������ת�뾶

	glMatrixMode(GL_MODELVIEW);//����Ϊ��ͼģʽ
	glLoadIdentity();
	gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);//���岼�֣��ӽ�λ�ã�������45����ǣ�������λ�ã�z������

														  //���ƺ�ɫ̫��
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(69600000, 20, 20);
	//������ɫ�ĵ���
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(day / 360.0 * 360, 0.0f, 0.0f, -1.0f);
	glTranslatef(150000000, 0.0f, 0.0f);
	glutSolidSphere(15945000, 20, 20);
	//���ƻ�ɫ�ġ�������
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
	glutCreateWindow("����");
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);
	glutMainLoop();
	return 0;
}