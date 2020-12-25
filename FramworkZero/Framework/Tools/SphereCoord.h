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

	// ������ǥ�� SetRectCoord_Y ��� �ٶ�
	void SetRectCoord(const Vector3& value);
	// ���� ���
	void SetRectCoord_Y(const Vector3& value);

	// ������ǥ -> ������ǥ
	// _Z, _Y : ������

	// ������ǥ�� RectCoord_Y ��� �ٶ�
	Vector3 RectCoord_Z();
	// ���� ���
	Vector3 RectCoord_Y();


	// ������ǥ -> ���Ϸ�
	// ��:phi, ��: theta, _ : ������

	// ������ǥ�� YawPitch_Z ��� �ٶ�
	Vector3 YawPitch_Y();
	// ���� ���
	Vector3 YawPitch_Z();

private:
	void ToAngle(float* outTheta, float* outPhi);

private:
	float rho;
	float theta;
	float phi;
};