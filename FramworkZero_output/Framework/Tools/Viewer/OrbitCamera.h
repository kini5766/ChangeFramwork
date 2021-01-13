#pragma once

#include "Tools/Viewer/Camera.h"

class OrbitCamera : public Camera
{
public:
	OrbitCamera();
	~OrbitCamera();

public:
	// Camera을(를) 통해 상속됨
	virtual void Update() override;
	virtual void ResizeScreen(float width, float height) override;

public:
	void SetTarget(class IFocus* value);

private:
	Perspective* perspective;

	float rotateSpeed = 0.002f;

	class SphereCoord* sphereCoord;
	class IFocus* target = nullptr;
	float distance = 15.0f;
	Raycast* ray;
};