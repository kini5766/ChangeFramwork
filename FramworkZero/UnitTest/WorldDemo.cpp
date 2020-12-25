#include "stdafx.h"
#include "WorldDemo.h"

void WorldDemo::Initialize()
{
	Context::Get()->MainCamera()->RotationDegree(13, 70, 0);
	Context::Get()->MainCamera()->Position(-20, 1, -3);

	shader = Shader::Load(L"01_Material.fxo");

	Mesh();
	Kachujin();
	PointLights();

	Lighting::Get()->GetBurntLight()->GetDesc()->Scale = Vector3(1, 1, 1);
	Lighting::Get()->GetBurntLight()->ApplyLight();
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
	Debug::Gizmo->SetTransform(Lighting::Get()->GetBurntLight()->GetTransform());
	Lighting::Get()->GetBurntLight()->ApplyTransform();

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

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = box->AddInstance()->GetTransform();
		transform->Scale(0.25f, 0.25f, 0.25f);
		transform->Position(Vector3(x, 0.25f * 0.5f, 0.0f));
		transform->Rotation(0, Math::Random(-(float)D3DX_PI, (float)D3DX_PI), 0);
	}
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

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* t = kachujin->AddInstance()->GetTransform();
		t->Position(x, 0, -5);
		t->Scale(0.01f, 0.01f, 0.01f);
	}

	kachujin->UpdateTransforms();
	kachujin->UpdateColors();
	kachujin->Pass(1);
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
	   Vector3(-30, 10, -30), 15.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f),
	   Color(1.0f, 0.0f, 0.0f, 1.0f),
	   Color(0.6f, 0.2f, 0.0f, 1.0f),
	   Color(0.6f, 0.3f, 0.0f, 1.0f),
	   Vector3(15, 10, -30), 10.0f, 0.3f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
	   Color(0.0f, 1.0f, 0.0f, 1.0f), //Diffuse
	   Color(0.0f, 0.7f, 0.0f, 1.0f), //Specular
	   Color(0.0f, 0.7f, 0.0f, 1.0f), //Emissive
	   Vector3(-5, 1, -17.5f), 5.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f),
	   Color(0.0f, 0.0f, 1.0f, 1.0f),
	   Color(0.0f, 0.0f, 0.7f, 1.0f),
	   Color(0.0f, 0.0f, 0.7f, 1.0f),
	   Vector3(-10, 1, -17.5f), 5.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);

}
