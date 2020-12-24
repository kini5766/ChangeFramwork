#pragma once
#include "Systems/IExecute.h"

class WorldDemo : public IExecute
{
public:
	// IExecute��(��) ���� ��ӵ�
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
