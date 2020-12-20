#pragma once

class WinDesc
{
public:
	static HINSTANCE GetInstance();
	static HWND GetHandle();
	static LPCWSTR AppName();

#pragma region Window

public:
	static void Set(WinDesc* value);

private:
	static WinDesc* instance;

public:
	HINSTANCE hInstance;
	HWND handle;
	LPCWSTR appName;

#pragma endregion
};

class Screen
{
public:
	static float Width();
	static float Height();
	static bool IsFullScreen();

#pragma region Window
public:
	static void Set(Screen* value);

private:
	static Screen* instance;

public:
	float width;
	float height;
	bool bFullScreen;
#pragma endregion
};

class Input
{
public:
	static IMouse* Mouse();
	static IKeyboard* Keyboard();

#pragma region Window
public:
	static void Set(Input* value);

private:
	static Input* instance;

public:
	void SetMouse(IMouse* value);
	void SetKeyboard(IKeyboard* value);

public:
	IMouse* mouse = nullptr;
	IKeyboard* keyboard = nullptr;
#pragma endregion
};

class Debug
{
public:
	static Performance* Performance;
	static class DebugLine* Line;
	//static class DebugLog* Log;
};