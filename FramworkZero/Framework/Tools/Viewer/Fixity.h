#pragma once

class Fixity : public Camera
{
public:
	Fixity();
	~Fixity();

private:
	// Camera을(를) 통해 상속됨
	virtual void Update() override;

public:
	// Camera을(를) 통해 상속됨
	virtual void ResizeScreen(float width, float height) override;

private:
	Orthographic* orthographic;
};
