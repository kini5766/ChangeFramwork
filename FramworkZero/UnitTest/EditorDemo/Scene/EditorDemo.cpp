#include "stdafx.h"
#include "EditorDemo.h"

#include "../Main/SceneValue.h"
#include "../Main/SceneLoader.h"

void EditorDemo::Initialize()
{
	scene = new SceneValue();

	SceneLoader loader;
	loader.Load(scene, L"World");

	scene->Initialize();
	shadow = new Shadow(Vector3(-64.0f, 64.0f, 64.0f), 128.0f);
}

void EditorDemo::Destroy()
{
	SafeDelete(scene);
	SafeDelete(shadow);
}

void EditorDemo::Update()
{
	scene->Update();
	shadow->Update();
}

void EditorDemo::PreRender()
{
	shadow->PreRender();

}

void EditorDemo::Render()
{
	scene->Render();
}

void EditorDemo::PostRender()
{
	scene->PostRender();
}
