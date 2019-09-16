#include "stdafx.h"
#include "SceneMgr.h"

#define BUILDINGLIFE 500
#define CHARLIFE 100

#define BULLETLIFE 15

#define ARROWLIFE 10
float dir = -1;
float ptcframe = 0;
float time = 0.0;
GLuint background_text = 0;
GLuint cookie_text = 0;
GLuint bat_text = 0;
GLuint building_text1 = 0;
GLuint building_text2 = 0;
GLuint char1_text = 0;
GLuint char2_text = 0;
GLuint boomchar1_text = 0;
GLuint boomchar2_text = 0;
GLuint snow_text = 0;
GLuint flower_text = 0;
// 이번 실습 해야할일
// 케릭터 속도 구현
// 빌딩 - 케릭터 충돌처리
// 체력 게이지
// 속도 일정하게
float skilltime = 1.0;
bool skillflag = true;
float skillnum = 1.0;
bool snowflag = false;
bool flowerflag = false;
auto m_sound2 = new Sound();

auto soundBG2 = m_sound2->CreateSound("gun-shot.wav");


SceneMgr::SceneMgr(int width, int height)
{
	m_Renderer = new Renderer(width, height);
	
	background_text = m_Renderer->CreatePngTexture("background.png");
	building_text1 = m_Renderer->CreatePngTexture("Abuilding.png");
	building_text2 = m_Renderer->CreatePngTexture("Abuilding.png");
	char1_text = m_Renderer->CreatePngTexture("char1_animation.png");
	char2_text = m_Renderer->CreatePngTexture("char2_animation.png");

	boomchar1_text = m_Renderer->CreatePngTexture("boomchar1.png");
	boomchar2_text = m_Renderer->CreatePngTexture("boomchar2.png");
	snow_text = m_Renderer->CreatePngTexture("snow.png");
	flower_text = m_Renderer->CreatePngTexture("flower.png");
	cookie_text = m_Renderer->CreatePngTexture("cookie.png");
	bat_text = m_Renderer->CreatePngTexture("bat.png");
	currentObjects = 0;
	AddObject({ -200,300,0,100 }, { 1.0,-1.0,0.0 }, 0, 1);
	AddObject({ 0,320,0,100 }, { 1.0,-1.0,0.0 }, 0, 1);
	AddObject({ 200,300,0,100 }, { 1.0,-1.0,0.0 }, 0, 1);
	AddObject({ -200,-200,0,100 }, { 1.0,1.0,0.0 }, 0, 2);
	AddObject({ 0,-220,0,100 }, { 1.0,1.0,0.0 }, 0, 2);
	AddObject({ 200,-200,0,100 }, { 1.0,1.0,0.0 }, 0, 2);
	auto m_sound = new Sound();

	auto soundBG = m_sound->CreateSound("bgm.xm");

	m_sound->PlaySound(soundBG, true, 0.2f);

	

}

SceneMgr::~SceneMgr()
{

}

