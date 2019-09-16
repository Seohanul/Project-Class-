#include "stdafx.h"
#include "Network.h"

Network Nw;

Network::Network()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("winsock initialize error");

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit(const_cast<char*>("socket()"));

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("bind()"));

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("listen()"));
}

Network::~Network()
{

}

void Network::err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void Network::err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void Network::AcceptClients()
{
	while (EnteredClientsNum < MAXPLAYER)
	{
		// 데이터 통신에 사용할 변수
		SOCKADDR_IN clientaddr;
		int addrlen;

		SOCKET client_sock;

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display(const_cast<char*>("accept()"));
			continue;
		}

		clientSockets.emplace_back(client_sock);

		retval = send(client_sock, (char*)&EnteredClientsNum, sizeof(EnteredClientsNum), 0);
		EnteredClientsNum++;
		if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));
		PTHREAD_START_ROUTINE;
		HANDLE hThread = CreateThread(NULL, 0, [](LPVOID)->DWORD {Nw.testFunc(); return 0; }, 0, 0, NULL);

		CloseHandle(hThread);

	}

	//#pragma region 최우진 코드
	//while (1)
	//{
	//	if (gameStart)
	//		break;

	//	if (CurrentClientNum < MAXPLAYER) {
	//		SOCKADDR_IN clientaddr;
	//		int addrlen;
	//		SOCKET client_sock;

	//		// accept()
	//		addrlen = sizeof(clientaddr);
	//		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
	//		if (client_sock == INVALID_SOCKET) {
	//			err_display(const_cast<char*>("accept()"));
	//			continue;
	//		}

	//		standBySockets.emplace_back(client_sock);
	//		STD cout << STD endl << "FileSender 접속 : IP 주소 = " << inet_ntoa(clientaddr.sin_addr) << ", 포트 번호= " << ntohs(clientaddr.sin_port) << STD endl;
	//		STD cout << client_sock << STD endl;
	//		PTHREAD_START_ROUTINE;
	//		HANDLE hThread = CreateThread(NULL, 0, [](LPVOID)->DWORD {Nw.testFunc(); return 0; }, 0, 0, NULL);
	//		CloseHandle(hThread);
	//		CurrentClientNum++;
	//	}
	//}
	//#pragma endregion
}

void Network::SendRenderData()
{
	for (auto client_sock : clientSockets) {
		SOCKADDR_IN clientaddr;
		int addrlen;

		addrlen = sizeof(clientaddr);
		getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

		int vecSize = rendererData.size();

		retval = send(client_sock, (char*)&vecSize, sizeof(vecSize), 0);
		if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));


		retval = send(client_sock, (char*)&rendererData[0], sizeof(RenderData) * rendererData.size(), 0);
		if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));
	}
	rendererData.clear();
}

