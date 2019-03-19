/*
## ���� ���� : 1 v n - overlapped callback
1. socket()            : ���ϻ���
2. bind()            : ���ϼ���
3. listen()            : ���Ŵ�⿭����
4. accept()            : ������
5. read()&write()
WIN recv()&send    : ������ �а���
6. close()
WIN closesocket    : ��������
*/

#include <iostream>
#include <map>

using namespace std;

#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024
#define SERVER_PORT        3500

struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;		//Overlapped ����ü�� recv�Ҷ� �ϳ��� �Ѱ������ ���Ϻ��� �ϳ���)
	WSABUF dataBuffer;				//WSABUF�� �ϳ��� �־����
	SOCKET socket;
	char messageBuffer[MAX_BUFFER];		// overlapped IO�ϱ� �ƿ� ���۰� ���� �־����
	int receiveBytes;
	int sendBytes;
};

// ����� IO���� ���۸� �ƹ����Գ� �س��� �������, �ϳ����� ���������,
// �񵿱�� IO���� ���۸� ���Ϻ��� ���� ���������� 


map <SOCKET, SOCKETINFO> clients; // ������ �������

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

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

		// �ʱ�ȭ
		clients[clientSocket] = SOCKETINFO{};
		memset(&clients[clientSocket], 0x00, sizeof(struct SOCKETINFO));
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].dataBuffer.len = MAX_BUFFER;
		clients[clientSocket].dataBuffer.buf = clients[clientSocket].messageBuffer;
		flags = 0;

		// �ݹ��Լ����� ( ����, numberofbyte, overlapped) �̷��Ը� �����Ƿ� � ���Ͽ��� ������ �� ���� ����.
		// �׷��� �̺�Ʈ���ٰ� 0�� �־���������� �ű�ٰ� ������ �־���
		// �ݹ��� ����Ұű� ������ event�� �����ִ��� ��� ����. ������ �Ⱦ��°� �׳� �̿���
		// ��ø ��Ĺ�� �����ϰ� �Ϸ�� ����� �Լ��� �Ѱ��ش�. 
		clients[clientSocket].overlapped.hEvent = (HANDLE)clients[clientSocket].socket;



		if (WSARecv(clients[clientSocket].socket, &clients[clientSocket].dataBuffer, 1, NULL, &flags, &(clients[clientSocket].overlapped), recv_callback))
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

	cout << "TRACE - Receive message : "
		<< clients[client_s].messageBuffer
		<< " (" << dataBytes << ") bytes)\n";

	clients[client_s].dataBuffer.len = dataBytes;
	memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	// ���� ������ �ڵ�� �ƴ�, recv�� send�� ��ٸ��� �ȵǰ� �׳� �ٷιٷ� recv �ؾ���
	// recv �� send�� ����, ����ü �� ���� �־���� 
	// �������� �ٷ� send
	if (WSASend(client_s, &(clients[client_s].dataBuffer), 1, &dataBytes, 0, &(clients[client_s].overlapped), send_callback) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("Error - Fail WSASend(error_code : %d)\n", WSAGetLastError());
		}
	}

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
		// WSASend(���信 ����)�� �ݹ��� ���
		clients[client_s].dataBuffer.len = MAX_BUFFER;
		clients[client_s].dataBuffer.buf = clients[client_s].messageBuffer;

		cout << "TRACE - Send message : "
			<< clients[client_s].messageBuffer
			<< " (" << dataBytes << " bytes)\n";
		memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
		clients[client_s].overlapped.hEvent = (HANDLE) client_s;
		// send ������ recv�� ȣ�� 
		if (WSARecv(client_s, &clients[client_s].dataBuffer, 1, &receiveBytes, &flags, &(clients[client_s].overlapped), recv_callback) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
			}
		}
	}
}
