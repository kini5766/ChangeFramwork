#pragma once

#include "SceneEditor/Main/IObjectEditor.h"

class EnemyInstancingEditor : public IObjectEditor
{
public:
	EnemyInstancingEditor();
	~EnemyInstancingEditor();

public:
	// IObjectEditor��(��) ���� ��ӵ�
	void ImGuiRender() override;
	void Save(BinaryWriter * w) override;
	void Load(BinaryReader * r) override;
	void On() override;
	void Off() override;
	void Initialize(E_SceneValue * value) override;

private:
	class E_SceneAction* renderer = nullptr;
	class EnemyInstancingDesc* desc;
	class EnemyInstancingFile* file;
	class EnemyInstancingImGui* imgui;
};