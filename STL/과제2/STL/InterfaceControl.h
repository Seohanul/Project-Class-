#pragma once

#include <Windows.h>
#include "common.h"
#include "keyCurControl.h"
#include "blockStageControl.h"
#include "scoreLevelControl.h"

class Data {
private:
	int time;
	int blockIndex;
	int Key;

public :
	Data(int a, int b, int c) :time(a), blockIndex(b), Key(c)
	{
	
	}
	int getT() {
		return time;
	}
	int getBIdx(){
		return blockIndex;
	}
	int getKey() {
		return Key;
	}
	void setT(int t) {
		time = t;
	}
	void setBIdx(int i) {
		blockIndex = i;
	}
	void setKey(int k) {
		Key = k;
	}
};
class UserInterface {
public:
	vector<Data> v;
public :
	
	void GamePlay();
	void Replay();

	void Save();
	void Load();
	
};
 