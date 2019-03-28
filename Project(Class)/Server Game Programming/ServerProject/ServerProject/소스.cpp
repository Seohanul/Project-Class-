#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdlib.h>
#include<iostream>
#include <map>
#include <vector>
#include "Pawn.h"
#define SERVERPORT 9000
#define BUFSIZE    50
#define MAX_BUFFER        1024
#define SERVER_PORT        3500

using namespace std;




// ����� IO���� ���۸� �ƹ����Գ� �س��� �������, �ϳ����� ���������,
// �񵿱�� IO���� ���۸� ���Ϻ��� ���� ���������� 

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);


// ���� �Լ� ���� ���
void err_display(char *msg);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);



struct ClientPos
{
	SOCKET sock;
	Pos position;
};


ClientPos CP;
struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;		//Overlapped ����ü�� recv�Ҷ� �ϳ��� �Ѱ������ ���Ϻ��� �ϳ���)
	WSABUF senddataBuffer;				//WSABUF�� �ϳ��� �־����
	WSABUF recvdataBuffer;				//WSABUF�� �ϳ��� �־����
	SOCKET socket;
	int Input;
	Pos pos;
	int receiveBytes;
	int sendBytes;
};



map <SOCKET, SOCKETINFO> clients; // ������ �������
map <SOCKET, Pawn*> MyCharaters;
//Pawn P;


int main()
{
	// Winsock Start - windock.dll �ε�
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "Error - Can not load 'winsock.dll' file\n";
		return 1;
	}

	// 1. ���ϻ���  
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED); //  WSA_FLAG_OVERLAPPED flag ������ �������
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		return 1;
	}

	// �������� ��ü����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. ���ϼ���
	if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error - Fail bind\n");
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}

	// 3. ���Ŵ�⿭����
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		printf("Error - Fail listen\n");
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	DWORD flags;

	while (1)
	{
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			return 1;
		}
		int num = clientSocket;
		int sendBytes = send(clientSocket, (char *)&num, sizeof(int), 0);
		// �ʱ�ȭ
		MyCharaters[clientSocket] = { new Pawn() };
	
		int vecSize = MyCharaters.size();
		int retval = send(clientSocket, (char*)&vecSize, sizeof(vecSize), 0);
		if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));

		for (auto & pawn : MyCharaters)
		{
			ClientPos cp;
			cp.sock = pawn.first;
			cp.position = pawn.second->GetCurrentLocation();
			retval = send(clientSocket, (char*)&cp, sizeof(ClientPos), 0);
			if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));
		}
		

		//ClientPos cp= { clients[clientSocket].socket, clients[clientSocket].pos };
		clients[clientSocket] = SOCKETINFO{};
		memset(&clients[clientSocket], 0x00, sizeof(struct SOCKETINFO));
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].recvdataBuffer.len = sizeof(int);
		clients[clientSocket].recvdataBuffer.buf = (char*)&clients[clientSocket].Input;
		flags = 0;

		// �ݹ��Լ����� ( ����, numberofbyte, overlapped) �̷��Ը� �����Ƿ� � ���Ͽ��� ������ �� ���� ����.
		// �׷��� �̺�Ʈ���ٰ� 0�� �־���������� �ű�ٰ� ������ �־���
		// �ݹ��� ����Ұű� ������ event�� �����ִ��� ��� ����. ������ �Ⱦ��°� �׳� �̿���
		// ��ø ��Ĺ�� �����ϰ� �Ϸ�� ����� �Լ��� �Ѱ��ش�. 
		clients[clientSocket].overlapped.hEvent = (HANDLE)clients[clientSocket].socket;



		if (WSARecv(clients[clientSocket].socket, &clients[clientSocket].recvdataBuffer, 1, NULL, &flags, &(clients[clientSocket].overlapped), recv_callback))
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("Error - IO pending Failure\n");
				return 1;
			}
		}
		else {
			cout << "Non Overlapped Recv return.\n";
			return 1;
		}
	}

	// 6-2. ���� ��������
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return 0;
}

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	// ��� ���ٷ� �ϴ°� ������ ���߿��غ���
	// �Ʊ� ���� �̺�Ʈ���� �޾ƿ� �������� ���� ������
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);

	DWORD sendBytes = 0;
	DWORD receiveBytes = 0;
	DWORD flags = 0;

	if (dataBytes == 0) // Ŭ���̾�Ʈ���� closesocket���� ���
	{
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	}  // Ŭ���̾�Ʈ�� closesocket�� ���� ���

	cout << clients[client_s].Input << endl;

	CP.position = MyCharaters[client_s]->Move(clients[client_s].Input);
	CP.sock = client_s;


	for (auto& client : clients)
	{
		client.second.senddataBuffer.len = sizeof(ClientPos);
		client.second.senddataBuffer.buf = (char*)&CP;
		memset(&(client.second.overlapped), 0x00, sizeof(WSAOVERLAPPED));
		client.second.overlapped.hEvent = (HANDLE)client.first;


		if (WSASend(client.first, &(client.second.senddataBuffer), 1, &dataBytes, 0, &(client.second.overlapped), send_callback) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("Error - Fail WSASend(error_code : %d)\n", WSAGetLastError());
			}
		}

	}

	/*clients[client_s].senddataBuffer.len = sizeof(ClientPos);
	clients[client_s].senddataBuffer.buf = (char*)&CP;
	memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;

*/
	// ���� ������ �ڵ�� �ƴ�, recv�� send�� ��ٸ��� �ȵǰ� �׳� �ٷιٷ� recv �ؾ���
	// recv �� send�� ����, ����ü �� ���� �־���� 
	// �������� �ٷ� send
	

}

void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	DWORD sendBytes = 0;
	DWORD receiveBytes = 0;
	DWORD flags = 0;

	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);

	if (dataBytes == 0)
	{
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	}  // Ŭ���̾�Ʈ�� closesocket�� ���� ���

	{
		/*for (auto& c : clients)
		{
			if (client_s != c.first)
				int sendBytes = send(c.first, (char*)&CP, sizeof(ClientPos), 0);
		}*/


		// WSASend(���信 ����)�� �ݹ��� ���
		clients[client_s].senddataBuffer.len = sizeof(int);
		clients[client_s].senddataBuffer.buf = (char*)&clients[client_s].Input;

		memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
		clients[client_s].overlapped.hEvent = (HANDLE)client_s;
		// send ������ recv�� ȣ�� 
		if (CP.sock == client_s)
		{
			if (WSARecv(client_s, &clients[client_s].senddataBuffer, 1, &receiveBytes, &flags, &(clients[client_s].overlapped), recv_callback) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
				}
			}
		}
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