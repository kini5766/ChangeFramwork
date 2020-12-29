#include "Framework.h"
#include "ModelSkinnedInstancing.h"
#include "ModelComputeAnimInst.h"

using namespace ShaderEffectName;

ModelSkinnedInstancing::ModelSkinnedInstancing(Shader* shader, const ModelDesc& desc)
{
	perframe = new PerFrame(shader);
	data = new ModelData();

	instanceBuffer = new VertexBuffer(worlds, MODEL_INSTANCE_MAX_COUNT, sizeof(Matrix), 1, true);
	instanceColorBuffer = new VertexBuffer(colors, MODEL_INSTANCE_MAX_COUNT, sizeof(Color), 2, true);

	data->ReadMaterial(desc.MaterialFile);
	data->ReadMesh(desc.MeshFile);
	for (auto& file : desc.ClipFiles)
		data->ReadClip(file);

	ApplyModel(shader);
}

ModelSkinnedInstancing::~ModelSkinnedInstancing()
{
	SafeDelete(instanceColorBuffer);
	SafeDelete(instanceBuffer);

	SafeDelete(renderer);
	SafeDelete(compute);
	SafeDelete(data);

	SafeDelete(perframe);
	SafeDelete(transform);
	SafeDelete(invBindPose);
}

void ModelSkinnedInstancing::Update()
{
	for (ModelSkinnedInstance* instance : instances)
		instance->Update();

	perframe->Update();
	if (compute != nullptr)
		compute->Update();

	renderer->Update();
}

void ModelSkinnedInstancing::Render()
{
	instanceBuffer->Render();
	instanceColorBuffer->Render();

	perframe->Render();
	renderer->RenderInstance(instances.size());
}


#pragma region Instance

ModelSkinnedInstance * ModelSkinnedInstancing::AddInstance()
{
	ModelSkinnedInstance* instance = nullptr;

	if (junkInstances.size() == 0)
	{
		UINT index = instances.size();

		instance = new ModelSkinnedInstance(this, index);
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

void ModelSkinnedInstancing::RemoveInstance(ModelSkinnedInstance * value)
{
	assert(value->Perent() == this);

	Transform* t = value->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->LossyWorld(m);

	junkInstances.push_back(value->Id());
}

void ModelSkinnedInstancing::UpdateTransforms()
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	for (ModelSkinnedInstance* instance : instances)
		instance->GetTransform()->UpdateWorld();

	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MODEL_INSTANCE_MAX_COUNT);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}

void ModelSkinnedInstancing::UpdateColors()
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	D3D::GetDC()->Map(instanceColorBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, colors, sizeof(Color) * MODEL_INSTANCE_MAX_COUNT);
	}
	D3D::GetDC()->Unmap(instanceColorBuffer->Buffer(), 0);
}

void ModelSkinnedInstancing::SetColor(UINT instance, const Color & color)
{
	colors[instance] = color;
}

#pragma endregion


void ModelSkinnedInstancing::GetAttachBones(UINT instace, Matrix * matrix)
{
	ID3D11Texture2D* texture = compute->CopyFromOutput();
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_READ, 0, &subResource);
	{
		// 2의 승수로 받기 때문에 넣을 때 크기가 다를 수 있음
		// RowPitch : 1줄, DepthPitch : 1장
		BYTE* start = (BYTE*)subResource.pData + (instace * subResource.RowPitch);
		memcpy(matrix, start, sizeof(Matrix) * boneCount);
	}
	D3D::GetDC()->Unmap(texture, 0);
}

KeyframeDesc * ModelSkinnedInstancing::GetAnimationDesc(UINT index)
{
	return compute->GetDesc(index);
}


void ModelSkinnedInstancing::ApplyModel(Shader* shader)
{
	for (Material* mat : data->Materials())
		mat->SetShader(shader);

	boneCount = data->BoneCount();

	if (data->ClipCount() != 0)
		compute = new ModelComputeAnimInst(data, &world);
	renderer = new SkinnedMeshRenderer();

	// Set BonesMap
	{
		UINT size = data->MeshCount();
		for (UINT i = 0; i < size; i++)
		{
			ModelMeshData* mesh = data->MeshByIndex(i);
			renderer->Renderers().push_back(new MeshRenderer(shader, mesh->Mesh));
		}
		if (data->ClipCount() == 0)
			renderer->BindPose()->SrvBonesMap = nullptr;
		else
			renderer->BindPose()->SrvBonesMap = compute->GetOutputBoneResultSrv();
	}

	// Set InvBindPose
	{
		Matrix* boneDesc = new Matrix[boneCount];

		for (UINT i = 0; i < boneCount; i++)
			D3DXMatrixInverse(&boneDesc[i], nullptr, &data->BoneByIndex(i)->Transform);

		invBindPose = new Texture2D(boneCount * 4, 1);
		invBindPose->SetColors(boneDesc);
		invBindPose->CreateTexture();
		invBindPose->CreateSRV();
		SafeDeleteArray(boneDesc);

		renderer->BindPose()->SrvInvBindPose = invBindPose->GetSRV();
	}

	renderer->SetMaterials(data->Materials().data(), data->Materials().size());

}


// --
// MeshInstance
// --

#include "ModelAnimation.h"


ModelSkinnedInstance::ModelSkinnedInstance(ModelSkinnedInstancing * perent, UINT id)
	: perent(perent), id(id)
{
	transform = new Transform();
	animation = new ModelAnimation(
		perent->GetModel(),
		perent->GetAnimationDesc(id)
	);
}

ModelSkinnedInstance::~ModelSkinnedInstance()
{
	SafeDelete(transform);
	SafeDelete(animation);
}

void ModelSkinnedInstance::Update()
{
	animation->Update();
}
