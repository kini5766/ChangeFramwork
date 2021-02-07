#pragma once

class Perspective : public Projection
{
public:
	Perspective(float width, float height, float zn = 0.1f, float zf = 1000.0f, float fov = (float)D3DX_PI * 0.25f);
	~Perspective();

public:
	void Resize(float width, float height);
	void Set(float width, float height, float zn = 0.1f, float zf = 1000.0f, float fov = (float)D3DX_PI * 0.25f) override;

private:
	float aspect;
};