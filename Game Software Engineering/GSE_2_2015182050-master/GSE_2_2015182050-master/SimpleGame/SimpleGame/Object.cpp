#include "stdafx.h"
#include "Object.h"

Object::Object()
{
	CurrentPosition.x = 50.0;
	CurrentPosition.y = 50.0;
	CurrentPosition.z = 1.0;
	CurrentPosition.s = 50.0;
	ObjVector.x = 0.0;
	ObjVector.y = 0.0;
	ObjVector.z = 1.0;

	Rgb.x = 1.0;
	Rgb.y = 1.0;
	Rgb.z = 1.0;
	Rgb.s = 1.0;
	lifetime = 0.0;
	life = 500;
	dead = false;
	team = 1;
	frame = 0.0;
	m_texCharacter = 0;
}

Object::~Object()
{
}

Data Object::getCurrentPosition()
{
	return CurrentPosition;
}

void Object::setCurrentPosition(Data pos)
{
	CurrentPosition.x = pos.x;
	CurrentPosition.y = pos.y;
	CurrentPosition.z = pos.z;
	CurrentPosition.s = pos.s;
}

Data Object::getRGB()
{
	return Rgb;
}

void Object::setRGB(Data rgb)
{
	Rgb.x = rgb.x;
	Rgb.y = rgb.y;
	Rgb.z = rgb.z;
	Rgb.s = rgb.s;
}



Data Object::getVector()
{
	return ObjVector;
}

void Object::setVector(Data vec)
{
	ObjVector.x = vec.x;
	ObjVector.y = vec.y;
	ObjVector.z = vec.z;
	ObjVector.s = vec.s;
}
bool Object::Checklifetime()
{
	if (lifetime >= 4000)
		dead = true;

	return dead;
}

bool Object::Checklife()
{
	if (life <= 0)
		dead = true;

	return dead;
}

float Object::getLife()
{
	return life;
}

void Object::setLife(float p)
{
	life = p;
}

void Object::setTeam(int t)
{
	team = t;
}

int Object::getTeam()
{
	return team;
}

void Object::settexChar(int t)
{
	m_texCharacter = t;
}

int Object::gettexChar()
{
	return m_texCharacter;
}

float Object::getframe()
{
	return frame;
}

void Object::setframe(float t)
{
	frame = t;
}

void Object::Update(float elapsedTime)
{
	float Time = elapsedTime;
	lifetime += 0.005;
	CurrentPosition.x = CurrentPosition.x + ObjVector.x * ObjVector.z * Time * 0.01;
	CurrentPosition.y = CurrentPosition.y + ObjVector.y * ObjVector.z * Time * 0.01;
	
	/*if (CurrentPosition.x > 250)
		ObjVector.x = -ObjVector.x;
	if (CurrentPosition.x <= -250)
		ObjVector.x = -ObjVector.x;
	if (CurrentPosition.y > 400)
		ObjVector.y = -ObjVector.y;
	if (CurrentPosition.y <= -400)
		ObjVector.y = -ObjVector.y;*/
}
