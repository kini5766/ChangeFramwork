#pragma once

class DebugBox
{
public:
	friend class Window;  // �����츸 �̱��� ����

private:
	DebugBox();
	~DebugBox();

public:
	void RenderBox(Transform* box, const Color& color);

private:  // Window
	void Render();

private:
	Vector3 vertex[8];

private:
	struct BoxDesc
	{
		Transform* Transform;
		Color Color;
	};
	vector<BoxDesc> boxes;
};