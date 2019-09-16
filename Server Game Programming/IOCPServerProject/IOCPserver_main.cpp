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
#include <unordered_set>
#include "Protocol.h"
using namespace std;

#include <winsock2.h>
#include <thread>
#include <mutex>

#include <vector>
#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024
#define START_X	4
#define START_Y	4
#define VIEW_RADIUS         3
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
	char packet_buf[MAX_BUFFER];		// �� ���޾��� ����� ����
	int prev_size;
	int x, y;
	mutex vlm;
	unordered_set<int> viewlist;
};

// ����� IO���� ���۸� �ƹ����Գ� �س��� �������, �ϳ����� ���������,
// �񵿱�� IO���� ���۸� ���Ϻ��� ���� ���������� 


SOCKETINFO clients[MAX_USER]; // ������ �������
HANDLE g_iocp;

void initialize()
{
	for (auto &cl : clients)
		cl.connected = false;
}

char get_new_id()
{
	while(true)
	for (int i = 0; i < MAX_USER; ++i)
		if (clients[i].connected == false) {
			clients[i].connected = true;
			return i;
		}
}

void error_display(const char *msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf, 0, NULL);
	cout << msg;
	wcout << L"���� " << lpMsgBuf <<"�ڵ� :  "<< err_no<< endl;
	LocalFree(lpMsgBuf);
	while (true)
	{
	}
}

