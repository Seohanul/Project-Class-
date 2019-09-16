#include "Pawn.h"

Pawn::Pawn()
{
	CurrentLocation = { 0,0,0 };
}

Pos & Pawn::Move(int key)
{
	switch (key)
	{
	case UP:
	{
		if (CurrentLocation.y < BOARD_SCALE * 4)
			CurrentLocation.y += BOARD_SCALE;
		break;
	}
	case DOWN:
	{
		if (CurrentLocation.y > BOARD_SCALE * -3)
			CurrentLocation.y -= BOARD_SCALE;
		break;
	}
	case LEFT:
	{
		if (CurrentLocation.x > BOARD_SCALE * -4)
			CurrentLocation.x -= BOARD_SCALE;
		break;
	}
	case RIGHT:
	{
		if (CurrentLocation.x < BOARD_SCALE * 3)
			CurrentLocation.x += BOARD_SCALE;
		break;
	}
	default:
		break;
	}

	return CurrentLocation;
}

Pos Pawn::GetCurrentLocation()
{
	return CurrentLocation;
}
