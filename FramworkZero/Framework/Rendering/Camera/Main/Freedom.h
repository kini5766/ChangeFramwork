#pragma once

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

public:
	// Camera을(를) 통해 상속됨
	virtual void Update() override;

public:
	void Speed(float move, float rotation = 0.1f);

private:
	float move = 20.0f;
	float rotation = 0.1f;
};