#pragma once

// 기존과는 다르게 트랜스폼 없음

// MeshData의 메쉬와 서브메쉬들 렌더
class MeshRenderer
{
public:
	MeshRenderer(MeshData* data);
	MeshRenderer(MeshData* data, ShaderSetter** mats, UINT matCount);
	~MeshRenderer();

public:
	void Render();
	void RenderInstance(UINT drawCount);

public:
	void Pass(UINT value) { pass = value; }
	void SetMaterial(ShaderSetter* value, UINT index = 0);
	const MeshData* GetData() const { return meshData; }

private: // renderer
	MeshData* meshData;

	ShaderSetter* defaultMaterial;
	vector<ShaderSetter*> materials;
	UINT pass = 0;

	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;
};