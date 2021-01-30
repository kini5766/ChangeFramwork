#pragma once

#include "../Main/IObjectSpawner.h"

class MeshInstancingSpawner : public IObjectSpawner
{
public:
	MeshInstancingSpawner();
	~MeshInstancingSpawner();

private:
	void Update();
	void Render();

private:
	// IObjectSpawner을(를) 통해 상속됨
	bool Load(BinaryReader * r) override;
	void Initialize(SceneValueUnit * scene) override;

	// render
private:
	Shader* shader;
	MeshInstancing* meshInstancing = nullptr;

private:
	struct InputValue
	{
		UINT Item;
		float _F1 = 1.0f;
		float _F2 = 1.0f;
		UINT _U1 = 1u;
		UINT _U2 = 1u;
	};
	MeshInstancing* GetInstancing(InputValue& value);
};