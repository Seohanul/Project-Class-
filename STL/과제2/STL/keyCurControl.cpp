﻿

#include <conio.h>
#include <windows.h>
#include "common.h"
#include "point.h"
#include "blockStageControl.h"
#include "InterfaceControl.h"

#define KEY_SENSITIVE        100
#define SYS_DELAY              20

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32

static int keyDelayRate;   // 값이 클수록 속도 증가

/* 함    수: void RemoveCursor(void)
 * 기    능: 깜빡 거리는 커서의 제거
 * 반    환: void
 *
 */
void RemoveCursor(void)
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&curInfo);
    curInfo.bVisible=0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&curInfo);
}

/* 함    수: point GetCurrentCursorPos(void)
 * 기    능: 현재 커서 위치 정보를 담은 구조체 변수 반환
 * 반    환: point
 *
 */
point GetCurrentCursorPos(void)
{
    point curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;
    
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.x=curInfo.dwCursorPosition.X;
    curPoint.y=curInfo.dwCursorPosition.Y;
    
    return curPoint;
}

/* 함    수: void SetCurrentCursorPos(int x, int y)
 * 기    능: 커서 위치를 설정
 * 반    환: void
 *
 */
void SetCurrentCursorPos(int x, int y)
{
    COORD pos={x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/* 함    수: int ProcessKeyInput(void)
 * 기    능: 키 입력 처리
 * 반    환: SPACE 선택 시 1
 *
 */

temp ProcessKeyInput(system_clock::time_point start)
{
    int i;
    int key;
	
	
    for(i=0; i<KEY_SENSITIVE; i++)
    {
        if(_kbhit() != 0) 
        {
			auto end = chrono::system_clock::now();
			auto diff = end - start;
			auto msec = chrono::duration_cast<chrono::milliseconds>(diff);
			int dtime = msec.count();	
			key = _getch();
			switch(key)
            {
            case LEFT:
               ShiftLeft();
			   return make_pair(dtime, LEFT);
                break;
     
            case RIGHT:
                ShiftRight();
				return make_pair(dtime, RIGHT);
                break;
            
            case UP:
                RotateBlock();
				return make_pair(dtime, UP);
                break;

            case SPACE:
                SolidCurrentBlock();
				return make_pair(dtime, SPACE);
            }
        }
    
        if(i % keyDelayRate == 0)
            Sleep(SYS_DELAY);
    }



    return make_pair(0,0);
}
int ReProcessKeyInput(system_clock::time_point start,int time, int num)
{
	int i;
	int key;

	for (i = 0; i<KEY_SENSITIVE; i++)
	{
		auto end = chrono::system_clock::now();
		auto diff = end - start;
		auto msec = chrono::duration_cast<chrono::milliseconds>(diff);
		int dtime = msec.count();
		if (dtime >= time) {
			key = num;
			switch (key)
			{
			case LEFT:
				ShiftLeft();
				return LEFT;
				break;

			case RIGHT:
				ShiftRight();
				return RIGHT;
				break;

			case UP:
				RotateBlock();
				return UP;
				break;

			case SPACE:
				SolidCurrentBlock();
				return SPACE;
			}

		}
		if (i % keyDelayRate == 0)
			Sleep(SYS_DELAY);
	}
	return 0;
}
/* 함    수: void InitKeyDelayRate(int rate)
 * 기    능: 속도 조절, 값이 클수록 속도 증가
 * 반    환: void
 *
 */
void InitKeyDelayRate(int rate)
{
    if(rate<1)
        return;
    
    keyDelayRate=rate;
}

/* 함    수: void KeyDelaySpeedCtl(int addSpeed)
 * 기    능: 속도를 변화시킨다 +는 증가, -는 감소
 * 반    환: void
 *
 */
void KeyDelaySpeedCtl(int addSpeed)
{
    keyDelayRate+=addSpeed;
}

/* end of file */