void SceneMgr::AddObject(Data Pos, Data vec, int type, int team)
{
	if (type == 0)
	{
		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (building_objects[i] == NULL)
			{
				building_objects[i] = new Object;
				building_objects[i]->setCurrentPosition(Pos);
				building_objects[i]->setRGB(vec);
				if(team ==1)
					building_objects[i]->settexChar( building_text1);
				else
					building_objects[i]->settexChar(building_text2);				
				building_objects[i]->setLife(BUILDINGLIFE);
				building_objects[i]->setTeam(team);
				return;
			}
		}
		
	}
	else if (type == 1) {
		if (team == 1)
		{
			for (int i = 0; i < 50; ++i)
			{

				if (archer_objects[i] == NULL)
				{
					archer_objects[i] = new Object;
					currentObjects++;
					archer_objects[i]->setTeam(team);
					archer_objects[i]->setCurrentPosition(Pos);
					archer_objects[i]->setVector(vec);
					archer_objects[i]->setLife(CHARLIFE);
					archer_objects[i]->settexChar(char1_text);
					archer_objects[i]->setRGB({ 1.0,0.0,0.0 });
					
					return;
				}

			}
		}
		else 
		{
			for (int i = 50; i < MAX_OBJECTS_COUNT; ++i)
			{

				if (archer_objects[i] == NULL)
				{
					archer_objects[i] = new Object;
					currentObjects++;
					archer_objects[i]->setTeam(team);
					archer_objects[i]->setCurrentPosition(Pos);
					archer_objects[i]->setVector(vec);
					archer_objects[i]->setLife(CHARLIFE);
					archer_objects[i]->settexChar(char2_text);
					archer_objects[i]->setRGB({ 0.0,0.0,1.0 });
					return;
				}

			}
		}
	}
	else if (type == 2) {
		for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
		{
			if (building_objects[i] == NULL)
				continue;
			int j = i * 100;
			int temp = j + 100;
			for (; j < temp; ++j)
			{
				if (bullet_objects[j] == NULL)
				{
					bullet_objects[j] = new Object;
					bullet_objects[j]->setTeam(building_objects[i]->getTeam());

					currentObjects++;
					Data temp1 = building_objects[i]->getCurrentPosition();
					temp1.x -= 1;
					temp1.y -= 1;
					temp1.s = 3;
					Data temp2 = building_objects[i]->getVector();
					temp2 = { 0, (float)((rand() % 2)+1), 0.0 };
					if (bullet_objects[j]->getTeam() == 1)
						temp2.y *= -1;
					dir *= -1;
					temp2.z = 600;
					bullet_objects[j]->setCurrentPosition(temp1);
					bullet_objects[j]->setVector(temp2);
					bullet_objects[j]->settexChar(m_Renderer->CreatePngTexture("bullet.png"));
					if(bullet_objects[j]->getTeam()==1)
						bullet_objects[j]->setRGB({ 1.0,0.0,0.0 });
					else
						bullet_objects[j]->setRGB({ 0.0,0.0,1.0 });
					bullet_objects[j]->setLife(BULLETLIFE);
					break;
				}
			}
			

		}
	}
	else if (type == 3) {
		
		if (team == 1)
		{
			for (int i = 0; i < 50; ++i)
			{
				if (archer_objects[i] == NULL)
					continue;
				int j = i * 50;
				int temp = j + 50;
				for (; j < temp; ++j)
				{
					if (arrow_objects[j] == NULL)
					{
						arrow_objects[j] = new Object;
						currentObjects++;
						Data temp1 = archer_objects[i]->getCurrentPosition();
						temp1.x -= 1;
						temp1.y -= 1;
						temp1.s = 10;
						Data temp2 = { 0,-5,1 };
						dir *= -1;
						arrow_objects[j]->setRGB({ 0.5f,0.2f,0.7f });
						temp2.z = 100;
						arrow_objects[j]->setCurrentPosition(temp1);
						arrow_objects[j]->setVector(temp2);

						arrow_objects[j]->setLife(ARROWLIFE);
						arrow_objects[j]->setTeam(team);
						break;
					}
				}
			}
		}
		else
		{
			for (int i = 50; i < MAX_OBJECTS_COUNT; ++i)
			{
				if (archer_objects[i] == NULL)
					continue;
				int j = i * 50;
				int temp = j + 50;
				for (; j < temp; ++j)
				{
					if (arrow_objects[j] == NULL)
					{
						arrow_objects[j] = new Object;
						currentObjects++;
						Data temp1 = archer_objects[i]->getCurrentPosition();
						temp1.x += 1;
						temp1.y += 1;
						temp1.s = 10;
						Data temp2 = { 0,5,1 };
						dir *= -1;
						arrow_objects[j]->setRGB({ 1.0,1.0,0.0 });
						temp2.z = 100;
						arrow_objects[j]->setCurrentPosition(temp1);
						arrow_objects[j]->setVector(temp2);

						arrow_objects[j]->setLife(ARROWLIFE);
						arrow_objects[j]->setTeam(team);
						break;
					}
				}
			}
		}
	}
	else if (type == 4) {
		if (team == 1)
		{
			for (int i = 0; i < 50; ++i)
			{

				if (boomchar_objects[i] == NULL)
				{
					boomchar_objects[i] = new Object;
					currentObjects++;
					boomchar_objects[i]->setTeam(team);
					boomchar_objects[i]->setCurrentPosition(Pos);
					boomchar_objects[i]->setVector(vec);
					boomchar_objects[i]->setLife(CHARLIFE);
					boomchar_objects[i]->settexChar(boomchar1_text);
					boomchar_objects[i]->setRGB({ 1.0,0.0,0.0 });

					return;
				}

			}
		}
		else
		{
			for (int i = 50; i < MAX_OBJECTS_COUNT; ++i)
			{

				if (boomchar_objects[i] == NULL)
				{
					boomchar_objects[i] = new Object;
					currentObjects++;
					boomchar_objects[i]->setTeam(team);
					boomchar_objects[i]->setCurrentPosition(Pos);
					boomchar_objects[i]->setVector(vec);
					boomchar_objects[i]->setLife(CHARLIFE);
					boomchar_objects[i]->settexChar(boomchar2_text);
					boomchar_objects[i]->setRGB({ 0.0,0.0,1.0 });
					return;
				}

			}
		}
	}



}

