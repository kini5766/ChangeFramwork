#include "Framework.h"
#include "Collider.h"

// https://3dmpengines.tistory.com/1339

bool Collider::Collision(Obb & box1, Obb & box2)
{
	Vector3 position = box2.Position - box1.Position;

	if (SepratePlane(position, box1.AxisX, box1, box2)) return false;
	if (SepratePlane(position, box1.AxisY, box1, box2)) return false;
	if (SepratePlane(position, box1.AxisZ, box1, box2)) return false;

	if (SepratePlane(position, box2.AxisX, box1, box2)) return false;
	if (SepratePlane(position, box2.AxisY, box1, box2)) return false;
	if (SepratePlane(position, box2.AxisZ, box1, box2)) return false;

	// 축 계산 이유
	// x축
	if (SepratePlane(position, Cross(box1.AxisX, box2.AxisX), box1, box2)) return false;
	if (SepratePlane(position, Cross(box1.AxisX, box2.AxisY), box1, box2)) return false;
	if (SepratePlane(position, Cross(box1.AxisX, box2.AxisZ), box1, box2)) return false;

	// y축
	if (SepratePlane(position, Cross(box1.AxisY, box2.AxisX), box1, box2)) return false;
	if (SepratePlane(position, Cross(box1.AxisY, box2.AxisY), box1, box2)) return false;
	if (SepratePlane(position, Cross(box1.AxisY, box2.AxisZ), box1, box2)) return false;
	
	// y축
	if (SepratePlane(position, Cross(box1.AxisZ, box2.AxisX), box1, box2)) return false;
	if (SepratePlane(position, Cross(box1.AxisZ, box2.AxisY), box1, box2)) return false;
	if (SepratePlane(position, Cross(box1.AxisZ, box2.AxisZ), box1, box2)) return false;

	return true;
}

Collider::Collider(Transform * transform, Transform * offset)
	: transform(transform), init(offset)
{
	lines[0] = Vector3(-0.5f, -0.5f, -0.5f);
	lines[1] = Vector3(-0.5f, +0.5f, -0.5f);
	lines[2] = Vector3(+0.5f, -0.5f, -0.5f);
	lines[3] = Vector3(+0.5f, +0.5f, -0.5f);
	lines[4] = Vector3(-0.5f, -0.5f, +0.5f);
	lines[5] = Vector3(-0.5f, +0.5f, +0.5f);
	lines[6] = Vector3(+0.5f, -0.5f, +0.5f);
	lines[7] = Vector3(+0.5f, +0.5f, +0.5f);
}

Collider::~Collider()
{
	SafeDelete(init);
}

bool Collider::IsIntersect(Collider * other)
{
	return Collision(this->obb, other->obb);
}

void Collider::Update()
{
	SetObb();
}

void Collider::Render(Color color)
{
	Vector3 dest[8];

	Transform temp;
	temp.World(transform->World());

	// 부모 transform, 자식 init
	if (init != nullptr)
		temp.World(init->World() * transform->World());

	Matrix world = temp.World();

	for (UINT i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&dest[i], &lines[i], &world);

	DebugLine::Get()->RenderLine(dest[0], dest[1], color);
	DebugLine::Get()->RenderLine(dest[1], dest[3], color);
	DebugLine::Get()->RenderLine(dest[3], dest[2], color);
	DebugLine::Get()->RenderLine(dest[2], dest[0], color);

	DebugLine::Get()->RenderLine(dest[4], dest[5], color);
	DebugLine::Get()->RenderLine(dest[5], dest[7], color);
	DebugLine::Get()->RenderLine(dest[7], dest[6], color);
	DebugLine::Get()->RenderLine(dest[6], dest[4], color);

	DebugLine::Get()->RenderLine(dest[0], dest[4], color);
	DebugLine::Get()->RenderLine(dest[1], dest[5], color);
	DebugLine::Get()->RenderLine(dest[2], dest[6], color);
	DebugLine::Get()->RenderLine(dest[3], dest[7], color);
}

void Collider::SetObb()
{
	Transform temp;
	temp.World(transform->World());

	// 부모 transform, 자식 init
	if (init != nullptr)
		temp.World(init->World() * transform->World());

	temp.Position(&obb.Position);

	D3DXVec3Normalize(&obb.AxisX, &temp.Right());
	D3DXVec3Normalize(&obb.AxisY, &temp.Up());
	D3DXVec3Normalize(&obb.AxisZ, &temp.Forward());

	Vector3 scale;
	temp.Scale(&scale);
	obb.HalfSize = scale * 0.5f;
}

// true는 충돌 실패
bool Collider::SepratePlane(const Vector3 & position, const Vector3 & direction, Obb & box1, Obb & box2)
{
	// 보는 방향 내적
	float tl = fabsf(D3DXVec3Dot(&position, &direction));

	float r = 0.0f;
	r += fabsf(D3DXVec3Dot(&(box1.AxisX * box1.HalfSize.x), &direction));
	r += fabsf(D3DXVec3Dot(&(box1.AxisY * box1.HalfSize.y), &direction));
	r += fabsf(D3DXVec3Dot(&(box1.AxisZ * box1.HalfSize.z), &direction));
	r += fabsf(D3DXVec3Dot(&(box2.AxisX * box2.HalfSize.x), &direction));
	r += fabsf(D3DXVec3Dot(&(box2.AxisY * box2.HalfSize.y), &direction));
	r += fabsf(D3DXVec3Dot(&(box2.AxisZ * box2.HalfSize.z), &direction));

	return tl > r;
}

Vector3 Collider::Cross(const Vector3 & v1, const Vector3 & v2)
{
	float x = v1.y * v2.z - v1.z * v2.y;
	float y = v1.z * v2.x - v1.x * v2.z;
	float z = v1.x * v2.y - v1.y * v2.x;

	return Vector3(x, y, z);
}
