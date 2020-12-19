#pragma once

class Window
{
public:
	static WPARAM Run(class IExecute* main, HINSTANCE instance);

private:
	static void Create(HINSTANCE instance);
	static void Destroy();

	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	static void MainRender();

private:
	static class IExecute* mainExecute;

private:
	static class WinValue
	{
	public:
		WinValue(const struct WindowSettings& settings);
		~WinValue();

		void CreateGame();
		void DeleteGame();

	public:
		WinDesc* desc;
		Screen* screen;

		Input* input;
		Mouse* mouse;
		Keyboard* keyboard;
		Time* time;
		//GameLogic* game;

	}* value;
};
