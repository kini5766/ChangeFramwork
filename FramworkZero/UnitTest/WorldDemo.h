#pragma once
#include "Systems/IExecute.h"

class WorldDemo : public IExecute
{
public:
	// IExecute을(를) 통해 상속됨
	void Initialize() override;
	void Destroy() override;
	void Update() override;
	void PreRender() override {};
	void Render() override;
	void PostRender() override {};
	void ResizeScreen() override {};

	void Kachujin();

private:
	Shader* shader;
	MeshInstancing* box;
	ModelSkinnedInstancing* kachujin;
	Matrix* attachBones;
	Transform* gizmo;
};
