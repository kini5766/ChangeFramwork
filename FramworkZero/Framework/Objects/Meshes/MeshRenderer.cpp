#include "Framework.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Shader * shader, MeshData* data)
	: meshData(data), defaultMaterial(new Material(shader))
{
	vertexBuffer = new VertexBuffer(meshData->Vertices, meshData->VertexCount, meshData->Stride);
	indexBuffer = new IndexBuffer(meshData->Indices, meshData->IndexCount);

	if (meshData->SubMeshes == nullptr)
		meshData->NewSubMesh();

	UINT SubMeshCount = meshData->SubMeshCount;
	materials.push_back(defaultMaterial);
}

MeshRenderer::~MeshRenderer()
{
	SafeDelete(indexBuffer);
	SafeDelete(vertexBuffer);
	SafeDelete(defaultMaterial);
}

void MeshRenderer::Render()
{
	vertexBuffer->Render();
	indexBuffer->Render();

	for (UINT i = 0; i < meshData->SubMeshCount; i++)
	{
		SubMeshData& subMesh = meshData->SubMeshes[i];

		D3D::GetDC()->IASetPrimitiveTopology(subMesh.topology);
		Material* mat = materials[subMesh.MaterialIndex];

		mat->Render();
		mat->GetShader()->DrawIndexed(
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
		Material* mat = materials[subMesh.MaterialIndex];

		mat->Render();
		mat->GetShader()->DrawIndexedInstanced(
			0, pass, subMesh.IndexCount, drawCount, subMesh.StartIndex);
	}
}
