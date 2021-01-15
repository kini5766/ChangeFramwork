#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"

#include "ExportFile.h"
#include "ModelDemo.h"
#include "AnimationDemo.h"

void Main::Initialize()
{
	Push(new ExportFile());
	Push(new ModelDemo());
	//Push(new AnimationDemo());
}


void Main::Destroy()
{
	for (IExecute* exe : executes)
	{
		exe->Destroy();
		SafeDelete(exe);
	}
}

void Main::Update()
{
	for (IExecute* exe : executes)
		exe->Update();
}

void Main::PreRender()
{
	for (IExecute* exe : executes)
		exe->PreRender();
}

void Main::Render()
{
	for (IExecute* exe : executes)
		exe->Render();
}

void Main::PostRender()
{
	for (IExecute* exe : executes)
		exe->PostRender();
}

void Main::ResizeScreen()
{
	for (IExecute* exe : executes)
		exe->ResizeScreen();
}

void Main::Push(IExecute * execute)
{
	executes.push_back(execute);

	execute->Initialize();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	Main* main = new Main();
	WPARAM wParam = Window::Run(main, instance);

	SafeDelete(main);

	return wParam;
}