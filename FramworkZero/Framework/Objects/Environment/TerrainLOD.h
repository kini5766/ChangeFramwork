#pragma once

class TerrainLOD
{
public:
	TerrainLOD(wstring ddsFile);
	~TerrainLOD();

public:
	void Update();
	void Render();
	void PreRender_Depth();

public:
	void BaseMap(wstring file);
	void Layer1(wstring file);
	void Layer2(wstring file);
	void Layer3(wstring file);

public:
	float GetWidth() const;
	float GetHeight() const;

	//float HeightScale() { return desc.HeightScale; }
	float& HeightScale() { return desc.HeightScale; }
	Vector2& Distance() { return desc.Distance; }

	// ÇÈ¼¿ °³¼ö
	UINT MapWidth() { return width; }
	UINT MapHeight() { return height; }

	Material* GetMaterial() { return material; }
	Transform* GetTransform() { return perTransform->GetTransform(); }

public:
	void ApplyHeightMap();
	void ApplyAlphasLayer1();
	void ApplyAlphasLayer2();
	void ApplyAlphasLayer3();

	float* HeightMapData() { return heights; }
	float* Layer1Data() { return layer1.Data; }
	float* Layer2Data() { return layer2.Data; }
	float* Layer3Data() { return layer3.Data; }

private:
	void ReadDataMap();
	void CreatePatchMeshData();


	// Renderer
private:
	Shader* shader;
	MeshData* meshData;
	PerTransform* perTransform;
	MeshRenderer* renderer;
	ShadowCaster* shadow;

	Material* material;

private:
	// µ¥ÀÌÅÍ ÆÄÀÏ
	wstring ddsFile;
	// ÇÈ¼¿ ÇØ»óµµ
	UINT width, height;

	// Ä­ ÇÕÄ¥ °³¼ö
	UINT cellsPerPatch = 32;

	// layerMaps
	Texture* baseMap = nullptr;

	// desc buffer
	ConstantBuffer* buffer;

	// height Map
	float* heights;
	ID3D11Texture2D* heightMap = nullptr;
	ID3D11ShaderResourceView* heightMapSRV = nullptr;

private:
	struct TerrainVertex
	{
		Vector3 Position;
		Vector2 Uv;
	};

private:
	struct Desc
	{
		Vector2 Distance = Vector2(1, 380);
		Vector2 TessFactor = Vector2(1, 64);

		float CellSpacing = 2.0f;
		float CellSpacingU;  // pixelSize x
		float CellSpacingV;  // pixelSize y
		float HeightScale = 0.275f;
	}desc;

private:
	struct Layer
	{
		// Ã¤³Î Á¤º¸ (from dds)
		float* Data = nullptr;
		ID3D11Texture2D* Texture2D;
		ID3D11ShaderResourceView* SRV = nullptr;
		ID3DX11EffectShaderResourceVariable* sSRV;

		// ¸Ê
		Texture* Map = nullptr;
		ID3DX11EffectShaderResourceVariable* sMap;

		~Layer()
		{
			SafeDeleteArray(Data);
			SafeRelease(Texture2D);
			SafeRelease(SRV);
			SafeDelete(Map);
		}
	};
	Layer layer1, layer2, layer3;
};
