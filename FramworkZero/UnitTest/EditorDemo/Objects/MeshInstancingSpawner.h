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
	virtual bool Load(BinaryReader * r) override;
	virtual void Initialize(SceneValueUnit * scene) override;

private:
	MeshInstancing* GetInstancing(UINT item);

	// render
private:
	Shader* shader;
	MeshInstancing* meshInstancing = nullptr;

private:
	float _f1 = 1.0f;
	float _f2 = 1.0f;
	UINT _u1 = 1u;
	UINT _u2 = 1u;
};