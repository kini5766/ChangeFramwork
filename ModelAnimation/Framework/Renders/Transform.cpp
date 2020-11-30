#include "Framework.h"
#include "Transform.h"
#include "Utilities/TransformData.h"

using namespace ShaderEffctConstantName;

Transform::Transform()
	: shader(nullptr), buffer(nullptr), sBuffer(nullptr)
{
	data = new TransformData(&bufferDesc.World);
}

Transform::Transform(Shader * shader)
{
	SetShader(shader);
	data = new TransformData(&bufferDesc.World);
}

Transform::~Transform()
{
	SafeDelete(buffer);
	SafeDelete(data);
}

void Transform::Update()
{
}

void Transform::Render()
{
	if (buffer == nullptr)
		return;

	data->UpdateWorld();
	buffer->Render();

	if (shader == nullptr)
		return;

	sBuffer->SetConstantBuffer(buffer->Buffer());
}

void Transform::SetShader(Shader * _shader)
{
	shader = _shader;
	CreateBuffer();
	sBuffer = shader->AsConstantBuffer(CB_World);
}

void Transform::CreateBuffer()
{
	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
}

Matrix & Transform::World()
{
	data->UpdateWorld();
	return bufferDesc.World;
}


#pragma region TransformData

void Transform::Set(Transform * value)
{
	data->Set(value->data);
}

void Transform::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void Transform::Position(const Vector3 & value)
{
	data->Position(value);
}

void Transform::Position(Vector3 * out)
{
	data->Position(out);
}

void Transform::Scale(float x, float y, float z)
{
	Scale(Vector3(x, y, z));
}

void Transform::Scale(const Vector3 & value)
{
	data->Scale(value);
}

void Transform::Scale(Vector3 * out)
{
	data->Scale(out);
}

void Transform::RotationDegree(float x, float y, float z)
{
	data->RotationDegree(x, y, z);
}

void Transform::RotationDegree(const Vector3 & value)
{
	data->RotationDegree(value);
}

void Transform::RotationDegree(Vector3 * out)
{
	data->RotationDegree(out);
}

void Transform::Rotation(float x, float y, float z)
{
	data->Rotation(x, y, z);
}

void Transform::Rotation(const Vector3 & value)
{
	data->Rotation(value);
}

void Transform::Rotation(const Quaternion & value)
{
	data->Rotation(value);
}

void Transform::Rotation(Quaternion * out)
{
	data->Rotation(out);
}

void Transform::RotateYawDegree(float deg)
{
	data->RotateYawDegree(deg * Math::Deg2Rad());
}

void Transform::RotateYaw(float rad)
{
	data->RotateYaw(rad);
}

void Transform::RotatePitchDegree(float deg)
{
	data->RotatePitchDegree(deg * Math::Deg2Rad());
}

void Transform::RotatePitch(float rad)
{
	data->RotatePitch(rad);
}

void Transform::RotateRollDegree(float deg)
{
	data->RotateRoll(deg * Math::Deg2Rad());
}

void Transform::RotateRoll(float rad)
{
	data->RotateRoll(rad);
}

Vector3 Transform::Forward()
{
	return data->Forward();
}

Vector3 Transform::Up()
{
	return data->Up();
}

Vector3 Transform::Right()
{
	return data->Right();
}

#pragma endregion
