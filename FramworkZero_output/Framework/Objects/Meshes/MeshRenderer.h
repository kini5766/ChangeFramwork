#pragma once

// �������� �ٸ��� Ʈ������ ����

// MeshData�� �޽��� ����޽��� ����
class MeshRenderer
{
public:
	MeshRenderer(Shader* shader, MeshData* data);
	~MeshRenderer();

public:
	void Update();
	void Render();
	void RenderInstance(UINT drawCount);

public:
	void Pass(UINT value) { pass = value; }
	Material* GetDefaultMaterial() const { return defaultMaterial; }
	void SetMaterial(UINT index, Material* material) { materials[index] = material; }
	Material* GetMaterial(UINT index) const { return materials[index]; }
	vector<Material*>& Materials() { return materials; }
	const MeshData* GetData() const { return meshData; }


private: // renderer
	MeshData* meshData;

	Shader* shader;
	Material* defaultMaterial;
	vector<Material*> materials;
	UINT pass = 0;

	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;
};