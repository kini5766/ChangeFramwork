#include "stdafx.h"
#include "WorldDemo.h"

#include "EditorDemo/Main/SceneValue.h"
#include "EditorDemo/Main/SceneEditor.h"
#include "Tools/Viewer/OrbitCamera.h"
#include "Component/WorldLightGroup.h"
#include "Player/WorldPlayer.h"

void WorldDemo::Initialize()
{
	shader = Shader::Load(L"01_Material.fxo");
	player = new WorldPlayer(shader);
	lights = new WorldLightGroup();
	LoadScene();

	OrbitCamera* camera = new OrbitCamera();
	camera->SetTarget(player->GetFocus());
	Context::Get()->MainCamera(camera);
}

void WorldDemo::Destroy()
{
	SafeDelete(lights);
	SafeDelete(player);
	SafeDelete(scene);

	SafeRelease(shader);
}

void WorldDemo::Update()
{
	player->Update();
	scene->Update();
}

void WorldDemo::PreRender()
{
}

void WorldDemo::Render()
{
	lights->Render();
	scene->Render();

	player->Render();
}

void WorldDemo::LoadScene()
{
	SceneEditor editor;
	editor.AddValue("IFocusPlayer", player->GetFocus());
	scene = editor.Takeout(L"World2");
	Debug::Gizmo->SetTransform(nullptr);
}
