#include "stdafx.h"
#include "WorldDemo.h"

#include "EditorDemo/Main/SceneValue.h"
#include "EditorDemo/Main/SceneEditor.h"
#include "Tools/Viewer/OrbitCamera.h"
#include "Component/WorldLightGroup.h"
#include "Player/WorldPlayer.h"
#include "Enemy/EnemyInstancing.h"
#include "Enemy/MeleeEnemy.h"
#include "Enemy/MagicianEnemy.h"

void WorldDemo::Initialize()
{
	shader = Shader::Load(L"01_Material.fxo");
	player = new WorldPlayer(shader);
	lights = new WorldLightGroup();
	LoadScene();

	OrbitCamera* camera = new OrbitCamera();
	camera->SetTarget(player->GetFocus());
	Context::Get()->MainCamera(camera);

	//meleeEnemy = new EnemyInstancing(player->GetFocus(), new MeleeEnemy(shader));
	//{
	//	Matrix w;
	//	Transform t;
	//	t.Scale(0.03f, 0.03f, 0.03f);
	//	t.Position(25.0f, 0.0f, 25.0f);
	//	t.RotationDegree(0.0f, -90.0f, 0.0f);
	//	t.LocalWorld(&w);
	//	vector<Vector3> patrolPoints;
	//	patrolPoints.push_back(Vector3(25.0f, 0.0f, 25.0f));
	//	patrolPoints.push_back(Vector3(0.0f, 0.0f, 25.0f));
	//	meleeEnemy->AddInstance(w, &patrolPoints);
	//}
	//magicianEnemy = new EnemyInstancing(player->GetFocus(), new MagicianEnemy(shader, player->GetFocus()));
	//{
	//	Matrix w;
	//	Transform t;
	//	t.Scale(0.03f, 0.03f, 0.03f);
	//	t.Position(-25.0f, 0.0f, -25.0f);
	//	t.RotationDegree(0.0f, -90.0f, 0.0f);
	//	t.LocalWorld(&w);
	//	vector<Vector3> patrolPoints;
	//	patrolPoints.push_back(Vector3(-25.0f, 0.0f, -25.0f));
	//	patrolPoints.push_back(Vector3(0.0f, 0.0f, -25.0f));
	//	magicianEnemy->AddInstance(w, &patrolPoints);
	//}
}

void WorldDemo::Destroy()
{
	//SafeDelete(magicianEnemy);
	//SafeDelete(meleeEnemy);

	SafeDelete(lights);
	SafeDelete(player);
	SafeDelete(scene);

	SafeRelease(shader);
}

void WorldDemo::Update()
{
	scene->Update();
	player->Update();
	
	//meleeEnemy->Update();
	//magicianEnemy->Update();
}

void WorldDemo::PreRender()
{
}

void WorldDemo::Render()
{
	lights->Render();
	scene->Render();

	//meleeEnemy->Render();
	//magicianEnemy->Render();
	player->Render();
}

void WorldDemo::LoadScene()
{
	SceneEditor editor;
	editor.AddValue("IFocusPlayer", player->GetFocus());
	scene = editor.Takeout(L"World2");
	Debug::Gizmo->SetTransform(nullptr);
}
