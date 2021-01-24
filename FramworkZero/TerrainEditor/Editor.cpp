#include "stdafx.h"
#include "Editor.h"

#include "Brush.h"
#include "File/TerrainFile.h"

void Editor::Initialize()
{
	Context::Get()->MainCamera()->RotationDegree(20, 0, 0);
	Context::Get()->MainCamera()->Position(126, 35, 39);
	((Freedom*)Context::Get()->MainCamera())->Speed(100);

	shader = Shader::Load(L"01_Mesh.fxo");

	brush = new Brush();
	menuFile = new TerrainFile(brush);
	menuFile->SetFuncOpen(bind(&Editor::OpenTerrain, this, placeholders::_1));

	sky = new CubeSky(L"Environment/Mountain1024.dds");
	peojTexure = new ProjectionTexture(Shader::Load(URI::Shaders + L"01_Terrain.fxo"), L"Environment/MagicCircle.png", 21.7f, 22.0f);
	shadow = new Shadow(Vector3(128, 0, 128), 65.0f);
	Shadow::SetGlobal(shadow);
}

void Editor::Destroy()
{
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
	ImGui::Begin("ImGui Test");
	{
		ImGui::SliderFloat3("Directional Light", Lighting::Get()->DirectionalDesc()->Direction, -1.0f, 1.0f);

		peojTexure->Update();
		//shadow->ImGuiRender();
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
		terrain->PreRender_Depth();
}

void Editor::Render()
{
	sky->Render();

	peojTexure->Render();

	if (terrain != nullptr)
	{
		brush->Render();
		terrain->Render();
	}
}

void Editor::OpenTerrain(wstring file)
{
	SafeDelete(terrain);

	terrain = new Terrain(file);
	terrain->BaseMap(L"Terrain/Dirt3.png");

	brush->SetTerrain(terrain);
}
