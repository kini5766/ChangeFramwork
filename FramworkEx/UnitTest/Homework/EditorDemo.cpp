#include "stdafx.h"
#include "EditorDemo.h"
#include "Editor/SceneValueEditor.h"
#include "Editor/SceneValue.h"

void EditorDemo::Initialize()
{
	shaderModel = new Shader(L"18_Animation.fx");
	value = new SceneValue();
	editor = new SceneValueEditor();

	//ModelRender* m = new ModelRender(shaderModel);
	//m->ReadMaterial(L"Tower/Tower");
	//m->ReadMesh(L"Tower/Tower");
	//m->GetTransform()->Position(0, -10, -10);
	//m->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	//m->Pass(1);
	//editor->AddDebug("Tower", m);
}

void EditorDemo::Destroy()
{
	SafeDelete(editor);
	SafeDelete(value);
	SafeDelete(shaderModel);
}

void EditorDemo::Update()
{
	editor->GetSceneValue(value);

	for (ModelAnimator* obj : value->ModelAnimObjs)
		obj->Update();

	for (ModelRender* obj : value->ModelObjs)
		obj->Update();

	for (Renderer* obj : value->RenderObjs)
		obj->Update();
}

void EditorDemo::Render()
{
	editor->Render();

	for (ModelAnimator* obj : value->ModelAnimObjs)
		obj->Render();

	for (ModelRender* obj : value->ModelObjs)
		obj->Render();

	for (Renderer* obj : value->RenderObjs)
		obj->Render();
}
