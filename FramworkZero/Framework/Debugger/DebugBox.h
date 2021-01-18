#pragma once

class DebugBox
{
public:
	friend class Window;  // 扩档快父 教臂沛 积己

private:
	DebugBox();
	~DebugBox();

public:
	void RenderBox(Transform* box, const Color& color = Color(0.0f, 1.0f, 0.0f, 1.0f));

private:
	Vector3 vertex[8];
};