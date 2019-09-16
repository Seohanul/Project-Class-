#include "InterfaceControl.h"
#include "blockStageControl.h"
#define START_CURPOS_X         (5*2)
#define START_CURPOS_Y         (0)
#define KEY_SENSITIVE        100
#define SYS_DELAY              20
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
static int keyDelayRate;   // ���� Ŭ���� �ӵ� ����
static int dtime;
void UserInterface::GamePlay()
{
	/* ���� �ӵ� ���� */
	InitKeyDelayRate(5);

	/* Ŀ�� ������ ���� */
	RemoveCursor();

	/* ���� ���� �׸��� */
	DrawGameBoard();

	/* ���� ����, ���� ǥ�� */
	ShowCurrentScoreAndLevel();
	auto start = chrono::system_clock::now();
	/* �ݺ������� ���ο� ����� ���� */
	while (1)
	{		
		/* �� ����� ������ġ ���� */
		InitNewBlockPos(START_CURPOS_X, START_CURPOS_Y);
		/* ��� ���� */
		int blockIdx = ChooseBlock();
		/* ���� ���� Ȯ�� */
		if (IsGameOver())
			break;
		/* ������ �۾� ���� */
		while (1)
		{
			/* ����� �Ʒ��� �� ĭ �̵� */
			if (BlockDown() == 0) // ��� �̵� ���� �� 
			{
				//AddCurrentBlockInfoToBoard(); /* ȣ�� ��ġ�� �̵� */
				break;
			}
			/* ���̸� Ű �Է� ó�� */		
			temp t = ProcessKeyInput(start);
			if (t.second == SPACE) {
				v.emplace_back(t.first, blockIdx, SPACE);	
				break;
			}
			else if (t.second == LEFT) {			
				v.emplace_back(t.first, blockIdx, LEFT);		
			}
			else if (t.second == RIGHT) {
				v.emplace_back(t.first, blockIdx, RIGHT);
			}
			else if (t.second == UP) {
				v.emplace_back(t.first, blockIdx, UP);
			}
		}
	}

	SetCurrentCursorPos(10, 10);

	cout << "GAME OVER ^^" << endl;
	

	
}

void UserInterface::Replay()
{
	//Reset();
	/* ���� �ӵ� ���� */
	InitKeyDelayRate(5);

	/* Ŀ�� ������ ���� */
	RemoveCursor();

	/* ���� ���� �׸��� */
	DrawGameBoard();

	/* ���� ����, ���� ǥ�� */
	ShowCurrentScoreAndLevel();

	/* �ݺ������� ���ο� ����� ���� */
	int i = 0;
	system_clock::time_point start = system_clock::now();
	while (1)
	{
		

		/* �� ����� ������ġ ���� */
		InitNewBlockPos(START_CURPOS_X, START_CURPOS_Y);

		/* ��� ���� */
		
		ReChooseBlock(v[i].getBIdx());
		
		/* ���� ���� Ȯ�� */
		if (IsGameOver())
			break;

		/* ������ �۾� ���� */
		while (1)
		{
			
			/* ����� �Ʒ��� �� ĭ �̵� */
			if (BlockDown() == 0) // ��� �̵� ���� �� 
			{
				//AddCurrentBlockInfoToBoard(); /* ȣ�� ��ġ�� �̵� */
				break;
			}
			
			int temp = ReProcessKeyInput(start, v[i].getT(), v[i].getKey());
			if (temp == SPACE) {
				i++;
				break;
			}
			else if (temp != SPACE && temp != 0) {
				i++;
			}
			
			
			/* ���̸� Ű �Է� ó�� */
		}
		
	}

	SetCurrentCursorPos(10, 10);

	cout << "GAME OVER ^^" << endl;



}



void UserInterface::Save()
{
	cout << "�����Ͻðڽ��ϱ�? (Y/N)" << endl;
	string s;
	cin >> s;
	if (s == "Y" || s == "y") {
		cout << "���ϸ��� �Է��Ͻʽÿ� (.txt ����) " << endl;
		cin >> s;
		ofstream out(s);

		auto p = v.begin();
		for (p = v.begin(); p != v.end(); ++p)
			out << (*p).getT() << ' ' << (*p).getBIdx() << ' ' << (*p).getKey() << endl;
	}
	
}

void UserInterface::Load()
{
	string s;
	cout << "���ϸ��� �Է��Ͻʽÿ� (.txt ����) " << endl;
	cin >> s;
	ifstream in(s);

	int a;
	int b;
	int c;
	
	while (in >> a >> b >> c)
		v.emplace_back(a, b, c);


}

