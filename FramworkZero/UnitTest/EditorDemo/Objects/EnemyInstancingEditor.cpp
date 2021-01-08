#include "stdafx.h"
#include "EnemyInstancingEditor.h"

#include "Component/TransformEditor.h"

EnemyInstancingEditor::EnemyInstancingEditor()
{
	shader = Shader::Load(L"01_Material.fxo");
	tImGui = new TransformEditor();
}

EnemyInstancingEditor::~EnemyInstancingEditor()
{
	SafeDelete(tImGui);
}

void EnemyInstancingEditor::ImGuiRender()
{
}

void EnemyInstancingEditor::Update()
{
}

void EnemyInstancingEditor::Render()
{
}

void EnemyInstancingEditor::Save(BinaryWriter * w)
{
}

void EnemyInstancingEditor::Load(BinaryReader * r)
{
}

void EnemyInstancingEditor::On()
{
}

void EnemyInstancingEditor::Off()
{
}

void EnemyInstancingEditor::AddInstance()
{
}

void EnemyInstancingEditor::Select(int i)
{
}
