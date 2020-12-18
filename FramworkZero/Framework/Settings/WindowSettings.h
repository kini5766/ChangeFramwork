#pragma once

struct WindowSettings
{
	LPCWSTR AppName = L"D3D Game";
	float Width = 1280;
	float Height = 720;
	bool bFullScreen = false;
};

struct D3DSettings
{
	bool bVsync = false;
	D3DXCOLOR ClearFlag = Color(0.3f, 0.3f, 0.3f, 1.0f);
};