#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <float.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "Math.h"



#define SERVERIP   "172.16.1.238"
#define SERVERPORT 9000
#define BUFSIZE 4096

#define CYAN_DEBUG_STATES
#define CYAN_DEBUG_COLLISION

enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

/* Depth Test */
#define FARTHEST 1.f
#define NEAREST -1.f

#define	STD		std::

constexpr float UPDATE_FREQUENCY	{ 120.f };
constexpr float UPDATE_TIME			{ 1.f / UPDATE_FREQUENCY };

using id_type = STD string;

template<class T>
inline u_int LastIdx(STD vector<T> v)
{
	return static_cast<u_int>(v.size() - 1);
}

/* Core */
class Framework;
class Scene;
class Cyan;

class State;
class Command;
class Gamepad;
class Renderer;

/* Components */
class ActorGraphics;
class VisualGraphics;
class Graphics;
class Physics;
class Input;

namespace FMOD
{
	class Sound;
	class DSP;
	class System;
	class Channel;
}