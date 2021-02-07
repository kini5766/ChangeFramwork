#pragma once

class OrbitCamera : public Camera
{
public:
	OrbitCamera();
	~OrbitCamera();

public:
	// Camera을(를) 통해 상속됨
	virtual void Update() override;

public:
	void SetTarget(class IFocus* value);

private:
	float rotateSpeed = 0.002f;

	class SphereCoord* sphereCoord;
	class IFocus* target = nullptr;
	float distance = 15.0f;
	Raycast* ray;
};