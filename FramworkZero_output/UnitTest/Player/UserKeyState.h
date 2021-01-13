#pragma once

class UserKeyState
{
public:
	UserKeyState();
	~UserKeyState();

public:
	void Update();

public:
	Vector2 GetAxis() const { return axis; }

private:
	Vector2 axis = Vector2(0.0f, 0.0f);
};