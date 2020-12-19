#include "Framework.h"
#include "Window.h"
#include "IExecute.h"

#include "Settings/WindowSettings.h"

IExecute* Window::mainExecute = nullptr;
Window::WinValue* Window::value = nullptr;

WPARAM Window::Run(IExecute * main, HINSTANCE instance)
{
	mainExecute = main;

	Create(instance);

	D3D::Create();

	Gui::Create();
	value->CreateGame();
	//DebugLine::Create();

	mainExecute->Initialize();

	MSG msg = { 0 };
	while (true)

	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			MainRender();
		}
	}
	mainExecute->Destroy();

	//DebugLine::Delete();
	value->DeleteGame();
	Gui::Delete();
	D3D::Delete();

	Shader::UnLoadAll();
	Destroy();

	return msg.wParam;
}

void Window::Create(HINSTANCE instance)
{
	WindowSettings settings;
	value = new WinValue(settings);
	value->desc->hInstance = (instance);

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = WinDesc::AppName();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (Screen::IsFullScreen())
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)Screen::Width();
		devMode.dmPelsHeight = (DWORD)Screen::Height();
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	HWND handle = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, WinDesc::AppName()
		, WinDesc::AppName()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, (HMENU)NULL
		, instance
		, NULL
	);
	assert(handle != NULL);
	value->desc->handle = (handle);


	RECT rect = { 0, 0, (LONG)Screen::Width(), (LONG)Screen::Height() };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)Screen::Width()) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)Screen::Height()) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		handle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);
	ShowWindow(handle, SW_SHOWNORMAL);
	SetForegroundWindow(handle);
	SetFocus(handle);

	ShowCursor(true);
}

void Window::Destroy()
{
	if (Screen::IsFullScreen())
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(WinDesc::GetHandle());

	UnregisterClass(WinDesc::AppName(), WinDesc::GetInstance());

	SafeDelete(value);
}

LRESULT CALLBACK Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	value->mouse->InputProc(message, wParam, lParam);

	if (Gui::Get()->MsgProc(handle, message, wParam, lParam))
		return true;

	if (message == WM_SIZE)
	{
		if (mainExecute != nullptr)
		{
			value->screen->width = ((float)LOWORD(lParam));
			value->screen->height = ((float)HIWORD(lParam));

			if (D3D::Get() != nullptr)
				D3D::Get()->ResizeScreen(Screen::Width(), Screen::Height());

			if (value->context != nullptr)
				value->context->ResizeScreen();

			mainExecute->ResizeScreen();
		}
	}

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

void Window::MainRender()
{
	value->time->Update();

	if (ImGui::IsMouseHoveringAnyWindow() == false)
	{
		// �ӱ���â ���� Ŀ�� ������ ������Ʈ ������
		value->keyboard->Update();
		value->mouse->Update();
	}

	Gui::Get()->Update();
	value->context->Update();
	//DebugLine::Get()->Update();

	mainExecute->Update();

	mainExecute->PreRender();
	
	D3D::Get()->SetRenderTarget();
	D3D::Get()->Clear();
	{
		value->context->Render();
		
		mainExecute->Render();

		mainExecute->PostRender();
		//DebugLine::Get()->Render();  // ����� ����
		Gui::Get()->Render();  // ����
	}
	D3D::Get()->Present();
}


// --
// WinValue
// --

Window::WinValue::WinValue(const WindowSettings& settings)
{
	desc = new WinDesc();
	screen = new Screen();

	WinDesc::Set(desc);
	Screen::Set(screen);
	desc->appName = (settings.AppName);

	screen->bFullScreen = (settings.bFullScreen);
	screen->height = (settings.Height);
	screen->width = (settings.Width);

}

Window::WinValue::~WinValue()
{
	SafeDelete(screen);
	SafeDelete(desc);
}

void Window::WinValue::CreateGame()
{
	time = new Time();
	input = new Input();
	mouse = new Mouse();
	keyboard = new Keyboard();
	context = new Context();
	//game = new GameLogic();
	Time::SetGlobal(time);
	Input::Set(input);
	input->SetMouse(mouse);
	input->SetKeyboard(keyboard);
	Context::Set(context);

	value->mouse->SetHandle(WinDesc::GetHandle());
	value->time->Start();

	Debug::Performance = new Performance();

	//GameLogic::Set(game);
}

void Window::WinValue::DeleteGame()
{
	SafeDelete(Debug::Performance);

	SafeDelete(context);
	SafeDelete(keyboard);
	SafeDelete(mouse);
	SafeDelete(input);
	SafeDelete(time);
	//SafeDelete(game);
}