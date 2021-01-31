#include "stdafx.h"
#include "EnemyInstancingEditor.h"

#include "EnemyInstancingDesc.h"
#include "EnemyInstancingFile.h"
#include "EnemyInstancingImGui.h"


EnemyInstancingEditor::EnemyInstancingEditor()
{
	desc = new EnemyInstancingDesc();
	file = new EnemyInstancingFile(desc);
	imgui = new EnemyInstancingImGui(desc);
}

EnemyInstancingEditor::~EnemyInstancingEditor()
{
	SafeDelete(desc);
	SafeDelete(file);
	SafeDelete(imgui)
}


void EnemyInstancingEditor::Save(BinaryWriter * w)
{
	file->Save(w);
}

void EnemyInstancingEditor::Load(BinaryReader * r)
{
	file->Load(r);
}


void EnemyInstancingEditor::ImGuiRender()
{
	imgui->ImGuiRender();
}

void EnemyInstancingEditor::On()
{
	desc->BoxColor = Color(1, 1, 1, 1);
}

void EnemyInstancingEditor::Off()
{
	desc->BoxColor = Color(1, 0, 1, 1);
	imgui->Select(-1);
}

void EnemyInstancingEditor::Initialize(E_SceneValue * value)
{
	E_SceneValueUnit unit = value->AddUnit();
	unit.Renderer->Action = bind(&EnemyInstancingDesc::Render, desc);
	unit.Renderer->bDestroy = false;
}

