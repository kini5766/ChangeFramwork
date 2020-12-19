#pragma once

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

	// Camera��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void ResizeScreen(float width, float height) override;

	void Speed(float move, float rotation = 0.002f);

private:
	float move = 20.0f;
	float rotation = 0.002f;

	Perspective* perspective;
};