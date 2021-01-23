#pragma once

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


	// Getter, Setter
public:
	void BaseMap(wstring file);
	UINT Width() const { return width; }
	UINT Height() const { return height; }
	VertexTerrain* Vertices() { return vertices; }

	Shader* GetShader() { return shader; }
	void Pass(UINT value) { pass = value; }
	Transform* GetTransform() { return perTransform->GetTransform(); }


	// Picker
public:
	float GetHeight(const Vector3& position);  // ���� 1ĭ�� ũ�Ⱑ 1�϶��� ��밡��
	Vector3 GetNormal(float x, float z);
	float GetVerticalRaycast(const Vector3& position);
	Vector3 GetRaycastPosition(Vector3* start = nullptr);
	bool GetMouseRaycast(Vector3* outPoint);

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
	ShadowTest* shadow;

public:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector3 Normal = Vector3(0, 0, 0);
		Vector2 Uv = Vector2(0, 0);
		Color _Color = Color(0, 0, 0, 0);
	};
};