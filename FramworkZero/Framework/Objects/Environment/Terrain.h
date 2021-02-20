#pragma once

constexpr auto TERRAIN_TEXTURE_HEIGHT = 32.0f;

class Terrain
{
public:
	struct VertexTerrain;
		
public:
	Terrain(wstring imageFile);
	~Terrain();

public:
	void Update();
	void PreRender_Depth();
	void Render();
	void RenderVisibleNormal();

	void RecalculateNormals();
	void ApplyVertex();
	void ApplyAlphasLayer1();
	void ApplyAlphasLayer2();
	void ApplyAlphasLayer3();


	// Getter, Setter
public:
	void BaseMap(wstring file);
	void Layer1(wstring file);
	void Layer2(wstring file);
	void Layer3(wstring file);

	UINT Width() const { return width; }
	UINT Height() const { return height; }
	VertexTerrain* Vertices() { return vertices; }
	const MeshData* GetMeshData() const { return meshData; }

	float* Layer1Data() { return layer1.Data; }
	float* Layer2Data() { return layer2.Data; }
	float* Layer3Data() { return layer3.Data; }

	Shader* GetShader() { return shader; }
	void Pass(UINT value) { pass = value; }
	Transform* GetTransform() { return perTransform->GetTransform(); }

private:
	void ReadHeightData();
	void CreateVertexData();
	void CreateIndexData();


private:
	VertexTerrain* vertices;
	Texture* baseMap = nullptr;
	float* heights;  // ����Ʈ���� ����
	UINT width, height;  // ����Ʈ�� �������� ���� (�� ĭ ���ڶ��)
	wstring imageFile;

	// render
private:
	Shader* shader;
	Material* material;
	MeshData* meshData;
	IndexBuffer* indexBuffer;
	VertexBuffer* vertexBuffer;
	PerTransform* perTransform;
	UINT pass = 3;
	ShadowCaster* shadowCaster;

public:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector3 Normal = Vector3(0, 0, 0);
		Vector2 Uv = Vector2(0, 0);
		Color _Color = Color(0, 0, 0, 0);
	};

private:
	struct Layer
	{
		// ä�� ���� (from dds)
		float* Data = nullptr;
		ID3D11Texture2D* Texture2D;
		ID3D11ShaderResourceView* SRV = nullptr;
		ID3DX11EffectShaderResourceVariable* sSRV;

		// ��
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
