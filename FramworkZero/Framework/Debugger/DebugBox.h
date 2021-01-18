#pragma once

class DebugBox
{
public:
	friend class Window;  // �����츸 �̱��� ����

private:
	DebugBox();
	~DebugBox();

public:
	void RenderBox(Transform* box, const Color& color = Color(0.0f, 1.0f, 0.0f, 1.0f));

private:
	Vector3 vertex[8];
};