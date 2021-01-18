#pragma once

class UIProjection : public Projection
{
public:
	UIProjection(float width, float height);
	~UIProjection();

	void Set(float width, float height, float zn = 0.0f, float zf = 0.0f, float fov = 0.0f) override;

private:

};