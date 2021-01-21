#pragma once

class BrushInput
{
public:
	enum class MouseState { NONE, UP, PRESS, DOWN, PICK };

public:
	BrushInput();
	~BrushInput();

	void Update();

public:
	MouseState GetState() { return state; }
	Vector3 GetMouse() { return mouse; }
	bool IsMouseMove() { return bMouseMove; }

private:
	Vector3 prevMousePos;
	bool bMouseMove;
	MouseState state;
	Vector3 mouse;

private:
	Vector3 pickMousePos;
};