void SceneMgr::DeleteObject(int i,int type)
{
	if (currentObjects == 0)
	{
		return;
	}
	if (type == 0)
	{
		if (building_objects[i] != NULL)
		{
			delete building_objects[i];
			building_objects[i] = NULL;
		}
	}
	if (type == 1) {
		if (archer_objects[i] != NULL) {
			delete archer_objects[i];
			archer_objects[i] = NULL;
		}
	}
	if (type == 2)
	{
		if (bullet_objects[i] != NULL)
		{
			delete bullet_objects[i];
			bullet_objects[i] = NULL;
		}
	}
	if (type == 3)
	{
		if (arrow_objects[i] != NULL)
		{
			delete arrow_objects[i];
			arrow_objects[i] = NULL;
		}
	}
	if (type == 4) {
		if (boomchar_objects[i] != NULL) {
			delete boomchar_objects[i];
			boomchar_objects[i] = NULL;
		}
	}
}
void SceneMgr::DrawAllObjects()
{
	time += 0.005;
	//m_Renderer->DrawSolidRect(0, 0, 0, 500, 0.0, 0.3, 0.3, 0.4,0.9);

	m_Renderer->DrawTexturedRect(0.0f,50.0f,0.0f,700,1.0f,1.0f,1.0f,1, background_text,0.99);

	m_Renderer->DrawTexturedRect(-185.0f, -350.0f, 0.0f, 125, 1.0f, 1.0f, 1.0f, skillnum, cookie_text, 0.99);
	m_Renderer->DrawTexturedRect(-65.0f, -350.0f, 0.0f, 125, 1.0f, 1.0f, 1.0f, skillnum, bat_text, 0.99);
	m_Renderer->DrawTexturedRect(65.0f, -350.0f, 0.0f, 125, 1.0f, 1.0f, 1.0f, skillnum, snow_text, 0.99);
	m_Renderer->DrawTexturedRect(185.0f, -350.0f, 0.0f, 125, 1.0f, 1.0f, 1.0f, skillnum, flower_text, 0.99);
	if (snowflag) {
		m_Renderer->DrawParticleClimate(0, 0, 0, 3, 1, 1, 1, 1, -0.1, -0.1, snow_text, time, 0.01);
	}
	if (flowerflag) {
		m_Renderer->DrawParticleClimate(0, 0, 0, 3, 1, 1, 1, 1, -0.1, -0.1, flower_text, time, 0.01);
	}
	for (int i = 0; i < MAX_OBJECTS_COUNT; i++)
	{
		if (building_objects[i] != NULL)
		{
			float r;
			float b;
			if (building_objects[i]->getTeam()==1)
			{
				r = 1.0;
				b = 0.0;
			}
			else
			{
				r = 0.0;
				b = 1.0;
			}
			m_Renderer->DrawSolidRectGauge(
				building_objects[i]->getCurrentPosition().x,
				building_objects[i]->getCurrentPosition().y + (building_objects[i]->getCurrentPosition().s * 2) / 3,
				0,
				building_objects[i]->getCurrentPosition().s,
				building_objects[i]->getCurrentPosition().s / 5,
				r,
				0,
				b,
				1,
				(building_objects[i]->getLife()) / BUILDINGLIFE,
				0.8
			);
			char array[20];
			sprintf(array, "%d", (int)building_objects[i]->getLife());
			
			m_Renderer->DrawTextW(building_objects[i]->getCurrentPosition().x-10, building_objects[i]->getCurrentPosition().y + (building_objects[i]->getCurrentPosition().s * 2) / 3 + 10, GLUT_BITMAP_HELVETICA_12, 0.0, 0.0, 0.0, array);
			m_Renderer->DrawTexturedRect(
				building_objects[i]->getCurrentPosition().x,
				building_objects[i]->getCurrentPosition().y,
				0,
				building_objects[i]->getCurrentPosition().s,
				building_objects[i]->getRGB().x,
				building_objects[i]->getRGB().y,
				building_objects[i]->getRGB().z,
				1,
				building_objects[i]->gettexChar(),
				0.1
			);
		}
		if (archer_objects[i] != NULL)
		{
			float r;
			float b;
			if (archer_objects[i]->getTeam() == 1)
			{
				r = 1.0;
				b = 0.0;
			}
			else
			{
				r = 0.0;
				b = 1.0;
			}
			m_Renderer->DrawSolidRectGauge(
				archer_objects[i]->getCurrentPosition().x,
				archer_objects[i]->getCurrentPosition().y + (archer_objects[i]->getCurrentPosition().s * 2) / 3,
				0,
				archer_objects[i]->getCurrentPosition().s,
				archer_objects[i]->getCurrentPosition().s / 5,
				r,
				0,
				b,
				1,
				(archer_objects[i]->getLife()) / CHARLIFE,
				0.8
			);
			// Renderer Test
			/*m_Renderer->DrawSolidRect(
				char_objects[i]->getCurrentPosition().x,
				char_objects[i]->getCurrentPosition().y,
				0,
				char_objects[i]->getCurrentPosition().s,
				char_objects[i]->getRGB().x,
				char_objects[i]->getRGB().y,
				char_objects[i]->getRGB().z,
				1,
				0.2
			);*/
			char array2[20];
			sprintf(array2, "%d", (int)archer_objects[i]->getLife());

			m_Renderer->DrawTextW(archer_objects[i]->getCurrentPosition().x - 10, archer_objects[i]->getCurrentPosition().y + (archer_objects[i]->getCurrentPosition().s * 2) / 3 + 10, GLUT_BITMAP_HELVETICA_12, 0.0, 0.0, 0.0, array2);

			int num = archer_objects[i]->getframe();
			m_Renderer->DrawTexturedRectSeq(
				archer_objects[i]->getCurrentPosition().x,
				archer_objects[i]->getCurrentPosition().y,
				0,
				archer_objects[i]->getCurrentPosition().s,
				archer_objects[i]->getRGB().x,
				archer_objects[i]->getRGB().y,
				archer_objects[i]->getRGB().z,
				1,
				archer_objects[i]->gettexChar(),
				num,
				0,
				6,
				1,
				0.2
			);
			num = (num + 1) % 6;
			archer_objects[i]->setframe(num);
		}

		if (boomchar_objects[i] != NULL)
		{
			float r;
			float b;
			if (boomchar_objects[i]->getTeam() == 1)
			{
				r = 1.0;
				b = 0.0;
			}
			else
			{
				r = 0.0;
				b = 1.0;
			}
			m_Renderer->DrawSolidRectGauge(
				boomchar_objects[i]->getCurrentPosition().x,
				boomchar_objects[i]->getCurrentPosition().y + (boomchar_objects[i]->getCurrentPosition().s * 2) / 3,
				0,
				boomchar_objects[i]->getCurrentPosition().s,
				boomchar_objects[i]->getCurrentPosition().s / 5,
				r,
				0,
				b,
				1,
				(boomchar_objects[i]->getLife()) / CHARLIFE,
				0.8
			);
			// Renderer Test
			/*m_Renderer->DrawSolidRect(
			char_objects[i]->getCurrentPosition().x,
			char_objects[i]->getCurrentPosition().y,
			0,
			char_objects[i]->getCurrentPosition().s,
			char_objects[i]->getRGB().x,
			char_objects[i]->getRGB().y,
			char_objects[i]->getRGB().z,
			1,
			0.2
			);*/
			char array2[20];
			sprintf(array2, "%d", (int)boomchar_objects[i]->getLife());

			m_Renderer->DrawTextW(boomchar_objects[i]->getCurrentPosition().x - 10, boomchar_objects[i]->getCurrentPosition().y + (boomchar_objects[i]->getCurrentPosition().s * 2) / 3 + 10, GLUT_BITMAP_HELVETICA_12, 0.0, 0.0, 0.0, array2);

			int num = boomchar_objects[i]->getframe();
			m_Renderer->DrawTexturedRectSeq(
				boomchar_objects[i]->getCurrentPosition().x,
				boomchar_objects[i]->getCurrentPosition().y,
				0,
				boomchar_objects[i]->getCurrentPosition().s,
				boomchar_objects[i]->getRGB().x,
				boomchar_objects[i]->getRGB().y,
				boomchar_objects[i]->getRGB().z,
				1,
				boomchar_objects[i]->gettexChar(),
				num,
				0,
				2,
				1,
				0.2
			);
			num = (num + 1) % 2;
			boomchar_objects[i]->setframe(num);
		}


	}
	for (int i = 0; i < 1000; ++i) {
		if (bullet_objects[i] != NULL)
		{
			
			// Renderer Test
			/*m_Renderer->DrawSolidRect(
				bullet_objects[i]->getCurrentPosition().x,
				bullet_objects[i]->getCurrentPosition().y,
				0,
				bullet_objects[i]->getCurrentPosition().s,
				bullet_objects[i]->getRGB().x,
				bullet_objects[i]->getRGB().y,
				bullet_objects[i]->getRGB().z,
				1,
				0.3
			);*/

			ptcframe = bullet_objects[i]->getframe();
			m_Renderer->DrawParticle(
				bullet_objects[i]->getCurrentPosition().x,
				bullet_objects[i]->getCurrentPosition().y,
				0,
				bullet_objects[i]->getCurrentPosition().s,
				bullet_objects[i]->getRGB().x,
				bullet_objects[i]->getRGB().y,
				bullet_objects[i]->getRGB().z,
				1,
				-bullet_objects[i]->getVector().x,
				-bullet_objects[i]->getVector().y,
				bullet_objects[i]->gettexChar(),
				ptcframe,
				0.1

			);
			ptcframe += 0.001;
			bullet_objects[i]->setframe(ptcframe);
		}
	}
	for (int i = 0; i < 5000; ++i) {

		if (arrow_objects[i] != NULL)
		{
			// Renderer Test
			m_Renderer->DrawSolidRect(
				arrow_objects[i]->getCurrentPosition().x,
				arrow_objects[i]->getCurrentPosition().y,
				0,
				arrow_objects[i]->getCurrentPosition().s,
				arrow_objects[i]->getRGB().x,
				arrow_objects[i]->getRGB().y,
				arrow_objects[i]->getRGB().z,
				1,
				0.3
			);

		}
	}
}
void SceneMgr::damagechar()
{
	for (int i = 0; i < 49; ++i)
	{
		if (archer_objects[i] == NULL)
			continue;
		float temp = archer_objects[i]->getLife();
		temp -= 50;
		archer_objects[i]->setLife(temp);
	}
}
void SceneMgr::recoverchar()
{
	for (int i = 50; i < 100; ++i)
	{
		if (archer_objects[i] == NULL)
			continue;
		float temp = archer_objects[i]->getLife();
		temp += 50;
		archer_objects[i]->setLife(temp);
	}
	for (int i = 3; i < 6; ++i)
	{
		float temp = building_objects[i]->getLife();
		temp += 30;
		building_objects[i]->setLife(temp);
	}
}

