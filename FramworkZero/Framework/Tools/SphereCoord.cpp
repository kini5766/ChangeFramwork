#include "Framework.h"
#include "SphereCoord.h"

#define PI Math::PI

SphereCoord::SphereCoord()
	: rho(1.0f), theta(PI * 0.5f), phi(0.0f)
{
}

SphereCoord::~SphereCoord()
{
}

void SphereCoord::Rho(float value)
{
	rho = value;
}

void SphereCoord::Theta(float value)
{
	theta = value;
}

void SphereCoord::Phi(float value)
{
	phi = value;
}

void SphereCoord::SetRectCoord(const Vector3 & value)
{
	Rho(D3DXVec3Length(&value));
	Vector3 normalized = value / Rho();
	Phi(atan2(normalized.y, normalized.x));
	Theta(acosf(normalized.z));
}

void SphereCoord::SetRectCoord_Y(const Vector3 & value)
{
	SetRectCoord(Vector3(value.z, value.x, value.y));
}

Vector3 SphereCoord::RectCoord_Z()
{
	float sinT = sinf(theta);
	float cosT = cosf(theta);
	float sinP = sinf(phi);
	float cosP = cosf(phi);

	return Vector3((rho * sinT * cosP), (rho * sinT * sinP), (rho * cosT));
}

Vector3 SphereCoord::RectCoord_Y()
{
	Vector3 result = RectCoord_Z();
	return Vector3(result.y, result.z, result.x);
}

Vector3 SphereCoord::RectCoord_YNormal()
{
	float sinT = sinf(theta);
	float cosT = cosf(theta);
	float sinP = sinf(phi);
	float cosP = cosf(phi);
	return Vector3(sinT * sinP, cosT, sinT * cosP);
}

Vector3 SphereCoord::YawPitch_Y()
{
	float thetaTemp;
	float phiTemp;
	ToAngle(&thetaTemp, &phiTemp);

	return Vector3(thetaTemp, phiTemp, 0.0f);
}

Vector3 SphereCoord::YawPitch_Z()
{
	Vector3 result = YawPitch_Y();
	result.x -= PI * 0.5f;  // 출발 좌표 보정 (구면 : 010), (오일러 : 001)
	return result;
}

void SphereCoord::Lerp(SphereCoord * out, const SphereCoord * s1, const SphereCoord * s2, float t)
{
	out->rho = (1 - t) * s1->rho + t * s1->rho;
	out->theta = Math::LerpBetweenTwoAngles(s1->theta, s2->theta, t);
	out->phi = Math::LerpBetweenTwoAngles(s1->phi, s2->phi, t);
	//Debug::Log->Print(to_string(s1->phi) + ", " + to_string(s2->phi));
}

void SphereCoord::ToAngle(float * outTheta, float * outPhi)
{
	if (rho >= 0.0f)
	{
		*outTheta = theta;
		*outPhi = phi;
		return;
	}

	// rho 음수 처리
	*outTheta = PI - theta;
	*outPhi = PI + phi;
}
