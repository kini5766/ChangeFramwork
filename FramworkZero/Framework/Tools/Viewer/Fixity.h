#pragma once

class Fixity : public Camera
{
public:
	Fixity();
	~Fixity();

private:
	// Camera��(��) ���� ��ӵ�
	virtual void Update() override;

public:
	// Camera��(��) ���� ��ӵ�
	virtual void ResizeScreen(float width, float height) override;

private:
	Orthographic* orthographic;
};
