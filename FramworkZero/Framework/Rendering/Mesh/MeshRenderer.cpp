#include "Framework.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(MeshData* data)
	: meshData(data)
{
	defaultMaterial = new ShaderSetter(URI::Shaders + L"01_Mesh.fxo");
	vertexBuffer = new VertexBuffer(meshData->Vertices, meshData->VertexCount, meshData->Stride);
	indexBuffer = new IndexBuffer(meshData->Indices, meshData->IndexCount);

	if (meshData->SubMeshes == nullptr)
		meshData->NewSubMesh();

	materials.assign(meshData->SubMeshCount, defaultMaterial);
}

MeshRenderer::MeshRenderer(MeshData* data, ShaderSetter** mats, UINT matCount)
	: meshData(data), defaultMaterial(nullptr)
	, materials(mats, mats + matCount)
{
	vertexBuffer = new VertexBuffer(meshData->Vertices, meshData->VertexCount, meshData->Stride);
	indexBuffer = new IndexBuffer(meshData->Indices, meshData->IndexCount);
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
		ShaderSetter* mat = materials[subMesh.MaterialIndex];

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
		ShaderSetter* mat = materials[subMesh.MaterialIndex];

		mat->Render();
		mat->GetShader()->DrawIndexedInstanced(
			0, pass, subMesh.IndexCount, drawCount, subMesh.StartIndex);
	}
}

void MeshRenderer::SetMaterial(ShaderSetter * value, UINT index)
{
	materials[index] = value;
}
