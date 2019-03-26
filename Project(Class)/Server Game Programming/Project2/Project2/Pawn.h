#pragma once
#include "GL/glut.h"
enum Key
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};

typedef struct pos
{
	float x;
	float y;
	float z;
}Pos;



typedef struct pawndata
{
	Pos p;
	int clientNum;
}PawnData;

class Pawn
{
	Pos CurrentLocation;



public:

	Pawn();

	Pawn(Pos p);

	void SetCurrentLocation(Pos &p);
	

	void Draw();
	

};