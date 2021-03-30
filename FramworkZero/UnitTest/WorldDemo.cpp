#include "stdafx.h"
#include "WorldDemo.h"

#include "Objects/PostProcessing/PostEffectTest.h"

#include "EditorDemo/Main/SceneValue.h"
#include "EditorDemo/Main/SceneLoader.h"
#include "Rendering/Camera/Main/OrbitCamera.h"
#include "Component/WorldLightGroup.h"
#include "Player/WorldPlayer.h"

void WorldDemo::Initialize()
{
	sky = new CubeSky(L"Environment/GrassCube1024.dds");
	lights = new WorldLightGroup();
	shadow = new Shadow(Vector3(-64.0f, 64.0f, 64.0f), 128.0f);
	water = new Water({ 125, 0, 0 });
	postEffect = new PostEffectTest();

	player = new WorldPlayer();
	scene = new SceneValue();
	Billboards();
	LoadScene();

	OrbitCamera* camera = new OrbitCamera();
	camera->SetTarget(player->GetFocus());
	Context::Get()->MainCamera(unique_ptr<Camera>(camera));

	water->GetTransform()->Position(128, 1, 128);

}

void WorldDemo::Destroy()
{
	SafeDelete(scene);
	SafeDelete(billboard);
	SafeDelete(player);

	SafeDelete(postEffect);
	SafeDelete(water);
	SafeDelete(shadow);
	SafeDelete(lights);
	SafeDelete(sky);
}

void WorldDemo::Update()
{
	billboard->Update();
	player->Update();
	scene->Update();
	sky->Update();
	water->Update();

	shadow->Update();
	postEffect->Update();

	Vector3 direction = Lighting::Get()->DirectionalDesc()->Direction;
	ImGui::SliderFloat3("direction", direction, -1.0f, 1.0f);
	Lighting::Get()->DirectionalDesc()->Direction = direction;
	postEffect->ImGuiRender();
	//shadow->ImGuiRender();

	static bool bDebugLine = true;
	ImGui::Checkbox("DebugLine", &bDebugLine);
	if (bDebugLine)
		Debug::Line->OnRendering();
	else
		Debug::Line->OffRendering();
}

void WorldDemo::PreRender()
{
	shadow->PreRender();
	player->PreRender();

	water->PreRender_Reflection();
	{
		sky->Render();
		billboard->Render();
		scene->Render();
		player->Render();
	}

	water->PreRender_Refraction();
	{
		sky->Render();
		billboard->Render();
		scene->Render();
		player->Render();
	}

	postEffect->BeginPreRender();
	{
		sky->Render();
		billboard->Render();
		water->Render();
		scene->Render();
		player->Render();
	}
	postEffect->EndPreRender();
}

void WorldDemo::Render()
{
	postEffect->Render();
}

void WorldDemo::PostRender()
{
	player->PostRender();
	scene->PostRender();
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
