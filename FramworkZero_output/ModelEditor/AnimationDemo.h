#pragma once
#include "Systems/IExecute.h"

class AnimationDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void Kachujin();

private:
	Shader* shader;
	MeshInstancing* box;
	ModelSkinnedInstancing* kachujin;
	Matrix* attachBones;
	Transform* gizmo;
};
