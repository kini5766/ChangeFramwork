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

private:
	Shader* shader;
	MeshInstancing* box;
};
