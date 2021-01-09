#include "stdafx.h"
#include "WorldDemo.h"

#include "Tools/Viewer/OrbitCamera.h"
#include "Player/WorldPlayer.h"
#include "Enemy/EnemyInstancing.h"
#include "Component/WorldLightGroup.h"
#include "EditorDemo/Main/SceneValue.h"
#include "EditorDemo/Main/SceneEditor.h"

void WorldDemo::Initialize()
{
	LoadScene();

	shader = Shader::Load(L"01_Material.fxo");

	player = new WorldPlayer(shader);
	enemy = new EnemyInstancing(shader, player->GetFocus());

	OrbitCamera* camera = new OrbitCamera();
	camera->SetTarget(player->GetFocus());
	Context::Get()->MainCamera(camera);

	lights = new WorldLightGroup();
}

void WorldDemo::Destroy()
{
	SafeDelete(enemy);

	SafeDelete(lights);
	SafeDelete(player);
	SafeDelete(scene);

	SafeRelease(shader);
}

void WorldDemo::Update()
{
	scene->Update();
	player->Update();
	
	enemy->Update();
}

void WorldDemo::PreRender()
{
}

void WorldDemo::Render()
{
	lights->Render();
	scene->Render();

	enemy->Render();
	player->Render();
}

void WorldDemo::LoadScene()
{
	SceneEditor editor;
	scene = editor.Takeout(L"World2");
	Debug::Gizmo->SetTransform(nullptr);
}
