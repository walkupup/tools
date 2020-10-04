#include<gl/glut.h>
#include <opencv2/opencv.hpp>
using namespace cv;

//OpenCV��ȡͼ��
Mat I = imread("d.bmp");
//���ó���
int width = I.cols;
int height = I.rows;
//����ͼ��ָ��
GLubyte* pixels;

const double PI = 3.1415926535897932384626433832795;
float x_pos = 0;
float y_pos = 0;
float z_pos = 15;
float theta = PI / 6;
float phi = PI / 2;
float RR = 5;
GLuint texture; // this variable stores the texture

GLuint load_texture()
{
	//OpenGL��������������ʾ
	GLuint texture_ID;

	//��ȡͼ��ָ��
	int pixellength = width*height * 3;
	pixels = new GLubyte[pixellength];
	memcpy(pixels, I.data, pixellength * sizeof(char));
	imshow("OpenCV", I);

	//��texture_ID����Ϊ2D������Ϣ
	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	//����Ŵ���Сʹ�����Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����ˮƽ��ֱ��������ʹ���ظ���ͼ
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//����ˮƽ��ֱ��������ʹ�ñ�Ե������ͼ(���ظ���ͼ��ѡһ)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//��ͼ���ڴ�����������Ϣ
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

	free(pixels);
	return texture_ID;
}

void display()
{
	// �����Ļ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//��ȡ�������
	GLuint image = load_texture();

	//��������OpenGL���ڣ�ԭ��λ��Ϊ���Ͻǣ�x������ң�y����ϵ��£�����ֵ����������ֵ��ͬ
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_MODELVIEW);//����Ϊ��ͼģʽ
	glRotatef(theta, 0.0f, 0.0f, -1.0f);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);

	//��ʾ����
	glEnable(GL_TEXTURE_2D);    //����ʹ������
	glBindTexture(GL_TEXTURE_2D, image);    //ѡ���������

											//ԭʼ��ȫ����ı���
	//glBegin(GL_POLYGON);    //����Ϊ�����������ͼ��ʽ����ʼ��ͼ
	//glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);    //�������ϽǶ�Ӧ�������Ͻ�
	//glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);    //�������½Ƕ�Ӧ�������½�
	//glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);    //�������½Ƕ�Ӧ�������½�
	//glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);    //�������ϽǶ�Ӧ�������Ͻ�
	//glEnd();    //������ͼ*/

				////������
				//glBegin(GL_POLYGON);
				//glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
				//glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
				//glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
				//glEnd();

				//    //�����
				//glBegin(GL_POLYGON);
				//glTexCoord2f(0, 0); glVertex2f(0, 0);
				//glTexCoord2f(0, 1); glVertex2f(0, height/3);
				//glTexCoord2f(1, 1); glVertex2f(width/4, height/3);
				//glTexCoord2f(1, 0); glVertex2f(width/4, 0);
				//glTexCoord2f(0.0f, 0.5f); glVertex2f(0, height/2);
				//glEnd();

				//����任
				glBegin(GL_POLYGON);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(width/4, height/4);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height*2/3);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(width*4/5, 50);
				glEnd();

				//��Ե��ͼЧ��
				/*glBegin(GL_POLYGON);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
				glTexCoord2f(0.0f, 2.0f); glVertex2f(0, height);
				glTexCoord2f(2.0f, 2.0f); glVertex2f(width, height);
				glTexCoord2f(2.0f, 0.0f); glVertex2f(width, 0);
				glEnd();*/

	glDisable(GL_TEXTURE_2D);    //��ֹʹ������




	//˫���潻����������ʾͼ��
	glutSwapBuffers();
}

// the function for the process keys for controlling the camera
void processKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w': theta += 0.1; break;
	case 's': theta -= 0.1; break;
	case 'a': phi += 0.1; break;
	case 'd': phi -= 0.1; break;
	case 't': if (RR < 100) RR += 0.1f; break;
	case 'g': if (RR > 4) RR -= 0.1f; break;
	case 'z':     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); break;
	case 'x':     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND); break;
	case 'c':     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); break;
	case 'p':
		//unsigned char data[100 * 100 * 3];
		cv::Mat img(300, 300, CV_8UC3);
		glReadPixels(0, 0, 300, 300, GL_RGB, GL_UNSIGNED_BYTE, img.data);
		cv::cvtColor(img, img, COLOR_RGB2BGR);
		cv::imwrite("1.jpg", img);
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	//��ʼ��GL
	glutInit(&argc, argv);
	//������ʾ����(˫���棬RGB��ʽ)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//���ô��ڳߴ磺width*height
	glutInitWindowSize(width, height);
	//���ô���λ�ã�����Ļ���Ͻ�����ֵ(100,100)��
	glutInitWindowPosition(100, 100);
	//���ô�������
	glutCreateWindow("OpenGL");
	//��ʾ������display�¼���Ҫ���б�д
	glutDisplayFunc(display);
	glutKeyboardFunc(processKeys);

	//�ظ�ѭ��GLUT�¼�
	glutMainLoop();
}