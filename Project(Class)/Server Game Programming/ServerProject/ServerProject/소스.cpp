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




// 동기식 IO에는 버퍼를 아무렇게나 해놔도 상관없고, 하나여도 상관없지만,
// 비동기식 IO에는 버퍼를 소켓별로 따로 만들어놔야함 

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);


// 소켓 함수 오류 출력
void err_display(char *msg);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);



struct ClientPos
{
	SOCKET sock;
	Pos position;
};


ClientPos CP;
struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;		//Overlapped 구조체도 recv할때 하나씩 넘겨줘야함 소켓별로 하나씩)
	WSABUF senddataBuffer;				//WSABUF도 하나씩 있어야함
	WSABUF recvdataBuffer;				//WSABUF도 하나씩 있어야함
	SOCKET socket;
	int Input;
	Pos pos;
	int receiveBytes;
	int sendBytes;
};



map <SOCKET, SOCKETINFO> clients; // 맵으로 묶어놓음
map <SOCKET, Pawn*> MyCharaters;
//Pawn P;


int main()
{
	// Winsock Start - windock.dll 로드
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "Error - Can not load 'winsock.dll' file\n";
		return 1;
	}

	// 1. 소켓생성  
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED); //  WSA_FLAG_OVERLAPPED flag 무조건 해줘야함
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		return 1;
	}

	// 서버정보 객체설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. 소켓설정
	if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error - Fail bind\n");
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return 1;
	}

	// 3. 수신대기열생성
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		printf("Error - Fail listen\n");
		// 6. 소켓종료
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
		// 초기화
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

		// 콜백함수에는 ( 에러, numberofbyte, overlapped) 이렇게만 있으므로 어떤 소켓에서 온지를 알 수가 없음.
		// 그래서 이벤트에다가 0을 넣어줘야하지만 거기다가 소켓을 넣어줌
		// 콜백을 사용할거기 때문에 event에 뭐가있는지 상관 안함. 어차피 안쓰는거 그냥 이용함
		// 중첩 소캣을 지정하고 완료시 실행될 함수를 넘겨준다. 
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

	// 6-2. 리슨 소켓종료
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return 0;
}

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	// 사실 람다로 하는게 더편함 나중에해보기
	// 아까 넣은 이벤트에서 받아온 소켓으로 소켓 구별함
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);

	DWORD sendBytes = 0;
	DWORD receiveBytes = 0;
	DWORD flags = 0;

	if (dataBytes == 0) // 클라이언트에서 closesocket했을 경우
	{
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	}  // 클라이언트가 closesocket을 했을 경우

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
	// 성능 생각한 코드는 아님, recv가 send를 기다리면 안되고 그냥 바로바로 recv 해야함
	// recv 용 send용 버퍼, 구조체 다 따로 있어야함 
	// 받은내용 바로 send
	

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
	}  // 클라이언트가 closesocket을 했을 경우

	{
		/*for (auto& c : clients)
		{
			if (client_s != c.first)
				int sendBytes = send(c.first, (char*)&CP, sizeof(ClientPos), 0);
		}*/


		// WSASend(응답에 대한)의 콜백일 경우
		clients[client_s].senddataBuffer.len = sizeof(int);
		clients[client_s].senddataBuffer.buf = (char*)&clients[client_s].Input;

		memset(&(clients[client_s].overlapped), 0x00, sizeof(WSAOVERLAPPED));
		clients[client_s].overlapped.hEvent = (HANDLE)client_s;
		// send 끝나면 recv또 호출 
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

// 소켓 함수 오류 출력
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