#include "stdafx.h"
#include "WorldDemo.h"

#include "Tools/Viewer/OrbitCamera.h"
#include "Character/WorldPlayer.h"
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

	float width = Screen::Width(), height = Screen::Height();
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

	postEffect = new PostEffect(L"01_PostProcessing.fxo");
	postEffect->SRV(renderTarget->SRV());
}

void WorldDemo::Destroy()
{
	SafeDelete(enemy);

	SafeDelete(postEffect);

	SafeDelete(viewport);
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);

	SafeDelete(lights);
	SafeDelete(player);
	SafeDelete(scene);

	SafeRelease(shader);
}

void WorldDemo::Update()
{
	scene->Update();
	player->Update();
	
	postEffect->Update();
	enemy->Update();
}

void WorldDemo::PreRender()
{
	viewport->RSSetViewport();
	renderTarget->PreRender(depthStencil);

	lights->Render();
	scene->Render();

	enemy->Render();
	player->Render();
}

void WorldDemo::Render()
{
	postEffect->Render();
}

void WorldDemo::LoadScene()
{
	SceneEditor editor;
	scene = editor.Takeout(L"World2");
}
