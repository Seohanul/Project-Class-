
#include "common.h"
#include "keyCurControl.h"

#define LEVEL_DIFF   2    // 단계별 속도 증가 정보
#define LEVER_UP_SCORE_DIFF  20   // 레벨이 증가하는 스코어 간격 정보

static int curGameLevel=1;
static int curGameScore=0;

/* 함    수: void ShowCurrentScoreAndLevel(void)
 * 기    능: 점수와 레벨 정보 출력
 * 반    환: void
 *
 */
void ShowCurrentScoreAndLevel(void)
{
    SetCurrentCursorPos(30, 4);
	cout << "§ 현재 레벨: " << curGameLevel << "    §" << endl;
   
    
    SetCurrentCursorPos(30, 7);
	cout << "§ 현재 점수: " << curGameScore << "    §" << endl;
   
}

/* 함    수: void GameLevelUp(void)
 * 기    능: 게임 레벨 향상
 * 반    환: void
 *
 */
void GameLevelUp(void)
{
    curGameLevel++;
    KeyDelaySpeedCtl(LEVEL_DIFF);
}

/* 함    수: void AddGameScore(int score)
 * 기    능: 게임 점수 추가
 * 반    환: void
 *
 */
void AddGameScore(int score)
{
    if(score<1)
        return;
    
    curGameScore+=score;
    
    /* 레벨 상승 확인 */
    if(curGameScore >= curGameLevel*LEVER_UP_SCORE_DIFF)
        GameLevelUp();
}

