#pragma once
#include "Systems/IExecute.h"

// index buffer�� �׸��� ������ �ִ� ����

// �ͷ��� ���� (���� ��)
// far cry map editor
// -> �귯��
// 1. ���� -> ������ (�ֺ� ���� ������ ��źȭ)
// 2. ������
// 3. �����
// 4. ������ (�������� ��������)
// 5. slope (2���� ���� ������ ��Ż�� ���� �ʺ�� ������ arnge)
// ex. ������ �ٲ��� ó�� ���� ��ġ �״�� ����

class TerrainDemo : public IExecute
{
public:
	~TerrainDemo();

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
	class Terrain* terrain;
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