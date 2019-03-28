#include "Pawn.h"

Pawn::Pawn()
{
	CurrentLocation = { 0,0,0 };
}

Pawn::Pawn(Pos p)
{
	CurrentLocation = p;
}

void Pawn::SetCurrentLocation(Pos & p)
{
	CurrentLocation = p;
}

void Pawn::Draw()
{
	glPushMatrix();
	glTranslatef(CurrentLocation.x, CurrentLocation.y, CurrentLocation.z);
	//glutSolidCone(10, 50, 10, 10);
	glutSolidTeapot(10);
	glPopMatrix();
}