void SceneMgr::blowsnow()
{
	if (skillflag) {
		
		snowflag = true;
		damagechar();
	}
}
void SceneMgr::blowflower()
{
	if (skillflag) {
		
		flowerflag = true;
		recoverchar();
	}
}
void SceneMgr::offsnow()
{
	snowflag = false;
}
void SceneMgr::offflower()
{
	flowerflag = false;
}
void SceneMgr::skillon()
{
	skillflag = true;
}
void SceneMgr::skilloff()
{
	skillflag = false;
}
void SceneMgr::Update(float elapsedTime)
{
	if (skillflag)
		skillnum = 1;
	else
		skillnum = 0.3;

	collisionCheck1();
	collisionCheck2();
	collisionCheck3();
	collisionCheck4();
	collisionCheck5();
	collisionCheck6();
	//collisionCheck7();
	collisionCheck8();
	if (currentObjects == 0)
		return;
	
	for (int i = 0; i < 1000; ++i)
	{
		if (bullet_objects[i] == NULL)
			continue;
		Data temp = bullet_objects[i]->getCurrentPosition();
		if (temp.x > 260 || temp.x < -260 || temp.y > 410 || temp.y < -310) {
			delete bullet_objects[i];
			bullet_objects[i] = NULL;
		}
		
	}
	
	for (int i = 0; i < 5000; ++i)
	{
		if (arrow_objects[i] == NULL)
			continue;
		Data temp = arrow_objects[i]->getCurrentPosition();
		if (temp.x > 250 || temp.x < -250 || temp.y > 400 || temp.y < -300) {
			delete arrow_objects[i];
			arrow_objects[i] = NULL;
		}

	}

	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (building_objects[i] != NULL)
		{
			if (building_objects[i]->Checklife())
			{
				delete building_objects[i];
				building_objects[i] = NULL;
				sndPlaySoundA("glass-break.wav", SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
			}

		}

		if (archer_objects[i] != NULL)
		{
			Data CurrentPosition = archer_objects[i]->getCurrentPosition();
			Data temp = archer_objects[i]->getVector();
			if (CurrentPosition.x > 230)
				temp.x *= -1;
			if (CurrentPosition.x <= -230)
				temp.x *= -1;
			if (CurrentPosition.y > 380)
				temp.y *= -1;
			if (CurrentPosition.y <= -250)
				temp.y *= -1;
			archer_objects[i]->setVector(temp);

			if (archer_objects[i]->Checklifetime() || archer_objects[i]->Checklife())
			{
				delete archer_objects[i];
				archer_objects[i] = NULL;
			}
			else
			{
				archer_objects[i]->Update(elapsedTime);

			}
		}
		if (archer_objects[i] != NULL)
		{
			Data CurrentPosition = archer_objects[i]->getCurrentPosition();
			Data temp = archer_objects[i]->getVector();
			
			if (CurrentPosition.y > 300)
				temp.y = 0;
			archer_objects[i]->setVector(temp);

			if (archer_objects[i]->Checklifetime() || archer_objects[i]->Checklife())
			{
				delete archer_objects[i];
				archer_objects[i] = NULL;
			}
			else
			{
				archer_objects[i]->Update(elapsedTime);

			}
		}
		if (boomchar_objects[i] != NULL)
		{
			Data CurrentPosition = boomchar_objects[i]->getCurrentPosition();
			Data temp = boomchar_objects[i]->getVector();

			if (CurrentPosition.y > 300)
				temp.y = 0;
			boomchar_objects[i]->setVector(temp);

			if (boomchar_objects[i]->Checklifetime() || boomchar_objects[i]->Checklife())
			{
				delete boomchar_objects[i];
				boomchar_objects[i] = NULL;
				sndPlaySoundA("explosion.wav", SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
			}
			else
			{
				boomchar_objects[i]->Update(elapsedTime);

			}
		}
		
	}
	for (int i = 0; i < 1000; ++i)
	{
		if (bullet_objects[i] != NULL)
		{
			if (bullet_objects[i]->Checklifetime() || bullet_objects[i]->Checklife())
			{
				delete bullet_objects[i];
				bullet_objects[i] = NULL;
			}
			else
			{
				bullet_objects[i]->Update(elapsedTime);
			}
		}
	}
	for (int i = 0; i < 5000; ++i)
	{
		if (arrow_objects[i] != NULL)
		{
			if (arrow_objects[i]->Checklifetime() || arrow_objects[i]->Checklife())
			{
				delete arrow_objects[i];
				arrow_objects[i] = NULL;
			}
			else
			{
				arrow_objects[i]->Update(elapsedTime);
			}
		}
	}
}

Data SceneMgr::getPosition(int i)
{
	
	Data temp = archer_objects[i]->getCurrentPosition();
	return temp;
}
Data SceneMgr::getRGB(int i)
{

	Data temp = archer_objects[i]->getRGB();
	return temp;
}
Data SceneMgr::getVector(int i)
{

	Data temp = archer_objects[i]->getVector();
	return temp;
}

int SceneMgr::getcurrentObjects()
{

	return currentObjects;
}

void SceneMgr::collisionCheck1()			
{
	Rect p1 = { 0, };
	Rect p2 = { 0, };
	
	for (int i = 0; i < 100; ++i)
	{

		if (archer_objects[i] == NULL)
			continue;
		Data temp1 = archer_objects[i]->getCurrentPosition();

		for (int j= 0; j < 1000; ++j)
		{
			if(bullet_objects[j] ==NULL)
				continue;
			if (archer_objects[i]->getTeam() == bullet_objects[j]->getTeam())
				continue;
			Data temp2 = bullet_objects[j]->getCurrentPosition();
			p1.x = temp1.x;
			p1.y = temp1.y;
			p1.width = temp1.s;
			p1.height = temp1.s;
			p2.x = temp2.x;
			p2.y = temp2.y;
			p2.width = temp2.s;
			p2.height = temp2.s;
			if (p1.x < p2.x + p2.width &&
				p1.x + p1.width > p2.x &&
				p1.y < p2.y + p2.height &&
				p1.height + p1.y > p2.y &&
				i !=j						){
				float temp = archer_objects[i]->getLife() - bullet_objects[j]->getLife();
				archer_objects[i]->setLife(temp);
				//DeleteObject(i, 1);
				DeleteObject(j, 2);
			}
			
			
		}
	}
}

void SceneMgr::collisionCheck2()
{
	Rect p1 = { 0, };
	Rect p2 = { 0, };

	for (int i = 0; i < 100; ++i)
	{

		if (archer_objects[i] == NULL)
			continue;
		Data temp1 = archer_objects[i]->getCurrentPosition();

		for (int j = 0; j < 5000; ++j)
		{
			if (arrow_objects[j] == NULL)
				continue;
			if (archer_objects[i]->getTeam() == arrow_objects[j]->getTeam())
				continue;
			Data temp2 = arrow_objects[j]->getCurrentPosition();
			p1.x = temp1.x;
			p1.y = temp1.y;
			p1.width = temp1.s;
			p1.height = temp1.s;
			p2.x = temp2.x;
			p2.y = temp2.y;
			p2.width = temp2.s;
			p2.height = temp2.s;
			if (p1.x < p2.x + p2.width &&
				p1.x + p1.width > p2.x &&
				p1.y < p2.y + p2.height &&
				p1.height + p1.y > p2.y &&
				i != (j/10)) {
				float temp = archer_objects[i]->getLife() - arrow_objects[j]->getLife();
				archer_objects[i]->setLife(temp);

				//DeleteObject(i, 1);
				DeleteObject(j, 3);
			}


		}
	}
}

void SceneMgr::collisionCheck3()
{
	Rect p1 = { 0, };
	Rect p2 = { 0, };
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (building_objects[i] == NULL)
			continue;
		Data temp1 = building_objects[i]->getCurrentPosition();
		for (int j = 0; j < 5000; ++j)
		{
			if (arrow_objects[j] == NULL)
				continue;
			if (building_objects[i]->getTeam() == arrow_objects[j]->getTeam())
				continue;
			Data temp2 = arrow_objects[j]->getCurrentPosition();
			p1.x = temp1.x;
			p1.y = temp1.y;
			p1.width = temp1.s;
			p1.height = temp1.s;
			p2.x = temp2.x;
			p2.y = temp2.y;
			p2.width = temp2.s;
			p2.height = temp2.s;
			if (p1.x < p2.x + p2.width &&
				p1.x + p1.width > p2.x &&
				p1.y < p2.y + p2.height &&
				p1.height + p1.y > p2.y)
			{
				float temp = building_objects[i]->getLife() - arrow_objects[j]->getLife();
				building_objects[i]->setLife(temp);

				//DeleteObject(i, 1);
				DeleteObject(j, 3);
			}


		}
	}
}

