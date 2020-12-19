#include "Framework.h"
#include "MeshInstancing.h"

MeshInstancing::MeshInstancing(Shader * shader, MeshData * data)
{
	meshData = new MeshData();
	MeshData::Copy<VertexMesh>(meshData, data);
	
	material = new Material(shader);
	perframe = new PerFrame(shader);
	vertexBuffer = new VertexBuffer(meshData->Vertices, meshData->VertexCount, meshData->Stride);
	indexBuffer = new IndexBuffer(meshData->Indices, meshData->IndexCount);

	instanceBuffer = new VertexBuffer(worlds, MESH_INSTANCE_MAX_COUNT, sizeof(Matrix), 1, true);
	instanceColorBuffer = new VertexBuffer(colors, MESH_INSTANCE_MAX_COUNT, sizeof(Color), 2, true);

	data->SafeDeleteData();
	SafeDelete(data);
}

MeshInstancing::~MeshInstancing()
{
	for (auto d : instances)
		SafeDelete(d);

	SafeDelete(instanceBuffer);
	SafeDelete(instanceColorBuffer);

	SafeDelete(material);
	SafeDelete(perframe);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	meshData->SafeDeleteData();
	SafeDelete(meshData);
}

void MeshInstancing::Update()
{
	perframe->Update();
}

void MeshInstancing::Render()
{
	instanceBuffer->Render();
	instanceColorBuffer->Render();

	perframe->Render();

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->Render();
	material->GetShader()->DrawIndexedInstanced(0, pass, meshData->IndexCount, instances.size());
}


MeshInstance * MeshInstancing::AddInstance()
{
	MeshInstance* instance = nullptr;

	if (junkInstances.size() == 0)
	{
		UINT index = instances.size();

		instance = new MeshInstance(this, index);
		instances.push_back(instance);

		instance->GetTransform()->ReplaceMatrixGetter(worlds + index);
		colors[index] = Color(0, 0, 0, 1);
	}
	else
	{
		UINT index = junkInstances.back();

		instance = instances[index];

		Matrix m;
		D3DXMatrixIdentity(&m);
		instance->GetTransform()->LossyWorld(m);

		colors[index] = Color(0, 0, 0, 1);

		junkInstances.pop_back();
	}

	return instance;
}

void MeshInstancing::RemoveInstance(MeshInstance * value)
{
	assert(value->Perent() == this);

	Transform* t = value->GetTransform();
	Transform* parent = t->GetParent();
	Transform** childs = t->GetChilds();
	for (UINT i = 0; i < t->ChildCount(); i++)
	{
		childs[i]->SetParent(parent);
	}
	t->SetParent(nullptr);

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->LossyWorld(m);

	junkInstances.push_back(value->Id());
}

void MeshInstancing::UpdateTransforms()
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	for (MeshInstance* instance : instances)
		instance->GetTransform()->UpdateWorld();

	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MESH_INSTANCE_MAX_COUNT);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}

void MeshInstancing::UpdateColors()
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	D3D::GetDC()->Map(instanceColorBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, colors, sizeof(Color) * MESH_INSTANCE_MAX_COUNT);
	}
	D3D::GetDC()->Unmap(instanceColorBuffer->Buffer(), 0);
}

void MeshInstancing::SetColor(UINT instance, const Color & color)
{
	colors[instance] = color;
}
