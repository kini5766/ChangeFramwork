#pragma once

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

public:
	// Camera��(��) ���� ��ӵ�
	virtual void Update() override;

public:
	void Speed(float move, float rotation = 0.002f);

private:
	float move = 20.0f;
	float rotation = 0.002f;
};