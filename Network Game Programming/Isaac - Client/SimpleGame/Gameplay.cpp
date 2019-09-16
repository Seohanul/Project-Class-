#include "stdafx.h"
#include "Gameplay.h"
#include "Framework.h"
#include "Renderer.h"
#include "Network.h"

void Gameplay::Init()
{
}

void Gameplay::Exit()
{
}


void Gameplay::Render(float fInterpolation)
{
	int Size = 0;

	/*for (size_t i = 0; i < NW.Positions.size(); ++i)
	{
	NW.RenderDevice.DrawTexRect(
	DX3 Load(NW.Positions[i]),
	{ 1.25, 1.25 },
	{ 1.f, 1.f, 1.f, 1.f },
	NW.TEX[0]
	);
	}*/

	Nw.RenderDevice.DrawTexRect(
		{ 0, 0, 10 },
		{ 20, 20 },
		{ 1.f, 1.f, 1.f, 1.f },
		Nw.TEX[4]
	);


	for (size_t i = 0; i < Nw.rendererData.size(); ++i)
	{
		for (size_t j = 0; Nw.rendererData[i].RenderDataSet[j].TextureID != -1; j++)
		{
			DX XMVECTOR Position = DX3 Load(Nw.rendererData[i].Position);
			DX XMVECTOR Size = DX2 Load(Nw.rendererData[i].RenderDataSet[j].SpriteSize);
			DX XMVECTOR Color = DX4 Load(Nw.rendererData[i].Color);
			Nw.RenderDevice.DrawShadow(Position, Size, Color);
			Nw.RenderDevice.DrawSprite
			(
				DX Add(Position, DX3 Load(Nw.rendererData[i].RenderDataSet[j].SpriteOffset))
				, Size
				, Color
				, Nw.rendererData[i].RenderDataSet[j].TextureID
				, DX2 Load(Nw.rendererData[i].RenderDataSet[j].Current)
				, DX2 Load(Nw.rendererData[i].RenderDataSet[j].Total)
			);
		}
	}
	//for (auto& i : ObjectSprite)
	//{
	//	DX XMVECTOR SpriteSize = i.GetSize();
	//	DX XMVECTOR SpriteOffset = i.GetOffset();

	//	//NW.RenderDevice.DrawShadow(Position, SpriteSize, Color);
	//	NW.RenderDevice.DrawSprite
	//	(
	//		DX Add(NW.Positions[i], SpriteOffset)
	//		, SpriteSize
	//		, Color
	//		, Engine.GetTexture(i.GetTexture())
	//		, Sprite.GetCurrent()
	//		, Sprite.GetTotal()
	//	);
	//}
}

void Gameplay::Update()
{

	// recv
	//Engine.Update();
}