void SceneMgr::collisionCheck4()
{
	Rect p1 = { 0, };
	Rect p2 = { 0, };
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (building_objects[i] == NULL)
			continue;
		Data temp1 = building_objects[i]->getCurrentPosition();
		for (int j = 0; j < 1000; ++j)
		{
			if (bullet_objects[j] == NULL)
				continue;
			if (building_objects[i]->getTeam() == bullet_objects[j]->getTeam())
				continue;
			Data temp2 = bullet_objects[j]->getCurrentPosition();
			p1.x = temp1.x;
			p1.y = temp1.y;
			p1.width = temp1.s;
			p1.height = temp1.s;
			p2.x = temp2.x;
			p2.y = temp2.y;
			p2.width = temp2.s;
			p2.height = temp2.s;
			if (p1.x < p2.x + p2.width &&
				p1.x + p1.width > p2.x &&
				p1.y < p2.y + p2.height &&
				p1.height + p1.y > p2.y)
			{
				float temp = building_objects[i]->getLife() - bullet_objects[j]->getLife();
				building_objects[i]->setLife(temp);

				//DeleteObject(i, 1);
				DeleteObject(j, 2);
			}


		}
	}
}

void SceneMgr::collisionCheck5()
{
	Rect p1 = { 0, };
	Rect p2 = { 0, };
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (building_objects[i] == NULL)
			continue;
		Data temp1 = building_objects[i]->getCurrentPosition();
		for (int j = 0; j < 100; ++j)
		{
			if (archer_objects[j] == NULL)
				continue;
			if (building_objects[i]->getTeam() == archer_objects[j]->getTeam())
				continue;
			Data temp2 = archer_objects[j]->getCurrentPosition();
			p1.x = temp1.x;
			p1.y = temp1.y;
			p1.width = temp1.s;
			p1.height = temp1.s;
			p2.x = temp2.x;
			p2.y = temp2.y;
			p2.width = temp2.s;
			p2.height = temp2.s;
			if (p1.x < p2.x + p2.width &&
				p1.x + p1.width > p2.x &&
				p1.y < p2.y + p2.height &&
				p1.height + p1.y > p2.y)
			{
				float temp = building_objects[i]->getLife() - archer_objects[j]->getLife();
				building_objects[i]->setLife(temp);

				//DeleteObject(i, 1);
				DeleteObject(j, 1);
			}


		}
	}
}



