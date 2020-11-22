#pragma once

class Transform
{
public:
	Transform();
	Transform(Shader* shader);
	~Transform();

	void Update();
	void Render();

	void Set(Transform* transfrom);  // 복사
	void SetShader(Shader* shader);

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
	void Rotation(Vector3* out);

	void Rotation(const Quaternion& value);
	void Rotation(Quaternion* out);

	void RotateYawDegree(float deg);
	void RotateYaw(float rad);

	void RotatePitchDegree(float deg);
	void RotatePitch(float rad);

	void RotateRollDegree(float deg);
	void RotateRoll(float rad);

	// 노말라이즈 생략
	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();


	void World(const Matrix& set);
	Matrix& World() { return bufferDesc.World; }

private:
	void UpdateWorld();

private:
	struct BufferDesc  // C버퍼
	{
		Matrix World;
	} bufferDesc;

private:
	Shader* shader;
	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	Vector3 position;
	Vector3 scale;
	Quaternion rotation;
};

// 생성자
// sBuffer = shader->AsConstantBuffer("CB_World")

// 업데이트
// sBuffer->SetConstantBuffer(buffer->Buffer())