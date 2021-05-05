#include "stdafx.h"
#include "MeshInstancingEditor.h"

#include "MeshInstancingDesc.h"
#include "MeshInstancingFile.h"
#include "MeshInstancingImGui.h"


MeshInstancingEditor::MeshInstancingEditor()
{
	desc = new MeshInstancingDesc();
	file = new MeshInstancingFile(desc);
	imgui = new MeshInstancingImGui(desc);
}

MeshInstancingEditor::~MeshInstancingEditor()
{
	SafeDelete(imgui);
	SafeDelete(file);
	SafeDelete(desc);
	renderer->bDestroy = true;
	updater->bDestroy = true;
}


void MeshInstancingEditor::Save(BinaryWriter * w)
{
	file->Save(w);
}

void MeshInstancingEditor::Load(BinaryReader * r)
{
	file->Load(r);
	imgui->SetImguiItem(desc->MeshType);
	imgui->Select(0);
}


void MeshInstancingEditor::ImGuiRender()
{
	imgui->ImGuiRender();
}

void MeshInstancingEditor::On()
{
	imgui->Select(0);
}

void MeshInstancingEditor::Off()
{
	imgui->Select(-1);
}


void MeshInstancingEditor::Initialize(E_SceneValue * value)
{
	 E_SceneValueUnit unit = value->AddUnit();

	 updater = unit.Updater;
	 updater->Action = bind(&MeshInstancingDesc::Update, desc);
	 updater->bDestroy = false;

	 renderer = unit.Renderer;
	 renderer->Action = bind(&MeshInstancingDesc::Render, desc);
	 renderer->bDestroy = false;
}

