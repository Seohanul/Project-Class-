#include "stdafx.h"
#include "Graphics.h"
#include "CyanEngine.h"

#ifdef CYAN_DEBUG_COLLISION
void DrawBoundingBoxes(const Renderer& RenderDevice,  DX FXMVECTOR Position, DX FXMVECTOR Size)
{
	DX XMVECTOR BBoxCenter = Collision::GetBBoxCenter(Position, DX GetZ(Size));
	RenderDevice.DrawCollisionRect(BBoxCenter, Size);
}	 
#endif

void Graphics::SetColor(DX XMVECTOR Color)
{
	m_Color = DX4 Store(Color);
}

void Graphics::SetAlpha(float Value)
{
	m_Color.w = Value;
}

DX XMVECTOR XM_CALLCONV Graphics::GetColor() const
{
	return DX4 Load(m_Color);
}

void Graphics::Render (
	const Renderer& RenderDevice, 
	Physics& ObjectPhysics, 
	const STD vector<Sprite>& ObjectSprite, 
	float Interpolation)
{
	DX XMVECTOR Color = DX4 Load(m_Color);
	DX XMVECTOR Position = DX Add
	(
		DX Scale(ObjectPhysics.GetPosition(), Interpolation),
		DX Scale(ObjectPhysics.GetPrevPosition(), 1.f - Interpolation)
	);
	RenderData dataSet;
	RenderData2 dataSet2;
	int idx = 0;
	for (auto& Sprite : ObjectSprite)
	{
		DX XMVECTOR SpriteOffset = Sprite.GetOffset();

		dataSet2.TextureID = Engine.GetTexture(Sprite.GetTexture());
		dataSet2.Current = DX2 Store(Sprite.GetCurrent());
		dataSet2.Total = DX2 Store(Sprite.GetTotal());
		dataSet2.SpriteOffset = DX3 Store(Sprite.GetOffset());
		dataSet2.SpriteSize = DX2 Store(Sprite.GetSize());

		dataSet.RenderDataSet[idx++] = dataSet2;
		/*RenderDevice.DrawShadow(Position, Sprite.GetSize(), Color);
		RenderDevice.DrawSprite
		(
			DX Add(Position, SpriteOffset)
			, Sprite.GetSize()
			, Color
			, Engine.GetTexture(Sprite.GetTexture())
			, Sprite.GetCurrent()
			, Sprite.GetTotal()
		);*/
	}
	dataSet.Color = DX4 Store(Color);
	//DX Print(Position);
	//printf("\n");
	dataSet.Position = DX3 Store(Position);
	//printf("%f %f %f\n", dataSet.Position.x, dataSet.Position.y, dataSet.Position.z);
	Nw.rendererData.emplace_back(dataSet);

#ifdef CYAN_DEBUG_COLLISION
	DrawBoundingBoxes(RenderDevice, Position, ObjectPhysics.Box().GetDimensions());
#endif
}