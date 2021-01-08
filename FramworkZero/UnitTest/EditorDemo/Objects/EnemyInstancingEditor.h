#pragma once

#include "../Main/IObjectEditor.h"

class EnemyInstancingEditor : public IObjectEditor
{
public:
	EnemyInstancingEditor();
	~EnemyInstancingEditor();

public:
	// IObjectEditor을(를) 통해 상속됨
	void ImGuiRender() override;
	void Update() override;
	void Render() override;
	void Save(class BinaryWriter * w) override;
	void Load(class BinaryReader * r) override;
	void On() override;
	void Off() override;

private:
	void AddInstance();
	void Select(int i);

	// render
private:
	Shader* shader;
	vector<class EnemyInstance*> meshes;
	class EnemyInstancing* enemyInstancing = nullptr;

	// imgui
private:
	int selected = -1;
	int imguiItem = 0;

	// save load
private:
	class TransformEditor* tImGui;
};