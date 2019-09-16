

#ifndef __KEY_CURSOR_H_
#define __KEY_CURSOR_H_

#include "point.h"
using temp = pair <int, int>;
void RemoveCursor(void);
point GetCurrentCursorPos(void);
void SetCurrentCursorPos(int x, int y);
temp ProcessKeyInput(system_clock::time_point start);
int ReProcessKeyInput(system_clock::time_point a,int time, int num);
void InitKeyDelayRate(int rate);
void KeyDelaySpeedCtl(int addSpeed);

#endif
/* end of file */