#pragma once

#pragma comment(lib, "ws2_32")
#include "targetver.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <float.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <fstream>

#define	STD		std::

#include "Math.h"
#include "Indices.h"
#include "EventDispatcher.h"

#define CYAN_DEBUG_STATES
#define CYAN_DEBUG_COLLISION

#define SERVERPORT 9000
#define BUFSIZE 4096

/* Depth Test */
#define FARTHEST 1.f
#define NEAREST -1.f

#pragma region 최우진 코드
#define SIGNUP 1
#define SIGNIN 2
#define LOGINSCENE 0
#define LOBBYSCENE 1
#define INGAMESCENE 2
#define MAXPLAYER 1
#pragma endregion

constexpr float UPDATE_FREQUENCY{ 120.f };
constexpr float UPDATE_TIME{ 1.f / UPDATE_FREQUENCY };

template<class T>
inline size_t Last(STD vector<T> v)
{
	return v.size() - 1;
}

template<class T>
inline void EraseByValue(STD vector<T>& v, const T& value)
{
	size_t Count = 0;
	v.erase(STD remove(v.begin(), v.end(), value), v.end());
}


template<class T>
inline void EraseByIndex(STD vector<T>& v, size_t Index)
{
	size_t Count = 0;
	v.erase(STD remove_if(v.begin(), v.end(), [&](const T& value)
	{
		return Count++ == Index;
	}), v.end());
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
class Graphics;
class Physics;
class Input;
class Sprite;

namespace FMOD
{
	class Sound;
	class DSP;
	class System;
	class Channel;
}