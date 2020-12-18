#pragma once

constexpr auto KEY_INPUT_MAX = 255;

class IKeyboard
{
public:
	virtual bool Down(DWORD key) = 0;
	virtual bool Up(DWORD key) = 0;
	virtual bool Press(DWORD key) = 0;
};

class Keyboard : public IKeyboard
{
public:
	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};

public:
	Keyboard();
	~Keyboard();

public:
	bool Down(DWORD key) override { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool Up(DWORD key) override  { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool Press(DWORD key) override { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }

public:
	void Update();

private:
	byte keyState[KEY_INPUT_MAX];
	byte keyOldState[KEY_INPUT_MAX];
	byte keyMap[KEY_INPUT_MAX];
};