void SceneMgr::collisionCheck6()
{
	Rect p1 = { 0, };
	Rect p2 = { 0, };

	for (int i = 0; i < 100; ++i)
	{

		if (boomchar_objects[i] == NULL)
			continue;
		Data temp1 = boomchar_objects[i]->getCurrentPosition();

		for (int j = 0; j < 1000; ++j)
		{
			if (bullet_objects[j] == NULL)
				continue;
			if (boomchar_objects[i]->getTeam() == bullet_objects[j]->getTeam())
				continue;
			Data temp2 = bullet_objects[j]->getCurrentPosition();
			p1.x = temp1.x;
			p1.y = temp1.y;
			p1.width = temp1.s;
			p1.height = temp1.s;
			p2.x = temp2.x;
			p2.y = temp2.y;
			p2.width = temp2.s;
			p2.height = temp2.s;
			if (p1.x < p2.x + p2.width &&
				p1.x + p1.width > p2.x &&
				p1.y < p2.y + p2.height &&
				p1.height + p1.y > p2.y &&
				i != j) {
				float temp = boomchar_objects[i]->getLife() - bullet_objects[j]->getLife();
				boomchar_objects[i]->setLife(temp);
				//DeleteObject(i, 1);
				DeleteObject(j, 2);
			}


		}
	}
}


