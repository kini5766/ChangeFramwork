#include "Framework.h"


#pragma region WinDesc

WinDesc* WinDesc::instance = nullptr;

HINSTANCE WinDesc::GetInstance() { return instance->hInstance; }

HWND WinDesc::GetHandle() { return instance->handle; }

LPCWSTR WinDesc::AppName() { return instance->appName; }

void WinDesc::Set(WinDesc * value) { instance = value; }

#pragma endregion


#pragma region Screen

Screen* Screen::instance = nullptr;

float Screen::Width() { return instance->width; }

float Screen::Height() { return instance->height; }

bool Screen::IsFullScreen() { return instance->bFullScreen; }

void Screen::Set(Screen * value) { instance = value; }

#pragma endregion


#pragma region Input

Input* Input::instance = nullptr;

IMouse * Input::Mouse()
{
	return instance->mouse;
}

IKeyboard * Input::Keyboard()
{
	return instance->keyboard;
}

void Input::Set(Input * value)
{
	instance = value;
}

void Input::SetMouse(IMouse * value)
{
	mouse = value;
}

void Input::SetKeyboard(IKeyboard * value)
{
	keyboard = value;
}

#pragma endregion


Performance* Debug::Performance = nullptr;
DebugLine* Debug::Line = nullptr;
DebugLog* Debug::Log = nullptr;
Gizmo* Debug::Gizmo = nullptr;
DebugBox* Debug::Box = nullptr;
