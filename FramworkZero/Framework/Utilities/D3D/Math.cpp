#include "Framework.h"
#include "Math.h"

const float Math::PI = 3.14159265f;
const float Math::EPSILON = 0.000001f;
const float Math::Deg2Rad = 0.01745329251f;
const float Math::Rad2Deg = 57.2957795131f;

float Math::Modulo(float val1, float val2)
{
	while (val1 - val2 >= 0)
		val1 -= val2;

	return val1;
}

float Math::Random(float r1, float r2)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = r2 - r1;
	float val = random * diff;

	return r1 + val;
}

D3DXVECTOR2 Math::RandomVec2(float r1, float r2)
{
	D3DXVECTOR2 result;
	result.x = Random(r1, r2);
	result.y = Random(r1, r2);

	return result;
}

D3DXVECTOR3 Math::RandomVec3(float r1, float r2)
{
	D3DXVECTOR3 result;
	result.x = Random(r1, r2);
	result.y = Random(r1, r2);
	result.z = Random(r1, r2);

	return result;
}

D3DXCOLOR Math::RandomColor3()
{
	D3DXCOLOR result;
	result.r = Math::Random(0.0f, 1.0f);
	result.g = Math::Random(0.0f, 1.0f);
	result.b = Math::Random(0.0f, 1.0f);
	result.a = 1.0f;

	return result;
}

D3DXCOLOR Math::RandomColor4()
{
	D3DXCOLOR result;
	result.r = Math::Random(0.0f, 1.0f);
	result.g = Math::Random(0.0f, 1.0f);
	result.b = Math::Random(0.0f, 1.0f);
	result.a = Math::Random(0.0f, 1.0f);

	return result;
}

void Math::LerpMatrix(OUT D3DXMATRIX & out, const D3DXMATRIX & m1, const D3DXMATRIX & m2, float amount)
{
	out._11 = m1._11 + (m2._11 - m1._11) * amount;
	out._12 = m1._12 + (m2._12 - m1._12) * amount;
	out._13 = m1._13 + (m2._13 - m1._13) * amount;
	out._14 = m1._14 + (m2._14 - m1._14) * amount;

	out._21 = m1._21 + (m2._21 - m1._21) * amount;
	out._22 = m1._22 + (m2._22 - m1._22) * amount;
	out._23 = m1._23 + (m2._23 - m1._23) * amount;
	out._24 = m1._24 + (m2._24 - m1._24) * amount;

	out._31 = m1._31 + (m2._31 - m1._31) * amount;
	out._32 = m1._32 + (m2._32 - m1._32) * amount;
	out._33 = m1._33 + (m2._33 - m1._33) * amount;
	out._34 = m1._34 + (m2._34 - m1._34) * amount;

	out._41 = m1._41 + (m2._41 - m1._41) * amount;
	out._42 = m1._42 + (m2._42 - m1._42) * amount;
	out._43 = m1._43 + (m2._43 - m1._43) * amount;
	out._44 = m1._44 + (m2._44 - m1._44) * amount;
}

D3DXQUATERNION Math::LookAt(const D3DXVECTOR3 & origin, const D3DXVECTOR3 & target, const D3DXVECTOR3 & up)
{
	D3DXVECTOR3 f = (origin - target);
	D3DXVec3Normalize(&f, &f);

	D3DXVECTOR3 s;
	D3DXVec3Cross(&s, &up, &f);
	D3DXVec3Normalize(&s, &s);

	D3DXVECTOR3 u;
	D3DXVec3Cross(&u, &f, &s);

	float z = 1.0f + s.x + u.y + f.z;
	float fd = 2.0f * sqrtf(z);

	D3DXQUATERNION result;

	if (z > Math::EPSILON)
	{
		result.w = 0.25f * fd;
		result.x = (f.y - u.z) / fd;
		result.y = (s.z - f.x) / fd;
		result.z = (u.x - s.y) / fd;
	}
	else if (s.x > u.y && s.x > f.z)
	{
		fd = 2.0f * sqrtf(1.0f + s.x - u.y - f.z);
		result.w = (f.y - u.z) / fd;
		result.x = 0.25f * fd;
		result.y = (u.x + s.y) / fd;
		result.z = (s.z + f.x) / fd;
	}
	else if (u.y > f.z)
	{
		fd = 2.0f * sqrtf(1.0f + u.y - s.x - f.z);
		result.w = (s.z - f.x) / fd;
		result.x = (u.x - s.y) / fd;
		result.y = 0.25f * fd;
		result.z = (f.y + u.z) / fd;
	}
	else
	{
		fd = 2.0f * sqrtf(1.0f + f.z - s.x - u.y);
		result.w = (u.x - s.y) / fd;
		result.x = (s.z + f.x) / fd;
		result.y = (f.y + u.z) / fd;
		result.z = 0.25f * fd;
	}

	return result;
}

