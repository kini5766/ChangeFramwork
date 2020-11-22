#pragma once

// ----------------------------------------------------------------------------
// TransformData
// ----------------------------------------------------------------------------

class TransformData
{
public:
	TransformData();
	~TransformData();

	void Set(TransformData* value);  // 복사

	void Position(float x, float y, float z);
	void Position(const Vector3& value);
	void Position(Vector3* out);

	void Scale(float x, float y, float z);
	void Scale(const Vector3& value);
	void Scale(Vector3* out);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(const Vector3& value);
	void RotationDegree(Vector3* out);  // 쿼터니온에서 변환한 거라 오류 있으니 출력 용도로만 사용

	void Rotation(float x, float y, float z);
	void Rotation(const Vector3& value);
	void Rotation(Vector3* out);  // 쿼터니온에서 변환한 거라 오류 있으니 출력 용도로만 사용

	void Rotation(const Quaternion& value);
	void Rotation(Quaternion* out);

	void RotateYawDegree(float deg);
	void RotateYaw(float rad);

	void RotatePitchDegree(float deg);
	void RotatePitch(float rad);

	void RotateRollDegree(float deg);
	void RotateRoll(float rad);

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	void World(const Matrix& set);
	void World(Matrix* out);

private:
	Vector3 position;
	Vector3 scale;
	Quaternion rotation;
};


// ----------------------------------------------------------------------------
// TransformFamily
// ----------------------------------------------------------------------------

class TransformFamily
{
public:
	TransformFamily();
	~TransformFamily();

	void SetParent(TransformFamily* value);

private:
	void RemoveParent();
	bool RemoveChild(TransformFamily* value);
	void AddChild(TransformFamily* value);

private:
	TransformFamily* parent = nullptr;
	vector<TransformFamily*> childs;
};