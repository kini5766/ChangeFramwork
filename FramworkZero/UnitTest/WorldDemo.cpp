#include "stdafx.h"
#include "WorldDemo.h"

#include "Tools/Viewer/OrbitCamera.h"
#include "Character/WorldPlayer.h"
#include "Component/WorldLightGroup.h"
#include "EditorDemo/Main/SceneValue.h"
#include "EditorDemo/Main/SceneEditor.h"

void WorldDemo::Initialize()
{
	LoadScene();

	shader = Shader::Load(L"01_Material.fxo");

	player = new WorldPlayer(shader);

	OrbitCamera* camera = new OrbitCamera();
	camera->SetTarget(player->GetFocus());
	Context::Get()->MainCamera(camera);

	lights = new WorldLightGroup();
	Colliders();

	float width = Screen::Width(), height = Screen::Height();
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

	postEffect = new PostEffect(L"01_PostProcessing.fxo");
	postEffect->SRV(renderTarget->SRV());
}

void WorldDemo::Destroy()
{
	SafeDelete(postEffect);

	SafeDelete(viewport);
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);

	SafeDelete(sendbox1);
	SafeDelete(sendbox2);

	SafeDelete(lights);
	SafeDelete(player);
	SafeDelete(scene);

	SafeRelease(shader);
}

void WorldDemo::Update()
{
	scene->Update();
	player->Update();

	// sendbox
	{
		sendbox2->OnSendMessage();
		sendbox3->UpdateCycle(Time::Delta());
		Debug::Box->RenderBox(sendbox1->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));
		Debug::Box->RenderBox(sendbox2->GetTransform(), Color(0.3f, 0.7f, 0.3f, 1.0f));
		Debug::Box->RenderBox(sendbox3->GetTransform(), Color(0.3f, 0.3f, 0.7f, 1.0f));
	}

	postEffect->Update();
}

void WorldDemo::PreRender()
{
	viewport->RSSetViewport();
	renderTarget->PreRender(depthStencil);

	lights->Render();
	scene->Render();
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

void WorldDemo::Colliders()
{
	sendbox1 = new SendBox(CollisionManager::Get()->CreateCollider());
	sendbox1->GetTransform()->Position(10.0f, 2.0f, 10.0f);
	sendbox1->GetTransform()->Scale(2.0f, 2.0f, 2.0f);
	sendbox1->SetSendMessageData(&message);
	sendbox1->SetLayer(COLLIDER_LAYER_HITBOX);
	sendbox1->SetTag(L"1 hit");

	sendbox2 = new SendBox(CollisionManager::Get()->CreateCollider());
	sendbox2->GetTransform()->Position(-10.0f, 1.0f, -10.0f);
	sendbox2->GetTransform()->Scale(2.0f, 2.0f, 2.0f);
	sendbox2->SetSendMessageData(&message);
	sendbox2->SetLayer(COLLIDER_LAYER_HITBOX);
	sendbox2->SetTag(L"update hit");

	sendbox3 = new SendBox(CollisionManager::Get()->CreateCollider());
	sendbox3->GetTransform()->Position(10.0f, 2.0f, -10.0f);
	sendbox3->GetTransform()->Scale(3.0f, 3.0f, 3.0f);
	sendbox3->SetSendMessageData(&message);
	sendbox3->SetLayer(COLLIDER_LAYER_HITBOX);
	sendbox3->SetTag(L"1 second hit");
	sendbox3->SetCycle(1.0f);
}
