
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include "GL/glut.h"

#include <stdlib.h>

#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <map>
#include <math.h>
#include "Pawn.h"


#define Pi 3.141592
#define BOARD_SCALE 50



#define MAX_BUFFER        1024
#define SERVER_IP        "127.0.0.1"
#define SERVER_PORT        3500



using namespace std;


GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void TimerFunction(int value);
GLvoid drawScene(GLvoid);
void err_quit(char *msg);
void err_display(char *msg);
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);
void testrecv();



struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	WSABUF dataBuffer;
	int receiveBytes;
	int sendBytes;
};

map<SOCKET, Pawn*> MyCharaters;
struct ClientPos
{
	SOCKET sock;
	Pos position;
};

vector< ClientPos> ClientsPosition;

SOCKET listenSocket;

SOCKETINFO *socketInfo;
DWORD sendBytes;
DWORD receiveBytes;
DWORD flags;
int mySocketNum = 0;

int main(int argc, char *argv[])
{

	// Winsock Start - winsock.dll 로드
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		return 1;
	}

	// 1. 소켓생성
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);   // overlapped flag 붙여줌  WSA_FLAG_OVERLAPPED
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		return 1;
	}


	// 서버정보 객체설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

	// 2. 연결요청
	if (connect(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		printf("Error - Fail to connect\n");
		// 4. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}
	else
	{
		printf("Server Connected\n* Enter Message\n->");
	}
	int receiveBytes = recv(listenSocket, (char*)&mySocketNum, sizeof(mySocketNum), 0);
	
	int vecSize = 0;
	
	int retval = recv(listenSocket, (char*)&vecSize, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display(const_cast<char*>("recv()"));
		exit(0);
	}
	cout << "클라 " << vecSize << " 명" << endl;
	for (int i = 0; i < vecSize; ++i)
	{
		ClientPos cp;
		retval = recvn(listenSocket, (char*)&cp, sizeof(ClientPos), 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char*>("recv()"));
			exit(0);
		}
		MyCharaters[cp.sock] = { new Pawn(cp.position) };
	}
	

	HANDLE hThread = CreateThread(NULL, 0, [](LPVOID arg)->DWORD { testrecv(); return 0; }, 0, 0, NULL);


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
	closesocket(listenSocket);
	CloseHandle(hThread);
	// 윈속 종료
	WSACleanup();

}

void testrecv()
{
	while (1)
	{
		//int receiveBytes = recv(listenSocket, (char *)&position, sizeof(position), 0);
		
		ClientPos cp;
		int retval = recv(listenSocket, (char*)&cp, sizeof(ClientPos), 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char*>("recv()"));
			exit(0);
		}
		if (MyCharaters.end() != MyCharaters.find(cp.sock))
			MyCharaters[cp.sock]->SetCurrentLocation(cp.position);
		else
			MyCharaters[cp.sock] = { new Pawn(cp.position) };
		drawScene();
	}
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
	for (auto &p : MyCharaters)
		p.second->Draw();
	//MyCharater.Draw();
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

void NetworkProcess(int input)
{

	socketInfo = (struct SOCKETINFO *)malloc(sizeof(struct SOCKETINFO));
	memset((void *)socketInfo, 0x00, sizeof(struct SOCKETINFO));
	socketInfo->dataBuffer.len = sizeof(input);
	socketInfo->dataBuffer.buf = (char *)&input;

	// 3-1. 데이터 쓰기
	int sendBytes = send(listenSocket, (char *)&input, sizeof(input), 0);
	//if (sendBytes > 0)
	//{
	//	printf("TRACE - Send message : %s (%d bytes)\n", (char *)&input, sendBytes);
	//	// 3-2. 데이터 읽기
	//	int receiveBytes = recv(listenSocket, (char *)&position, sizeof(position), 0);
	//}
}

void specialKey(int key, int x, int y)
{
	int retval = 0;
	int input = 0;
	switch (key)
	{
	case GLUT_KEY_LEFT:
	{
		input = LEFT;

		NetworkProcess(input);

		//MyCharater.SetCurrentLocation(position);
			
		
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		input = RIGHT;
		NetworkProcess(input);
		
		break;
	}
	case GLUT_KEY_DOWN:
	{
		input = DOWN;
		NetworkProcess(input);

		
		break;
	}
	case GLUT_KEY_UP:
	{
		input = UP;
		NetworkProcess(input);

		break;
	}
	default:
		break;
	}
	drawScene();
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


// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}