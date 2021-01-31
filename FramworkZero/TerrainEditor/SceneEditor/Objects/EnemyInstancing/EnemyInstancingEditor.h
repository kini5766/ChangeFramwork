#pragma once

#include "SceneEditor/Main/IObjectEditor.h"

class EnemyInstancingEditor : public IObjectEditor
{
public:
	EnemyInstancingEditor();
	~EnemyInstancingEditor();

public:
	// IObjectEditor을(를) 통해 상속됨
	void ImGuiRender() override;
	void Save(BinaryWriter * w) override;
	void Load(BinaryReader * r) override;
	void On() override;
	void Off() override;
	void Initialize(E_SceneValue * value) override;

private:
	class EnemyInstancingDesc* desc;
	class EnemyInstancingFile* file;
	class EnemyInstancingImGui* imgui;
};