#include "stdafx.h"
#include "Viewer.h"

void Viewer::Initialize()
{
	Context::Get()->MainCamera()->RotationDegree(20, 0, 0);
	Context::Get()->MainCamera()->Position(1, 36, -85);

	particle = new Particle(L"WaterFall");

	shader = Shader::Load(L"40_AreaLighting.fxo");
	sky = new CubeSky(L"Environment/SunsetCube1024.dds");

	Meshes();
}

void Viewer::Destroy()
{
	SafeDelete(particle);
	SafeDelete(sky);
	SafeDelete(sphere);
	SafeDelete(grid);

	SafeRelease(shader);
}

void Viewer::Update()
{
	ImGui::SliderFloat3("LightDirection", Lighting::Get()->DirectionalDesc()->Direction, -1, 1);

	// Pass
	{
		UINT passCount = particle->GetMaterial()->GetShader()->PassCount();
		ImGui::SliderInt("Pass", (int*)&particle->GetData().Type, 0, passCount - 1);
	
		static bool bWire = false;
		ImGui::Checkbox("Wire", &bWire);
		grid->Pass((bWire)? 3 : 0);
		sphere->Pass((bWire)? 3 : 0);
	}

	sky->Update();
	grid->Update();

	Vector3 p;
	sphere->GetTransform()->Position(&p);
	if (Input::Mouse()->Press(1) == false)
	{
		const Vector3& F = Context::Get()->MainCamera()->Forward();
		const Vector3& R = Context::Get()->MainCamera()->Right();
		const Vector3& U = Context::Get()->MainCamera()->Up();

		if (Input::Keyboard()->Press('W'))
			p += Vector3(F.x, 0.0f, F.z) * 20.0f * Time::Delta();
		if (Input::Keyboard()->Press('S'))
			p -= Vector3(F.x, 0.0f, F.z) * 20.0f * Time::Delta();

		if (Input::Keyboard()->Press('A'))
			p -= Vector3(R.x, 0.0f, R.z) * 20.0f * Time::Delta();
		if (Input::Keyboard()->Press('D'))
			p += Vector3(R.x, 0.0f, R.z) * 20.0f * Time::Delta();
	
		if (Input::Keyboard()->Press('E'))
			p += U * 20.0f * Time::Delta();
		if (Input::Keyboard()->Press('Q'))
			p -= U * 20.0f * Time::Delta();
	}
	sphere->GetTransform()->Position(p);
	sphere->Update();

	particle->Emit(p);
	particle->Update();
}

void Viewer::Render()
{
	sky->Render();

	sphere->Render();
	grid->Render();

	particle->Render();
}

void Viewer::Meshes()
{
	grid = new Mesh(shader, MeshPlane(15, 15));
	grid->GetTransform()->Position(0, 0, 0);
	grid->GetTransform()->Scale(20, 1, 20);
	grid->GetMaterial()->DiffuseMap("Floor.png");
	//grid->GetMaterial()->NormalMap("Floor_Normal.png");
	grid->GetMaterial()->NormalMap("Environment/Wave.dds");
	grid->GetMaterial()->SpecularMap("Floor_Specular.png");
	grid->GetMaterial()->Specular(1, 1, 1, 20);

	sphere = new Mesh(shader, MeshSphere(0.5f, 20, 20));
	sphere->GetTransform()->Position(0, 5, 0);
	sphere->GetTransform()->Scale(5, 5, 5);
	sphere->GetMaterial()->DiffuseMap("Bricks.png");
	sphere->GetMaterial()->NormalMap("Bricks_Normal.png");
	sphere->GetMaterial()->SpecularMap("Bricks_Specular.png");
	sphere->GetMaterial()->Specular(1, 1, 1, 20);
	sphere->GetMaterial()->Emissive(0.15f, 0.15f, 0.15f, 0.3f);

	meshes.push_back(grid);
	meshes.push_back(sphere);
}

void Viewer::Pass(UINT pass)
{
	for (Mesh* mesh : meshes)
		mesh->Pass(pass);
}
