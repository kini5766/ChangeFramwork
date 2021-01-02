#include "stdafx.h"
#include "WorldDemo.h"

#include "Tools/Viewer/OrbitCamera.h"
#include "Character/Kachujin.h"
#include "Character/CharacterController.h"
#include "EditorDemo/SceneValue.h"
#include "EditorDemo/SceneEditor.h"

void WorldDemo::Initialize()
{
	shader = Shader::Load(L"01_Material.fxo");

	Mesh();
	Kachujin();
	BurntLight();
	PointLights();
	SpotLights();

	character = new KachujinInstance(kachujin->GetInstance(0));
	player = new CharacterController
	(
		character->GetInstance()->GetTransform(), 
		character->GetAnimator()
	);

	OrbitCamera* camera = new OrbitCamera();
	camera->SetTarget(player);
	Context::Get()->MainCamera(camera);

	Colliders();
}

void WorldDemo::Destroy()
{
	SafeDelete(scene);
	SafeDelete(player);
	SafeDelete(character);

	SafeDelete(kachujin);
	SafeRelease(shader);
}

void WorldDemo::Update()
{
	character->Update();
	player->Update();

	scene->Update();
	kachujin->Update();

	sendbox2->OnSendMessage();
	Debug::Box->RenderBox(sendbox1->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));
	Debug::Box->RenderBox(sendbox2->GetTransform(), Color(0.3f, 0.7f, 0.3f, 1.0f));
}

void WorldDemo::Render()
{
	if (ImGui::Button("unSelect"))
	{
		Debug::Gizmo->SetTransform(nullptr);
	}
	if (ImGui::Button("Direction Light"))
	{
		Debug::Gizmo->SetTransform(Lighting::Get()->GetDirectional()->GetTransform());
	}
	if (ImGui::Button("Point Light"))
	{
		Debug::Gizmo->SetTransform(Lighting::Get()->GetPointLight(0)->GetTransform());
	}
	if (ImGui::Button("Burnt Light"))
	{
		Debug::Gizmo->SetTransform(Lighting::Get()->GetBurntLight()->GetTransform());
	}
	if (ImGui::Button("Spot Light"))
	{
		Debug::Gizmo->SetTransform(Lighting::Get()->GetSpotLight(0)->GetTransform());
	}
	if (ImGui::Button("Character"))
	{
		Debug::Gizmo->SetTransform(kachujin->GetInstance(0)->GetTransform());
	}

	Lighting::Get()->GetDirectional()->ApplyTransform();
	Lighting::Get()->GetPointLight(0)->ApplyTransform();
	Lighting::Get()->GetBurntLight()->ApplyTransform();
	Lighting::Get()->GetSpotLight(0)->ApplyTransform();
	kachujin->UpdateTransforms();

	kachujin->Render();
	scene->Render();
}

void WorldDemo::Mesh()
{
	SceneEditor editor;
	scene = editor.Takeout(L"World2");
}

void WorldDemo::Kachujin()
{
	kachujin = new ModelSkinnedInstancing(shader, 
		{
			/*매쉬*/ L"Kachujin/Mesh",
			/*매터리얼*/ L"Kachujin/Mesh",
			/*클립*/ {
				L"Kachujin/Idle",
				L"Kachujin/Walk",
				L"Kachujin/Run",
				L"Kachujin/Slash",
				L"Kachujin/Uprock",
			}
		});

	ModelSkinnedInstance* instance = kachujin->AddInstance();
	instance->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
	instance->GetTransform()->RotationDegree(0.0f, -90.0f, 0.0f);

	kachujin->UpdateTransforms();
	kachujin->UpdateColors();
	kachujin->Pass(1);
}

void WorldDemo::BurntLight()
{
	Lighting::Get()->GetBurntLight()->GetDesc()->Scale = Vector3(16, 15, 14);
	Lighting::Get()->GetBurntLight()->GetDesc()->Position = Vector3(12, 6, 0);
	Lighting::Get()->GetBurntLight()->ApplyLight();
}

void WorldDemo::PointLights()
{
	PointLightDesc light;
	light =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
	   Color(0.0f, 0.3f, 1.0f, 1.0f), //Diffuse
	   Color(0.0f, 0.0f, 0.7f, 1.0f), //Specular
	   Color(0.0f, 0.0f, 0.7f, 1.0f), //Emissive
	   Vector3(15, 3, 0), 8.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);

}

void WorldDemo::SpotLights()
{
	SpotLightDesc light;
	light =
	{
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Color(0.7f, 1.0f, 0.0f, 1.0f),
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Vector3(-15, 20, -30), 25.0f,
	   Vector3(0, -1, 0), 30.0f, 0.9f
	};
	Lighting::Get()->AddSpotLight(light);

	light =
	{
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Vector3(0, 20, -30), 30.0f,
	   Vector3(0, -1, 0), 40.0f, 0.9f
	};
	Lighting::Get()->AddSpotLight(light);
}

void WorldDemo::Colliders()
{
	sendbox1 = CollisionManager::Get()->CreateSendBox();
	sendbox1->GetTransform()->Position(10.0f, 2.0f, 10.0f);
	sendbox1->GetTransform()->Scale(2.0f, 2.0f, 2.0f);
	sendbox1->SetSendMessageData(&message);
	sendbox1->SetLayer(COLLIDER_LAYER_HITBOX);
	sendbox1->SetTag(L"1 hit");

	sendbox2 = CollisionManager::Get()->CreateSendBox();
	sendbox2->GetTransform()->Position(-10.0f, 1.0f, -10.0f);
	sendbox2->GetTransform()->Scale(2.0f, 2.0f, 2.0f);
	sendbox2->SetSendMessageData(&message);
	sendbox2->SetLayer(COLLIDER_LAYER_HITBOX);
	sendbox2->SetTag(L"update hit");
}
