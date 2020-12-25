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

	// 게임좌표는 SetRectCoord_Y 사용 바람
	void SetRectCoord(const Vector3& value);
	// 자주 사용
	void SetRectCoord_Y(const Vector3& value);

	// 구면좌표 -> 직교좌표
	// _Z, _Y : 위쪽축

	// 게임좌표는 RectCoord_Y 사용 바람
	Vector3 RectCoord_Z();
	// 자주 사용
	Vector3 RectCoord_Y();


	// 구면좌표 -> 오일러
	// 앞:phi, 뒤: theta, _ : 시작축

	// 게임좌표는 YawPitch_Z 사용 바람
	Vector3 YawPitch_Y();
	// 자주 사용
	Vector3 YawPitch_Z();

private:
	void ToAngle(float* outTheta, float* outPhi);

private:
	float rho;
	float theta;
	float phi;
};