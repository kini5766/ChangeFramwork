#pragma once

class Transform
{
public:
	Transform();
	Transform(Shader* shader);
	~Transform();

public:
	void Update();
	void Render();

	void SetShader(Shader* shader);

	void CreateBuffer();
	ConstantBuffer* Buffer() { return buffer; }
	Matrix& World();

private:
	struct BufferDesc  // C버퍼
	{
		Matrix World;
	} bufferDesc;

private:
	Shader* shader;
	ID3DX11EffectConstantBuffer* sBuffer;
	ConstantBuffer* buffer;

	class TransformData* data;


#pragma region TransformData

public:
	void Set(Transform* value);  // 복사

	void Position(float x, float y, float z);
	void Position(const Vector3& value);
	void Position(Vector3* out);

	void Scale(float x, float y, float z);
	void Scale(const Vector3& value);
	void Scale(Vector3* out);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(const Vector3& value);
	void RotationDegree(Vector3* out);

	void Rotation(float x, float y, float z);
	void Rotation(const Vector3& value);

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

#pragma endregion


};