void SceneMgr::collisionCheck7()
{
	Rect p1 = { 0, };
	Rect p2 = { 0, };

	for (int i = 0; i < 100; ++i)
	{

		if (boomchar_objects[i] == NULL)
			continue;
		Data temp1 = boomchar_objects[i]->getCurrentPosition();

		for (int j = 0; j < 5000; ++j)
		{
			if (boomchar_objects[j] == NULL)
				continue;
			if (boomchar_objects[i]->getTeam() == arrow_objects[j]->getTeam())
				continue;
			Data temp2 = arrow_objects[j]->getCurrentPosition();
			p1.x = temp1.x;
			p1.y = temp1.y;
			p1.width = temp1.s;
			p1.height = temp1.s;
			p2.x = temp2.x;
			p2.y = temp2.y;
			p2.width = temp2.s;
			p2.height = temp2.s;
			if (p1.x < p2.x + p2.width &&
				p1.x + p1.width > p2.x &&
				p1.y < p2.y + p2.height &&
				p1.height + p1.y > p2.y &&
				i != (j / 10)) {
				float temp = boomchar_objects[i]->getLife() - arrow_objects[j]->getLife();
				boomchar_objects[i]->setLife(temp);

				//DeleteObject(i, 1);
				DeleteObject(j, 3);
			}


		}
	}
}


void SceneMgr::collisionCheck8()
{
	Rect p1 = { 0, };
	Rect p2 = { 0, };
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i)
	{
		if (building_objects[i] == NULL)
			continue;
		Data temp1 = building_objects[i]->getCurrentPosition();
		for (int j = 0; j < 100; ++j)
		{
			if (boomchar_objects[j] == NULL)
				continue;
			if (building_objects[i]->getTeam() == boomchar_objects[j]->getTeam())
				continue;
			Data temp2 = boomchar_objects[j]->getCurrentPosition();
			p1.x = temp1.x;
			p1.y = temp1.y;
			p1.width = temp1.s;
			p1.height = temp1.s;
			p2.x = temp2.x;
			p2.y = temp2.y;
			p2.width = temp2.s;
			p2.height = temp2.s;
			if (p1.x < p2.x + p2.width &&
				p1.x + p1.width > p2.x &&
				p1.y < p2.y + p2.height &&
				p1.height + p1.y > p2.y)
			{
				float temp = building_objects[i]->getLife() - boomchar_objects[j]->getLife();
				building_objects[i]->setLife(temp);
				sndPlaySoundA("explosion.wav", SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
				//DeleteObject(i, 1);
				DeleteObject(j, 4);
			}


		}
	}
}