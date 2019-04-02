/*
## 소켓 서버 : 1 v n - overlapped callback
1. socket()            : 소켓생성
2. bind()            : 소켓설정
3. listen()            : 수신대기열생성
4. accept()            : 연결대기
5. read()&write()
WIN recv()&send    : 데이터 읽고쓰기
6. close()
WIN closesocket    : 소켓종료
*/

#include <iostream>
#include <map>
#include "Protocol.h"
using namespace std;

#include <winsock2.h>
#include <thread>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024
#define START_X	4
#define START_Y	4

struct OVER_EX
{
	WSAOVERLAPPED overlapped;		
	WSABUF dataBuffer;
	char messageBuffer[MAX_BUFFER];	
	bool is_recv;
};

struct SOCKETINFO
{
	bool	connected;
	OVER_EX	over;
	SOCKET socket;
	char packet_buf[MAX_BUFFER];		// 다 못받았을 경우의 버퍼
	int prev_size;
	int x, y;
};

// 동기식 IO에는 버퍼를 아무렇게나 해놔도 상관없고, 하나여도 상관없지만,
// 비동기식 IO에는 버퍼를 소켓별로 따로 만들어놔야함 


SOCKETINFO clients[MAX_USER]; // 맵으로 묶어놓음
HANDLE g_iocp;

void initialize()
{
	for (auto &cl : clients)
		cl.connected = false;
}

char get_new_id()
{
	for (int i = 0; i < MAX_USER; ++i)
		if (clients[i].connected == false) {
			clients[i].connected = true;
			return i;
		}
}

void send_packet(int key, char* packet)
{
	SOCKET client_s = clients[key].socket;

	
	OVER_EX * over = new OVER_EX;

	over->dataBuffer.len = packet[0];
	over->dataBuffer.buf = over->messageBuffer;
	memcpy(over->messageBuffer, packet, packet[0]);
	ZeroMemory(&(over->overlapped), 0x00, sizeof(WSAOVERLAPPED));
	
	if (WSASend(client_s, &over->dataBuffer, 1, NULL, 0, &(over->overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "Error - Fail WSASend(error_code : " << WSAGetLastError() << endl;
		}
	}


}



void send_pos_packet(char to, char obj)
{
	sc_packet_pos packet;
	packet.id = obj;
	packet.size = sizeof(packet);
	packet.type = SC_POS;
	packet.x = clients[obj].x;
	packet.y = clients[obj].y;

	send_packet(to, reinterpret_cast<char*>(&packet));
}


void process_packet(char id, char* buf)
{
	cs_packet_up * packet = reinterpret_cast<cs_packet_up *> (buf);

	char x = clients[id].x;
	char y = clients[id].y;

	switch (packet->type)
	{
	case CS_UP: 
		--y;
		if (y < 0) y = 0;
		break;
	case CS_DOWN:
		++y;
		if (y >= WORLD_HEIGHT) y = WORLD_HEIGHT -1;
		break;
	case CS_LEFT:
		if (0 < x)
			x--;
		break;
	case CS_RIGHT:
		if (WORLD_WIDTH - 1 > x)
			x++;
		break;
	default:
		cout << "Unknown Packet Type Error\n";
		while (true);
		
	}
	clients[id].x = x;
	clients[id].y = y;


	for (int i = 0; i < MAX_USER; ++i)
		if(true == clients[id].connected)
			send_pos_packet(i, id);
}

void do_recv(char id)
{
	DWORD flags = 0;

	SOCKET client_s = clients[id].socket;

	OVER_EX * over = &clients[id].over;

	over->dataBuffer.len = MAX_BUFFER;
	over->dataBuffer.buf = over->messageBuffer;
	ZeroMemory(&(over->overlapped), 0x00, sizeof(WSAOVERLAPPED));
	// send 끝나면 recv또 호출 
	if (WSARecv(client_s, &over->dataBuffer, 1, NULL, &flags, &(over->overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			cout << "Error - Fail WSARecv(error_code : " << WSAGetLastError() << endl;
		}
	}

}




void worker_thread()
{
	while (true)
	{
		DWORD io_byte;
		ULONG key;
		OVER_EX * lpover_ex;
		bool is_error = GetQueuedCompletionStatus(g_iocp, &io_byte, &key, reinterpret_cast<LPWSAOVERLAPPED *>(&lpover_ex), INFINITE);


		if (lpover_ex->is_recv) {
			int rest_size = io_byte;
			char* ptr = lpover_ex->messageBuffer;
			char packet_size = 0;

			if (0 < clients[key].prev_size)
				packet_size = clients[key].packet_buf[0];

			while (rest_size > 0) {
				if (0 == packet_size)
					packet_size = ptr[0];
				int required = packet_size - clients[key].prev_size;
				if (rest_size >= required) {
					memcpy(clients[key].packet_buf + clients[key].prev_size, ptr, required);
					process_packet(key, clients[key].packet_buf);
					rest_size -= required;
					ptr += required;
					packet_size = 0;
				}
				else {
					memcpy(clients[key].packet_buf + clients[key].prev_size, ptr, rest_size);
					rest_size = 0;
				}

			}
			do_recv(key);
		}
		else {
			delete lpover_ex;
		}

	}
}

void do_accept()
{

	// Winsock Start - windock.dll 로드
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "Error - Can not load 'winsock.dll' file\n";
		return ;
	}

	// 1. 소켓생성  
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED); //  WSA_FLAG_OVERLAPPED flag 무조건 해줘야함
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Error - Invalid socket\n";
		return ;
	}

	// 서버정보 객체설정
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. 소켓설정
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "Error - Fail bind\n";
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return ;
	}

	// 3. 수신대기열생성
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		cout << "Error - Fail listen\n";
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return ;
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
			cout << "Error - Accept Failure\n";
			return ;
		}

		char new_id = get_new_id();

		// 초기화
		
		memset(&clients[new_id], 0x00, sizeof(struct SOCKETINFO));
		clients[new_id].socket = clientSocket;
		clients[new_id].over.dataBuffer.len = MAX_BUFFER;
		clients[new_id].over.dataBuffer.buf = clients[clientSocket].over.messageBuffer;
		clients[new_id].x = START_X;
		clients[new_id].y = START_X;
		flags = 0;

		// recv 하기전에 IOCP에 넣어야함

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, new_id, 0);


		do_recv(new_id);
	}

	// 6-2. 리슨 소켓종료
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

}

int main()
{
	initialize();
	vector<thread> worker_threads;

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	for (int i = 0; i < 4; ++i)
		worker_threads.emplace_back(thread{ worker_thread });
	thread accept_thread{ do_accept };

	accept_thread.join();
	for (auto&th : worker_threads)
		th.join();
	CloseHandle(g_iocp);


	return 0;
}


