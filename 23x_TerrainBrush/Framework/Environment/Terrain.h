#pragma once

// 터레인
// 각진 지형 스무딩 (평균)
// 지형 울퉁불퉁 노이즈 (랜덤)
// 원형 지형 상승 하강
// 경사면 나름대로 정의해서 (선택)
// 경사면 (높이가 다른 두 지점을 마우스를 누르면 생김) 
// -> 경사의 폭 (임구이)

// 공부
//지오메트리쉐이더,
//헐쉐이더,
//테셀레이션,
//도메인쉐이더,
//컴퓨트쉐이더

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

	// 기울기를 이용
	float GetHeight(Vector3& position);

	// 레이 캐스트 이용
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
