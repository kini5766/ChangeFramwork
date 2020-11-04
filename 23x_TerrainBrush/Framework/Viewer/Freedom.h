#pragma once

#include "Camera.h"

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

public:
	// Camera을(를) 통해 상속됨
	void Update() override;

public:
	void Speed(float move, float rotation);

private:
	float move = 20.0f;
	float rotation = 20.0f;
};