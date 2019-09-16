#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Renderer.h"
#include "Sound.h"
#include <vector>
#define OBJECT_BUILDING 0
#define OBJECT_CHARACTER 1
#define OBJECT_BULLET 2
#define OBJECT_ARROW 3
using namespace std;

#define MAX_OBJECTS_COUNT 100

class SceneMgr
{
public:

	SceneMgr(int width, int height);
	
	~SceneMgr();

	void AddObject(Data Pos, Data vec, int type, int team);

	void DeleteObject(int i, int type);
	void DrawAllObjects();
	void Update(float elapsedTime);

	Data getPosition(int i);
	Data getRGB(int i);
	Data getVector(int i);
	void skillon();
	void skilloff();
	void blowsnow();
	void blowflower();
	void offsnow();
	void offflower();
	void damagechar();
	void recoverchar();
	void collisionCheck1();
	void collisionCheck2();
	void collisionCheck3();
	void collisionCheck4();
	void collisionCheck5();
	void collisionCheck6();
	void collisionCheck7();
	void collisionCheck8();
	int getcurrentObjects();
private:
	Object* building_objects[MAX_OBJECTS_COUNT];
	Object* archer_objects[MAX_OBJECTS_COUNT];
	Object* boomchar_objects[MAX_OBJECTS_COUNT];
	Object* bullet_objects[1000];
	Object* arrow_objects[5000];

	int currentObjects;
	Renderer* m_Renderer;

};
