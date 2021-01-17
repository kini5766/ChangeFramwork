#include "Framework.h"
#include "MeshInstancing.h"

MeshInstancing::MeshInstancing(Shader * shader, MeshData * data)
	: meshData(data)
{
	renderer = new MeshRenderer(shader, data);
	perframe = new PerFrame(shader);

	instanceBuffer = new VertexBuffer(worlds, MESH_INSTANCE_MAX_COUNT, sizeof(Matrix), 1, true);
}

MeshInstancing::~MeshInstancing()
{
	for (auto d : instances)
		SafeDelete(d);

	SafeDelete(instanceBuffer);

	SafeDelete(perframe);
	SafeDelete(renderer);

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

	perframe->Render();
	renderer->RenderInstance(instances.size());
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
	}
	else
	{
		UINT index = junkInstances.back();

		instance = instances[index];

		Matrix m;
		D3DXMatrixIdentity(&m);
		instance->GetTransform()->GlobalWorld(m);

		junkInstances.pop_back();
	}

	return instance;
}

void MeshInstancing::RemoveInstance(MeshInstance * value)
{
	assert(value->Perent() == this);

	Transform* t = value->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->GlobalWorld(m);

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
