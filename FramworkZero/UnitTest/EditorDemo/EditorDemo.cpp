#include "stdafx.h"
#include "EditorDemo.h"

#include "SceneEditor.h"

void EditorDemo::Initialize()
{
	editor = new SceneEditor();
}

void EditorDemo::Destroy()
{
	SafeDelete(editor);
}

void EditorDemo::Update()
{
	editor->Update();
}

void EditorDemo::Render()
{
	if (ImGui::Button("Direction Light"))
	{
		Debug::Gizmo->SetTransform(Lighting::Get()->GetDirectional()->GetTransform());
	}
	editor->Render();
}
