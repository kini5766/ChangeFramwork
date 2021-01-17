#pragma once

#include "../Main/IObjectEditor.h"

class EnemyInstancingEditor : public IObjectEditor
{
public:
	EnemyInstancingEditor(class IFocus* player);
	~EnemyInstancingEditor();

public:
	// IObjectEditor을(를) 통해 상속됨
	void ImGuiRender() override;
	void Update() override;
	void Render() override;
	void Save(BinaryWriter * w) override;
	void Load(BinaryReader * r) override;
	bool LoadTakeOut(BinaryReader * r) override;
	void On() override;
	void Off() override;

private:
	void CreateEnemy(int item);
	void AddInstance();
	void Select(int i);
	void SetScale(Transform* t);

	// render
private:
	class EnemyInstancing* enemy;
	class IFocus* player;

	// save load
private:
	class TransformEditor* tImGui;

	// imgui
private:
	int selected = -1;
	// 0 : Melee, 1 : Magician
	int enemyType = 0;
	Transform* modelTemp = nullptr;

	const char* enemyTypes = {
		"Melee\0"
		"Magician\0"
	};

	struct EnemyInstanceDesc
	{
		Transform Transform;
		vector<Vector3> PatrolPoints;
	};
	vector<EnemyInstanceDesc*> descs;
};