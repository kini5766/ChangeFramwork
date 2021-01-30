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
}

void EditorDemo::Destroy()
{
	SafeDelete(scene);
}

void EditorDemo::Update()
{
	scene->Update();
}

void EditorDemo::Render()
{
	scene->Render();
}
