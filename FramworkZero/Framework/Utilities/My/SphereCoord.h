#pragma once

class SphereCoord
{
public:
	SphereCoord();
	~SphereCoord();

	float Rho() { return rho; }
	float Theta() { return theta; }
	float Phi() { return phi; }

	void Rho(float value);
	void Theta(float value);
	void Phi(float value);

	void SetRectCoord(const Vector3& value);
	void SetRectCoord_Y(const Vector3& value);

	// 구면좌표 -> 직교좌표
	// _Z, _Y : 위쪽축

	Vector3 RectCoord_Z();
	Vector3 RectCoord_Y();


	// 구면좌표 -> 오일러
	// 앞:phi, 뒤: theta, _ : 시작축

	Vector3 YawPitch_Y();
	Vector3 YawPitch_Z();

private:
	void ToAngle(float* outTheta, float* outPhi);

private:
	float rho;
	float theta;
	float phi;
};