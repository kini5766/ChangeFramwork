#include "Framework.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Shader * shader, MeshData* data)
	: meshData(data), defultMaterial(new Material(shader))
{
	vertexBuffer = new VertexBuffer(meshData->Vertices, meshData->VertexCount, meshData->Stride);
	indexBuffer = new IndexBuffer(meshData->Indices, meshData->IndexCount);

	if (meshData->SubMeshes == nullptr)
		meshData->NewSubMesh();

	UINT SubMeshCount = meshData->SubMeshCount;
	materials = new Material*[SubMeshCount];
	for (UINT i = 0; i < SubMeshCount; i++)
		materials[i] = defultMaterial;
}

MeshRenderer::~MeshRenderer()
{
	meshData->SafeDeleteData();
	SafeDelete(meshData);
	SafeDelete(defultMaterial);
	SafeDeleteArray(materials);
}

void MeshRenderer::Update()
{
}

void MeshRenderer::Render()
{
	vertexBuffer->Render();
	indexBuffer->Render();

	for (UINT i = 0; i < meshData->SubMeshCount; i++)
	{
		SubMeshData& subMesh = meshData->SubMeshes[i];

		D3D::GetDC()->IASetPrimitiveTopology(subMesh.topology);

		materials[i]->Render();
		materials[i]->GetShader()->DrawIndexed(
			0, pass, subMesh.IndexCount, subMesh.StartIndex);
	}
}

void MeshRenderer::RenderInstance(UINT drawCount)
{
	vertexBuffer->Render();
	indexBuffer->Render();

	for (UINT i = 0; i < meshData->SubMeshCount; i++)
	{
		SubMeshData& subMesh = meshData->SubMeshes[i];

		D3D::GetDC()->IASetPrimitiveTopology(subMesh.topology);

		materials[i]->Render();
		materials[i]->GetShader()->DrawIndexedInstanced(
			0, pass, subMesh.IndexCount, drawCount, subMesh.StartIndex);
	}
}

void MeshRenderer::SetMaterials(Material ** _materials, UINT count)
{
	// 주소복사
	memcpy_s(materials, sizeof(Material*) * meshData->SubMeshCount, _materials, sizeof(Material*) * count);
}
