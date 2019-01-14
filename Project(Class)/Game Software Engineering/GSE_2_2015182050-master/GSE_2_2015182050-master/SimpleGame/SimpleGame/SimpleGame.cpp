/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "SceneMgr.h"
#include "Object.h"
using namespace std;
Renderer *g_Renderer = NULL;

SceneMgr *m_obj;
float g_prevTime;
float dir1 = 1;
float dir2 = 1;
float dir3 = 1;

int tick = 0;
int selectnum = 0;
bool LButtonflag = false;
int width = 500;
int height = 800;
float mousetime = 0;
bool timeflag = true;
int charnum1 = 0;
int charnum2 = 0;
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClearDepth(1.0f);
	
	
	
	// Renderer Test
	float currenttime = (float)timeGetTime()*0.01f;
	float elapsedTime = currenttime - g_prevTime;
	g_prevTime = currenttime;
	m_obj->Update(elapsedTime);
	

	m_obj->DrawAllObjects();
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();

}

void MouseInput(int button, int state, int x, int y)
{

	int xdir = 1;
	int ydir = 1;
	x = x - width / 2;
	y = height / 2 - y;
	if (!timeflag) {
		return;
	}
	cout << x << endl;
	cout << y << endl;

	if (y > 0)
		return;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		if (y > -400 && y < -300)
		{
			if (x > -250 && x < -125)
				selectnum = 1;
			else if (x > -125 && x < 0)
				selectnum = 2;
			else if (x > 0 && x < 125) {
				m_obj->blowsnow();
				m_obj->skilloff();
			}
			else if (x > 125 && x < 250) {

				m_obj->blowflower();
				m_obj->skilloff();
			}
			return;
		}
		else if (y > -300 && y < 0)
		{
			if (x < -125)
			{
				xdir = -1;
			}
			else if (x >= -125 && x < 125)
			{
				xdir = 0;
			}
			else if (x > 125)
			{
				xdir = 1;
			}

		}


		if (selectnum != 0)
		{
			
			if (selectnum == 1) 
			{
				Data temp1 = { xdir * 200.0f ,-100 + charnum2,0.0,30 };

				Data temp2 = { 0,0,300 ,0.0 };
				m_obj->AddObject(temp1, temp2, 1, 2);
				m_obj->skilloff();
			}
			else if (selectnum == 2) 
			{
				Data temp1 = { xdir * 200.0f ,-100,0.0,24 };

				Data temp2 = { 0,3,300 ,0.0 };
				m_obj->AddObject(temp1, temp2, 4, 2);
				m_obj->skilloff();
			}
				RenderScene();
			timeflag = false;

			selectnum = 0;
		}
	}
}
	

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

void Timer(int value)
{
	charnum1+=2;

	dir3 = rand() % 10;
	if (dir3 >= 0 && dir3 <= 3)
		dir1 = -1;
	else if (dir3 > 3 && dir3 <= 7)
		dir1 = 1;
	else if (dir3 >= 7 && dir3 <= 9)
		dir1 = 0;
	dir2 *= -1;
	Data temp1 = { 0.0, 0,0.0,5 };
	//Data temp2 = { (char)rand() % 256, (char)rand() % 256, (char)rand() % 256,1.0 }; // »ö±ò·£´ý

	Data temp2 = { 0,-50 ,50 ,0.0 };
	m_obj->AddObject(temp1, temp2, 2,1);
	sndPlaySoundA("laser.wav", SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
	m_obj->AddObject(temp1, temp2, 3, 1);
	
	temp1 = { (float)(dir1 * 200) ,200.0f - charnum1,0.0,30 };

	temp2 = { 0,0 ,300.0 ,0.0 };
	int num = rand() % 2;
	if(num)
		m_obj->AddObject(temp1, temp2, 1, 1);
	else 
	{
		temp1 = { (float)(dir1 * 200) ,200.0f,0.0,30 };

		temp2 = { 0,-3 ,300.0 ,0.0 };
		m_obj->AddObject(temp1, temp2, 4, 1);
	}
	m_obj->AddObject(temp1, temp2, 3, 2);
	glutTimerFunc(4000, Timer, 1);

}

void Timer2(int value)
{
	m_obj->offsnow();
	m_obj->offflower();
	charnum2+=2;
	timeflag = true;
	glutTimerFunc(6000, Timer2, 1);
	m_obj->skillon();
}
int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}
	m_obj = new SceneMgr(width, height);
	// Initialize Renderer

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);
	glutTimerFunc(4000, Timer, 1);
	glutTimerFunc(6000, Timer2, 2);

	glutMainLoop();

	delete g_Renderer;
	

	return 0;
}
