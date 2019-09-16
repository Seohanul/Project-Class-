#pragma once

class Scene
{
	friend Framework;

public:

	Scene() {}
	virtual ~Scene() {}

	virtual void Init() {}
	virtual void Exit() {}
	virtual void Render(float fInterpolation) {}
	virtual void Update() { }

protected:
	static Framework*	m_Framework;
};