#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdlib.h>
#include<iostream>
#define SERVERPORT 9000
#define BUFSIZE    50
#define BOARD_SCALE 50

using namespace std;

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

	

	Pos& Move(int key)
	{
		switch (key)
		{
		case UP:
		{
			if (CurrentLocation.y < BOARD_SCALE * 4)
				CurrentLocation.y += BOARD_SCALE;
			break;
		}
		case DOWN:
		{
			if (CurrentLocation.y > BOARD_SCALE * -3)
				CurrentLocation.y -= BOARD_SCALE;
			break;
		}
		case LEFT:
		{
			if (CurrentLocation.x > BOARD_SCALE * -4)
				CurrentLocation.x -= BOARD_SCALE;
			break;
		}
		case RIGHT:
		{
			if (CurrentLocation.x < BOARD_SCALE * 3)
				CurrentLocation.x += BOARD_SCALE;
			break;
		}
		default:
			break;
		}

		return CurrentLocation;
	}

};



Pawn P;

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf,msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
//	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
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




int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit(const_cast<char*>("socket()"));

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("bind()"));

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("listen()"));

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int len;
	Pos position;
	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display(const_cast<char*>("accept()"));
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		//printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		//	inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		cout << "[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = " << inet_ntoa(clientaddr.sin_addr) <<"��Ʈ ��ȣ= "<< ntohs(clientaddr.sin_port) << endl;
		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			// ������ �ޱ�(���� ����)
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char*>("recv()"));
				break;
			}
			else if (retval == 0)
				break;
			position = P.Move(len);
			// ������ �ޱ�(���� ����)
			retval = send(client_sock, (char *)&position, sizeof(Pos), 0);
		}

		// closesocket()
		closesocket(client_sock);
		//printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		//	inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}