#pragma once

#include "SceneEditor/Main/IObjectEditor.h"

class MeshInstancingEditor : public IObjectEditor
{
public:
	MeshInstancingEditor();
	~MeshInstancingEditor();

public:
	// IObjectEditor을(를) 통해 상속됨
	void Save(class BinaryWriter * w) override;
	void Load(class BinaryReader * r) override;

	void ImGuiRender() override;
	void On() override;
	void Off() override;

	void Initialize(E_SceneValue * value) override;

private:
	class MeshInstancingDesc* desc;

	class MeshInstancingFile* file;
	class MeshInstancingImGui* imgui;
};