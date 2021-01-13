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
	sky = new CubeSky(L"Environment/SnowCube1024.dds", shader);
	lights = new WorldLightGroup();

	player = new WorldPlayer(shader);
	LoadScene();
	Billboards();

	OrbitCamera* camera = new OrbitCamera();
	camera->SetTarget(player->GetFocus());
	Context::Get()->MainCamera(camera);
}

void WorldDemo::Destroy()
{
	SafeDelete(sky);
	SafeDelete(lights);
	SafeDelete(billboard);
	SafeDelete(player);
	SafeDelete(scene);

	SafeRelease(shader);
}

void WorldDemo::Update()
{
	billboard->Update();
	player->Update();
	scene->Update();
	sky->Update();
}

void WorldDemo::PreRender()
{
}

void WorldDemo::Render()
{
	sky->Render();
	lights->Render();
	billboard->Render();
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

void WorldDemo::Billboards()
{
	billboard = new Billboard(shader);
	billboard->SetTextures({
		L"Terrain/grass_14.tga",
		L"Terrain/grass_07.tga",
		L"Terrain/grass_11.tga",
		}
	);

	for (UINT i = 0; i < 4800; i++)
	{
		Vector2 position = Math::RandomVec2(-120.0f, 120.0f);
		Vector2 scale = Math::RandomVec2(2.5f, 5);
		billboard->Add(Vector3(position.x, scale.y * 0.5f, position.y), scale, 0);
	}
	for (UINT i = 0; i < 4800; i++)
	{
		Vector2 position = Math::RandomVec2(-120.0f, 120.0f);
		Vector2 scale = Math::RandomVec2(2.5f, 5);
		billboard->Add(Vector3(position.x, scale.y * 0.5f, position.y), scale, 1);
	}
	for (UINT i = 0; i < 4800; i++)
	{
		Vector2 position = Math::RandomVec2(-120.0f, 120.0f);
		Vector2 scale = Math::RandomVec2(2.5f, 5);
		billboard->Add(Vector3(position.x, scale.y * 0.5f, position.y), scale, 2);
	}
}
