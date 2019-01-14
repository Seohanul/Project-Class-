#pragma once
#include "stdafx.h"
typedef struct data {
	float x;
	float y;
	float z;
	float s;
}Data;

typedef struct Rect {
	float x;
	float y;
	float width;
	float height;
};
class Object
{

public:
	Object();
	~Object();

	Data getCurrentPosition();
	void setCurrentPosition(Data p);

	Data getRGB();
	void setRGB(Data p);

	float getLife();
	void setLife(float p);

	Data getVector();
	bool Checklifetime();
	bool Checklife();
	void setVector(Data p);
	void Update(float elapsedTime);
	void setTeam(int t);
	int getTeam();
	void settexChar(int t);
	int gettexChar();
	void setframe(float t);
	float getframe();
private:
	Data CurrentPosition;
	Data ObjVector;
	Data Rgb;
	float lifetime;
	float life;
	float frame;
	bool dead;
	int team;

	int m_texCharacter;
};