void send_packet(int key, char* packet)
{
	SOCKET client_s = clients[key].socket;

	
	OVER_EX * over = new OVER_EX{};
//	OVER_EX *over = reinterpret_cast<OVER_EX *>(malloc(sizeof(OVER_EX)));
	// reinterpret_cast<OVER_EX *>(malloc(sizeof(
	over->dataBuffer.len = packet[0];
	over->dataBuffer.buf = over->messageBuffer;
	memcpy(over->messageBuffer, packet, packet[0]);
	ZeroMemory(&(over->overlapped), sizeof(WSAOVERLAPPED));
	over->is_recv = false;

	if (WSASend(client_s, &over->dataBuffer, 1, NULL, 0, &(over->overlapped), NULL) == SOCKET_ERROR)
	{
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING)
		{
			//cout << "Error - Fail WSASend(error_code : " << WSAGetLastError() << endl;
			error_display("ERROR_SEND", err_no);
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

void send_login_ok_packet(char to)
{
	sc_packet_login_ok packet;
	packet.id = to;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	
	send_packet(to, reinterpret_cast<char*>(&packet));
}


void send_put_player_packet(char to, char obj)
{
	sc_packet_put_player packet;
	packet.id = obj;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;
	packet.x = clients[obj].x;
	packet.y = clients[obj].y;

	send_packet(to, reinterpret_cast<char*>(&packet));
}


void send_remove_player_packet(char to, char id)
{
	sc_packet_remove_player packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;

	send_packet(to, reinterpret_cast<char*>(&packet));
}

bool Is_Near_Object(int a, int b)
{
	if (VIEW_RADIUS < abs(clients[a].x - clients[b].x))
		return false;

	if (VIEW_RADIUS < abs(clients[a].y - clients[b].y))
		return false;

	return true;
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
		if(true == clients[i].connected)
			send_pos_packet(i, id);


	clients[id].vlm.lock();
	//if (0 != clients[id].viewlist.size())
	unordered_set<int> old_viewlist = clients[id].viewlist;
	clients[id].vlm.unlock();
	unordered_set<int> new_viewlist;

	for (int i = 0; i < MAX_USER; ++i) {
		if ((true == clients[i].connected) && (true == Is_Near_Object(id, i) && (i != id)))
			new_viewlist.insert(i);
	}   // MAX_USER ��ŭ ������ ���鼭 ���� Ŭ���� �þ߿� � Ŭ�� ���Դ��� üũ�ؼ� new_list�� �־���
	// �״ϱ� �̹� new list���� ���þ߿� �����ִ� ģ������ �� ��������

	for (auto cl : new_viewlist) {

		if (0 != old_viewlist.count(cl)) { // oldviewlist�� cl�� ��� ������?
			clients[cl].vlm.lock();
			if (0 != clients[cl].viewlist.count(id)) // �� Ŭ���� viewlist�� ���� �ִٸ�
				send_pos_packet(cl, id);         // �� Ŭ�󿡰� ���� �̵��Ѱ� �˷����
			else {                           // �� Ŭ���� �þ߿� ���� ���ٸ�? ��, �� �þ߿��� �� Ŭ�� �ִµ� �� Ŭ�� �þ߿��� ���� ���°� �̹Ƿ� �߰�������� (��, ���� �����̸鼭 �þ߿� ���� Ŭ�� ����)
				clients[cl].viewlist.insert(id);  // �� Ŭ�� �þ߿��ٰ� ���� �߰������
				send_put_player_packet(cl, id); // �׸��� ��� ���� �þ߿� ���°Ŵϱ� �÷��̾ �߰��������
			}
			clients[cl].vlm.unlock();
		}
		else // �����̱� ���� �� �þ߿� Ŭ�� �����ٸ�? ��, ���� ���� �߰��ؾߵǴ� ��Ȳ
		{
			clients[id].vlm.lock();
			//      old_viewlist.insert(cl);
			   //   clients[id].viewlist.insert(cl);
				  //clients[id].viewlist.emplace(cl);
			clients[id].viewlist.insert(cl);
			send_put_player_packet(id, cl);
			if (0 != clients[cl].viewlist.count(id))
				send_pos_packet(cl, id);
			else {
				clients[cl].viewlist.insert(id);
				send_put_player_packet(cl, id);
			}
			clients[id].vlm.unlock();
		}
	}

	for (auto cl : old_viewlist) {      //���⼱ �� �Ұ�����
		if (0 != new_viewlist.count(cl)) continue;  // �̷������� �ϴ°ǵ� ���⼭�� erase�ؾ���
		send_remove_player_packet(cl, id);
		send_remove_player_packet(id, cl);
	}



}

void do_recv(char id)
{
	cout<<"recv"<<endl;
	DWORD flags = 0;
	DWORD len;

	SOCKET client_s = clients[id].socket;

	OVER_EX * over = &clients[id].over;

	over->dataBuffer.len = MAX_BUFFER;
	over->dataBuffer.buf = over->messageBuffer;
	ZeroMemory(&(over->overlapped), sizeof(WSAOVERLAPPED));
	// send ������ recv�� ȣ�� 
	if (WSARecv(client_s, &over->dataBuffer, 1, NULL, &flags, &(over->overlapped), NULL) == SOCKET_ERROR)
	{
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING)
		{
			error_display("ERROR_RECV", err_no);

			//cout << "Error - Fail WSARecv(error_code : " << WSAGetLastError() << endl;
		}
	}

}


void disconnect(int id)
{
	for (int i = 0; i < MAX_USER; ++i) {
		if (false == clients[i].connected) continue;
		send_remove_player_packet(i, id);
	}
	closesocket(clients[id].socket);
	clients[id].viewlist.clear();
	clients[id].connected = false;
}

void worker_thread()
{
	while (true)
	{
		DWORD io_byte;
		ULONG key;
		OVER_EX * lpover_ex;
		BOOL is_error = GetQueuedCompletionStatus(g_iocp, &io_byte, &key, reinterpret_cast<LPWSAOVERLAPPED *>(&lpover_ex), INFINITE);

		if (FALSE == is_error)
			error_display("GQCS", WSAGetLastError());

		if (0 == io_byte) disconnect(key);

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

	// Winsock Start - windock.dll �ε�
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		cout << "Error - Can not load 'winsock.dll' file\n";
		return ;
	}

	// 1. ���ϻ���  
	SOCKET listenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED); //  WSA_FLAG_OVERLAPPED flag ������ �������
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Error - Invalid socket\n";
		return ;
	}

	// �������� ��ü����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. ���ϼ���
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		cout << "Error - Fail bind\n";
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		return ;
	}

	// 3. ���Ŵ�⿭����
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		cout << "Error - Fail listen\n";
		// 6. ��������
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

	while (true)
	{
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			cout << "Error - Accept Failure\n";
			return ;
		}

		char new_id = get_new_id();

		// �ʱ�ȭ
		
		memset(&clients[new_id], 0x00, sizeof(struct SOCKETINFO));
		clients[new_id].socket = clientSocket;
		clients[new_id].over.dataBuffer.len = MAX_BUFFER;
		clients[new_id].over.dataBuffer.buf = clients[clientSocket].over.messageBuffer;
		clients[new_id].over.is_recv = true;
		clients[new_id].x = START_X;
		clients[new_id].y = START_Y;
		clients[new_id].viewlist.clear();
		flags = 0;

		// recv �ϱ����� IOCP�� �־����

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), g_iocp, new_id, 0);
		clients[new_id].connected = true;

		send_login_ok_packet(new_id);
		for (int i = 0; i < MAX_USER; ++i) {
			if (false == clients[i].connected) continue;
			send_put_player_packet(i, new_id);
		}
		for (int i = 0; i < MAX_USER; ++i) {
			if (false == clients[i].connected) continue;
			if (i == new_id) continue;
			send_put_player_packet(new_id, i);
		}

		do_recv(new_id); 
	}

	// 6-2. ���� ��������
	closesocket(listenSocket);

	// Winsock End
	WSACleanup();

	return;
}

int main()
{
	vector<thread> worker_threads;
	_wsetlocale(LC_ALL, L"korean");
	wcout.imbue(locale("korean"));
	initialize();

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


