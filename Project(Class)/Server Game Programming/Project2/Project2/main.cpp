#include "GL/glut.h"
#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <math.h>

#define Pi 3.141592
#define BOARD_SCALE 50
using namespace std;


GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void TimerFunction(int value);
GLvoid drawScene(GLvoid);

typedef struct pos
{
	float x;
	float y;
	float z;
}Pos;

class Pawn
{
	Pos CurrentLocation;



public:

	Pawn()
	{
		CurrentLocation.z = 0;
	}

	void MoveFoward()
	{
		if(CurrentLocation.y < BOARD_SCALE * 4)
			CurrentLocation.y += BOARD_SCALE;
	}

	void MoveBack()
	{
		if (CurrentLocation.y > BOARD_SCALE * -3)
			CurrentLocation.y -= BOARD_SCALE;
	}
	void MoveRight()
	{
		if (CurrentLocation.x < BOARD_SCALE * 3)
			CurrentLocation.x += BOARD_SCALE;
	}
	void MoveLeft()
	{
		if (CurrentLocation.x > BOARD_SCALE * -4)
			CurrentLocation.x -= BOARD_SCALE;
	}



	void Draw()
	{
		glPushMatrix();
		glTranslatef(CurrentLocation.x, CurrentLocation.y, CurrentLocation.z);
		//glutSolidCone(10, 50, 10, 10);
		glutSolidTeapot(10);
		glPopMatrix();
	}

};

Pawn P;

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정 
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정 
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정 
	glutCreateWindow("Example2"); // 윈도우 생성 (윈도우 이름) 

	glutDisplayFunc(drawScene); // 출력 함수의 지정s

	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(specialKey);
	 
	glutTimerFunc(50, TimerFunction, 0);

	glutReshapeFunc(Reshape);
	glutMainLoop();
}


void drawBoard()
{
	bool colorFlag = true;
	glPushMatrix(); // &&

	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(-200.0, -150.0, 100.0);
	glPushMatrix(); // %%
	glScalef(1.0, 1.0, 0.1);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (colorFlag)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0.25, 0.25, 0.25);
			glutSolidCube(50);
			glTranslatef(50.0, 0.0, 0.0);
			colorFlag = !colorFlag;
		}
		glTranslatef(-400.0, 50.0, 0.0);
		colorFlag = !colorFlag;
	}
	glColor3f(0.7, 0.1, 0.2);
	glRotatef(-180.0, 1.0, 0.0, 0.0);
	glTranslatef(200.0, 200.0, 50.0);
	P.Draw();
	glPopMatrix(); //%%
	glPopMatrix();
}

// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();

//	glRotatef(xangle, 1.0, 0.0, 0.0);
//	glRotatef(yangle, 0.0, 1.0, 0.0);
//	glRotatef(zangle, 0.0, 0.0, 1.0);
	glPushMatrix(); // start

	glRotatef(30, 1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	drawBoard();
	
	glPopMatrix();
	
	
	glutSwapBuffers();

}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, w / h, 1.0, 1000.0);
	glTranslatef(0.0, -100.0, -700.0);

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -100.0, 0.0, 1.0, 0.0);
	//glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
	//glOrtho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	
	case 'q':
	case 'Q':
	{
		exit(0);
	}
	default:
		break;
	}
}
void specialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
	{
		P.MoveLeft();
			
		
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		
		P.MoveRight();
			
		
		break;
	}
	case GLUT_KEY_DOWN:
	{

		P.MoveBack();
		
		break;
	}
	case GLUT_KEY_UP:
	{
	
		P.MoveFoward();
		break;
	}
	default:
		break;
	}
}

void TimerFunction(int value)
{

	glutPostRedisplay(); // 화면 재 출력
	glutTimerFunc(50, TimerFunction, 1); // 타이머함수 재 설정
}


void Mouse(int button, int state, int x, int y)
{
	drawScene();
	if (1)
	{
		return;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x = (x - 400) / 40;
		y = (300 - y) / 30;



	}
}



