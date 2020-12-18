#pragma once
#include "Systems/IExecute.h"

class WorldDemo : public IExecute
{
public:
	~WorldDemo();

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

	ID3D11Buffer* vertexBuffer;

	Matrix world;
	Matrix world2;
};
