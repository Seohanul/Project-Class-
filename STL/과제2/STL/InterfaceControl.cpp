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
static int keyDelayRate;   // 값이 클수록 속도 증가
static int dtime;
void UserInterface::GamePlay()
{
	/* 게임 속도 설정 */
	InitKeyDelayRate(5);

	/* 커서 깜빡임 제거 */
	RemoveCursor();

	/* 게임 보드 그리기 */
	DrawGameBoard();

	/* 현재 점수, 레벨 표시 */
	ShowCurrentScoreAndLevel();
	auto start = chrono::system_clock::now();
	/* 반복적으로 새로운 블록의 등장 */
	while (1)
	{		
		/* 새 블록의 등장위치 설정 */
		InitNewBlockPos(START_CURPOS_X, START_CURPOS_Y);
		/* 블록 선택 */
		int blockIdx = ChooseBlock();
		/* 게임 종료 확인 */
		if (IsGameOver())
			break;
		/* 내리는 작업 시작 */
		while (1)
		{
			/* 블록을 아래로 한 칸 이동 */
			if (BlockDown() == 0) // 블록 이동 실패 시 
			{
				//AddCurrentBlockInfoToBoard(); /* 호출 위치의 이동 */
				break;
			}
			/* 게이머 키 입력 처리 */		
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
	/* 게임 속도 설정 */
	InitKeyDelayRate(5);

	/* 커서 깜빡임 제거 */
	RemoveCursor();

	/* 게임 보드 그리기 */
	DrawGameBoard();

	/* 현재 점수, 레벨 표시 */
	ShowCurrentScoreAndLevel();

	/* 반복적으로 새로운 블록의 등장 */
	int i = 0;
	system_clock::time_point start = system_clock::now();
	while (1)
	{
		

		/* 새 블록의 등장위치 설정 */
		InitNewBlockPos(START_CURPOS_X, START_CURPOS_Y);

		/* 블록 선택 */
		
		ReChooseBlock(v[i].getBIdx());
		
		/* 게임 종료 확인 */
		if (IsGameOver())
			break;

		/* 내리는 작업 시작 */
		while (1)
		{
			
			/* 블록을 아래로 한 칸 이동 */
			if (BlockDown() == 0) // 블록 이동 실패 시 
			{
				//AddCurrentBlockInfoToBoard(); /* 호출 위치의 이동 */
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
			
			
			/* 게이머 키 입력 처리 */
		}
		
	}

	SetCurrentCursorPos(10, 10);

	cout << "GAME OVER ^^" << endl;



}



void UserInterface::Save()
{
	cout << "저장하시겠습니까? (Y/N)" << endl;
	string s;
	cin >> s;
	if (s == "Y" || s == "y") {
		cout << "파일명을 입력하십시오 (.txt 포함) " << endl;
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
	cout << "파일명을 입력하십시오 (.txt 포함) " << endl;
	cin >> s;
	ifstream in(s);

	int a;
	int b;
	int c;
	
	while (in >> a >> b >> c)
		v.emplace_back(a, b, c);


}

