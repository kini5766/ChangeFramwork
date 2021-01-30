#include "stdafx.h"
#include "WorldDemo.h"

#include "Objects/PostProcessing/PostEffectTest.h"

#include "EditorDemo/Main/SceneValue.h"
#include "EditorDemo/Main/SceneLoader.h"
#include "Tools/Viewer/OrbitCamera.h"
#include "Component/WorldLightGroup.h"
#include "Player/WorldPlayer.h"

void WorldDemo::Initialize()
{
	sky = new CubeSky(L"Environment/GrassCube1024.dds");
	lights = new WorldLightGroup();

	player = new WorldPlayer();
	scene = new SceneValue();
	LoadScene();
	Billboards();

	OrbitCamera* camera = new OrbitCamera();
	camera->SetTarget(player->GetFocus());
	Context::Get()->MainCamera(unique_ptr<Camera>(camera));

	postEffect = new PostEffectTest();
	Debug::Line->OffRendering();
}

void WorldDemo::Destroy()
{
	SafeDelete(postEffect);
	SafeDelete(sky);
	SafeDelete(lights);
	SafeDelete(billboard);
	SafeDelete(player);
	SafeDelete(scene);
}

void WorldDemo::Update()
{
	billboard->Update();
	player->Update();
	scene->Update();
	sky->Update();

	postEffect->Update();
	postEffect->ImGuiRender();
}

void WorldDemo::PreRender()
{
	postEffect->BeginPreRender();
	{
		sky->Render();
		lights->Render();
		billboard->Render();
		scene->Render();

		player->Render();
	}
	postEffect->EndPreRender();
}

void WorldDemo::Render()
{
	postEffect->Render();
}

void WorldDemo::LoadScene()
{
	scene->AddValue("IFocusPlayer", player->GetFocus());

	SceneLoader loader;
	loader.Load(scene, L"World2");

	scene->Initialize();
}

void WorldDemo::Billboards()
{
	billboard = new Billboard();
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
