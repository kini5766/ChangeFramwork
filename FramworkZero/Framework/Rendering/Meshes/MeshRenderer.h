#pragma once

// �������� �ٸ��� Ʈ������ ����

// MeshData�� �޽��� ����޽��� ����
// Materials �ε��� == ����޽� �ε���
class MeshRenderer
{
public:
	// MeshData* : �״�� ��� delete�� �� Ŭ��������
	MeshRenderer(Shader* shader, MeshData* data);
	~MeshRenderer();

public:
	void Update();
	void Render();
	void RenderInstance(UINT drawCount);

public:
	void Pass(UINT value) { pass = value; }
	Material* GetDefultMaterial() { return defultMaterial; }
	// Material* ���� ����
	void SetMaterials(Material** materials, UINT count);
	void SetMaterial(UINT index, Material* material) { materials[index] = material; }
	Material* GetMaterial(UINT index) { return materials[index]; }
	Material** GetMaterials() { return materials; }
	const MeshData* GetData() { return meshData; }


private: // renderer
	MeshData* meshData;

	Shader* shader;
	Material* defultMaterial;
	// ����޽� ����ŭ �迭 �Ҵ�
	// materials[]->
	Material** materials;
	UINT pass = 0;

	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;
};