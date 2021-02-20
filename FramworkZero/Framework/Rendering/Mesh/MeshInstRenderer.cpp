#include "Framework.h"
#include "MeshInstRenderer.h"

MeshInstRenderer::MeshInstRenderer(unique_ptr<MeshData> data)
	: meshData(move(data))
{
	renderer = new MeshRenderer(meshData.get());
	instanceBuffer = new VertexBuffer(worlds, MESH_INSTANCE_MAX_COUNT, sizeof(Matrix), 1, true);
}

MeshInstRenderer::~MeshInstRenderer()
{
	for (auto d : instances)
		SafeDelete(d);

	SafeDelete(instanceBuffer);
	SafeDelete(renderer);

	meshData->SafeDeleteData();
	meshData.reset();
}

void MeshInstRenderer::Render()
{
	instanceBuffer->Render();
	renderer->RenderInstance(instances.size());
}


// --
// MeshInstance
// --

#pragma region MeshInstance

MeshInstance * MeshInstRenderer::AddInstance()
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

void MeshInstRenderer::RemoveInstance(MeshInstance * value)
{
	assert(value->Perent() == this);

	Transform* t = value->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->GlobalWorld(m);

	junkInstances.push_back(value->Id());
}

void MeshInstRenderer::UpdateTransforms()
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

#pragma endregion

