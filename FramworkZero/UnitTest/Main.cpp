#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"

#include "WorldDemo.h"
#include "EditorDemo/EditorDemo.h"

void Main::Initialize()
{
	//Push(new WorldDemo());
	Push(new EditorDemo());
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

	//AddTaskUpdate(execute);

	execute->Initialize();
}

/*
void Main::AddTaskUpdate(IExecute * execute)
{
	TaskDesc* task = new TaskDesc();
	task->Etc = "Main->AddTaskUpdate";
	task->Working = bind(&Main::TaskUpdate, this, (execute));
	GameLogic::AddUpdate(task);
}

void Main::TaskUpdate(IExecute * execute)
{
	execute->Update();
	TaskDesc* task = new TaskDesc();
	task->Etc = "Main->TaskUpdate";
	task->Working = bind(&Main::TaskUpdate, this, (execute));
	GameLogic::AddUpdate(task);
}
*/

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	Main* main = new Main();
	WPARAM wParam = Window::Run(main, instance);

	SafeDelete(main);

	return wParam;
}