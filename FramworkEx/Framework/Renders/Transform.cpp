#include "Framework.h"
#include "Transform.h"

using namespace ShaderEffctConstantName;

Transform::Transform()
	: shader(nullptr), buffer(nullptr), sBuffer(nullptr)
	, position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0)
{
	D3DXMatrixIdentity(&bufferDesc.World);
}

Transform::Transform(Shader * shader)
	: position(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0)
{
	SetShader(shader);
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
	if (shader = nullptr)
		return;

	buffer->Render();
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
	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
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

void Transform::Rotation(float x, float y, float z)
{
	Rotation(Vector3(x, y, z));
}

void Transform::Rotation(const Vector3 & value)
{
	rotation = value;

	UpdateWorld();
}

void Transform::Rotation(Vector3 * out)
{
	*out = rotation;
}

void Transform::RotationDegree(float x, float y, float z)
{
	RotationDegree(Vector3(x, y, z));
}

void Transform::RotationDegree(const Vector3 & value)
{
	rotation = value * Math::Deg2Rad();

	UpdateWorld();
}

void Transform::RotationDegree(Vector3 * out)
{
	*out = rotation * Math::Rad2Deg();  // Math  ToDegree
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
	// D3DXMatrixDecompose()  // 쿼터니온
	Math::MatrixDecompose(set, scale, rotation, position);

	memcpy(&bufferDesc.World, &set, sizeof(Matrix));
}

void Transform::UpdateWorld()
{
	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	bufferDesc.World = S * R * T;
}
