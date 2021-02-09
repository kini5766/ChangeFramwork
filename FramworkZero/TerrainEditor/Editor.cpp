#include "stdafx.h"
#include "Editor.h"

#include "Brush.h"
#include "File/TerrainFile.h"

void Editor::Initialize()
{
	Context::Get()->MainCamera()->GetTransform()->RotationEuler(
		EulerAngle::Degree(20, 0, 0));
	Context::Get()->MainCamera()->GetTransform()->Position(126, 35, 39);
	((Freedom*)Context::Get()->MainCamera())->Speed(100, 0.1f);

	shader = Shader::Load(L"01_Mesh.fxo");

	brush = new Brush();
	menuFile = new TerrainFile(brush);
	menuFile->SetFuncOpen(bind(&Editor::OpenTerrain, this, placeholders::_1));

	sky = new CubeSky(L"Environment/Mountain1024.dds");
	peojTexure = new ProjectionTexture(Shader::Load(L"01_TerrainLOD.fxo"), L"Environment/MagicCircle.png", 217.0f, 220.0f);
	shadow = new Shadow(Vector3(-64.0f, 64.0f, 64.0f), 512.0f);
	Shadow::SetGlobal(shadow);

	water = new Water({ 125, 0, 0 });
	water->GetTransform()->Position(128, 32, 128);
	Debug::Gizmo->SetTransform(water->GetTransform());
}

void Editor::Destroy()
{
	SafeDelete(water);

	Shadow::SetGlobal(nullptr);
	SafeDelete(shadow);
	SafeDelete(peojTexure);
	SafeDelete(sky);

	SafeDelete(menuFile);
	SafeDelete(brush);
	SafeDelete(terrain);

	SafeRelease(shader);
}

void Editor::Update()
{
	water->Update();

	ImGui::Begin("ImGui Test");
	{
		ImGui::SliderFloat3("Directional Light", Lighting::Get()->DirectionalDesc()->Direction, -1.0f, 1.0f);

		peojTexure->Update();
		shadow->ImGuiRender();

		if (terrain != nullptr)
		{
			// HeightScale
			ImGui::SliderFloat("Height Scale", &terrain->HeightScale(), 0.001f, 1.0f);

			// Min Max Distance
			ImGui::SliderFloat2("Distance", terrain->Distance(), 1.0f, 1000.0f);
		}
	}
	ImGui::End();

	menuFile->RenderImGui();

	sky->Update();
	if (terrain != nullptr)
	{
		brush->Update();
		//terrain->RenderVisibleNormal();
		terrain->Update();
	}
}

void Editor::PreRender()
{
	shadow->PreRender();

	if (terrain != nullptr)
	{
		water->PreRender_Reflection();
		{
			sky->Render();
			terrain->Render();
		}

		water->PreRender_Refraction();
		{
			sky->Render();
			terrain->Render();
		}
	}
}

void Editor::Render()
{
	sky->Render();

	peojTexure->Render();

	if (terrain != nullptr)
	{
		water->Render();
		brush->Render();
		terrain->Render();
	}
}

void Editor::OpenTerrain(wstring file)
{
	SafeDelete(terrain);

	terrain = new TerrainLOD(file);
	terrain->BaseMap(L"Terrain/Sand (Beach Wet2).jpg");
	terrain->Layer1(L"Terrain/Cliff (Layered Rock).jpg");
	terrain->Layer2(L"Terrain/Grass (Meadows).jpg");
	terrain->Layer3(L"Terrain/Snow.jpg");

	brush->SetTerrain(terrain);
}
