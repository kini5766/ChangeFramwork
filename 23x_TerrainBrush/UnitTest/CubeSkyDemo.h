#pragma once
#include "Systems/IExecute.h"

// index buffer�� �׸��� ������ �ִ� ����

class CubeSkyDemo : public IExecute
{
public:
	~CubeSkyDemo();

	// IExecute��(��) ���� ��ӵ�
	void Initialize() override;
	void Ready() override {};
	void Destroy() override;
	void Update() override;
	void PreRender() override {};
	void Render() override;
	void PostRender() override {};
	void ResizeScreen() override {};

private:
	Shader* shader;

	MeshCube* cube;
	MeshGrid* grid;
	MeshCylinder* cylinder[10];
	MeshSphere* sphere[10];

	Shader* terrainShader;
	class Terrain* terrain;

	class CubeSky* sky;
};

// IA -> VS -> RS -> PS -> OM
// IA : ��ǲ����� cpu -> gpu
// ��� �ڿ� : Vertex Buffer, Texture

// VS : ���� ���, NDC���� -> ������� ��ȯ

// RS : (�ϴ����� ���Ƽ� ����) 3d -> 2d (NDC����)
// RendertargetView
// DepthStencilView

// PS : ���伥

// OM : (�ƿ�ǲ����) ����Ÿ�ٺ� + �������ٽ� ��ħ
// ����Ÿ�ٺ�, �������ٽǺ� ������ ���ƾ���

// ȭ�� �������� ���� �޼�
// NDC ��ǥ : ��ġ��

// ���ҽ� : ����
// ���긮�ҽ���Ÿ : ���� Ÿ�� ���� ��