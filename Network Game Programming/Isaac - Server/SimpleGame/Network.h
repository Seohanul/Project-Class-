#pragma once
#pragma pack(1)
struct Pos {
	int x;
	int y;
};
#pragma pack()


struct KeyData {
	int key = 0;
	int clientNum = 0;
	bool pressed = false;
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

class Network
{
public:
	Network();
	~Network();

	static Network GameNetwork;
	bool tempReady = false;
	void err_quit(char *msg);
	void err_display(char *msg);

	void AcceptClients();
	void SendRenderData();
	//	DWORD WINAPI ServerMain(LPVOID p);
	void testFunc();
	STD vector<DX XMVECTOR> Positions;
	STD vector <RenderData> rendererData;
	STD queue<KeyData> InputQueue;
	//STD vector<STD queue<KeyData>> InputQueues;
	int recvn(SOCKET s, char * buf, int len, int flags);
private:
	int CurrentClientNum = 0;

	STD vector<SOCKET> standBySockets;
	STD vector<SOCKET> clientSockets;
	STD vector<UserData> loginUser;

	STD vector<UserData> tempVECT;

	int retval = 0;
	SOCKET listen_sock;

	UserData savedPlayerData[255];

	int EnteredClientsNum = 0;		// 지워야 함
	int lobbyClientsNum = -1;
	int ready = 0;

	bool gameStart;
	bool noActorNumber;
	bool noSavedFile;
	float timeCount = 3;

	void ReadUserData();
	void WriteUserData(UserData& player);
	void CheckUserData(UserData& player, SOCKET& sock);
};



extern Network Nw;