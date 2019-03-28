#pragma once

#define BOARD_SCALE 50

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

class Pawn
{
	Pos CurrentLocation;

public:
	Pawn();
	
	Pos& Move(int key);

	Pos GetCurrentLocation();
};