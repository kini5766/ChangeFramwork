#pragma once
#include "Systems/IExecute.h"

// index buffer�� �׸��� ������ �ִ� ����

class GridDemo : public IExecute
{
public:
	~GridDemo();

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

	UINT width = 128;
	UINT height = 128;

	UINT virtexCount;
	Vertex* vertices;
	ID3D11Buffer* vertexBuffer;

	UINT indexCount;
	UINT* indices;
	ID3D11Buffer* indexBuffer;
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