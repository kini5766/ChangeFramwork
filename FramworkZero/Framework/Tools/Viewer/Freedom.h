#pragma once

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

	// Camera을(를) 통해 상속됨
	virtual void Update() override;
	virtual void ResizeScreen(float width, float height) override;

	void Speed(float move, float rotation = 0.002f);

private:
	float move = 20.0f;
	float rotation = 0.002f;

	Perspective* perspective;
};