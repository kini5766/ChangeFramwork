#pragma once

class HPBar
{
public:
	HPBar();
	~HPBar();

public:
	void Update(float hpPer, float hpMax);
	void PostRender();

public:
	Transform* GetTransform() { return hpbarPos; }

private:
	Render2D* hpbarG;
	Render2D* hpbarR;

	Texture* green;
	Texture* red;

	Transform* hpbarPos;
	float barXScale = 100.0f;
};