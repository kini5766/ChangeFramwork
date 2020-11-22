#include "Framework.h"
#include "Transform.h"

using namespace ShaderEffctConstantName;

Transform::Transform()
	: shader(nullptr), buffer(nullptr), sBuffer(nullptr)
	, position(0, 0, 0), scale(1, 1, 1)
{
	D3DXQuaternionIdentity(&rotation);
	D3DXMatrixIdentity(&bufferDesc.World);
}

Transform::Transform(Shader * shader)
	: position(0, 0, 0), scale(1, 1, 1)
{
	SetShader(shader);
	D3DXQuaternionIdentity(&rotation);
	D3DXMatrixIdentity(&bufferDesc.World);
}

Transform::~Transform()
{
	SafeDelete(buffer);
}

void Transform::Update()
{
}

void Transform::Render()
{
	if (buffer != nullptr)
		buffer->Render();

	if (shader == nullptr)
		return;

	sBuffer->SetConstantBuffer(buffer->Buffer());
}

void Transform::Set(Transform * transfrom)
{
	position = transfrom->position;
	scale = transfrom->scale;
	rotation = transfrom->rotation;
	UpdateWorld();
}

void Transform::SetShader(Shader * _shader)
{
	shader = _shader;
	CreateBuffer();
	sBuffer = shader->AsConstantBuffer(CB_World);
}

void Transform::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void Transform::Position(const Vector3 & value)
{
	position = value;

	UpdateWorld();
}

void Transform::Position(Vector3 * out)
{
	*out = position;
}

void Transform::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void Transform::Scale(const Vector3 & value)
{
	scale = value;

	UpdateWorld();
}

void Transform::Scale(Vector3 * out)
{
	*out = scale;
}

void Transform::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Transform::RotationDegree(const Vector3 & value)
{
	Rotation(value * Math::Deg2Rad());
}

void Transform::RotationDegree(Vector3 * out)
{
	Rotation(out);
	(*out) *= Math::Rad2Deg();
}

void Transform::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Transform::Rotation(const Vector3 & value)
{
	D3DXQuaternionRotationYawPitchRoll(&rotation, value.y, value.x, value.z);
	UpdateWorld();
}

void Transform::Rotation(Vector3 * out)
{
	Quaternion& q = rotation;

	float xy2 = 2 * q.x * q.y;
	float xz2 = 2 * q.x * q.z;
	float yz2 = 2 * q.y * q.z;

	float xx2 = 2 * q.x * q.x;
	float yy2 = 2 * q.y * q.y;
	float zz2 = 2 * q.z * q.z;

	float xw2 = 2 * q.x * q.w;
	float yw2 = 2 * q.y * q.w;
	float zw2 = 2 * q.z * q.w;

	out->x = asin(xy2 + zw2);
	out->y = atan2(yw2 - xz2, 1 - yy2 - zz2);
	out->z = atan2(xw2 - yz2, 1 - xx2 - zz2);
}

void Transform::Rotation(const Quaternion & value)
{
	rotation = value;
	UpdateWorld();
}

void Transform::Rotation(Quaternion * out)
{
	*out = rotation;
}

void Transform::RotateYawDegree(float deg)
{
	RotateYaw(deg * Math::Deg2Rad());
}

void Transform::RotateYaw(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, 0.0f, rad, 0.0f);

	rotation = euler * rotation;
}

void Transform::RotatePitchDegree(float deg)
{
	RotatePitch(deg * Math::Deg2Rad());
}

void Transform::RotatePitch(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, rad, 0.0f, 0.0f);

	rotation = euler * rotation;
}

void Transform::RotateRollDegree(float deg)
{
	RotateRoll(deg * Math::Deg2Rad());
}

void Transform::RotateRoll(float rad)
{
	Quaternion euler;
	D3DXQuaternionRotationYawPitchRoll(&euler, 0.0f, 0.0f, rad);

	rotation *= euler;
}

Vector3 Transform::Forward()
{
	// 노말라이즈 생략
	return Vector3(bufferDesc.World._31, bufferDesc.World._32, bufferDesc.World._33);
}

Vector3 Transform::Up()
{
	// 노말라이즈 생략
	return Vector3(bufferDesc.World._21, bufferDesc.World._22, bufferDesc.World._23);
}

Vector3 Transform::Right()
{
	// 노말라이즈 생략
	return Vector3(bufferDesc.World._11, bufferDesc.World._12, bufferDesc.World._13);
}

void Transform::World(const Matrix & set)
{
	D3DXMatrixDecompose(&scale, &rotation, &position, &set);  // 쿼터니온
	//Math::MatrixDecompose(set, scale, rotation, position);  // 오일러

	memcpy(&bufferDesc.World, &set, sizeof(Matrix));
}

void Transform::CreateBuffer()
{
	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
}

void Transform::UpdateWorld()
{
	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	bufferDesc.World = S * R * T;
}