int Math::Random(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}

float Math::Gaussian(float val, UINT blurCount)
{
	float a = 1.0f / sqrtf(2 * PI * (float)blurCount * (float)blurCount);
	float c = 2.0f * (float)blurCount * (float)blurCount;
	float b = exp(-(val * val) / c);

	return a * b;
}

void Math::MatrixDecompose(const D3DXMATRIX & m, OUT Vector3 & S, OUT Vector3 & R, OUT Vector3 & T)
{
	D3DXQUATERNION rotation;
	D3DXMatrixDecompose(&S, &rotation, &T, &m);

	D3DXMATRIX temp;
	D3DXMatrixRotationQuaternion(&temp, &rotation);

	R.x = asin(-temp._32);
	R.y = atan2(temp._31, temp._33);
	R.z = atan2(temp._12, temp._22);
}

void Math::MakeMatrixRotationUpNormalAngle(OUT D3DXMATRIX & out, const Vector3 & normal, float rad)
{
	Vector3 axisY = normal;
	D3DXVec3Normalize(&axisY, &normal);

	Vector3 axisZ;
	Vector3 axisX;
	if (axisY.z == 0 && axisY.y == 0)
	{
		axisX = Vector3(axisY.y, -axisY.x, 0.0f);
		RotateVector(axisX, axisX, axisY, rad);
		D3DXVec3Cross(&axisZ, &axisX, &axisY);
	}
	else
	{
		axisZ = Vector3(0.0f, -axisY.z, axisY.y);
		RotateVector(axisZ, axisZ, axisY, rad);
		D3DXVec3Cross(&axisX, &axisY, &axisZ);
	}

	D3DXVec3Normalize(&axisZ, &axisZ);
	D3DXVec3Normalize(&axisX, &axisX);

	out = D3DXMATRIX
	(
		axisX.x, axisX.y, axisX.z, 0.0f,
		axisY.x, axisY.y, axisY.z, 0.0f,
		axisZ.x, axisZ.y, axisZ.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void Math::RotateVector(OUT Vector3 & out, const Vector3 & target, const Vector3 & axis, float rad)
{
	// axis와 수직인 평면을 만들고
	// 그 평면을 2D상 좌표로 만들어 회전 시키고 3D상 벡터로 복구

	// 평면의 크기는 target과 axis 사이의 각도가 
	// 직교할 수록 1에 가까워지고
	// 평행할 수록 0에 가까워진다.

	// zCoord : 평면의 수직 벡터 (정규벡터 아님)
	// xCoord : 평면 상의 x축
	// yCoord : 평면 상의 y축
	Vector3 zCoord = axis * D3DXVec3Dot(&target, &axis);
	// 그램슈미트 직교화 (정규벡터 아님)
	Vector3 xCoord = target - zCoord;
	// 외적을 통한 직교화 (정규벡터 아님)
	Vector3 yCoord;
	D3DXVec3Cross(&yCoord, &axis, &xCoord);

	// x축에서 y축으로 각도만큼 화전
	xCoord *= cos(rad);
	yCoord *= sin(rad);

	// 3D상 벡터로 복구
	out.x = xCoord.x + yCoord.x + zCoord.x;
	out.y = xCoord.y + yCoord.y + zCoord.y;
	out.z = xCoord.z + yCoord.z + zCoord.z;
}

void Math::RotateVector(OUT Vector3 & out, const Quaternion & q, const Vector3 & point)
{
	float num = q.x * 2;
	float num2 = q.y * 2;
	float num3 = q.z * 2;
	float num4 = q.x * num;
	float num5 = q.y * num2;
	float num6 = q.z * num3;
	float num7 = q.x * num2;
	float num8 = q.x * num3;
	float num9 = q.y * num3;
	float num10 = q.w * num;
	float num11 = q.w * num2;
	float num12 = q.w * num3;

	out.x = (1.0f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
	out.y = (num7 + num12) * point.x + (1.0f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
	out.x = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.0f - (num4 + num5)) * point.z;
}
