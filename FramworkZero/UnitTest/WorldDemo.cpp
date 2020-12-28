#include "stdafx.h"
#include "WorldDemo.h"

void WorldDemo::Initialize()
{
	Context::Get()->MainCamera()->RotationDegree(13, 70, 0);
	Context::Get()->MainCamera()->Position(-20, 1, -3);

	shader = Shader::Load(L"01_Material.fxo");

	Mesh();
	Kachujin();
	BurntLight();
	PointLights();
	SpotLights();
}

void WorldDemo::Destroy()
{
	SafeDelete(plane);

	SafeDelete(box);
	SafeDelete(kachujin);
	SafeRelease(shader);
}

void WorldDemo::Update()
{
	box->Update();
	kachujin->Update();
	plane->Update();
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
	if (ImGui::Button("Box"))
	{
		Debug::Gizmo->SetTransform(box->GetInstance(0)->GetTransform());
	}


	Lighting::Get()->GetDirectional()->ApplyTransform();
	Lighting::Get()->GetPointLight(0)->ApplyTransform();
	Lighting::Get()->GetBurntLight()->ApplyTransform();
	Lighting::Get()->GetSpotLight(0)->ApplyTransform();
	kachujin->UpdateTransforms();
	box->UpdateTransforms();

	box->Render();
	kachujin->Render();
	plane->Render();
}

void WorldDemo::Mesh()
{
	plane = new MeshInstancing(shader, new MeshPlane(2.5f, 2.5f));
	Transform* t = plane->AddInstance()->GetTransform();
	t->Scale(12, 1, 12);

	Material* m = plane->GetRenderer()->GetDefaultMaterial();
	m->DiffuseMap(L"Floor.png");
	m->SpecularMap(L"Floor_Specular.png");
	m->Specular(1, 1, 1, 20);
	m->NormalMap(L"Floor_Normal.png");

	plane->UpdateTransforms();
	plane->UpdateColors();


	box = new MeshInstancing(shader, new MeshCube());
	box->GetRenderer()->GetDefaultMaterial()->DiffuseMap(L"Box.png");

	Transform* transform;
	for (float x = -50; x <= 50; x += 2.5f)
	{
		transform = box->AddInstance()->GetTransform();
		transform->Scale(0.25f, 0.25f, 0.25f);
		transform->Position(Vector3(x, 0.25f * 0.5f, 0.0f));
		transform->Rotation(0, Math::Random(-(float)D3DX_PI, (float)D3DX_PI), 0);
	}

	transform = box->AddInstance()->GetTransform();
	transform->Position(12, 7, 7);
	transform->Scale(16, 14, 2);

	transform = box->AddInstance()->GetTransform();
	transform->Position(12, 7, -7);
	transform->Scale(16, 14, 2);

	transform = box->AddInstance()->GetTransform();
	transform->Position(12, 13, 0);
	transform->Scale(16, 2, 12);

	box->UpdateTransforms();
	box->UpdateColors();
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
