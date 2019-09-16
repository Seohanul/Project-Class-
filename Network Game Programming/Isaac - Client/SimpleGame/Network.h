#pragma once
#include "Renderer.h"
#pragma pack(1)
struct Pos {
	int x;
	char c;
	int y;
};
#pragma pack()

struct RenderData2 {
	DX XMFLOAT2 Current;
	DX XMFLOAT2 Total;
	DX XMFLOAT3 SpriteOffset;
	DX XMFLOAT2 SpriteSize;
	size_t TextureID = -1;
};

struct RenderData {
	DX XMFLOAT3 Position;
	DX XMFLOAT4 Color;
	RenderData2 RenderDataSet[3];
};

struct KeyData {
	int key;
	int clientNum;
	bool pressed;
};

struct UserData {
	char name[255];
	char nickname[255];
	char password[255];

	int loginState;
	int sceneNumber;
	int index;
	int actorNumber;

	bool isAlreadyExist;
	bool isAlreadyLoggedIn;
};

class Network
{
public:
	Network();
	~Network();

	void err_quit(char *msg);
	void err_display(char *msg);
	//static DWORD WINAPI ProcessClient(LPVOID arg);
	//void recvstart();
	//void testFunc(SOCKET sock);a
	void SendInput(KeyData k);
	void ReceiveRenderData();
	STD vector<DX XMFLOAT3> Positions;
	STD vector <RenderData> rendererData;
	void Init();

	Renderer RenderDevice;
	int TEX[5];
	SOCKET clientSock;
	int retval;
	int recvn(SOCKET s, char * buf, int len, int flags);
	int ClientNum;

	int vecSize = 0;
};

struct Arg {
	Network* p;
	SOCKET sock;
};

extern Network Nw;