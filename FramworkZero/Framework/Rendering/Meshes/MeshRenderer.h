#pragma once

// 기존과는 다르게 트랜스폼 없음

// MeshData의 메쉬와 서브메쉬들 렌더
// Materials 인덱스 == 서브메쉬 인덱스
class MeshRenderer
{
public:
	// MeshData* : 그대로 사용 delete도 이 클레스에서
	MeshRenderer(Shader* shader, MeshData* data);
	~MeshRenderer();

public:
	void Update();
	void Render();
	void RenderInstance(UINT drawCount);

public:
	void Pass(UINT value) { pass = value; }
	Material* GetDefultMaterial() { return defultMaterial; }
	// Material* 단위 복사
	void SetMaterials(Material** materials, UINT count);
	void SetMaterial(UINT index, Material* material) { materials[index] = material; }
	Material* GetMaterial(UINT index) { return materials[index]; }
	Material** GetMaterials() { return materials; }
	const MeshData* GetData() { return meshData; }


private: // renderer
	MeshData* meshData;

	Shader* shader;
	Material* defultMaterial;
	// 서브메쉬 수만큼 배열 할당
	// materials[]->
	Material** materials;
	UINT pass = 0;

	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;
};