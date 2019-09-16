#pragma once
#include <chrono>

#define IS_PRESSED(key) (GetAsyncKeyState(key) & 0x8000)

struct KeyState
{
	bool pressed = false;
	bool released = true;
};

// Have To Fix (Later, should make class for this (keyboard or Input)
#pragma pack(1)
struct Input
{
	char key;
	bool pressed = false;
};
#pragma pack()

class Framework
{
	/* Chrono Time */
	using TimeDuration = STD chrono::duration<float>;
	using Time = STD chrono::high_resolution_clock;
	using TimePoint = Time::time_point;

public:	

	Framework() {}
	~Framework();


	void Initialize(const STD string& strWindowsTitle, int width, int height, int argc, char* argv[]);
	void Fullscreen();
	void Close();

	void GetWindowSizei(int* WinWidth, int * WinHeight) const;
	void GetWindowSizef(float * WinWidth, float * WinHeight) const;

	void Keyboard();
	
	void Loop();
	void Run();

	BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);

	void ToScene(Scene*& pScene) = delete;
	void ToScene(Scene*&& pScene);
	
private:
	void BindFunctions();
	void ResetClock();
	void ChangeScenes();
	void SafeClose();
	
private:
	bool		m_CloseLoop		{ false };
	STD string  m_WindowTitle	{ "Default Title" };
	int			m_WindowWidth	{ 500 };
	int			m_WindowHeight	{ 500 };

	Scene*		m_CurrentScene	{ NULL };
	Scene*		m_ShiftScene	{ NULL };

	// Have To Fix (Later, should make class for this (keyboard or Input)
	std::map<int, KeyState> Inputs;

// Framework Timer

	TimePoint	m_CurrentTime;
	TimePoint	m_PreviousTime;
	float		m_TimeFrame;
	float		m_TimeAccumulator;
	
};



extern Framework Fw;

