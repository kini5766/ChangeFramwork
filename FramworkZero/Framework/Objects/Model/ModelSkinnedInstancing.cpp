#include "Framework.h"
#include "ModelSkinnedInstancing.h"
#include "ModelComputeAnimInst.h"

using namespace ShaderEffectName;

ModelSkinnedInstancing::ModelSkinnedInstancing(Shader* shader, const ModelDesc& desc)
{
	perframe = new PerFrame(shader);
	data = new ModelData();

	instanceBuffer = new VertexBuffer(worlds, MODEL_INSTANCE_MAX_COUNT, sizeof(Matrix), 1, true);

	data->ReadMaterial(desc.MaterialFile);
	data->ReadMesh(desc.MeshFile);
	for (auto& file : desc.ClipFiles)
		data->ReadClip(file);

	ApplyModel(shader);
}

ModelSkinnedInstancing::~ModelSkinnedInstancing()
{
	SafeDelete(instanceBuffer);

	SafeDelete(renderer);
	SafeDelete(compute);
	SafeDelete(data);

	SafeDelete(perframe);

	SafeRelease(srvInvBindPose);
	SafeRelease(invBindPose);
}

void ModelSkinnedInstancing::Update()
{
	if (data->ClipCount() != 0)
	{
		for (ModelSkinnedInstance* instance : instances)
			instance->Update();
	}

	perframe->Update();
	if (compute != nullptr)
		compute->Update();

	renderer->Update();
}

void ModelSkinnedInstancing::Render()
{
	instanceBuffer->Render();

	perframe->Render();
	renderer->RenderInstance(instances.size());
}

void ModelSkinnedInstancing::UpdateBoneTracking()
{
	ID3D11Texture2D* texture = compute->CopyFromOutput();
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_READ, 0, &subResource);
	{
		UINT size = instances.size();
		for (UINT i = 0; i < size; i++)
		{
			// 2의 승수로 받기 때문에 넣을 때 크기가 다를 수 있음
			// RowPitch : 1줄, DepthPitch : 1장
			BYTE* start = (BYTE*)subResource.pData + (i * subResource.RowPitch);
			instances[i]->UpdateBoneTracking((Matrix*)start);
		}
	}
	D3D::GetDC()->Unmap(texture, 0);
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

void ModelSkinnedInstancing::RemoveInstance(ModelSkinnedInstance * value)
{
	assert(value->Perent() == this);

	Transform* t = value->GetTransform();
	t->UnLink();

	Matrix m;
	ZeroMemory(m, sizeof(Matrix));
	t->GlobalWorld(m);

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

#pragma endregion


BlendDesc * ModelSkinnedInstancing::GetAnimationDesc(UINT index)
{
	return compute->GetDesc(index);
}


void ModelSkinnedInstancing::ApplyModel(Shader* shader)
{
	for (Material* mat : data->Materials())
		mat->SetShader(shader);


	if (data->ClipCount() != 0)
		compute = new ModelComputeAnimInst(data);
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
		UINT boneCount = data->BoneCount();
		Matrix* boneDesc = new Matrix[boneCount * 2];

		for (UINT i = 0; i < boneCount; i++)
		{
			D3DXMatrixInverse(&boneDesc[i * 2 + 0], nullptr, &data->BoneByIndex(i)->Transform);
			memcpy(&boneDesc[i * 2 + 1], &data->BoneByIndex(i)->Transform, sizeof(Matrix));
		}

		Texture2DDesc tex2DDesc;
		D3D11_TEXTURE2D_DESC& desc = tex2DDesc.Desc();
		desc.Width = boneCount * 2 * 4;
		desc.Height = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;  // 쉐이더에 사용할 텍스쳐
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		tex2DDesc.SetColors(boneDesc);

		invBindPose = tex2DDesc.CreateTexture(4u * 4u);
		srvInvBindPose = tex2DDesc.CreateSRV();
		SafeDeleteArray(boneDesc);

		renderer->BindPose()->SrvInvBindPose = srvInvBindPose;
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

	boneCount = perent->GetModel()->BoneCount();
}

ModelSkinnedInstance::~ModelSkinnedInstance()
{
	SafeDelete(transform);
	SafeDelete(animation);

	if (bBoneTracking)
		SafeDeleteArray(bones);
}

void ModelSkinnedInstance::Update()
{
	animation->Update();
}

ModelAnimation * ModelSkinnedInstance::GetAnimation()
{
	return animation;
}

void ModelSkinnedInstance::UpdateBoneTracking(Matrix * tracking)
{
	if (bBoneTracking == false)
	{
		bones = new Matrix[boneCount];
		bBoneTracking = true;
	}
	memcpy(bones, tracking, sizeof(Matrix) * boneCount);
}

Matrix ModelSkinnedInstance::GetAttachBone(UINT instace) 
{ 
	assert(bBoneTracking);
	return bones[instace]; 
}
