#include "stdafx.h"
#include "EditorDemo.h"

#include "../Main/SceneEditor.h"

void EditorDemo::Initialize()
{
	editor = new SceneEditor();

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();
	viewport = new Viewport(Screen::Width(), Screen::Height());

}

void EditorDemo::Destroy()
{
	SafeDelete(viewport);
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);
	SafeDelete(editor);
}

void EditorDemo::Update()
{
	ImGui::Begin
	(
		"EditorDemo", NULL,
		ImGuiWindowFlags_NoDecoration |
		//ImGuiWindowFlags_NoInputs |
		//ImGuiWindowFlags_NoMove |
		//ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus
	);
	ImGui::DockSpace(ImGui::GetID("EditorDemo"));//, ImVec2(Screen::Width(), Screen::Height()));
	ImGui::End();

	editor->Update();
}

void EditorDemo::PreRender()
{
	viewport->RSSetViewport();
	renderTarget->PreRender(depthStencil);
	{
		editor->Render();
		Debug::Line->Render();
	}
}

void EditorDemo::Render()
{
	if (ImGui::Button("Direction Light"))
	{
		Debug::Gizmo->SetTransform(Lighting::Get()->GetDirectional()->GetTransform());
	}
	ImGui::Begin("Scene");
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		ImVec2 v;
		v.x = vMax.x - vMin.x;
		v.y = vMax.y - vMin.y;

		ImGui::Image((void*)renderTarget->SRV(), v);
	}
	ImGui::End();
}
