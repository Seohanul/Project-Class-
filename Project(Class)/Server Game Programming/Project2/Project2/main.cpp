
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include "GL/glut.h"


#include <stdlib.h>

#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <math.h>

#define Pi 3.141592
#define BOARD_SCALE 50


#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    50

using namespace std;


GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void TimerFunction(int value);
GLvoid drawScene(GLvoid);
void err_quit(char *msg);
void err_display(char *msg);


enum Key
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};

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


	void SetCurrentLocation(Pos &p)
	{
		CurrentLocation = p;
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




Pawn MyCharater;
SOCKET sock;
int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");



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

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();

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
	MyCharater.Draw();
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
	int retval = 0;
	int input = 0;
	Pos position = {};
	switch (key)
	{
	case GLUT_KEY_LEFT:
	{
		input = LEFT;
		retval = send(sock, (char *)&input, sizeof(int), 0);
		retval = recv(sock, (char *)&position, sizeof(Pos), 0);

		MyCharater.SetCurrentLocation(position);
		//P.MoveLeft();
			
		
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		input = RIGHT;
		retval = send(sock, (char *)&input, sizeof(int), 0);
		retval = recv(sock, (char *)&position, sizeof(Pos), 0);

		MyCharater.SetCurrentLocation(position);

		//P.MoveRight();
			
		
		break;
	}
	case GLUT_KEY_DOWN:
	{
		input = DOWN;
		retval = send(sock, (char *)&input, sizeof(int), 0);
		retval = recv(sock, (char *)&position, sizeof(Pos), 0);

		MyCharater.SetCurrentLocation(position);

		//P.MoveBack();
		
		break;
	}
	case GLUT_KEY_UP:
	{
		input = UP;
		retval = send(sock, (char *)&input, sizeof(int), 0);
		retval = recv(sock, (char *)&position, sizeof(Pos), 0);

		MyCharater.SetCurrentLocation(position);

		//P.MoveFoward();
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



void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}