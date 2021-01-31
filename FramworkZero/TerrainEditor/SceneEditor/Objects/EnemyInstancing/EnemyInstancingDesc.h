#pragma once

#include "SceneEditor/Component/ComponentTransform.h"

struct EnemyInstanceDesc
{
	Transform Transform;
	vector<Vector3> PatrolPoints;
};

class EnemyInstancingDesc
{
public:
	// 0 : Melee, 1 : Magician
	int EnemyType = 0;
	ComponentTransform* TImGui;
	vector<EnemyInstanceDesc*> Units;

	Color BoxColor = Color(1, 0, 1, 1);
	Transform* ModelTemp = nullptr;

public:
	EnemyInstancingDesc();
	~EnemyInstancingDesc();

public:
	void AddInstance();
	void SetScale(Transform * t);

public:
	void Render();

};