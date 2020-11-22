#include "Framework.h"
#include "Transform.h"
#include "Utilities/TransformData.h"

using namespace ShaderEffctConstantName;

Transform::Transform()
	: shader(nullptr), buffer(nullptr), sBuffer(nullptr)
{
	D3DXMatrixIdentity(&bufferDesc.World);
	data = new TransformData();
}

Transform::Transform(Shader * shader)
{
	SetShader(shader);
	D3DXMatrixIdentity(&bufferDesc.World);
	data = new TransformData();
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

	UpdateWorld();
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

void Transform::Set(Transform * value)
{
	data->Set(value->data);
	changed = true;
}

void Transform::Position(float x, float y, float z)
{
	Position(Vector3(x, y, z));
}

void Transform::Position(const Vector3 & value)
{
	data->Position(value);
	changed = true;
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
	changed = true;
}

void Transform::Scale(Vector3 * out)
{
	data->Scale(out);
}

void Transform::RotationDegree(float x, float y, float z)
{
	data->RotationDegree(x, y, z);
	changed = true;
}

void Transform::RotationDegree(const Vector3 & value)
{
	data->RotationDegree(value);
	changed = true;
}

void Transform::RotationDegree(Vector3 * out)
{
	data->RotationDegree(out);
}

void Transform::Rotation(float x, float y, float z)
{
	data->Rotation(x, y, z);
	changed = true;
}

void Transform::Rotation(const Vector3 & value)
{
	data->Rotation(value);
	changed = true;
}

void Transform::Rotation(Vector3 * out)
{
	data->Rotation(out);
}

void Transform::Rotation(const Quaternion & value)
{
	data->Rotation(value);
	changed = true;
}

void Transform::Rotation(Quaternion * out)
{
	data->Rotation(out);
}

void Transform::RotateYawDegree(float deg)
{
	data->RotateYawDegree(deg * Math::Deg2Rad());
	changed = true;
}

void Transform::RotateYaw(float rad)
{
	data->RotateYaw(rad);
	changed = true;
}

void Transform::RotatePitchDegree(float deg)
{
	data->RotatePitchDegree(deg * Math::Deg2Rad());
	changed = true;
}

void Transform::RotatePitch(float rad)
{
	data->RotatePitch(rad);
	changed = true;
}

void Transform::RotateRollDegree(float deg)
{
	data->RotateRoll(deg * Math::Deg2Rad());
	changed = true;
}

void Transform::RotateRoll(float rad)
{
	data->RotateRoll(rad);
	changed = true;
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

void Transform::LocalWorld(const Matrix & set)
{
	data->World(set);
	changed = true;
}

void Transform::World(const Matrix & set)
{
	if (parent == nullptr)
	{
		data->World(set);
		memcpy(&bufferDesc.World, &set, sizeof(Matrix));
		changed = false;
		return;
	}

	memcpy(&bufferDesc.World, &set, sizeof(Matrix));
	{
		Matrix invParent;
		D3DXMatrixInverse(&invParent, nullptr, &parent->World());
		data->World(invParent * set);
	}
	changed = false;
}

Matrix & Transform::World() 
{
	UpdateWorld();
	return bufferDesc.World; 
}

void Transform::UpdateWorld()
{
	if (changed == false)
		return;

	changed = false;

	data->World(&bufferDesc.World);
	if (parent == nullptr)
		return;

	Matrix& matrixParent = parent->World();
	bufferDesc.World *= matrixParent;
}

void Transform::SetParent(Transform * value)
{
	RemoveParent();
	if (value != nullptr)
		value->AddChild(this);
	parent = value;
}

void Transform::RemoveParent()
{
	if (parent == nullptr)
		return;

	parent->RemoveChild(this);
}

bool Transform::RemoveChild(Transform * value)
{
	vector<Transform*>::iterator iter = childs.begin();
	while (iter != childs.end())
	{
		if ((*iter) == value)
		{
			childs.erase(iter);
			return true;
		}

		++iter;
	}
	return false;
}

void Transform::AddChild(Transform * value)
{
	childs.push_back(value);
}


// ----------------------------------------------------------------------------
// TransformBuffer
// ----------------------------------------------------------------------------

TransformBuffer::TransformBuffer()
{
	D3DXMatrixIdentity(&bufferDesc.World);
	data = new TransformData();
	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
}

TransformBuffer::~TransformBuffer()
{
	SafeDelete(buffer);
	SafeDelete(data);
}

void TransformBuffer::UpdateWorld()
{
	data->World(&bufferDesc.World);
}

void TransformBuffer::Render()
{
	buffer->Render();
}

TransformData * TransformBuffer::Transform()
{
	return data;
}
