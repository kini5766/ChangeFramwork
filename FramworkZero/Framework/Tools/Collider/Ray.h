#pragma once

// ¹ÝÁ÷¼±
struct Ray
{
	Ray() {};
	Ray(const Vector3& position, const Vector3& direction)
		: Position(position), Direction(direction) {};

	Vector3 Position = Vector3(0, 0, 0);
	Vector3 Direction = Vector3(0, 0, 0);
};

struct RaycastOutput
{
	RaycastOutput(const Ray& ray) : Ray(ray) {}

	Ray Ray;
	bool IsActive = true;
	bool IsCollision = false;
	float OutMinDistance = 0.0f;
};

class Raycast
{
public:
	Raycast(const RaycastOutput* output);
	~Raycast();

public:
	bool IsCollision() const { return output->IsCollision; }
	float GetMinDistance() const { return output->OutMinDistance; }
	void SetRay(const Ray& value) { inputRay = value; bools[0] = true; }
	void SetActiveSelf(bool value) { bools[1] = value; bools[0] = true; }
	void Destroy() { bools[2] = true; bools[0] = true; }

public:
	bool IsInput() const { return bools[0]; }
	bool IsActive() const { return bools[1]; }
	bool IsDestroy() const { return bools[2]; }
	Ray GetRay() const { return inputRay; }

public:
	Ray inputRay;
	// [0] : input, 
	// [1] : inputActive
	// [2] : inputDestroy
	// [3] : temp
	bitset<4> bools;
	const RaycastOutput* output;
};