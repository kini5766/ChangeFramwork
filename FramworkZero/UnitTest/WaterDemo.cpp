#include "stdafx.h"
#include "WaterDemo.h"

void WaterDemo::Initialize()
{
	Context::Get()->MainCamera()->GetTransform()->RotationEuler(EulerAngle::Degree(20, 0, 0));
	Context::Get()->MainCamera()->GetTransform()->Position(126, 36, -39);
	((Freedom*)Context::Get()->MainCamera())->Speed(50.0f);

	shadow = new Shadow(Vector3(128, 0, 128), 128.0f);

	sky = new CubeSky(L"Environment/SnowCube1024.dds");
	terrain = new Terrain(L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt.png");
	//terrain->GetMaterial()->NormalMap(L"Terrain/NormalMap.png");

	Mesh();
	Airplane();
	Kachujin();
	//Weapon();

	PointLights();
	SpotLights();

	water = new Water({ 125, 0, 0 });
	water->GetTransform()->Position(128, 5, 128);
}

void WaterDemo::Destroy()
{
	SafeDelete(water);

	SafeDelete(airplane);
	SafeDelete(kachujin);
	//SafeDelete(weapon);

	SafeDelete(cube);
	SafeDelete(cylinder);
	SafeDelete(sphere);

	SafeDelete(terrain);
	SafeDelete(sky);
	SafeDelete(shadow);
}

void WaterDemo::Update()
{
	sky->Update();

	cube->Update();
	cylinder->Update();
	sphere->Update();

	airplane->Update();
	kachujin->Update();

	//kachujin->UpdateBoneTracking();
	//UINT count = kachujin->GetInstanceCount();
	//for (UINT i = 0; i < count; i++)
	//{
	//	weapon->GetInstance(i)->GetTransform()->LocalWorld(kachujin->GetInstance(i)->GetAttachBone(40));
	//}

	//weapon->UpdateTransforms();
	//weapon->Update();

	terrain->Update();

	water->Update();
	shadow->Update();
}

void WaterDemo::PreRender()
{
	shadow->PreRender();

	water->PreRender_Reflection();
	{
		sky->Render();

		sphere->Render();
		cylinder->Render();
		cube->Render();

		airplane->Render();
		kachujin->Render();
		//weapon->Render();

		terrain->Render();
	}

	water->PreRender_Refraction();
	{
		sky->Render();

		sphere->Render();
		cylinder->Render();
		cube->Render();

		airplane->Render();
		kachujin->Render();
		//weapon->Render();

		terrain->Render();
	}

}

void WaterDemo::Render()
{
	sky->Render();

	sphere->Render();
	cylinder->Render();
	cube->Render();

	airplane->Render();
	kachujin->Render();
	//weapon->Render();

	terrain->Render();

	water->Render();

	//ImGui::Begin("Map");
	//{
	//	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	//	ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	//	ImVec2 v;
	//	v.x = vMax.x - vMin.x;
	//	v.y = vMax.y - vMin.y;
	//	ImGui::Image((void*)shadow->SRV(), v);
	//}
	//ImGui::End();
}

void WaterDemo::PostRender()
{
}

void WaterDemo::Mesh()
{

	MeshInstance* transform = nullptr;
	// cube
	{
		cube = new MeshInstancing(unique_ptr<MeshData>(new MeshCube()));
		transform = cube->AddInstance();
		transform->GetTransform()->Position(128, 0, 128);
		transform->GetTransform()->Scale(20, 10, 20);
		//SetTransform(transform);
	}

	cylinder = new MeshInstancing(unique_ptr<MeshData>(new MeshCylinder(0.3f, 0.5f, 3.0f, 20, 20)));
	sphere = new MeshInstancing(unique_ptr<MeshData>(new MeshSphere(0.5f, 20, 20)));
	for (UINT i = 0; i < 5; i++)
	{
		float posZ = (float)i * 15.0f - 15.0f;

		// cylinder
		transform = cylinder->AddInstance();
		transform->GetTransform()->Position(128 - 30, 6, 128 + posZ);
		transform->GetTransform()->Scale(5, 5, 5);
		//SetTransform(transform);

		transform = cylinder->AddInstance();
		transform->GetTransform()->Position(128 + 30, 6, 128 + posZ);
		transform->GetTransform()->Scale(5, 5, 5);
		//SetTransform(transform);


		// sphere
		transform = sphere->AddInstance();
		transform->GetTransform()->Position(128 - 30, 15.5f, 128 + posZ);
		transform->GetTransform()->Scale(5, 5, 5);
		//SetTransform(transform);

		transform = sphere->AddInstance();
		transform->GetTransform()->Position(128 + 30, 15.5f, 128 + posZ);
		transform->GetTransform()->Scale(5, 5, 5);
		//SetTransform(transform);
	}

	cube->UpdateTransforms();
	cylinder->UpdateTransforms();
	sphere->UpdateTransforms();

	{
		Material* stone = cube->GetMaterial();
		stone->DiffuseMap(L"Stones.png");
		stone->SpecularMap(L"Stones_Specular.png");
		stone->Specular(1, 1, 1, 20);
		stone->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		stone->NormalMap(L"Stones_Normal.png");

		Material* brick = cylinder->GetMaterial();
		brick->DiffuseMap(L"Bricks.png");
		brick->SpecularMap(L"Bricks_Specular.png");
		brick->Specular(1.0f, 1.0f, 1.0f, 20);
		brick->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		brick->NormalMap(L"Bricks_Normal.png");

		Material* wall = sphere->GetMaterial();
		wall->DiffuseMap(L"Wall.png");
		wall->SpecularMap(L"Wall_Specular.png");
		wall->Specular(1, 1, 1, 20);
		wall->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		wall->NormalMap(L"Wall_Normal.png");
	}
}

void WaterDemo::Airplane()
{
	airplane = new ModelInstancing({
		L"B787/Airplane",
		L"B787/Airplane" 
	});

	ModelInstance* transform = airplane->AddInstance();
	transform->GetTransform()->Position(128 + 2.0f, 9.91f, 128 + 2.0f);
	transform->GetTransform()->Scale(0.004f, 0.004f, 0.004f);
	//SetTransform(transform);
	airplane->UpdateTransforms();
}

void WaterDemo::Kachujin()
{
	kachujin = new ModelInstancing({
		/*Mesh*/ L"Kachujin/Mesh",
		/*Material*/ L"Kachujin/Mesh",
		/*Å¬¸³*/ {
			L"Kachujin/Idle",
			L"Kachujin/Walk",
			L"Kachujin/Run",
			L"Kachujin/Slash",
			L"Kachujin/Fall",
		} 
	});

	ModelInstance* t;

	t = kachujin->AddInstance();
	t->GetTransform()->Position(128 + 0, 0, 128 - 30);
	t->GetTransform()->Scale(0.075f, 0.075f, 0.075f);
	//SetTransform(t);
	t->GetAnimator()->Play(0);

	t = kachujin->AddInstance();
	t->GetTransform()->Position(128 - 15, 0, 128 - 30);
	t->GetTransform()->Scale(0.075f, 0.075f, 0.075f);
	//SetTransform(t);
	t->GetAnimator()->Play(1);

	t = kachujin->AddInstance();
	t->GetTransform()->Position(128 - 30, 0, 128 - 30);
	//SetTransform(t);
	t->GetTransform()->Scale(0.075f, 0.075f, 0.075f);
	t->GetAnimator()->Play(2);

	t = kachujin->AddInstance();
	t->GetTransform()->Position(128 + 15, 0, 128 - 30);
	t->GetTransform()->Scale(0.075f, 0.075f, 0.075f);
	//SetTransform(t);
	t->GetAnimator()->Play(3);

	t = kachujin->AddInstance();
	t->GetTransform()->Position(128 + 30, 0, 128 - 30);
	t->GetTransform()->Scale(0.075f, 0.075f, 0.075f);
	//SetTransform(t);
	t->GetAnimator()->Play(4);

	kachujin->UpdateTransforms();
}

//void WaterDemo::Weapon()
//{
//	weapon = new ModelInstancing({ L"Weapon/Sword", L"Weapon/Sword" });
//
//	UINT count = kachujin->GetInstanceCount();
//	for (UINT i = 0; i < count; i++)
//	{
//		ModelInstance* t = weapon->AddInstance();
//
//	}
//
//	weaponTransform->Position(-2.9f, 1.45f, -6.45f);
//	weaponTransform->Scale(0.5f, 0.5f, 0.75f);
//	weaponTransform->RotationEuler(EulerAngle::Degree(0, 0, 1));
//}

void WaterDemo::PointLights()
{
	PointLightDesc light;
	light =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
	   Color(0.0f, 0.3f, 1.0f, 1.0f), //Diffuse
	   Color(0.0f, 0.0f, 0.7f, 1.0f), //Specular
	   Color(0.0f, 0.0f, 0.7f, 1.0f), //Emissive
	   Vector3(128 - 30, 10, 128 - 30), 15.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f),
	   Color(1.0f, 0.0f, 0.0f, 1.0f),
	   Color(0.6f, 0.2f, 0.0f, 1.0f),
	   Color(0.6f, 0.3f, 0.0f, 1.0f),
	   Vector3(128 + 15, 10, 128 - 30), 10.0f, 0.3f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
	   Color(0.0f, 1.0f, 0.0f, 1.0f), //Diffuse
	   Color(0.0f, 0.7f, 0.0f, 1.0f), //Specular
	   Color(0.0f, 0.7f, 0.0f, 1.0f), //Emissive
	   Vector3(128 - 5, 1, 128.0f - 17.5f), 5.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);

	light =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f),
	   Color(0.0f, 0.0f, 1.0f, 1.0f),
	   Color(0.0f, 0.0f, 0.7f, 1.0f),
	   Color(0.0f, 0.0f, 0.7f, 1.0f),
	   Vector3(128 - 10, 1, 128.0f - 17.5f), 5.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(light);
}

void WaterDemo::SpotLights()
{
	SpotLightDesc light;
	light =
	{
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Color(0.7f, 1.0f, 0.0f, 1.0f),
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Vector3(128 - 15, 20, 128 - 30), 25.0f,
	   Vector3(0, -1, 0), 30.0f, 0.9f
	};
	Lighting::Get()->AddSpotLight(light);

	light =
	{
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Vector3(128 + 0, 20, 128 - 30), 30.0f,
	   Vector3(0, -1, 0), 40.0f, 0.9f
	};
	Lighting::Get()->AddSpotLight(light);
}

//void WaterDemo::SetTransform(Transform * transform)
//{
//	Vector3 position;
//	transform->Position(&position);
//
//	Vector3 scale;
//	transform->Scale(&scale);
//
//	float y = terrain->GetHeight(position);
//	position.y += scale.y * 0.5f;
//	transform->Position(position);
//}
