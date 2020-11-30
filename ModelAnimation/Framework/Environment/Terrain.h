#pragma once

// �ͷ���
// ���� ���� ������ (���)
// ���� �������� ������ (����)
// ���� ���� ��� �ϰ�
// ���� ������� �����ؼ� (����)
// ���� (���̰� �ٸ� �� ������ ���콺�� ������ ����) 
// -> ����� �� (�ӱ���)

// ����
//������Ʈ�����̴�,
//�潦�̴�,
//�׼����̼�,
//�����ν��̴�,
//��ǻƮ���̴�

#define TEXTURE_HEIGHT 32.0f

class Terrain : public Renderer
{
public:
	typedef VertexTextureNormal TerrainVertex;

public:
	Terrain(Shader* shader, wstring heightMap);
	~Terrain();

	void Update();
	void Render();

	void BaseMap(wstring file);
	void LayerMap(wstring layerMap, wstring alphaMap);

	// ���⸦ �̿�
	float GetHeight(Vector3& position);

	// ���� ĳ��Ʈ �̿�
	float GetHeightPick(Vector3& position);

	Vector3 GetPickedPosition();

	void SaveHeightMap();

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();

	void UpdateBrush();
	void UpdateVertices();
	void UpdateLine();

private:
	UINT width;
	UINT height;

	Texture* heightMap;

	Texture* baseMap = nullptr;
	ID3DX11EffectShaderResourceVariable* sBaseMap;

	Texture* layerMap = nullptr;
	ID3DX11EffectShaderResourceVariable* sLayerMap;

	Texture* alphaMap = nullptr;
	ID3DX11EffectShaderResourceVariable* sAlphaMap;

	TerrainVertex* vertices;
	UINT* indices;

	Vector3 prevMousePos;
	class TerrainBrushDemo* brush;

private:
	struct LineDesc
	{
		Color Color = D3DXCOLOR(0xFFFFFFFF);
		UINT Visible = 0;
		float Thickness = 0.01f;
		float Size = 5.0f;
		float Padding;
	}lineDesc;
	ConstantBuffer* lineBuffer;
	ID3DX11EffectConstantBuffer* sLineBuffer;
};