void Network::testFunc()
{
	SOCKET client_sock = clientSockets[CurrentClientNum++];
	while (1) {
		static float FixedRecvTime = 0;
		FixedRecvTime += UPDATE_TIME;
		if (FixedRecvTime >= 1.0 / 30.f)
		{
			SOCKADDR_IN clientaddr;
			int addrlen;
			addrlen = sizeof(clientaddr);
			getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

			int retval = 0;

			KeyData k = { 0, };
			retval = recv(client_sock, (char *)&k, sizeof(k), 0);
			if (retval == SOCKET_ERROR) {
				err_display(const_cast<char*>("recv()"));
				closesocket(client_sock);
				continue;
			}
			// 스레드별로 변수 필요함 (순서 0, 1, 2 처럼) = clientNum 
			// 받은 key 큐에다 넣기 
			//STD cout << k.key << "\t" << k.pressed << "\t" << k.clientNum << STD endl;
			InputQueue.push(k);
		}
	}

	//	#pragma region 최우진 코드
	//	UserData receivedPlayerData;
	//	receivedPlayerData.sceneNumber = 0;
	//
	//	SOCKET client_sock = standBySockets[CurrentClientNum - 1];
	//	bool hasActorNumber = false;
	//
	//	while (receivedPlayerData.sceneNumber == LOGINSCENE) {
	//		if (gameStart)
	//			return;
	//
	//		system("cls");
	//		STD cout << "Login" << STD endl;
	//		retval = recvn(client_sock, (char *)&receivedPlayerData, sizeof(receivedPlayerData), 0);
	//		if (retval == SOCKET_ERROR) {
	//			break;
	//		}
	//
	//		ReadUserData();
	//
	//		if (receivedPlayerData.loginState == SIGNUP)
	//			WriteUserData(receivedPlayerData);
	//
	//		else if (receivedPlayerData.loginState == SIGNIN)
	//			CheckUserData(receivedPlayerData, client_sock);
	//
	//		retval = send(client_sock, (char *)&receivedPlayerData, sizeof(receivedPlayerData), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display((char*)"send()");
	//			break;
	//		}
	//
	//		if (receivedPlayerData.sceneNumber == LOBBYSCENE) {
	//			receivedPlayerData.sceneNumber = LOBBYSCENE;
	//			break;
	//		}
	//	}
	//
	//	while (receivedPlayerData.sceneNumber == LOBBYSCENE)
	//	{
	//		if (gameStart)
	//			return;
	//
	//		if (!hasActorNumber) {
	//			receivedPlayerData.actorNumber = lobbyClientsNum;
	//			tempVECT.emplace_back(receivedPlayerData);
	//			hasActorNumber = true;
	//
	//			retval = send(client_sock, (char *)&receivedPlayerData, sizeof(receivedPlayerData), 0);
	//			if (retval == SOCKET_ERROR) {
	//				break;
	//			}
	//		}
	//
	//		if (lobbyClientsNum == MAXPLAYER - 1) {
	//			timeCount -= 0.01f;
	//			STD cout << timeCount << STD endl;
	//
	//			if (timeCount <= 0) {
	//				receivedPlayerData.sceneNumber = INGAMESCENE;
	//				retval = send(client_sock, (char *)&receivedPlayerData, sizeof(receivedPlayerData), 0);
	//				if (retval == SOCKET_ERROR) {
	//					break;
	//				}
	//
	//				timeCount = 0;
	//				gameStart = true;
	//				break;
	//			}
	//		}
	//	}
	//
	//	while (receivedPlayerData.sceneNumber == INGAMESCENE)
	//	{
	//		SOCKADDR_IN clientaddr;
	//		int addrlen;
	//		addrlen = sizeof(clientaddr);
	//		getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	//
	//		int retval = 0;
	//
	//		KeyData k = { 0, };
	//		retval = recvn(client_sock, (char *)&k, sizeof(k), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display(const_cast<char*>("recv()"));
	//			closesocket(client_sock);
	//			continue;
	//		}
	//
	//		STD cout << k.key << "\t" << k.pressed << "\t" << k.clientNum << STD endl;
	//		InputQueue.push(k);
	//	}
	//#pragma endregion
	closesocket(client_sock);
}

void Network::ReadUserData()
{
	char buffer[255];
	STD ifstream readFile("UserDataFile.txt");
	savedPlayerData->index = -1;

	try {
		if (readFile.is_open())
		{
			while (!readFile.eof()) {
				savedPlayerData->index++;

				readFile.getline(buffer, sizeof(char) * 256);
				strcpy(savedPlayerData[savedPlayerData->index].name, buffer);

				readFile.getline(buffer, sizeof(char) * 256);
				strcpy(savedPlayerData[savedPlayerData->index].nickname, buffer);

				readFile.getline(buffer, sizeof(char) * 256);
				strcpy(savedPlayerData[savedPlayerData->index].password, buffer);
			}
			readFile.close();
		}
		else
		{
			throw false;
		}
	}
	catch (bool)
	{
		noSavedFile = true;
	}
}

void Network::WriteUserData(UserData& player)
{
	STD ofstream writeFile("UserDataFile.txt", STD ios::app);

	if (noSavedFile)
	{
		player.isAlreadyExist = false;
		noSavedFile = false;
	}

	else {
		for (int i = 0; i < savedPlayerData->index; i++)
		{
			if (!strcmp(player.nickname, savedPlayerData[i].nickname)) {
				player.isAlreadyExist = true;
				return;
			}
			else {
				player.isAlreadyExist = false;
			}
		}
	}

	if (!player.isAlreadyExist)
	{
		if (writeFile.is_open())
		{
			writeFile << player.name << STD endl;
			writeFile << player.nickname << STD endl;
			writeFile << player.password << STD endl;
			writeFile.close();
		}
		STD cout << "아이디를 성공적으로 저장했습니다." << STD endl;
	}

	else
		STD cout << "아이디가 중복 됩니다." << STD endl;
}

void Network::CheckUserData(UserData& player, SOCKET& sock)
{
	bool loggedIn = false;

	for (auto loggedInUser : loginUser)
	{
		if (!strcmp(loggedInUser.nickname, player.nickname)) {
			loggedIn = true;
			player.isAlreadyLoggedIn = true;
		}
	}

	for (int i = 0; i < savedPlayerData->index; i++)
	{
		if (!strcmp(player.nickname, savedPlayerData[i].nickname) && !loggedIn)
		{
			if (!strcmp(player.password, savedPlayerData[i].password))
			{
				lobbyClientsNum++;
				player.sceneNumber = LOBBYSCENE;
				clientSockets.emplace_back(sock);
				loginUser.emplace_back(player);
			}
		}
	}
}

int Network::recvn(SOCKET s, char * buf, int len, int flags)
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